
#include <boost/bind.hpp>
#include "basic_service.h"

namespace rpc
{

BasicService::BasicService(Handler *handler):
    handler_(handler)
{
}

BasicService::~BasicService()
{
}

void BasicService::start(size_t nthreads)
{
	for (size_t i = 0; i < nthreads; ++i)
	{
		threads_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
	}
}

void BasicService::stop()
{
	io_service_.stop();
}

void BasicService::wait()
{
	threads_.join_all();
}

}
