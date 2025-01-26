#include <concepts>
#include <ranges>
#include <set>
#include <type_traits>
#include <vector>
template <typename Coll>
concept SupportsPushBack = requires(Coll &coll, typename Coll::value_type val) {
  coll.push_back(val);
};

template <typename From, typename To>
concept ConvertsWithoutNarrowing =
    std::convertible_to<From, To> && requires(From &&x) {
      {
        std::type_identity_t<To[]>{std::forward<From>(x)}
      } -> std::same_as<To[1]>;
    };

template <typename Coll, typename T>
  requires ConvertsWithoutNarrowing<T, typename Coll::value_type>
void add(Coll &coll, const T &val) {
  if constexpr (SupportsPushBack<Coll>) {
    coll.push_back(val);
  } else {
    coll.insert(val);
  }
}
template <typename Coll, std::ranges::input_range T>
  requires ConvertsWithoutNarrowing<std::ranges::range_value_t<T>,
                                    typename Coll::value_type>
void add(Coll &coll, const T &val) {
  if constexpr (SupportsPushBack<Coll>) {
    coll.insert(coll.end(), std::ranges::begin(val), std::ranges::end(val));
  } else {
    coll.insert(std::ranges::begin(val), std::ranges::end(val));
  }
}

// 接受布尔条件的类型特性，若为 false 则产生无效代码。
// template <typename Coll, typename T,
//           typename = std::enable_if_t<!std::is_floating_point_v<T>>>
// void add(Coll &coll, const T &val) {
//   coll.push_back(val);
// }

int main() {
  std::vector<int> iVec;
  add(iVec, 42);

  std::set<int> iSet;
  add(iSet, 42);

  short s = 42;
  add(iVec, s);

  [[maybe_unused]]
  long long ll = 42;
  // add(iVec, ll);

  std::vector<double> dVec;
  add(dVec, 42.0);
  add(dVec, .1f);

  add(iVec, iSet);
  add(iSet, iVec);

  int Vals[] = {1, 2, 3};
  add(iVec, Vals);
}
