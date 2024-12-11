#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <functional>
#include <ranges>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <execution>

using i64 = std::int64_t;
using u64 = std::int64_t;

struct hash {
    auto operator()(const std::pair<i64,i64>& p) const {
        return ((((u64)p.first) << (32UL)) | ((u64)p.second)); 
    }
};

std::unordered_map<std::pair<i64,i64>,i64, hash> memo{};
auto hash = [](const auto& y, const auto& x) { return ((((u64)y) << (32UL)) | ((u64)x)); };

auto breakdown(i64 x, i64 blinks) -> i64 {
    if (blinks == 0) {
        return 1;
    }
    else {
        if (memo.contains(std::make_pair(x, blinks))) {
            return memo.at(std::make_pair(x, blinks));
        }
        if (x == 0) return breakdown(1, blinks-1);
        const auto nd = i64(std::log10l(x)) + 1;
        if (nd%2 == 0) {
            auto pow = 1;
            for (auto j = nd/2; j; j--) pow*=10;
            std::array<i64, 2> xs{x/(pow), x%(pow)};
            std::array<i64, 2> rs{0,0};
            std::transform(std::execution::seq, xs.begin(), xs.end(), rs.begin(), [&](auto v) {
                return breakdown(v, blinks - 1);
            });
            memo[std::make_pair(xs[0], blinks-1)] = rs[0];
            memo[std::make_pair(xs[1], blinks-1)] = rs[1];
            return std::ranges::fold_left(rs, 0, std::plus());
        } else {
            const auto& res = breakdown(x*2024, blinks-1);
            memo[std::make_pair(x*2024, blinks-1)] = res;
            return res;
        }
    }
}

int main() {

    auto f = std::ifstream("input.txt");
    std::string line{};
    std::getline(f, line);
    auto ls = std::istringstream(line);
    auto ns = std::vector<i64>{};

    i64 n{0};
    while (ls >> n) ns.push_back(n);

    i64 answer1 { 0 };
    i64 answer2 { 0 };
    for (auto&& x : ns) {
        answer1 += breakdown(x, 25);
        answer2 += breakdown(x, 75);
    }

    std::cout << "answer1=" << answer1 << '\n';
    std::cout << "answer2=" << answer2 << '\n';

    return 0;
}
