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

#ifndef GRAMMAR_HPP_INCLUDED
#define GRAMMAR_HPP_INCLUDED
#include <vector>
#include <string>
#include <unordered_map>
#include <random>

class Grammar
{
public:
  Grammar(const std::string& axiom_);
  void addRule(int lhs, std::string str, double prob=1);
  const std::string& operator()(int ch);
  bool hasRule(int ch) const;

  std::string axiom;

private:
  struct Rule
  {
    void addClause(const std::string& str, double prob=1);
    const std::string& choose(
      std::discrete_distribution<unsigned>& distr,
      std::mt19937_64& rndGen);
    std::vector<std::string> clauses;
    std::vector<double> probs;
  };

  std::unordered_map<int, Rule> rules;
  std::mt19937_64 rndGen;
  std::discrete_distribution<unsigned> distr;
};

#endif // GRAMMAR_H
