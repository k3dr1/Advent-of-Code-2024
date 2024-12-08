#include <cstdint>
#include <fstream>
#include <ranges>
#include <iostream>
#include <string>
#include <vector>

using i64 = std::int64_t;

i64 concatenate(i64 a, i64 b) {
    auto oldb = b;
    for (; b > 0; b/=10) a*=10;
    return a + oldb;
}


bool brute(const i64 target, i64 current, const std::vector<i64>& nums) {
    if ((target == current) && nums.size() == 0) return true;
    if ((nums.size() == 0) || (target < current)) return false;
    else {
        const auto tailnums = std::vector(nums.begin()+1, nums.end());
        return (brute(target, current+nums.at(0), tailnums) 
        || brute(target, current*nums.at(0), tailnums));
    }
}

bool brute2(const i64 target, i64 current, const std::vector<i64>& nums) {
    if ((target == current) && nums.size() == 0) return true;
    if ((nums.size() == 0) || (target < current)) return false;
    else {
        const auto tailnums = std::vector(nums.begin()+1, nums.end());
        return (brute2(target, current+nums.at(0), tailnums) 
        || brute2(target, concatenate(current, nums.at(0)), tailnums)
        || brute2(target, current*nums.at(0), tailnums));
    }
}

int main() {
    std::uint64_t answer1{0};
    std::uint64_t answer2{0};
    auto f = std::ifstream("input.txt");
    for (std::string line{}; std::getline(f, line);) {
        auto target = std::stoll(line.substr(0, line.find(':')));
        auto r = line.substr(line.find(':') + 2)
                | std::views::split(' ')
                | std::views::transform([](const auto& s) { 
                return std::atoll(std::string(&*s.begin(), std::ranges::distance(s)).c_str()); });
        auto nums = std::vector<i64>(r.begin(), r.end());
        auto current = nums.at(0);
        const auto tailnums = std::vector(nums.begin()+1, nums.end());
        if (brute(target, current, tailnums)) {
            answer1 += target;
            answer2 += target;
        }
        else if (brute2(target, current, tailnums)) answer2 += target;
    }

    std::cout << "answer1=" << answer1 << '\n';
    std::cout << "answer2=" << answer2 << '\n';
}
