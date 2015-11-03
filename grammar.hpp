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

template <class T>
class Grammar {
public:
  Grammar(T axiom): _axiom(axiom), rules() {}

  void add_rule(T lhs, std::vector<T> rhs, double prob) {
    rules[std::move(lhs)].addClause(std::move(rhs), prob);
  }

  template <class Rndgen>
  const std::vector<T>& operator()(char ch, Rndgen &rndgen) {
    return rules.at(ch).choose(distr, rndgen);
  }

  bool has_rule(const T &x) const {
    return rules.count(x);
  }

  template <class Rndgen, class Op>
  void gen_seq(unsigned k, Rndgen &rndgen, Op op = Op()) {
    T *axiom_ptr = &_axiom;
    gen_seq(k, axiom_ptr, axiom_ptr + 1, rndgen, op);
  }

  template <class Iter, class Rndgen, class Op>
  void gen_seq(unsigned k, Iter begin, Iter end, Rndgen &rndgen, Op op = Op())
{
  if (k) {
    for (; begin != end; ++begin) {
      if (has_rule(*begin)) {
        const auto &vec = operator()(*begin, rndgen);
        gen_seq(k-1, vec.begin(), vec.end(), rndgen, op);
      } else {
        op(*begin);
      }
    }
  } else {
    for (; begin != end; ++begin) {
      op(*begin);
    }
  }
}

private:
  struct Rule {
    void addClause(std::vector<T> str, double prob=1) {
      clauses.emplace_back(std::move(str));
      probs.emplace_back(prob);
    }

    template <class Rndgen>
    const std::vector<T>& choose(
        std::discrete_distribution<unsigned>& distr, Rndgen& rndgen)
    {
      if (clauses.size() == 1) {
        return clauses[0];
      }
      distr.param(
          std::discrete_distribution<unsigned>::param_type(probs.cbegin(),
                                                           probs.cend()));
      return clauses[distr(rndgen)];
    }

    std::vector<std::vector<T>> clauses;
    std::vector<double> probs;
  };

  T _axiom;

  std::unordered_map<char, Rule> rules;
  std::discrete_distribution<unsigned> distr;
};

#endif
