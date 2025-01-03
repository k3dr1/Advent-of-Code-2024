#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using i64 = std::int64_t;
constexpr std::array<int, 4> chars{'^', '>', 'v', '<', };

int rotate(int dir) { return (dir+1)%4; }
bool check_bounds(int y, int x, const auto& grid) { return !((y < 0 || grid.size() <= y) || (x < 0 || grid.at(0).size() <= x)); }

inline std::pair<int,int> advance(int y, int x, int dir)
{
    switch (dir) {
    case 0:
        y -= 1;
        break;
    case 1:
        x += 1;
        break;
    case 2:
        y += 1;
        break;
    case 3:
        x -= 1;
        break;
    }
    return {y, x};
}

std::pair<int,int> right_of(int y, int x, int dir) { return advance(y, x, (dir+1)%4); }

// Turns right and explores until it finds a loop or leaves the grid
bool detect_loop(int y, int x, int dir, const auto& grid) {
    const auto starty = y;
    const auto startx = x;
    const auto [obstacley, obstaclex] = advance(y, x, dir);
    dir = rotate(dir);

    auto hash = [](const int y, const int x, const int dir) -> i64 {
        return (i64(y)) ^ ((i64(x))<<32) ^ ((i64(dir))<<62);
    };
    std::unordered_map<i64, bool> visited{};

    while (check_bounds(y, x, grid)) {
        if (visited.contains(hash(y, x, dir))) {
            return true;
        }
        visited[hash(y, x, dir)] = true;
        auto [nexty, nextx] = advance(y, x, dir);
        if (!check_bounds(nexty, nextx, grid)) {
            break;
        }
        // Early exit, there's already a loop there
        if (grid.at(nexty).at(nextx) == chars[dir]) return true;
        if (grid.at(nexty).at(nextx) == '#' || (nexty == obstacley && nextx == obstaclex)) {
            dir = rotate(dir);
            continue;
        } else {
            y = nexty;
            x = nextx;
        }
    }
    return false;
}

int main() {
    auto covered{0};
    auto obstacle_count{0};

    auto f = std::fstream("input.txt");
    auto blockhash = [](const int y, const int x) -> i64 { return ((i64(y))<<32) ^ (i64(x)); };
    std::unordered_map<i64,int> blocks{};
    std::vector<std::vector<char>> grid{};

    for(std::string line{}; std::getline(f, line);) {
        while (line.back() == '\r' || line.back() == '\n') line.pop_back();
        grid.emplace_back(line.begin(), line.end());
    }

    auto startx = -1;
    auto starty = -1;
    auto start_found{false};

    for (int y = 0; y < grid.size() && !start_found; y++) {
        for (int x = 0; x < grid.at(0).size() && !start_found; x++) {
            if (grid.at(y).at(x) == '^') {
                starty = y;
                startx = x;
                start_found = true;
            }
        }
    }

    // 0 up, 1 right, 2 down, 3 left,
    auto direction = 0;
    auto curx = startx;
    auto cury = starty;
    while (true) {
        grid.at(cury).at(curx) = chars[direction];
        auto [nexty, nextx] = advance(cury, curx, direction);
        if (!check_bounds(nexty, nextx, grid)) {
            break;
        }
        if (grid.at(nexty).at(nextx) == '#') {
            direction = rotate(direction);
            continue;
        } else {
            if (grid.at(nexty).at(nextx) == '.' && detect_loop(cury, curx, direction, grid)) {
                    blocks[blockhash(nexty,nextx)]++;
            }
            cury = nexty;
            curx = nextx;
        }
    }

    for (auto row : grid) {
        covered += std::ranges::count(row, '^');
        covered += std::ranges::count(row, '>');
        covered += std::ranges::count(row, 'v');
        covered += std::ranges::count(row, '<');
    }
    obstacle_count = blocks.size();

    std::cout << "answer1=" << covered << '\n';
    std::cout << "answer2=" << obstacle_count << '\n';

    return 0;
}
