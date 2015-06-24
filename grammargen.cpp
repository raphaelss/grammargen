#include <iostream>
#include <string>
#include <stdexcept>
#include "grammar.hpp"

void gen_seq(Grammar &g, unsigned k, const std::string& str);

int main(int argc, char **argv) {
  int n;
  if (argc < 3 || argc % 3 != 0 || (n = std::stoi(argv[1])) < 0) {
    std::cerr << "Usage: " << argv[0]
              << " number_of_iterations [rules]\n"
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
  gen_seq(g, n, g.axiom);
  std::cout << std::endl;
  return 0;
}

void gen_seq(Grammar &g, unsigned k, const std::string& str)
{
  if (k) {
    for(const auto& ch : str) {
      if(g.hasRule(ch)) {
        gen_seq(g, k-1, g(ch));
      } else {
        std::cout << ch;
      }
    }
  } else {
    std::cout << str;
  }
}
