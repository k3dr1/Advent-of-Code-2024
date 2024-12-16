#include <cstdint>
#include <fstream>
#include <iostream>
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

auto solve_part1(const std::vector<std::vector<char>>& grid) -> ll {
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

auto dijkstra(vec2 start, ll start_dir, vec2 end, const std::vector<std::vector<char>>& grid, std::unordered_map<std::pair<vec2, ll>, ll, hasher> visited) {
    auto comp = [](const node& l, const node& r) { return l.distance > r.distance; };

    std::priority_queue<node, 
                        std::vector<node>, 
                        decltype(comp)> pq {comp};
    // [pos, dir] -> [dist]
    //std::unordered_map<std::pair<vec2, ll>, ll, hasher> visited{};

    pq.push({start, 0, start_dir});
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

struct vhash {
    bool operator()(const vec2& v) const {
        return (v.x<<32) ^ (v.y); 
    }
};

auto solve_part2_checking_each_node(const std::vector<std::vector<char>>& grid) -> ll {
    auto start = gridfind('S', grid);
    auto end = gridfind('E', grid);
    auto comp = [](const node& l, const node& r) { return l.distance > r.distance; };

    std::priority_queue<node, 
                        std::vector<node>, 
                        decltype(comp)> pq {comp};
    // [pos, dir] -> [dist]
    std::unordered_map<std::pair<vec2, ll>, ll, hasher> visited{};

    auto shortest_distance{0};
    pq.push({start, 0, 1});
    while (!pq.empty()) {
        auto [pos, dist, dir] = pq.top();
        pq.pop();
        visited[std::make_pair(pos, dir)] = dist;
        if (pos == end) {
            shortest_distance = dist;
            break;
        }
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

    std::println("Shortest_path is {}", shortest_distance);
    auto num_optimal_cells{0};
    std::unordered_map<vec2, ll, vhash> optimal_cells{};
    auto counter{0};
    for (auto& cell : visited) {

        auto [pos_dir, dist_from_start] = cell;
        auto dist_to_end = dijkstra(pos_dir.first, pos_dir.second, end, grid, {});

        //std::println("Checking node {},{}", pos_dir.first.x, pos_dir.first.y);
        //std::println("from_start: {}", dist_from_start);
        //std::println("calculated dist_to_end: {}", dist_to_end);

        if ((dist_from_start + dist_to_end) == shortest_distance) {
            optimal_cells[pos_dir.first]++;
        }
        counter++;
    }

    return optimal_cells.size();
}

int main() {

    auto f = std::ifstream("example.txt");
    auto grid = std::vector<std::vector<char>>{};

    for (std::string line {}; std::getline(f, line);) {
        line = line.substr(0, line.find_first_of('\r', '\n'));
        grid.emplace_back(line.begin(), line.end());
    }

    printgrid(grid);

    auto ans1 = solve_part1(grid);
    auto ans2 = solve_part2_checking_each_node(grid);

    // not 480, but it's 481 for some reason :skull:
    std::println("Ans1: {}", ans1);
    std::println("Ans2: {}", ans2);

    return 0;
}
