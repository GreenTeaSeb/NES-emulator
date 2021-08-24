#ifndef BITOPS_H
#define BITOPS_H

#endif // BITOPS_H
#include <bit>
#include <bitset>
#include <cstdint>

template<std::size_t N>
void
bit_set(std::bitset<N>& num, auto start, auto end, auto bits)
{
  for (int n = 0; start <= end; start++, n++) {
    auto bit = (bits >> n) & 1U;
    num[start] = bit;
  }
}
template<std::size_t N, std::size_t M>
void
bit_set(std::bitset<N>& num,
        auto start,
        auto end,
        std::bitset<N>& data,
        auto start2,
        auto end2)
{
  for (; start <= end && start2 <= end2; start++, start2++) {
    num[start] = data[start2];
  }
}

auto
bit_set(auto num, auto data, uint16_t mask)
{
  auto pos = mask & 1 ? std::countr_one(mask) : std::countl_zero(mask);
  return (data << pos) | (num & mask);
}

auto
bit_replace_all(auto num, auto data, uint16_t mask)
{
  return (data) | (num & mask);
}
