#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <print>

using ll = std::int64_t;

struct vec2 {
    ll x;
    ll y;
    auto operator+(const auto& other) const { return vec2 { this->x + other.x, this->y + other.y }; }
    auto operator-(const auto& other) const { return vec2 { this->x - other.x, this->y - other.y }; }
    auto operator*(const ll& scalar) const { return vec2 { scalar * (this->x), scalar * (this->y) }; }
    bool operator==(const auto& other) const { return (this->x == other.x) && (this->y == other.y); }
};

const auto numpad = std::vector<std::vector<char>> {
    { '7', '8', '9' },
    { '4', '5', '6' },
    { '1', '2', '3' },
    { '#', '0', 'A' },
};

const auto dirpad = std::vector<std::vector<char>> {
    { '#', '^', 'A' },
    { '<', 'v', '>' },
};

auto find_char(char ch, const std::vector<std::vector<char>>& grid) -> vec2 {
    for (ll y = 0; y < grid.size(); y++) 
        for (ll x = 0; x < grid.at(0).size(); x++) 
            if (grid.at(y).at(x) == ch) return {x,y};
    return {-1,-1};
}

auto numpath(char a, char b) -> std::string {

    if (a == '7' && b == 'A') return ">>vvvA";
    if (a == '7' && b == '0') return ">vvvA";

    if (a == '4' && b == 'A') return ">>vvA";
    if (a == '4' && b == '0') return ">vvA";

    if (a == '1' && b == 'A') return ">>vA";
    if (a == '1' && b == '0') return ">vA";

    if (a == '0' && b == '1') return "^<A";
    if (a == '0' && b == '4') return "^^<A";
    if (a == '0' && b == '7') return "^^^<A";

    if (a == 'A' && b == '1') return "^<<A";
    if (a == 'A' && b == '4') return "^^<<A";
    if (a == 'A' && b == '7') return "^^^<<A";

    std::string res{};
    auto vec = find_char(b, numpad) - find_char(a, numpad);
    // horizontal
    res += std::string(std::abs(vec.x), vec.x >= 0 ? '>' : '<');
    // vertical
    res += std::string(std::abs(vec.y), vec.y >= 0 ? 'v' : '^');
    // then accept
    res += 'A';
    return res;
}

auto dirpath(char a, char b) -> std::string {

    if (a == 'A' && b == '<') return "v<<A";
    if (a == '^' && b == '<') return "v<A";

    if (a == '<' && b == 'A') return ">>^A";
    if (a == '<' && b == '^') return ">^A";

    std::string res{};
    auto vec = find_char(b, dirpad) - find_char(a, dirpad);
    // horizontal
    res += std::string(std::abs(vec.x), vec.x >= 0 ? '>' : '<');
    // vertical
    res += std::string(std::abs(vec.y), vec.y >= 0 ? 'v' : '^');
    // then accept
    res += 'A';
    return res;
}

auto num2dir(const std::string& code) {
    std::string res{};
    res += numpath('A', code.at(0));
    for (ll i = 1; i < code.size(); i++) res += numpath(code.at(i-1), code.at(i));
    return res;
}

auto dir2dir(const std::string& code) {
    std::string res{};
    res += dirpath('A', code.at(0));
    for (ll i = 1; i < code.size(); i++) res += dirpath(code.at(i-1), code.at(i));
    return res;
}

auto decode_dir(const std::string& code) {
    auto pos = vec2{2,0};
    std::string res{};
    for (char ch : code) {
        //std::println("decoded: {}", res);
        //std::println("pos {},{}", pos.x, pos.y);
        switch (ch) {
            case 'A': {
                res += dirpad.at(pos.y).at(pos.x);
                break;
            }
            case '^': {
                pos = pos + vec2{0, -1};
                break;
            }
            case '>': {
                pos = pos + vec2{1, 0};
                break;
            }
            case 'v': {
                pos = pos + vec2{0, 1};
                break;
            }
            case '<': {
                pos = pos + vec2{-1, 0};
                break;
            }
        }
    }
    return res;
}

auto decode_num(const std::string& code) {
    auto pos = vec2{2,3};
    std::string res{};
    for (char ch : code) {
        //std::println("decoded: {}", res);
        //std::println("pos {},{}", pos.x, pos.y);
        switch (ch) {
            case 'A': {
                res += numpad.at(pos.y).at(pos.x);
                break;
            }
            case '^': {
                pos = pos + vec2{0, -1};
                break;
            }
            case '>': {
                pos = pos + vec2{1, 0};
                break;
            }
            case 'v': {
                pos = pos + vec2{0, 1};
                break;
            }
            case '<': {
                pos = pos + vec2{-1, 0};
                break;
            }
        }
    }
    return res;
}

auto translate1(const std::string& code) {
    auto d0 = num2dir(code);
    auto d1 = dir2dir(d0);
    auto d2 = dir2dir(d1);

    std::println(" N: {}", code);
    std::println("D0: {}", d0);
    std::println("D1: {}", d1);
    std::println("D2: {}", d2);

    const auto length = d2.length();
    const auto numeric = std::stoll(code);

    auto dd1 = decode_dir(d2);
    auto dd0 = decode_dir(dd1);
    auto dn = decode_num(dd0);
    std::println("D1: {} (decoded)", dd1);
    std::println("D0: {} (decoded)", dd0);
    std::println(" N: {} (decoded)", dn);

    if (dd1 != d1) std::println("d2 not equal");
    if (dd0 != d0) std::println("d1 not equal");
    if (dn != code) std::println("d0 not equal");

    std::println("{}*{}={}", length, numeric, length*numeric);
    std::println("");

    return length*numeric;
}

auto part1(const std::vector<std::string>& codes) -> ll {
    ll res{};
    for (const auto& code : codes) {
        auto complexity = translate1(code);
        res += complexity;
    }
    return res;
}

int main() {

    ll ans1{0};
    ll ans2{0};

    auto f = std::ifstream("input.txt");
    auto codes = std::vector<std::string>{};
    for (std::string line{}; std::getline(f, line);) {
        codes.emplace_back(line.begin(), line.end());
    }

    ans1 = part1(codes);

    // 284652 is too high
    std::println("ans1 {}", ans1);

    return 0;
}
