/*
 *  This file is parte of grammargen. A tool to generate sequences of
 *  characters from a given grammar.
 *  Copyright (C) 2015 Raphael Santos, http://www.raphaelss.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdio>
#include <random>
#include <stdexcept>
#include <string>
#include "grammar.hpp"

struct op {
  void operator()(char c) {
    std::putchar(c);
  }

  void operator()(const std::string &c) {
    std::fwrite(c.data(), c.size(), sizeof(std::string::value_type), stdout);
  }
};

int main(int argc, char **argv) {
  int n;
  if (argc < 3 || argc % 3 != 0 || (n = std::stoi(argv[1])) < 0) {
    std::fprintf(stderr, "Usage: %s number_of_iterations axiom [rules]\n"
                         "       rule = probability char string\n", argv[0]);
    return 1;
  }
  Grammar g(argv[2]);
  argc -= 3;
  argv += 3;
  double prob;
  while (argc) {
    try {
      if ((prob = std::stod(argv[0])) <= 0) {
        throw std::invalid_argument("not a positive number");
      }
    } catch (const std::invalid_argument &e) {
      std::fprintf(stderr, "grammargen error: %s is not a positive number\n",
                   argv[0]);
      return 1;
    }
    g.addRule(argv[1][0], argv[2], prob);
    argc -= 3;
    argv += 3;
  }
  std::random_device rd{};
  std::mt19937_64 rndgen{rd()};
  gen_seq<op>(g, n, g.axiom, rndgen);
  putchar('\n');
  return 0;
}
