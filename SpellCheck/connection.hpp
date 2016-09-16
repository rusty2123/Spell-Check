//
// connection.hpp
// ~~~~~~~~~~~~~~
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

#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

//#define BOOST_ASIO_DISABLE_THREADS
#include <array>
#include <memory>
#include <boost/asio.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "request_dispatcher.hpp"
#include "request_parser.hpp"

namespace http {
	namespace server {

		class connection_manager;

		/// Represents a single connection from a client.
		class connection
			: public std::enable_shared_from_this<connection>
		{
		public:
			connection(const connection&) = delete;
			connection& operator=(const connection&) = delete;

			/// Construct a connection with the given socket.
			explicit connection(boost::asio::ip::tcp::socket socket,
				connection_manager& manager, request_dispatcher& dispatcher);

			/// Start the first asynchronous operation for the connection.
			void start();

			/// Stop all asynchronous operations associated with the connection.
			void stop();

		private:
			/// Perform an asynchronous read operation.
			void do_read();

			/// Perform an asynchronous write operation.
			void do_write();

			/// Socket for the connection.
			boost::asio::ip::tcp::socket socket_;

			/// The manager for this connection.
			connection_manager& connection_manager_;

			/// The handler used to process the incoming request.
			request_dispatcher& request_dispatcher_;

			/// Buffer for incoming data.
			std::array<char, 8192> buffer_;

			/// The incoming request.
			request request_;

			/// The parser for the incoming request.
			request_parser request_parser_;

			/// The reply to be sent back to the client.
			reply reply_;

			/// The body of the reply sent back to the client.
			std::string content;
		};

		typedef std::shared_ptr<connection> connection_ptr;

	} // namespace server
} // namespace http

#endif // HTTP_CONNECTION_HPP