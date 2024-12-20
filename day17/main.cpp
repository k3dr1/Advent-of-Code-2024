#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <ranges>
#include <string>
#include <print>

using ll = std::int64_t;
using ull = std::int64_t;

auto run2(ll rega, ll regb, ll regc, const std::vector<ll>& program) {
    auto ip{0ll};
    std::vector<ll> out{};
    auto combo = [&](const ll opr) {
        if (0 <= opr && opr <= 3) return opr;
        if (opr == 4) return rega;
        if (opr == 5) return regb;
        if (opr == 6) return regc;
        std::exit(-1);
    };

    auto program_string = std::ranges::fold_left(program,
            "",
            [](std::string l, const auto& r) {
                return l + std::to_string(r) + ",";
            });

    while (ip < program.size()) {
        auto opcode = program.at(ip);
        auto operand = program.at(ip+1);

        switch (opcode) {
            case 0: {
                rega = rega/(1<<(combo(operand)));
                ip+=2;
                break;
            }
            case 1: {
                regb = regb^operand;
                ip+=2;
                break;
            }
            case 2: {
                regb = combo(operand)%8;
                ip+=2;
                break;
            }
            case 3: {
                if (rega == 0) {
                    ip+=2;
                } else {
                    ip = operand;
                }
                break;
            }
            case 4: {
                regb = regb ^ regc;
                ip+=2;
                break;
            }
            case 5: {
                out.push_back(combo(operand)%8);
                ip+=2;
                break;
            }
            case 6: {
                regb = rega/(1<<(combo(operand)));
                ip+=2;
                break;
            }
            case 7: {
                regc = rega/(1<<(combo(operand)));
                ip+=2;
                break;
            }
        }
    }

    return out;
}

auto part2(const std::vector<ll>& program) {
    auto A = 0ll;
    auto cursim = 1;
    while (true) {
        auto out = run2(A, 0, 0, program);
        auto sim = 0;
        for (int i = 1; i <= out.size() && (program.at(program.size() - i) == out.at(out.size() - i)); i++) sim++;
        if (sim == program.size()) return A;
        if (cursim < sim) {
            cursim = sim;
            A *= 8;
        } else {
            A++;
        }
    }
}

int main() {
    auto f = std::ifstream("input.txt");
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

    auto program_string = std::ranges::fold_left(program,
        "",
        [](std::string l, const auto& r) {
            return l + std::to_string(r) + ",";
        });
    program_string = program_string.substr(0, program_string.size()-1);

    auto ans1 = run2(reg[0], reg[1], reg[2], program);
    auto ans1_string = std::ranges::fold_left(ans1,
        "",
        [](std::string l, const auto& r) {
            return l + std::to_string(r) + ",";
        });
    ans1_string = ans1_string.substr(0, ans1_string.size()-1);
    std::println("ans1 {}", ans1_string);

    auto ans2 = part2(program);
    std::println("ans2 {}", ans2);

    return 0;
}
