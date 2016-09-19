//
// spell.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2016 Gabriel Foust (gfoust at harding dot edu)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include "spell.hpp"

namespace spell {

	class WordList : public std::vector<std::string> {
	public:
		WordList() {
			std::ifstream fin("wordlist.txt");
			clear();

			std::string word;
			while (std::getline(fin, word)) {
				if (word.size() > 0 && word[0] != '#' && word.find('\'') == std::string::npos) {
					push_back(word);
				}
			}

			std::cout << "Loaded " << size() << " words" << std::endl;
		}

	};
	
	const WordList wordlist;

	class DistanceTable {
		int data_size, row_size;
		std::unique_ptr<int> data;

	public:
		DistanceTable() : data_size(0), row_size(0) { }

		int* operator[](int row) {
			return data.get() + row * row_size;
		}

		void resize(int rows, int cols) {
			if (rows * cols > data_size) {
				data_size = rows * cols;
				data.reset(new int[data_size]);
			}
			row_size = cols;
		}
	};

	thread_local DistanceTable table;

	unsigned string_distance(const std::string& a, const std::string& b) {
		table.resize(a.size() + 1, b.size() + 1);

		for (unsigned j = 0; j <= b.size(); ++j) {
			table[0][j] = j;
		}

		for (unsigned i = 1; i <= a.size(); ++i) {
			table[i][0] = i;
			for (unsigned j = 1; j <= b.size(); ++j) {
				table[i][j] = std::min({
					table[i - 1][j] + 1,
					table[i][j - 1] + 1,
					table[i - 1][j - 1] + (a[i - 1] == b[j - 1] ? 0 : 1)
				});
			}
		}

		return table[a.size()][b.size()];
	}

	inline bool operator <(const candidate& a, const candidate& b) {
		return a.distance == b.distance ? a.word > b.word : a.distance < b.distance;
	}

	std::vector<candidate> spellcheck(const std::string& word) {
		std::priority_queue<candidate> candidates;
		for (const std::string& possibility : wordlist) {
			candidate c = { possibility, string_distance(word, possibility) };
			candidates.push(c);
			if (candidates.size() > 20) {
				candidates.pop();
			}
		}

		std::vector<candidate> list(candidates.size());
		int i = candidates.size() - 1;
		while (candidates.size() > 0) {
			list[i--] = std::move(candidates.top());
			candidates.pop();
		}
		return list;
	}
}