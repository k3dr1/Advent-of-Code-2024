#include <cstdint>
#include <fstream>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <string>
#include <print>

struct strhash {
    auto operator()(const std::string& s) const {
        auto res{0ll};
        for (const auto [i, c] : std::views::enumerate(s)) res += i*c;
        return res;
    }
};

auto memo = std::unordered_map<std::string, bool, strhash>{};
auto constructible(std::string_view design, const std::vector<std::string>& patterns) -> bool {
    if (design.length() == 0) return true;
    else {
        if (memo.contains(std::string(design))) return memo[std::string(design)];
        for (const auto& pattern : patterns) {
            if (design.starts_with(pattern) 
                && constructible(design.substr(pattern.length()), patterns)) {
                memo[std::string(design)] = true;
                return true;
            }
        }
        memo[std::string(design)] = false;
        return false;
    }
}

auto memo2 = std::unordered_map<std::string, std::int64_t, strhash>{};
auto constructible2(std::string_view design, const std::vector<std::string>& patterns) -> std::int64_t {
    if (design.length() == 0) return 1;
    else {
        auto res{0ll};
        if (memo2.contains(std::string(design))) return memo2[std::string(design)];
        for (const auto& pattern : patterns) {
            if (design.starts_with(pattern)) {
                res += constructible2(design.substr(pattern.length()), patterns);
            }
        }
        memo2[std::string(design)] = res;
        return res;
    }
}

int main() {

    std::int64_t ans1{};
    std::int64_t ans2{};

    auto f = std::ifstream("input.txt");

    std::string line{};
    std::getline(f, line);

    std::vector<std::string> designs{};
    auto available = line | std::views::split(',')
        | std::views::transform([](const auto& s) { return std::string(&*s.begin(), std::ranges::distance(s)); })
        | std::views::transform([](const std::string& s) { return s.substr(s.find_first_not_of(' ')); })
        | std::ranges::to<std::vector<std::string>>();

    for (;std::getline(f, line);) {
        if (!line.empty()) designs.push_back(line);
    }

    for (const auto& design : designs) {
        ans1 += constructible(design, available);
        ans2 += constructible2(design, available);
    }

    std::println("ans1 {}", ans1);
    std::println("ans2 {}", ans2);

    return 0;
}
