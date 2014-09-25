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
