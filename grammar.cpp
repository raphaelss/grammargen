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



