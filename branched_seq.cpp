#include <iostream>
#include <memory>
#include <vector>
#include <boost/variant.hpp>

template <class T>
using branched_seq = std::vector<
  typename boost::make_recursive_variant<
    T,
    std::vector<boost::recursive_variant_>
  >::type
>;

template <class T>
struct print {
  print(): level(0) {}

  void operator()(const T &x) {
    for (unsigned i = 1; i < level; ++i)
      std::cout << '\t';
    std::cout << x << std::endl;
  }

  void operator()(const branched_seq<T> &vec) {
    ++level;
    for (const auto &x : vec) {
      boost::apply_visitor(*this, x);
    }
    --level;
  }

  unsigned level;
};

int main() {
  branched_seq<int> bs {1, 2, 3, 4};
  bs.push_back(5);
  bs.push_back(bs);
  bs.push_back(6);
  bs.push_back(7);
  bs.push_back(bs);
  bs.push_back(8);
  bs.push_back(9);
  bs.push_back(bs);
  bs.push_back(10);
  print<int> p {};
  p(bs);
  return 0;
}
