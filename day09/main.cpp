#include <algorithm>
#include <atomic>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <ranges>

int main() {

    using i64 = std::int64_t;
    i64 answer1{0};
    i64 answer2{0};
    auto f = std::ifstream("input.txt");

    for (std::string line {}; std::getline(f, line);) {
        std::vector<i64> diskmap {};
        auto line_idx = 0;
        for (; line_idx < line.size(); line_idx++) {
            if (line_idx % 2 == 0) {
                for (auto i = 0; i < line.at(line_idx) - '0'; i++) {
                    diskmap.push_back((line_idx / 2) + '0');
                }
            } else {
                for (auto i = 0; i < line.at(line_idx) - '0'; i++) {
                    diskmap.push_back('.');
                }
            }
        }

        auto diskmap2 = std::vector(diskmap.begin(), diskmap.end());

        // Part 1
        for (i64 i = diskmap.size() - 1; 0 <= i; i--) {
            auto first_dot = std::ranges::find(diskmap, '.') - diskmap.begin();
            if (i <= first_dot) break;
            else std::swap(diskmap.at(i), diskmap.at(first_dot));
        }

        // Part 2
        for (i64 i = diskmap2.size() - 1; 0 <= i; i--) {
            auto j = i;
            while (0 <= j && diskmap2.at(j) == diskmap2.at(i)) j--;
            for (auto d = 0; d < (j+1); d++) {
                if (diskmap2.at(d) == '.') {
                    auto e = d;
                    while (e < (j+1) && diskmap2.at(e) == '.') e++;
                    // if fits
                    if ((i-j) <= (e - d)) {
                        // move the file
                        for (auto k = 0; k < (i-j); k++) std::swap(diskmap2.at(d+k), diskmap2.at(i-k));
                        break;
                    }
                }
            }
            i = j+1;
        }

        answer1 = std::ranges::fold_left(
            diskmap
                | std::views::take_while([](auto&& el) { return el != '.'; })
                | std::views::enumerate
                | std::views::transform([](const auto& p) {
                      const auto& [l, r] = p;
                      return l * (r - '0');
                  }),
            i64 {}, std::plus<> {});

        answer2 = std::ranges::fold_left(
            diskmap2
                | std::views::enumerate
                | std::views::transform([](const auto& p) {
                      const auto& [l, r] = p;
                      return l * (r - '0');
                  }),
            i64 {},
            std::plus<> {});

        std::cout << "answer1=" << answer1 << '\n';
        std::cout << "answer2=" << answer2 << '\n';
    }

    return 0;
}
