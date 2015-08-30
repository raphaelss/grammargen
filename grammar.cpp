/*
 *  This file is parte of grammargen. A tool to generate sequences of
 *  characters from a given grammar.
 *  Copyright (C) 2014 Raphael Santos, http://www.raphaelss.com
 *
 *   This program is free software: you can redistribute it and/or modify
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

Grammar::Grammar(const std::string& axiom_)
  :axiom(axiom_), rules(), rndGen(std::random_device()()) {}

void Grammar::addRule(int lhs, std::string str, double prob)
{
  rules[lhs].addClause(str, prob);
}

bool Grammar::hasRule(int ch) const
{
  return rules.count(ch);
}

const std::string& Grammar::operator()(int ch)
{
  return rules.at(ch).choose(distr, rndGen);
}

void Grammar::Rule::addClause(const std::string& str, double prob)
{
  clauses.emplace_back(str);
  probs.emplace_back(prob);
}

const std::string& Grammar::Rule::choose(
  std::discrete_distribution<unsigned>& distr,
  std::mt19937_64& rndGen)
{
  if(clauses.size() == 1) {
    return clauses[0];
  }
  distr.param(std::discrete_distribution<unsigned>::param_type(
    probs.cbegin(), probs.cend())
  );
  return clauses[distr(rndGen)];
}



