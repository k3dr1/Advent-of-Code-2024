#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

int main() {

    auto answer1{0ll};
    auto answer2{0ll};
    auto f = std::fstream{"input.txt"};
    auto line = std::string{};
    auto lines = std::vector<std::string>{};

    while (std::getline(f, line)) lines.push_back(line);

    auto lefts = std::vector<long long>{};
    auto rights = std::vector<long long>{};
    for (auto&& e : lines) {
        lefts.push_back(std::stoi(e.substr(0, 6)));
        rights.push_back(std::stoi(e.substr(6)));
    }

    std::ranges::sort(lefts);
    std::ranges::sort(rights);

    for (int i = 0; i < lefts.size(); i++) answer1 += std::abs(lefts.at(i) - rights.at(i));
    for (auto&& num : lefts) answer2 += std::ranges::count(rights, num) * num;

    std::cout << "part1=" << answer1 << '\n';
    std::cout << "part2=" << answer2 << '\n';

    return 0;
}
