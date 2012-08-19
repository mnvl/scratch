
#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>
#include <iosfwd>
#include <rpc/header.h>
#include <rpc/handler.h>
#include <rpc/connection.h>
#include <rpc/server_service.h>
#include <rpc/client_service.h>

namespace
{

char const HOST[] = "127.0.0.1";
char const SERVICE[] = "17171";

static size_t const MESSAGE_SIZE[] = {
	0,
	1,
	41,
	2650,
	4091,
	4092,
	4093,
	4095,
	4096,
	4097,
	20123,
	30132,
	40213,
	50312,
	65530,
	65531
};

struct Server: public rpc::Handler
{
	rpc::ServerService service;
	bool completed;

	Server():
		service(this),
		completed(false)
	{
		service.start(HOST, SERVICE, 4);
	}

	~Server()
	{
		service.stop();
		service.wait();
	}

	void on_connect(boost::shared_ptr<rpc::Connection> const &connection_ptr)
	{
	}

	void on_read(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::uint16_t id, rpc::Buffer &in)
	{
		BOOST_REQUIRE(id >= 0 && id < sizeof(MESSAGE_SIZE) / sizeof(MESSAGE_SIZE[0]));
		BOOST_REQUIRE(in.read_bytes_available() == MESSAGE_SIZE[id]);

		std::vector<char> message(in.read_bytes_available());
		if (message.size()) in.read(&message[0], message.size());
		for (size_t j = 0; j < message.size(); j++) BOOST_REQUIRE(message[j] == (char) j);

		boost::shared_ptr<rpc::Buffer> out(new rpc::Buffer());

		{
			rpc::Header h;
			h.id = id;
			h.len = message.size();
			out->write(&h, sizeof(h));
		}

		if (message.size()) out->write(&message[0], message.size());

		connection_ptr->write(out);

		completed = true;
	}

	void on_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::system::error_code const &error)
	{
		BOOST_REQUIRE (completed);
	}
};

struct Client: public rpc::Handler
{
	rpc::ClientService service;
	int index_;
	bool completed;

	Client():
		service(this),
		index_(0),
		completed(false)
	{
	}

	~Client()
	{
		service.stop();
		service.wait();
	}

	void go()
	{
		service.connect(HOST, SERVICE);
		service.start(4);
	}

	void on_connect(boost::shared_ptr<rpc::Connection> const &connection_ptr)
	{
		send(connection_ptr);
	}

	void send(boost::shared_ptr<rpc::Connection> const &connection_ptr)
	{
		boost::shared_ptr<rpc::Buffer> buffer(new rpc::Buffer());

		std::vector<char> message(MESSAGE_SIZE[index_]);
		for (size_t j = 0; j < message.size(); j++) message[j] = (char) j;

		{
			rpc::Header h;
			h.id = index_;
			h.len = message.size();
			buffer->write(&h, sizeof(h));
		}

		if (!message.empty()) buffer->write(&message[0], message.size());

		connection_ptr->write(buffer);
	}

	void on_read(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::uint16_t id, rpc::Buffer &in)
	{
		BOOST_REQUIRE (id == index_);
		BOOST_REQUIRE (in.read_bytes_available() == MESSAGE_SIZE[index_]);

		std::vector<char> message(MESSAGE_SIZE[index_]);
		if (!message.empty()) in.read(&message[0], message.size());

		for (size_t j = 0; j < message.size(); j++) BOOST_REQUIRE (message[j] == (char) j);

		if (++index_ < sizeof(MESSAGE_SIZE) / sizeof(MESSAGE_SIZE[0]))
		{
			send(connection_ptr);
		}
		else
		{
			connection_ptr->socket().close();
			completed = true;
		}
	}

	void on_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::system::error_code const &error)
	{
		BOOST_REQUIRE (completed);
	}
};

}

BOOST_AUTO_TEST_SUITE(test_service)

BOOST_AUTO_TEST_CASE(test_echo)
{
	Server server;
	Client client;

	client.go();

	client.service.wait();

	server.service.stop();
	server.service.wait();
}

BOOST_AUTO_TEST_SUITE_END()
