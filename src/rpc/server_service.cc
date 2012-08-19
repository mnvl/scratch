
#include <boost/bind.hpp>
#include "connection.h"
#include "server_service.h"

#include <util/logger.h>

namespace rpc
{

ServerService::ServerService(Handler *handler):
	BasicService(handler),
    acceptor_(get_io_service())
{
}

ServerService::~ServerService()
{
}

void ServerService::start(std::string const &host_name, std::string const &service_name, size_t nthreads)
{
	boost::asio::ip::tcp::resolver resolver(get_io_service());
	boost::asio::ip::tcp::resolver::query query(host_name, service_name);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();

	schedule_accept();

    BasicService::start(nthreads);

	LOG_INFO("service on host " << host_name << ":" << service_name << " up and running in "
		<< nthreads << " threads");
}

void ServerService::schedule_accept()
{
	ConnectionPtr connection_ptr(new Connection(get_handler(), get_io_service()));

   	acceptor_.async_accept(connection_ptr->socket(),
		boost::bind(&ServerService::handle_accept, this, connection_ptr, boost::asio::placeholders::error));
}

void ServerService::handle_accept(ConnectionPtr const &connection_ptr, boost::system::error_code const &error)
{
	schedule_accept();
	connection_ptr->start();
}

}
