//
// reply.hpp
// ~~~~~~~~~
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

#ifndef HTTP_REPLY_HPP
#define HTTP_REPLY_HPP

//#define BOOST_ASIO_DISABLE_THREADS
#include <string>
#include <sstream>
#include <vector>
#include <boost/asio.hpp>

namespace http {
	namespace server {

		/// A reply to be sent to a client.
		struct reply
		{
			/// The status of the reply.
			enum status_type
			{
				ok = 200,
				created = 201,
				accepted = 202,
				no_content = 204,
				multiple_choices = 300,
				moved_permanently = 301,
				moved_temporarily = 302,
				not_modified = 304,
				bad_request = 400,
				unauthorized = 401,
				forbidden = 403,
				not_found = 404,
				internal_server_error = 500,
				not_implemented = 501,
				bad_gateway = 502,
				service_unavailable = 503
			} status;

			/// The headers to be included in the reply.
			std::map<std::string, std::string> headers;

			/// The content to be sent in the reply.
			std::stringstream content;

			/// Convert the reply into a vector of buffers. The buffers do not own the
			/// underlying memory blocks, therefore the reply object must remain valid and
			/// not be changed until the write operation has completed.
			std::vector<boost::asio::const_buffer> to_buffers(std::string& content_str);

			/// Get a stock reply.
			static reply stock_reply(status_type status);
		};

	} // namespace server
} // namespace http

#endif // HTTP_REPLY_HPP