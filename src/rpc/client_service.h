#pragma once

#include <boost/enable_shared_from_this.hpp>
#include "basic_service.h"

namespace rpc
{

class Connection;

class ClientService:
		public BasicService,
		public boost::enable_shared_from_this<ClientService>
{
public:
	ClientService(Handler *handler);
	~ClientService();

	void connect(std::string const &host, std::string const &port);

private:
	void handle_connect(const boost::system::error_code& error, boost::shared_ptr<Connection> const &connection,
		boost::asio::ip::tcp::resolver::iterator endpoint_it);
};

}
