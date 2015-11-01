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

#include "grammar.hpp"
#include <algorithm>

Grammar::Grammar(std::string axiom_)
    :axiom(axiom_), rules() {}

void Grammar::addRule(char lhs, std::string str, double prob) {
  rules[lhs].addClause(str, prob);
}

bool Grammar::hasRule(char ch) const {
  return rules.count(ch);
}

void Grammar::Rule::addClause(const std::string& str, double prob) {
  clauses.emplace_back(str);
  probs.emplace_back(prob);
}

