
#include <deque>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include "handler.h"
#include "connection.h"
#include "client_service.h"
#include "client.h"

#include <util/logger.h>

namespace rpc
{

class Client::Impl:
	public rpc::Handler,
	public boost::noncopyable
{
public:
	Impl():
		service_(this)
	{
	}

	~Impl()
	{
		stop();
		wait();
	}

	void connect(std::string const &host, std::string const &port)
	{
		service_.connect(host, port);
	}

	void start()
	{
		service_.start(1);
	}

	void stop()
	{
		service_.stop();
	}

	void wait()
	{
		service_.wait();
	}

	bool is_connected() const
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		return is_connected_;
	}

	bool have_error() const
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		return have_error_;
	}

	std::string error_message() const
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		return error_message_;
	}

#if defined(PROTOBUF_FOUND)
	void send(boost::uint16_t id, google::protobuf::Message const &message)
	{
		connection_->write(id, message);
	}
#endif

	boost::tuple<boost::uint16_t, rpc::BufferPtr> receive()
	{
		boost::unique_lock<boost::mutex> lock(mutex_);

		if (incoming_messages_.empty()) return boost::tuple<boost::uint16_t, rpc::BufferPtr>();

		boost::tuple<boost::uint16_t, rpc::BufferPtr> result = incoming_messages_.front();
		incoming_messages_.pop_front();
		return result;
	}

private:
	mutable boost::mutex mutex_;
	rpc::ClientService service_;
	rpc::ConnectionPtr connection_;
	bool is_connected_, have_error_;
	std::string error_message_;
	std::deque<boost::tuple<boost::uint16_t, rpc::BufferPtr> > incoming_messages_;

    void on_connect(boost::shared_ptr<rpc::Connection> const &connection_ptr)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);

		boost::system::error_code dummy_error_code;
		LOG_DEBUG("connected to " << connection_ptr->socket().remote_endpoint(dummy_error_code));

		is_connected_ = true;
		connection_ = connection_ptr;
	}

	void on_read(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::uint16_t id, rpc::Buffer &buf)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		incoming_messages_.push_back(boost::make_tuple(id, rpc::BufferPtr(new rpc::Buffer(buf))));
	}

	void on_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::system::error_code const &error)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);

		have_error_ = true;
		error_message_ = error.message();

		boost::system::error_code dummy_error_code;
		LOG_ERROR("error on " << connection_ptr->socket().remote_endpoint(dummy_error_code) << ": " << error);
	}
};

Client::Client()
{
}

Client::~Client()
{
}

void Client::connect(std::string const &host, std::string const &port)
{
	impl_.reset(new Impl());
	impl_->connect(host, port);
	impl_->start();
}

void Client::disconnect()
{
	impl_->stop();
	impl_->wait();
	impl_.reset();
}

bool Client::is_connected() const
{
	return impl_ && impl_->is_connected();
}

bool Client::have_error() const
{
	return impl_->have_error();
}

std::string Client::error_message() const
{
	return impl_->error_message();
}

#if defined(PROTOBUF_FOUND)
void Client::send(boost::uint16_t id, google::protobuf::Message const &message)
{
	return impl_->send(id, message);
}
#endif

boost::tuple<boost::uint16_t, rpc::BufferPtr> Client::receive()
{
	return impl_->receive();
}

}
