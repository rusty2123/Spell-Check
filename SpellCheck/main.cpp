//
// main.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016 Gabriel Foust (gfoust at harding dot edu)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <boost/asio.hpp>
#include "server.hpp"
#include "request.hpp"
#include "reply.hpp"
#include "spell.hpp"
#include "safe_queue.cpp"
#include "safe_map.cpp"
#include <thread>

safe_queue<function<void()>> work_queue;
safe_map<string, string> cache;
safe_map<string, time_t> most_recent;
vector<thread> threads;

// Handle request by doing spell check on query string
// Render results as JSON
void spellcheck_request(const http::server::request& req, http::server::reply& rep, http::server::done_callback done)
{

	// Set up reply
	rep.status = http::server::reply::status_type::ok;
	rep.headers["Content-Type"] = "application/json";

	if (cache.find(req.query))
	{
		rep.content << cache.get_value(req.query);
		most_recent.add_item(req.query, time(0));
	}
	else
	{
		most_recent.add_item(req.query, time(0));
		// Loop over spellcheck results
		bool first = true;
		rep.content << "[";
		for (auto& candidate : spell::spellcheck(std::ref(req.query))) {
			if (first) {
				first = false;
			}
			else {
				rep.content << ", ";
			}
			rep.content << "\n  { \"word\" : \"" << candidate.word << "\",  "
				<< "\"distance\" : " << candidate.distance << " }";
		}
		rep.content << "\n]";

		cache.add_item(req.query, rep.content.str());
	}
	done();
}

void run_threads()
{
	while (true)
		work_queue.dequeue()();
}

void clean_cache()
{
	while (true)
	{
		this_thread::sleep_for(60s);
		for (auto it = most_recent.begin(); it != most_recent.end(); ++it)
		{

			if (difftime(time(0), most_recent[it->first]) > 60)
			{
				cache.remove_item(it->first);
				it = most_recent.remove_item(it->first);
			}
			if (it == most_recent.end())
				break;
		}
	}
}

void make_threads()
{
	int num_threads = thread::hardware_concurrency();

	if (num_threads <= 0)
		num_threads = 2;

	for (int i = 0; i < num_threads - 2; i++)
		threads.push_back(thread(run_threads));

	threads.push_back(thread(clean_cache));
}

void cachedump_request(const http::server::request& req, http::server::reply& rep, http::server::done_callback done)
{
	rep.status = http::server::reply::status_type::ok;
	rep.headers["Content-Type"] = "application/json";

	rep.content << "[\n";
	for (auto it = most_recent.begin(); it != most_recent.end(); it++)
	{
		most_recent.lock_shared();
		rep.content << "    { \"word\" : " << "\"" << it->first << "\"" << ", "
					<< "\"" << "seconds_elapsed" << "\" : " << difftime(time(0), it->second) << " },\n";
		most_recent.unlock_shared();
	}
	rep.content << "]";
	done();
}

// Called by server whenever a request is received
// Must fill in reply, then call done()
void handle_request(const http::server::request& req, http::server::reply& rep, http::server::done_callback done)
{
	std::cout << req.method << ' ' << req.uri << std::endl;
	if (req.path == "/spell") {
		work_queue.enqueue( [&req, &rep, done] () { spellcheck_request(std::ref(req), std::ref(rep), done); });
	}
	else if (req.path == "/cachedump") {
		cachedump_request(req, rep, done);
	}
	else {
		rep = http::server::reply::stock_reply(http::server::reply::not_found);
		done();
	}
}

// Initialize and run server
int main()
{
	try
	{
		make_threads();
		std::cout << "Listening on port 8000..." << std::endl;
		http::server::server s("localhost", "8000", handle_request);
		s.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}

	for (auto& t : threads)
	{
		if (t.joinable())
			t.join();
	}

	return 0;
}