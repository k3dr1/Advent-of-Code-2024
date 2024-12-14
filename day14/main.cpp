#include <cstdint>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

using ll = std::int64_t;

auto add(std::pair<ll, ll> a, decltype(a) b) -> decltype(a) { return { a.first + b.first, a.second + b.second }; }
auto clear_grid(std::vector<std::vector<char>>& grid) {
    for (auto& row : grid)
        for (auto& el : row)
            el = '0';
}

int main() {
    constexpr ll WIDTH = 101;
    constexpr ll HEIGHT = 103;
    auto grid = std::vector<std::vector<char>>(HEIGHT, std::vector<char>(WIDTH, '0'));
    std::vector<std::string> lines{};

    auto f = std::ifstream("input.txt");
    for (std::string line {}; std::getline(f, line);) {
        lines.push_back(line);
        auto p = std::make_pair(std::stoll(line.substr(line.find(',') + 1)),
                                std::stoll(line.substr(line.find('=') + 1)));
        auto v = std::make_pair(std::stoll(line.substr(line.find_last_of(',') + 1)),
                                std::stoll(line.substr(line.find_last_of('=') + 1)));
        std::println("p = {} {}; v = {} {}", p.first, p.second, v.first, v.second);
        for (ll i = 0; i < 100; i++) {
            p = add(p,v);
            p.first = ((p.first+HEIGHT)%HEIGHT);
            p.second = ((p.second+WIDTH)%WIDTH);
        }
        std::println("trying to place at {} {}", p.first, p.second);
        grid.at(p.first).at(p.second)++;
    }

    auto topleft = 0;
    auto topright = 0;
    auto botleft = 0;
    auto botright = 0;
    for (ll y = 0; y < HEIGHT; y++) {
        for (ll x = 0; x < WIDTH; x++) {
            constexpr auto midheight = ((HEIGHT-1)/2);
            constexpr auto midwidth = ((WIDTH-1)/2);
            if ((y == midheight) || (x == midwidth)) {
                std::print(" ");
                continue;
            }
            if (y < midheight) {
                if (x < midwidth) {
                    topleft += grid.at(y).at(x) - '0';
                } else {
                    topright += grid.at(y).at(x) - '0';
                }
            } else {
                if (x < midwidth) {
                    botleft += grid.at(y).at(x) - '0';
                } else {
                    botright += grid.at(y).at(x) - '0';
                }
            }
        }
        std::print("\n");
    }

    std::println("Ans1: {}",topleft*topright*botleft*botright);

    auto grid2 = std::vector<std::vector<char>>(HEIGHT, std::vector<char>(WIDTH, '0'));

    for (ll i = 0; i < 10'000; i++) {
        clear_grid(grid2);
        for (const auto& line : lines) {
            auto p = std::make_pair(std::stoll(line.substr(line.find(',') + 1)),
                                    std::stoll(line.substr(line.find('=') + 1)));
            auto v = std::make_pair(std::stoll(line.substr(line.find_last_of(',') + 1)),
                                    std::stoll(line.substr(line.find_last_of('=') + 1)));
            for (ll j = 0; j < i; j++) {
                p = add(p,v);
                p.first = ((p.first+HEIGHT)%HEIGHT);
                p.second = ((p.second+WIDTH)%WIDTH);
            }
            grid2.at(p.first).at(p.second)++;
        }

        bool no_robot_occupies_the_same_spot{true};
        for (auto& row : grid2)
            for (auto& val : row)
                no_robot_occupies_the_same_spot &= ((val == '0') || (val == '1'));
        if (no_robot_occupies_the_same_spot) {
            std::println("Ans2: {}", i);
            break;
        }
    }

    return 0;
}
