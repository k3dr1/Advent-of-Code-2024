#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using i64 = std::int64_t;

char to_char(int dir) {
    switch (dir) {
        case 0: return '^';
        case 1: return '>';
        case 2: return 'v';
        case 3: return '<';
        default: return '\0';
    }
}

bool check_bounds(int y, int x, const auto& grid) {
    return !((y < 0 || grid.size() <= y) || (x < 0 || grid.at(0).size() <= x));
}

int rotate(int dir) {
    return (dir+1)%4;
}

std::pair<int,int> advance(int y, int x, int dir)
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

// Turns right and explores until it finds a loop or leaves the grid
bool detect_loop(int y, int x, int dir, const auto& grid) {
    const auto starty = y;
    const auto startx = x;
    dir = rotate(dir);

    auto hash = [](int y, int x, int dir) {
        return std::to_string(y) + "," + std::to_string(x) + "," + std::to_string(dir);
    };
    std::unordered_map<std::string, bool> visited{};

    auto steps{0};
    while (check_bounds(y, x, grid)) {
        if (0 < steps && (visited.contains(hash(y, x, dir)))) {
            return true;
        }
        visited[hash(y, x, dir)] = true;
        auto [nexty, nextx] = advance(y, x, dir);
        if (!check_bounds(nexty, nextx, grid)) {
            break;
        }
        if (grid.at(nexty).at(nextx) == '#' || grid.at(nexty).at(nextx) == 'O') {
            dir = rotate(dir);
            continue;
        } else {
            y = nexty;
            x = nextx;

            steps++;
        }
    }
    return false;
}

std::pair<int,int> right_of(int y, int x, int dir) {
    return advance(y, x, (dir+1)%4);
}

void printgrid(auto grid)
{
    for (auto& row : grid) {
        for (auto el : row) {
            std::cout << el;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

int main() {

    auto answer1{0};
    auto answer2{0};
    auto f = std::fstream("test2.txt");
    std::unordered_map<std::string,int> blocks{};

    std::vector<std::vector<char>> grid{};

    for(std::string line{}; std::getline(f, line);) {
        while (line.back() == '\r' || line.back() == '\n') line.pop_back();
        grid.emplace_back(line.begin(), line.end());
    }

    auto startx = -1;
    auto starty = -1;

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid.at(0).size(); x++) {
            if (grid.at(y).at(x) == '^') {
                starty = y;
                startx = x;
            }
        }
    }

    auto height = grid.size();
    auto width =  grid.at(0).size();

    // 0 up
    // 1 right
    // 2 down
    // 3 left
    auto direction = 0;
    auto curx = startx;
    auto cury = starty;
    while (true) {
        grid.at(cury).at(curx) = to_char(direction);
        auto [nexty, nextx] = advance(cury, curx, direction);

        if (!check_bounds(nexty, nextx, grid)) {
            break;
        }
        if (grid.at(nexty).at(nextx) == '#') {
            direction = rotate(direction);
            continue;
        } else {
            auto bak = grid.at(nexty).at(nextx);
            grid.at(nexty).at(nextx) = 'O';

            if (bak == '.' && detect_loop(cury, curx, direction, grid)) {
                // Make sure to not put obstacle on the start position
                if (!(nexty == starty && nextx == startx)) {
                    blocks[std::to_string(nexty) + "," + std::to_string(nextx)]++;
                }
            }

            grid.at(nexty).at(nextx) = bak;

            cury = nexty;
            curx = nextx;
        }
    }

    auto covered{0};
    std::cout << "==FINAL==" << '\n';
    for (auto row : grid) {
        for (auto el : row) {
            std::cout << el;
        }
        std::cout << '\n';

        covered += std::ranges::count(row, '^');
        covered += std::ranges::count(row, '>');
        covered += std::ranges::count(row, 'v');
        covered += std::ranges::count(row, '<');
    }

    //std::vector<std::vector<char>> obstacle_grid(height, std::vector(width, '.'));
    //for (auto pair : blocks) {
    //    auto [coord,count] = pair;
    //    auto _y = std::stoi(coord);
    //    auto _x = std::stoi(coord.substr(coord.find(',') + 1));
    //    std::cout << "obstacle at y="  << _y << " x=" << _x << '\n';
    //    obstacle_grid.at(_y).at(_x) = 'O';
    //    answer2++;
    //}

    //std::cout << "OBSTACLE GRID" << '\n';
    //printgrid(obstacle_grid);

    std::cout << "covered=" << covered << '\n';
    std::cout << "answer2=" << answer2 << '\n';

    return 0;
}
