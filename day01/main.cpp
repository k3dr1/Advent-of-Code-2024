#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>

int main() {

    auto answer1{0ll}, answer2{0ll};
    auto f = std::ifstream{"input.txt"};
    auto line = std::string{};
    auto lefts = std::vector<long long>{};
    auto rights = std::vector<long long>{};
    while (std::getline(f, line)) {
        lefts.push_back(std::stoi(line.substr(0, line.find(' '))));
        rights.push_back(std::stoi(line.substr(line.find_last_of(' '))));
    }
    std::ranges::sort(lefts);
    std::ranges::sort(rights);
    for (auto&& [l,r] : std::views::zip(lefts, rights)) {
        answer1 += std::abs(l - r);
        answer2 += std::ranges::count(rights, l) * l;
    }
    std::cout << "part1=" << answer1 << '\n';
    std::cout << "part2=" << answer2 << '\n';
    return 0;
}
