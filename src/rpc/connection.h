#pragma once

#include <list>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/cstdint.hpp>

#if defined(PROTOBUF_FOUND)
#include <google/protobuf/message.h>
#endif

#include "buffer.h"
#include "handler.h"

namespace rpc
{

typedef boost::shared_ptr<class Connection> ConnectionPtr;

class Connection:
		public boost::noncopyable,
		public boost::enable_shared_from_this<Connection>
{
public:
	Connection(Handler *handler, boost::asio::io_service &io_service);

	boost::asio::ip::tcp::socket &socket() { return socket_; }

	void start();
	void write(BufferPtr const &buffer_ptr);

#if defined(PROTOBUF_FOUND)
	void write(boost::uint16_t id, google::protobuf::Message const &message);
#endif

private:
	static size_t const MINIMAL_ASYNC_READ_BUFFER_SIZE = 1024;

	Handler *handler_;
	boost::asio::io_service::strand strand_;
	boost::asio::ip::tcp::socket socket_;

	boost::mutex write_queue_mutex_;
	std::list<BufferPtr > write_queue_;

	void schedule_write();
	void read(BufferPtr const &buffer_ptr);
	void handle_read(BufferPtr const &buffer_ptr,
		size_t bytes_transferred, boost::system::error_code const &error);
	void handle_write(size_t bytes_transferred, boost::system::error_code const &error);
};

}
