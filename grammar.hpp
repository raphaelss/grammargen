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

#include <random>
#include <unordered_map>
#include <vector>

#include "branched_seq.hpp"

namespace impl {

template <class T>
constexpr const T &get(const T &x) {
  return x;
}

template <class T>
const T &get(const typename branched_seq<T>::value_type &x) {
  return boost::get<T>(x);
}

}

template <class T>
class Grammar {
public:
  Grammar(T axiom): _axiom(axiom), rules() {}

  void add_rule(T lhs, branched_seq<T> rhs, double prob) {
    rules[std::move(lhs)].addClause(std::move(rhs), prob);
  }

  template <class Rndgen>
  const branched_seq<T>& operator()(const T &ch, Rndgen &rndgen) {
    return rules.at(ch).choose(distr, rndgen);
  }

  bool has_rule(const T &x) const {
    return rules.count(x);
  }

  template <class Rndgen, class Op>
  void apply_rule(unsigned k, const T &x, Rndgen &rndgen, Op &op) {
    if (has_rule(x)) {
      const auto &seq = operator()(x, rndgen);
      if (--k) {
        for (const auto &elem : seq) {
          apply_rule(k, impl::get<T>(elem), rndgen, op);
        }
      } else {
        for (const auto &elem : seq) {
          op(impl::get<T>(elem));
        }
      }
    } else {
      op(x);
    }
  }

  template <class Rndgen, class Op>
  void gen_seq(unsigned k, Rndgen &rndgen, Op op = Op()) {
    if (k) {
      apply_rule(k, _axiom, rndgen, op);
      return;
    }
    op(_axiom);
  }

private:
  struct Rule {
    void addClause(branched_seq<T> str, double prob=1) {
      clauses.emplace_back(std::move(str));
      probs.emplace_back(prob);
    }

    template <class Rndgen>
    const branched_seq<T>& choose(
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

    std::vector<branched_seq<T>> clauses;
    std::vector<double> probs;
  };

  T _axiom;

  std::unordered_map<T, Rule> rules;
  std::discrete_distribution<unsigned> distr;
};

#endif
