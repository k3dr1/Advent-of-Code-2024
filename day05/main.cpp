#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

int main()
{
    using namespace std::views;
    using std::swap, std::string, std::ranges::find, std::swap;
    using i64 = std::int64_t;
    auto f = std::ifstream("input.txt");
    i64 ans1{0};
    i64 ans2{0};
    std::unordered_map<i64, std::vector<i64>> rules{};
    std::vector<std::vector<i64>> updates{};

    auto first_section { true };
    for (string line {}; std::getline(f, line);) {
        if (line == "\r") {
            first_section = false;
            continue;
        }
        if (first_section) {
            rules[std::stoll(line)].push_back(std::stoll(line.substr(line.find_first_of('|') + 1)));
        } else {
            auto nums = line
                | split(',')
                | transform([](const auto& s) { return std::atoll(string(&*s.begin(), std::ranges::distance(s)).c_str()); });
            updates.emplace_back(nums.begin(), nums.end());
        }
    }
    for (auto& update : updates) {
        auto correct = std::ranges::fold_left(
            iota(0uz, update.size())
                | transform([&update](auto i) { return update | drop(i); })
                | transform([&update, &rules](auto&& r) {
                      auto& head = *r.begin();
                      return std::ranges::fold_left(r
                              | drop(1)
                              | transform([&update, &rules, &head](auto& upper) {
                                    return (rules.contains(upper) && find(rules[upper], head) != rules[upper].end()) ? swap(head, upper), false : true;
                                }),
                          true, std::logical_and<>());
                  }),
            true, std::logical_and<>());
        ans1 += correct ? update.at(update.size() / 2) : 0;
        ans2 += !correct ? update.at(update.size() / 2) : 0;
    }
    std::cout << "ans1=" << ans1 << '\n';
    std::cout << "ans2=" << ans2 << '\n';
    return 0;
}
