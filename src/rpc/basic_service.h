#pragma once

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <util/logger.h>
#include "handler.h"

namespace rpc
{

class BasicService:
		public boost::noncopyable
{
public:
	BasicService(Handler *handler);
	virtual ~BasicService();

    void start(size_t nthreads);
	void stop();
	void wait();

    Handler *get_handler() const { return handler_; }
    boost::asio::io_service &get_io_service() { return io_service_; }
    boost::asio::io_service const &get_io_service() const { return io_service_; }

private:
	boost::asio::io_service io_service_;
	boost::thread_group threads_;
	Handler *handler_;
};

}
