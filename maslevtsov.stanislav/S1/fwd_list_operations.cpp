#include "fwd_list_operations.hpp"
#include <limits>
#include <ostream>
#include <fwd_list/definition.hpp>

namespace {
  unsigned long long checked_sum(unsigned long long a, unsigned long long b)
  {
    const unsigned long long max_ull = std::numeric_limits< unsigned long long >::max();
    if (max_ull - a > b) {
      return a + b;
    }
    throw std::overflow_error("overflow");
  }

  void print_list_elements(std::ostream& out, const maslevtsov::list_t& list) noexcept
  {
    if (!list.empty()) {
      out << *list.cbegin();
      for (auto j = ++list.cbegin(); j != list.cend(); ++j) {
        out << ' ' << *j;
      }
    }
  }

  void get_column_elements(maslevtsov::list_t& list, const maslevtsov::pairs_list_t& pairs_list, std::size_t border)
  {
    maslevtsov::list_t cur_list = pairs_list.cbegin()->second;
    if (cur_list.size() > border) {
      auto it = cur_list.cbegin();
      for (std::size_t k = 0; k != border; ++k, ++it)
      {}
      list.push_back(*it);
    }
    for (auto j = ++pairs_list.cbegin(); j != pairs_list.cend(); ++j) {
      maslevtsov::list_t cur_list = j->second;
      if (cur_list.size() > border) {
        auto it = cur_list.cbegin();
        for (std::size_t k = 0; k != border; ++k, ++it)
        {}
        list.push_back(*it);
      }
    }
  }

  std::size_t get_cur_sum(const maslevtsov::pairs_list_t& pairs_list, std::size_t border)
  {
    std::size_t result = 0;
    maslevtsov::list_t cur_list = pairs_list.cbegin()->second;
    if (cur_list.size() > border) {
      auto it = cur_list.cbegin();
      for (std::size_t k = 0; k != border; ++k, ++it)
      {}
      result = checked_sum(result, *it);
    }
    for (auto j = ++pairs_list.cbegin(); j != pairs_list.cend(); ++j) {
      maslevtsov::list_t cur_list = j->second;
      if (cur_list.size() > border) {
        auto it = cur_list.cbegin();
        for (std::size_t k = 0; k != border; ++k, ++it)
        {}
        result = checked_sum(result, *it);
      }
    }
    return result;
  }
}

std::size_t maslevtsov::get_max_pairs_list_size(const pairs_list_t& list) noexcept
{
  std::size_t maximum = list.cbegin()->second.size();
  for (auto i = ++list.cbegin(); i != list.cend(); ++i) {
    maximum = std::max(maximum, i->second.size());
  }
  return maximum;
}

void maslevtsov::get_lists_sums(list_t& sums, const pairs_list_t& pairs_list)
{
  std::size_t max_pairs_list_size = get_max_pairs_list_size(pairs_list);
  for (std::size_t i = 0; i != max_pairs_list_size; ++i) {
    std::size_t cur_sum = get_cur_sum(pairs_list, i);
    sums.push_back(cur_sum);
  }
}

void maslevtsov::print_lists_info(std::ostream& out, const pairs_list_t& pairs_list)
{
  std::size_t max_pairs_list_size = get_max_pairs_list_size(pairs_list);
  for (std::size_t i = 0; i != max_pairs_list_size; ++i) {
    list_t column_elements;
    get_column_elements(column_elements, pairs_list, i);
    if (!column_elements.empty()) {
      print_list_elements(out, column_elements);
      out << '\n';
    }
  }
  list_t sums;
  get_lists_sums(sums, pairs_list);
  if (sums.empty()) {
    out << 0;
    return;
  }
  print_list_elements(out, sums);
}
