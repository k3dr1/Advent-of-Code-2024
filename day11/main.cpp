#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <ranges>
#include <print>

using i64 = std::int64_t;
using u64 = std::uint64_t;

struct hash {
    auto operator()(const std::pair<i64,i64>& p) const {
        return ((((u64)p.first) << (32UL)) | ((u64)p.second)); 
    }
};

auto num_digits(auto n) -> i64 {
    i64 res { 0 };
    while (n > 0) {
        n /= 10;
        res++;
    }
    return res;
}

std::unordered_map<std::pair<i64,i64>,i64, hash> memo{};
auto breakdown(i64 n, i64 blinks) -> i64 {
    if (blinks == 0) {
        return 1;
    }
    else {
        if (n == 0) return breakdown(1, blinks-1);
        if (memo.contains(std::make_pair(n, blinks))) return memo.at(std::make_pair(n, blinks));
        const auto nd = num_digits(n);
        if (nd%2 == 0) {
            auto pow = 1;
            for (auto j = nd/2; j; j--) pow*=10;
            const auto res = breakdown(n/(pow), blinks-1) + breakdown(n%(pow), blinks-1);
            memo[std::make_pair(n, blinks)] = res;
            return res;
        } else {
            const auto res = breakdown(n*2024, blinks-1);
            memo[std::make_pair(n, blinks)] = res;
            return res;
        }
    }
}

auto top_down(const auto& nums, i64 blinks) {
    i64 res{};
    for (const auto& n : nums) {
        res += breakdown(n, blinks);
    }
    return res;
}

auto bottom_up(const auto& nums, i64 blinks) {
    i64 blink = 0;
    auto readmap = std::unordered_map<i64, i64> {};
    auto writemap = std::unordered_map<i64, i64> {};
    for (const auto num : nums) readmap[num]++;
    for (; blink < blinks; blink++) {
        writemap.clear();
        for (const auto& [n, count] : readmap) {
            if (n == 0) {
                writemap[1ll] += count;
                continue;
            }
            const auto nd = num_digits(n);
            if (!(nd&1)) {
                auto pow = 1ll;
                for (auto j = nd/2; j; j--) pow*=10ll;
                writemap[(n/(pow))] += count;
                writemap[(n%(pow))] += count;
            } else {
                writemap[n*2024] += count;
            }
        }
        std::swap(readmap, writemap);
    }
    return std::ranges::fold_left(readmap | std::views::values, 0ll, std::plus<>());
}

int main() {
    auto f = std::ifstream("bigboy.txt");
    std::string line{};
    std::getline(f, line);
    auto ls = std::istringstream(line);
    auto ns = std::vector<i64>{};

    i64 n{0};
    while (ls >> n) ns.push_back(n);

    std::println("Top down : answer1={}, answer2={}", top_down(ns, 25), top_down(ns, 75));
    std::println("Bottom up: answer1={}, answer2={}", bottom_up(ns, 25), bottom_up(ns, 75));

    return 0;
}
