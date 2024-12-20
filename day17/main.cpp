#include <cstdint>
#include <vector>
#include <fstream>
#include <ranges>
#include <string>
#include <print>

using ll = std::int64_t;

int main() {
    auto f = std::ifstream("example.txt");

    auto reg = std::vector<ll>{};

    for (std::string line{}; std::getline(f, line);){
        if (line.size() < 3) break;
        reg.push_back(std::stoll(line.substr(12)));
    }

    std::string line{};
    std::getline(f, line);
    auto program = line.substr(line.find(' '))
        | std::views::split(',')
        | std::views::transform([](const auto& s) { 
            return std::stoll(std::string(&*s.begin(), std::ranges::distance(s)).c_str()); })
        | std::ranges::to<std::vector<ll>>();

    auto ip = 0;

    while (ip < program.size()) {
        auto opcode = program.at(ip);
        auto operand = program.at(ip+1);
    }
}
