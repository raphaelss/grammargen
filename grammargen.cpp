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

#include <iostream>
#include <string>
#include <stdexcept>
#include "grammar.hpp"

struct op {
  template <class T>
  void operator()(T x) {
    std::cout << x;
  }
};

int main(int argc, char **argv) {
  int n;
  if (argc < 3 || argc % 3 != 0 || (n = std::stoi(argv[1])) < 0) {
    std::cerr << "Usage: " << argv[0]
              << " axiom number_of_iterations [rules]\n"
              << "       rule = probability char string\n";
    return 1;
  }
  Grammar g(argv[2]);
  argc -= 3;
  argv += 3;
  double prob;
  while (argc) {
    try {
      prob = std::stod(argv[0]);
    } catch (const std::invalid_argument &e) {
      std::cerr << "grammargen error: "
                << argv[0] << " is not a positive number\n";
      return 1;
    }
    if (prob <= 0) {
      std::cerr << "grammargen error: "
                << argv[0] << " is not a positive number\n";
      return 1;
    }
    g.addRule(argv[1][0], argv[2], prob);
    argc -= 3;
    argv += 3;
  }
  gen_seq<op>(g, n, g.axiom);
  std::cout << std::endl;
  return 0;
}
