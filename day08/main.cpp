#include <cstdint>
#include <ranges>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

bool check_bounds(int y, int x, const auto& grid) { return !((y < 0 || grid.size() <= y) || (x < 0 || grid.at(0).size() <= x)); }

int main() {

    using i64 = std::int64_t;

    i64 answer1{0};
    i64 answer2{0};
    auto f = std::ifstream("input.txt");

    std::vector<std::vector<char>> grid{};
    std::unordered_map<char, std::vector<std::pair<i64,i64>>> freq_map{};

    for (std::string line{}; std::getline(f, line);) {
        while (line.back() == '\n' || line.back() == '\r') line.pop_back();
        grid.emplace_back(line.begin(), line.end());
    }

    for (i64 y = 0; y < grid.size(); y++) {
        for (i64 x = 0; x < grid.at(0).size(); x++) {
            freq_map[grid.at(y).at(x)].push_back(
                std::make_pair(y, x));
        }
    }

    std::vector<std::vector<char>> anti_grid1(grid.size(), std::vector<char>(grid.at(0).size(), '.'));
    std::vector<std::vector<char>> anti_grid2(grid.size(), std::vector<char>(grid.at(0).size(), '.'));

    for (const auto& p : freq_map) {
        if (p.first == '.') continue;
        auto resonants = p.second;

        for (i64 i = 0; i < resonants.size(); i++) {
            for (i64 j = i + 1; j < resonants.size(); j++) {
                const auto& [i_y, i_x] = resonants.at(i);
                const auto& [j_y, j_x] = resonants.at(j);
                auto y_diff = j_y - i_y;
                auto x_diff = j_x - i_x;

                // part1
                auto back = std::make_pair(i_y - y_diff, i_x - x_diff);
                auto forward  = std::make_pair(j_y + y_diff, j_x + x_diff);
                if (check_bounds(back.first, back.second, anti_grid1)) {
                    anti_grid1.at(back.first).at(back.second) = '#';
                }
                if (check_bounds(forward.first, forward.second, anti_grid1)) {
                    anti_grid1.at(forward.first).at(forward.second) = '#';
                }

                //part2
                anti_grid2.at(i_y).at(i_x) = '#';
                anti_grid2.at(j_y).at(j_x) = '#';
                while (check_bounds(back.first, back.second, anti_grid1)) {
                    anti_grid2.at(back.first).at(back.second) = '#';
                    back.first  -= y_diff;
                    back.second -= x_diff;
                }
                while (check_bounds(forward.first, forward.second, anti_grid1)) {
                    anti_grid2.at(forward.first).at(forward.second) = '#';
                    forward.first  += y_diff;
                    forward.second += x_diff;
                }
            }
        }
    }

    answer1 = std::ranges::count(anti_grid1 | std::views::join, '#');
    answer2 = std::ranges::count(anti_grid2 | std::views::join, '#');

    std::cout << "Answer1=" << answer1 << '\n';
    std::cout << "Answer2=" << answer2 << '\n';

    return 0;
}
