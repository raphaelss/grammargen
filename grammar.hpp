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

#ifndef GRAMMAR_HPP_INCLUDED
#define GRAMMAR_HPP_INCLUDED
#include <vector>
#include <string>
#include <unordered_map>
#include <random>

class Grammar {
public:
  Grammar(std::string axiom_);
  void addRule(char lhs, std::string str, double prob=1);

  template <class RndGen>
  const std::string& operator()(char ch, RndGen &rndgen) {
    return rules.at(ch).choose(distr, rndgen);
  }

  bool hasRule(char ch) const;

  std::string axiom;

private:
  struct Rule {
    void addClause(const std::string& str, double prob=1);

    template <class RndGen>
    const std::string& choose(
        std::discrete_distribution<unsigned>& distr, RndGen& rndgen)
    {
      if (clauses.size() == 1) {
        return clauses[0];
      }
      distr.param(
          std::discrete_distribution<unsigned>::param_type(probs.cbegin(),
                                                           probs.cend()));
      return clauses[distr(rndgen)];
    }

    std::vector<std::string> clauses;
    std::vector<double> probs;
  };

  std::unordered_map<char, Rule> rules;
  std::discrete_distribution<unsigned> distr;
};

template <class Op, class RndGen>
void gen_seq(Grammar &g, unsigned k, const std::string &str,
             RndGen &rndgen, Op op = Op())
{
  if (k) {
    for(const auto& ch : str) {
      if(g.hasRule(ch)) {
        gen_seq(g, k-1, g(ch, rndgen), rndgen, op);
      } else {
        op(ch);
      }
    }
  } else {
    op(str);
  }
}

template <class Op, class RndGen>
void gen_seq(Grammar &g, unsigned k, RndGen &rndgen, Op op = Op()) {
  gen_seq(g, k, g.axiom, rndgen, op);
}

#endif
