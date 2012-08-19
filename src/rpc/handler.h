#pragma once

#include <iosfwd>
#include <boost/cstdint.hpp>
#include <boost/system/error_code.hpp>
#include <boost/shared_ptr.hpp>
#include "buffer.h"

namespace rpc
{

class Connection;

class Handler
{
public:
    struct CloseConnectionException {};

    virtual void on_connect(boost::shared_ptr<Connection> const &connection_ptr) = 0;
	virtual void on_read(boost::shared_ptr<Connection> const &connection_ptr, boost::uint16_t id, Buffer &in) = 0;
	virtual void on_error(boost::shared_ptr<Connection> const &connection_ptr, boost::system::error_code const &error) = 0;
};

}
