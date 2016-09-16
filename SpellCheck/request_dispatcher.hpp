//
// request_dispatcher.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016 Gabriel Foust (gfoust at harding dot edu)
//
// Derived from sources found at
// http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/examples/cpp11_examples.html
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_DISPATCHER_HPP
#define HTTP_REQUEST_DISPATCHER_HPP

#include <string>
#include "request_handler.hpp"

namespace http {
	namespace server {

		struct reply;
		struct request;

		/// The common handler for all incoming requests.
		class request_dispatcher
		{
		public:
			request_dispatcher(const request_dispatcher&) = delete;
			request_dispatcher& operator=(const request_dispatcher&) = delete;

			/// Construct with a directory containing files to be served.
			explicit request_dispatcher(request_handler handler);

			/// Handle a request and produce a reply.
			void handle_request(request& req, reply& rep, done_callback done);

		private:

			/// Handle the request and generate the reply.
			request_handler request_handler_;

			/// Perform URL-decoding on a string. Returns false if the encoding was
			/// invalid.
			static bool url_decode(request& req);
		};

	} // namespace server
} // namespace http

#endif // HTTP_REQUEST_DISPATCHER_HPP