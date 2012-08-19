
#include <assert.h>
#include <iosfwd>
#include <boost/bind.hpp>
#include <boost/thread/locks.hpp>
#include "header.h"
#include "server_service.h"
#include "connection.h"

namespace rpc
{

Connection::Connection(Handler *Handler, boost::asio::io_service &io_service):
	handler_(Handler),
	strand_(io_service),
	socket_(io_service)
{
}

void Connection::start()
{
	read(BufferPtr(new Buffer()));
}

void Connection::read(BufferPtr const &buffer_ptr)
{
	if (buffer_ptr->write_bytes_available() < MINIMAL_ASYNC_READ_BUFFER_SIZE)
	{
		buffer_ptr->enlarge();
	}

	socket_.async_read_some(boost::asio::buffer(buffer_ptr->write_ptr(), buffer_ptr->write_bytes_available()),
		strand_.wrap(boost::bind(&Connection::handle_read, shared_from_this(), buffer_ptr,
				boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)));
}

void Connection::write(BufferPtr const &buffer_ptr)
{
	bool no_write_in_progress = false;

	{
		boost::unique_lock<boost::mutex> lock(write_queue_mutex_);
		no_write_in_progress = write_queue_.empty();
		write_queue_.push_back(buffer_ptr);
	}

	if (no_write_in_progress)
	{
		schedule_write();
	}
}

#if defined(PROTOBUF_FOUND)
void Connection::write(boost::uint16_t id, google::protobuf::Message const &message)
{
	size_t message_size = message.ByteSize();

	boost::shared_ptr<rpc::Buffer> buffer(new rpc::Buffer());

	rpc::Header header;
	header.id = id;
	header.len = message_size;
	buffer->write(&header, sizeof(header));

	if (buffer->write_bytes_available() < message_size) {
		buffer->enlarge();
	}

	assert (buffer->write_bytes_available() >= message_size);

	message.SerializeToArray(buffer->write_ptr(), buffer->write_bytes_available());
	buffer->seek_write(message_size);

	write(buffer);
}
#endif

void Connection::schedule_write()
{
	BufferPtr buffer_ptr;

	{
		boost::unique_lock<boost::mutex> lock(write_queue_mutex_);
		assert (!write_queue_.empty());
		buffer_ptr = write_queue_.front();
	}

	boost::asio::async_write(socket_,
		boost::asio::buffer(buffer_ptr->read_ptr(), buffer_ptr->read_bytes_available()),
		strand_.wrap(boost::bind(&Connection::handle_write, shared_from_this(),
		boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)));
}

void Connection::handle_read(BufferPtr const &buffer_ptr,
	size_t bytes_transferred, boost::system::error_code const &error)
{
	if (error)
	{
		return handler_->on_error(shared_from_this(), error);
	}

	buffer_ptr->seek_write(bytes_transferred);

	if (bytes_transferred == 0 ||
		buffer_ptr->read_bytes_available() < sizeof(Header) ||
		sizeof(Header) + reinterpret_cast<Header const *>(buffer_ptr->begin_ptr())->len > buffer_ptr->read_bytes_available())
	{
		return read(buffer_ptr);
	}

	Header h;
	buffer_ptr->read((char *) &h, sizeof(h));

	size_t total_length = h.len + sizeof(Header);

	size_t write_offset_saved = buffer_ptr->write_offset();
	buffer_ptr->set_write(total_length);

	try
	{
		handler_->on_read(shared_from_this(), h.id, *buffer_ptr);
	}
	catch(Handler::CloseConnectionException const &)
	{
		return;
	}

	buffer_ptr->set_read(total_length);
	buffer_ptr->set_write(write_offset_saved);
	buffer_ptr->consume(total_length);

	read(buffer_ptr);
}

void Connection::handle_write(size_t bytes_transferred, boost::system::error_code const &error)
{
	BufferPtr buffer_ptr;

	bool more_writes_pending;

	{
		boost::unique_lock<boost::mutex> lock(write_queue_mutex_);
		buffer_ptr = write_queue_.front();
		write_queue_.pop_front();

		more_writes_pending = !write_queue_.empty();
	}

	if (more_writes_pending)
	{
		schedule_write();
	}

	if (error)
	{
		handler_->on_error(shared_from_this(), error);
	}
}

}
