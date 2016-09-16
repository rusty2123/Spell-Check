//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016 Gabriel Foust (gfoust at harding dot edu)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP
#include <functional>
#include "request.hpp"
#include "reply.hpp"

namespace http {
	namespace server {

		typedef std::function<void()> done_callback;

		typedef std::function<void(const request&, reply&, done_callback)> request_handler;

	}
}

#endif