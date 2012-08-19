#pragma once

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#if defined(PROTOBUF_FOUND)
#include <google/protobuf/message.h>
#endif

#include "buffer.h"

namespace rpc
{

class Client: public boost::noncopyable
{
public:
	Client();
	~Client();

	void connect(std::string const &host, std::string const &port);
	void disconnect();

	bool is_connected() const;
	bool have_error() const;
	std::string error_message() const;

#if defined(PROTOBUF_FOUND)
	void send(boost::uint16_t id, google::protobuf::Message const &message);
#endif

	boost::tuple<boost::uint16_t, rpc::BufferPtr> receive();

private:
	class Impl;
	boost::scoped_ptr<Impl> impl_;
};

}
