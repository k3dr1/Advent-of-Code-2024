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

auto tuple_difference = [](auto&& p) { const auto& [l, r] = p; return l - r; };
auto is_safe(auto&& diffs) -> bool {
    return std::ranges::fold_left(diffs, true, [](const auto& l, const auto& r) { return l && (0 < r && r < 4); })
        || std::ranges::fold_left(diffs, true, [](const auto& l, const auto& r) { return l && (-4 < r && r < 0); });
}
auto is_safe2(auto&& diffs) -> bool {
    auto res { false };
    for (const auto missing : std::views::iota(0z, std::ranges::distance(diffs))) {
        res |= is_safe(std::views::enumerate(diffs)
            | std::views::filter([missing](const auto& p) -> bool { const auto& [i, v] = p; return { i != missing }; })
            | std::views::transform([](const auto& p) { const auto& [_, v] = p; return v; })
            | std::views::adjacent<2>
            | std::views::transform(tuple_difference));
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
        auto nums = line | std::views::split(' ')
            | std::views::transform( [](const auto& s) { return std::string(&*s.begin(), std::ranges::distance(s)); })
            | std::views::transform( [](const auto& e) { return std::atoll(e.c_str()); });
        auto diffs = nums | std::views::adjacent<2> | std::views::transform(tuple_difference);
        answer1 += is_safe(diffs);
        answer2 += is_safe2(nums);
    }
    std::cout << "Answer1: " << answer1 << '\n';
    std::cout << "Answer2: " << answer2 << '\n';
    return 0;
}
