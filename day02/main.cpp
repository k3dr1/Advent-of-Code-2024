#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>
#include <ostream>

using i64 = std::int64_t;

auto is_safe(auto&& diffs) -> bool {
    return std::ranges::fold_left(diffs, true,
               [](auto&& l, auto&& r) { return l && (0 < r && r < 4); })
        || std::ranges::fold_left(diffs, true,
            [](auto&& l, auto&& r) { return l && (-4 < r && r < 0); });
}

auto is_safe2(auto&& diffs) -> bool
{
    auto res { false };
    for (const auto missing : std::views::iota(0uz, (size_t)std::ranges::distance(diffs))) {
        auto r = std::views::enumerate(diffs)
            | std::views::drop_while(
                [&missing](auto&& p) {
                    auto&& [i, _] = p;
                    return i == missing;
                })
            | std::views::transform(
                [](auto&& p) {
                    auto&& [_, v] = p;
                    return v;
                });
        res |= is_safe(r);
    }
    return res;
}

auto main() -> int {
    i64 answer1{0};
    i64 answer2{0};
    auto f = std::fstream{"input.txt"};
    std::string line{};
    std::vector<std::string> lines{};

    while (std::getline(f, line)) {
        lines.emplace_back(line);
    }

    for (auto line : lines) {
        auto diffs = line
            | std::views::split(' ')
            | std::ranges::views::transform(
                [](const auto& s) { return std::string(&*s.begin(), std::ranges::distance(s)); })
            | std::views::transform(
                [](const auto& e) { return std::atoll(e.c_str()); })
            | std::views::adjacent<2>
            | std::views::transform(
                [](const auto& p) {
                  const auto& [l, r] = p;
                  return l - r; });
        // 1 2 6 7 8
        //  1 4 1 1
        //
        // 0 1 5 6
        //  1 4 1
        answer1 += is_safe(diffs);
        answer2 += is_safe2(diffs);
    }

    std::cout << "Answer1: " << answer1 << '\n';
    std::cout << "Answer2: " << answer2 << '\n';

    return 0;
}
