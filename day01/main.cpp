#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <ranges>

using ll = long long;

int main() {
    auto answer1{0ll}, answer2{0ll};
    auto f = std::ifstream{"input.txt"};
    auto line = std::string{};
    auto lefts = std::vector<ll>{};
    auto rights = std::vector<ll>{};
    auto rights_freq = std::unordered_map<ll, ll>{};
    while (std::getline(f, line)) {
        lefts.emplace_back(std::stoi(line.substr(0, line.find(' '))));
        rights.emplace_back(std::stoi(line.substr(line.find_last_of(' '))));
        rights_freq[rights.back()]++;
    }
    std::ranges::sort(lefts);
    std::ranges::sort(rights);
    for (auto&& [l,r] : std::views::zip(lefts, rights)) {
        answer1 += std::abs(l - r);
        answer2 += rights_freq[l] * l;
    }
    std::cout << "part1=" << answer1 << '\n';
    std::cout << "part2=" << answer2 << '\n';
    return 0;
}
