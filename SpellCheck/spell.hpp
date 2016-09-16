//
// spell.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016 Gabriel Foust (gfoust at harding dot edu)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SPELL_SPELL_HPP
#define SPELL_SPELL_HPP
#include <iostream>
#include <string>
#include <vector>

namespace spell {

	struct candidate {
		std::string word;
		unsigned distance;
	};

	std::vector<candidate> spellcheck(const std::string& word);
}

#endif