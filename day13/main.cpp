#include <fstream>
#include <print>
#include <string>

using i64 = std::int64_t;

struct vec2 {
    i64 x;
    i64 y;
    auto operator+(const auto& other) const { return vec2 { this->x + other.x, this->y + other.y }; }
    auto operator-(const auto& other) const { return vec2 { this->x - other.x, this->y - other.y }; }
    auto operator*(const i64& scalar) const { return vec2 { scalar * (this->x), scalar * (this->y) }; }
    bool operator==(const auto& other) const { return (this->x == other.x) && (this->y == other.y); }
};

auto solve(vec2 A, vec2 B, vec2 R /* Prize */) {
    auto determinant = (A.x * B.y - B.x * A.y);
    auto u = (B.y * R.x - B.x * R.y) / (determinant);
    auto v = (A.x * R.y - A.y * R.x) / (determinant);
    return A * u + B * v == R ? u * 3 + v : 0;
}

int main() {
    auto f = std::ifstream { "input.txt" };
    auto ans1 { 0ll };
    auto ans2 { 0ll };
    while (true) {
        std::string line {};
        std::getline(f, line);
        if (line.size() < 3)
            break;
        vec2 A = { std::stoll(line.substr(line.find_first_of('+') + 1)), std::stoll(line.substr(line.find_last_of('+') + 1)) };
        std::getline(f, line);
        vec2 B = { std::stoll(line.substr(line.find_first_of('+') + 1)), std::stoll(line.substr(line.find_last_of('+') + 1)) };
        std::getline(f, line);
        vec2 P = { std::stoll(line.substr(line.find_first_of('=') + 1)), std::stoll(line.substr(line.find_last_of('=') + 1)) };
        std::getline(f, line);
        ans1 += solve(A, B, P);
        ans2 += solve(A, B, P + vec2 { 10000000000000, 10000000000000 });
    }
    std::println("ans1 {}", ans1);
    std::println("ans2 {}", ans2);
    return 0;
}
