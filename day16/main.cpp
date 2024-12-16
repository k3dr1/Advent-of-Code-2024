#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <print>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using ll = std::int64_t;

struct vec2 {
    ll x;
    ll y;
    auto operator+(const auto& other) const { return vec2 { this->x + other.x, this->y + other.y }; }
    auto operator-(const auto& other) const { return vec2 { this->x - other.x, this->y - other.y }; }
    auto operator*(const ll& scalar) const { return vec2 { scalar * (this->x), scalar * (this->y) }; }
    bool operator==(const auto& other) const { return (this->x == other.x) && (this->y == other.y); }
};

//  |
// ---> x
//  |
//  v y

constexpr auto up    = vec2{ 0, -1}; // 0
constexpr auto right = vec2{ 1,  0}; // 1
constexpr auto down  = vec2{ 0,  1}; // 2
constexpr auto left  = vec2{-1,  0}; // 3

constexpr auto directions = std::array<vec2, 4> {
    up,
    right,
    down,
    left,
};

void printgrid(const auto& grid) {
    for (const auto& row : grid) {
        for (const auto& val : row) {
            std::print("{}",val);
        }
        std::println("");
    }
}

vec2 gridfind(char ch, const auto& grid) {
    for (ll y = 0; y < grid.size(); y++) {
        for (ll x = 0; x < grid.at(0).size(); x++) {
            if (grid.at(y).at(x) == ch) return vec2{x,y};
        }
    }
    return vec2{-1,-1};
}

struct node {
    vec2 pos;
    ll distance;
    ll direction;
};

struct hasher {
    ll operator()(const std::pair<vec2, ll>& v) const {
        return v.first.x * 137 + v.first.y ^ v.second;
    }
};

auto solve_part1(const std::vector<std::vector<char>>& grid) {
    auto start = gridfind('S', grid);
    auto end = gridfind('E', grid);
    auto comp = [](const node& l, const node& r) { return l.distance > r.distance; };

    std::priority_queue<node, 
                        std::vector<node>, 
                        decltype(comp)> pq {comp};
    // [pos, dir] -> [dist]
    std::unordered_map<std::pair<vec2, ll>, ll, hasher> visited{};

    pq.push({start, 0, 1});
    while (!pq.empty()) {
        auto [pos, dist, dir] = pq.top();
        pq.pop();
        if (pos == end) return dist;
        visited[std::make_pair(pos, dir)] = dist;
        // Advance
        auto next_pos = pos + directions[dir];
        if (grid.at(next_pos.y).at(next_pos.x) != '#') {
            if (!visited.contains(std::make_pair(next_pos, dir)) || visited.at(std::make_pair(next_pos, dir)) > (dist+1)) {
                pq.push(node{next_pos,dist+1,dir});
            } 
        }

        // Clockwise
        auto cw_dir = (dir+1)%4;
        if (!visited.contains(std::make_pair(pos, cw_dir)) || visited.at(std::make_pair(pos, cw_dir)) > (dist)) {
            pq.push(node { pos, dist + 1000, cw_dir });
        } 

        // Counter-clockwise
        auto ccw_dir = (dir+3)%4;
        if (!visited.contains(std::make_pair(pos, ccw_dir)) || visited.at(std::make_pair(pos, ccw_dir)) > (dist)) {
            pq.push(node { pos, dist + 1000, ccw_dir });
        } 
    }
    std::unreachable();
}

int main() {

    auto f = std::ifstream("input.txt");
    auto grid = std::vector<std::vector<char>>{};

    for (std::string line {}; std::getline(f, line);) {
        line = line.substr(0, line.find_first_of('\r', '\n'));
        grid.emplace_back(line.begin(), line.end());
    }

    printgrid(grid);

    std::println("Ans1: {}", solve_part1(grid));

    return 0;
}
