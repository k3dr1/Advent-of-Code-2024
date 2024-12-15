#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <print>

using ll = std::int64_t;

auto printgrid(const auto& grid) {
    for (const auto& row : grid) {
        for (const auto& val : row) {
            std::print("{}", val);
        }
        std::println("");
    }
}

auto chartovec(char ch) {
    switch (ch) {
        case '^': return std::make_pair(-1,  0);
        case '>': return std::make_pair( 0,  1);
        case 'v': return std::make_pair( 1,  0);
        case '<': return std::make_pair( 0, -1);
        default: {
            std::println("Error in chartovec");
            std::exit(-1);
        }
    }
}

auto add(std::pair<ll,ll> a, decltype(a) b) -> decltype(a) {
    return {a.first + b.first, a.second + b.second};
}

bool check_bounds(int y, int x, const auto& grid) { return !((y < 0 || grid.size() <= y) || (x < 0 || grid.at(0).size() <= x)); }

auto find_robot(const auto& grid) -> std::pair<ll,ll> {
    for (ll y = 0; y < grid.size(); y++) {
        for (ll x = 0; x < grid.at(0).size(); x++) {
            if (grid.at(y).at(x) == '@') {
                return std::make_pair(y, x);
            }
        }
    }
    return std::make_pair(-1, -1);
}

auto try_move(char dirchar, auto& grid) -> bool {
    auto dirvec = chartovec(dirchar);
    auto original_robot = find_robot(grid);
    auto pos = original_robot;
    auto can_move{true};
    while (check_bounds(pos.first, pos.second, grid)) {
        if (grid.at(pos.first).at(pos.second) == '#') {
            can_move = false;
            break;
        }
        if (grid.at(pos.first).at(pos.second) == '.') {
            can_move = true;
            break;
        }
        pos = add(pos, dirvec);
    }
    if (can_move) {
        auto revdir = std::make_pair(-1*dirvec.first, -1*dirvec.second);
        auto before = add(pos,revdir);
        while (pos != original_robot) {
            std::swap(grid.at(pos.first).at(pos.second), grid.at(before.first).at(before.second));
            pos = add(pos,revdir);
            before = add(before,revdir);
        }
        return true;
    }
    return false;
}

auto can_box_move_vert2(char dirchar, std::pair<ll, ll> pos, const std::vector<std::vector<char>>& grid) -> bool {
    auto above = add(pos, chartovec(dirchar));
    auto above_right = add(above, chartovec('>'));
    if (!check_bounds(above.first, above.second, grid)) return false;
    if (!check_bounds(above_right.first, above_right.second, grid)) return false;
    auto above_val = grid.at(above.first).at(above.second);
    auto above_right_val = grid.at(above_right.first).at(above_right.second);

    if (above_val == '.' && above_right_val == '.') {
        return true;
    }
    if (above_val == '[' && above_right_val == ']') {
        return can_box_move_vert2(dirchar, above, grid);
    }
    if (above_val == ']' && above_right_val == '[') {
        return can_box_move_vert2(dirchar, add(above, chartovec('<')), grid)
            && can_box_move_vert2(dirchar, above_right, grid);
    }
    if (above_val == ']' && above_right_val == '.') {
        return can_box_move_vert2(dirchar, add(above, chartovec('<')), grid);
    }
    if (above_val == '.' && above_right_val == '[') {
        return can_box_move_vert2(dirchar, above_right, grid);
    }
    if (above_val == '#' || above_right_val == '#') {
        return false;
    }

    std::println("Unhandled situation in can_move_vert2: {} {},{}", dirchar, pos.first, pos.second);
    std::exit(-1);
}

auto can_robot_move_vert2(char dirchar, const std::vector<std::vector<char>>& grid) {
    auto robot = find_robot(grid);
    auto above = add(robot, chartovec(dirchar));
    if (!check_bounds(above.first, above.second, grid)) return false;
    if (grid.at(above.first).at(above.second) == '.') return true;
    if (grid.at(above.first).at(above.second) == '#') return false;
    if (grid.at(above.first).at(above.second) == '[') return can_box_move_vert2(dirchar, above, grid);
    if (grid.at(above.first).at(above.second) == ']') return can_box_move_vert2(dirchar, add(above, chartovec('<')), grid);
    return false;
}

auto box_move2(char dirchar, std::pair<ll, ll> box, std::vector<std::vector<char>>& grid) -> bool {
    auto box_right = add(box, chartovec('>'));
    auto above = add(box, chartovec(dirchar));
    auto above_right = add(above, chartovec('>'));
    if (!check_bounds(above.first, above.second, grid) 
        || !check_bounds(above_right.first, above_right.second, grid)) {
        std::println("Unexpected out-of-bounds in ripple_move2");
    }
    auto above_val = grid.at(above.first).at(above.second);
    auto above_right_val = grid.at(above_right.first).at(above_right.second);

    if (above_val == '.' && above_right_val == '.') {
        std::swap(grid.at(box.first).at(box.second), 
                  grid.at(above.first).at(above.second));
        std::swap(grid.at(box_right.first).at(box_right.second), 
                  grid.at(above_right.first).at(above_right.second));
        return true;
    }

    if (above_val == '[' && above_right_val == ']') {
        box_move2(dirchar, above, grid);
        std::swap(grid.at(box.first).at(box.second), 
                  grid.at(above.first).at(above.second));
        std::swap(grid.at(box_right.first).at(box_right.second), 
                  grid.at(above_right.first).at(above_right.second));
        return true;
    }

    if (above_val == ']' && above_right_val == '[') {
        box_move2(dirchar, add(above, chartovec('<')), grid);
        box_move2(dirchar, above_right, grid);
        std::swap(grid.at(box.first).at(box.second), 
                  grid.at(above.first).at(above.second));
        std::swap(grid.at(box_right.first).at(box_right.second), 
                  grid.at(above_right.first).at(above_right.second));
        return true;
    }

    if (above_val == ']' && above_right_val == '.') {
        box_move2(dirchar, add(above, chartovec('<')), grid);
        std::swap(grid.at(box.first).at(box.second), 
                  grid.at(above.first).at(above.second));
        std::swap(grid.at(box_right.first).at(box_right.second), 
                  grid.at(above_right.first).at(above_right.second));
        return true;
    }

    if (above_val == '.' && above_right_val == '[') {
        box_move2(dirchar, above_right, grid);
        std::swap(grid.at(box.first).at(box.second), 
                  grid.at(above.first).at(above.second));
        std::swap(grid.at(box_right.first).at(box_right.second), 
                  grid.at(above_right.first).at(above_right.second));
        return true;
    }

    std::println("Unhandled situation in box_move2: {} {},{}", dirchar, box.first, box.second);
    std::exit(-1);
}

auto try_robot_move(char dirchar, std::vector<std::vector<char>>& grid) {
    auto robot = find_robot(grid);
    auto above = add(robot, chartovec(dirchar));
    if (grid.at(above.first).at(above.second) == '.') {
        std::swap(grid.at(robot.first).at(robot.second), grid.at(above.first).at(above.second));
        return true;
    } 
    if (grid.at(above.first).at(above.second) == '#') {
        return false;
    }
    if (grid.at(above.first).at(above.second) == '[') {
        if (box_move2(dirchar, above, grid)) {
            std::swap(grid.at(robot.first).at(robot.second), grid.at(above.first).at(above.second));
            return true;
        }
        return false;
    }
    if (grid.at(above.first).at(above.second) == ']') {
        if (box_move2(dirchar, add(above, chartovec('<')), grid)) {
            std::swap(grid.at(robot.first).at(robot.second), grid.at(above.first).at(above.second));
            return true;
        }
        return false;
    }
    return false;
}

int main() {
    auto f = std::ifstream("input.txt");
    auto grid = std::vector<std::vector<char>>{};
    std::string moves{};

    for (std::string line{}; std::getline(f, line);) {
        if (line.size() < 3) break;
        grid.emplace_back(line.begin(), line.end());
    }
    for (std::string line{}; std::getline(f, line);) {
        while (line.back() == '\r' || line.back() == '\n') line = line.substr(0, line.size() - 1);
        moves.append(line);
    }

    ll ans1{0};
    ll ans2{0};

    // expanding grid
    decltype(grid) wide_grid = {};
    for (const auto& row : grid) {
        std::string wide_row{""};
        for (char val : row) {
            switch (val) {
                case '#': wide_row += ("##");break;
                case 'O': wide_row += ("[]");break;
                case '.': wide_row += ("..");break;
                case '@': wide_row += ("@.");break;
            }
        }
        wide_grid.emplace_back(wide_row.begin(), wide_row.end());
        std::println("wide_row: {}", wide_row);
    }

    // Part 1
    for (ll i = 0; i < moves.size(); i++) {
        try_move(moves.at(i), grid);
    }
    for (ll y = 0; y < grid.size(); y++) {
        for (ll x = 0; x < grid.at(0).size(); x++) {
            if (grid.at(y).at(x) == 'O') {
                ans1 += y*100 + x;
            }
        }
    }

    for (ll i = 0; i < moves.size(); i++) {
        const char move = moves.at(i);
        if (move == '<' || move == '>') try_move(move, wide_grid);
        else if (move == '^' || move == 'v') {
            if (can_robot_move_vert2(move, wide_grid)) {
                try_robot_move(move, wide_grid);
            }
        } else std::println("Unexpected move {}", move);
    }
    for (ll y = 0; y < wide_grid.size(); y++) {
        for (ll x = 0; x < wide_grid.at(0).size(); x++) {
            if (wide_grid.at(y).at(x) == '[') {
                ans2 += y*100 + x;
            }
        }
    }

    std::println("Answer1: {}", ans1);
    std::println("Answer2: {}", ans2);

}
