#pragma once

#include <boost/enable_shared_from_this.hpp>
#include "basic_service.h"

namespace rpc
{

class Connection;

class ServerService:
		public BasicService,
		public boost::enable_shared_from_this<ServerService>
{
public:
	ServerService(Handler *handler);
	~ServerService();

	void start(std::string const &host_name, std::string const &service_name, size_t nthreads);

private:
	boost::asio::ip::tcp::acceptor acceptor_;

	void schedule_accept();
	void handle_accept(boost::shared_ptr<Connection> const &connection_ptr,
		boost::system::error_code const &error);
};

}
