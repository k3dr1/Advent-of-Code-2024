#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <ranges>
#include <utility>
#include <vector>
#include <execution>

using namespace std::views;
using std::make_pair;
using i64 = std::int64_t;
using u64 = std::uint64_t;

bool check_bounds(int y, int x, const auto& grid) { return !((y < 0 || grid.size() <= y) || (x < 0 || grid.at(0).size() <= x)); }

template<bool IsPart2>
i64 num_trails(i64 _y, i64 _x, const std::vector<std::vector<char>>& grid) {
    i64 res{0};
    auto hash = [](const auto& y, const auto& x) { return ((((u64)y) << (32UL)) | ((u64)x)); };
    std::vector<std::pair<i64,i64>> stk{make_pair(_y,_x)};
    std::unordered_map<i64,bool> visited{};
    while (!stk.empty()) {
        const auto [y, x] = stk.back();
        stk.pop_back();
        visited[hash(y,x)] = true;
        if (grid.at(y).at(x) == '9') {
            res++;
        } else {
            auto curval = grid.at(y).at(x);
            if (check_bounds(y-1, x, grid) && ((curval + 1) == grid[y-1][x])
                && (IsPart2 || (!visited.contains(hash(y-1, x))))) stk.push_back(make_pair(y-1, x));
            if (check_bounds(y+1, x, grid) && ((curval + 1) == grid[y+1][x])
                && (IsPart2 || (!visited.contains(hash(y+1, x))))) stk.push_back(make_pair(y+1, x));
            if (check_bounds(y, x+1, grid) && ((curval + 1) == grid[y][x+1])
                && (IsPart2 || (!visited.contains(hash(y, x+1))))) stk.push_back(make_pair(y, x+1));
            if (check_bounds(y, x-1, grid) && ((curval + 1) == grid[y][x-1])
                && (IsPart2 || (!visited.contains(hash(y, x-1))))) stk.push_back(make_pair(y, x-1));
        }
    }
    return res;
}

int main() {
    std::atomic_uint64_t answer1{0};
    std::atomic_uint64_t answer2{0};
    auto f = std::ifstream{"input.txt"};
    auto grid = std::vector<std::vector<char>>{};
    for (std::string line{}; std::getline(f, line);) grid.emplace_back(line.begin(), line.end());
    auto trailheads = iota(0uz, grid.size())
        | transform([&](const auto& num) { return zip(repeat(num, grid.at(0).size()), iota(0uz, grid.at(0).size())); })
        | join
        | filter([&](const auto& p) { const auto& [y, x] = p; return (grid.at(y).at(x) == '0'); })
        | common
        | std::ranges::to<std::vector<std::pair<i64, i64>>>();

    std::for_each(
        std::execution::par_unseq,
        trailheads.cbegin(),
        trailheads.cend(),
        [&](const auto& p) {
            const auto& [y, x] = p;
            answer1 += num_trails<false>(y, x, grid);
            answer2 += num_trails<true>(y, x, grid);
        });

    std::cout << "answer1=" << answer1 << '\n';
    std::cout << "answer2=" << answer2 << '\n';
    return 0;
}
