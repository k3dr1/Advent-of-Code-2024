#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <optional>
#include <ranges>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include <print>

using ll = std::int64_t;

struct hash {
    auto operator()(const std::pair<ll,ll>& p) const {
        return ((((std::uint64_t)p.first) << (32UL)) | ((std::uint64_t)p.second)); 
    }
};

auto find_char(char ch, const std::vector<std::vector<ll>>& grid) {
    for (ll y = 0; y < grid.size(); y++) {
        for (ll x = 0; x < grid.size(); x++) { 
            if (grid.at(y).at(x) == ch) return std::make_pair(y, x);
        }
    }
    return std::make_pair(-1ll, -1ll);
}

constexpr ll hashtag = -1;

auto get_path(std::vector<std::vector<ll>>& grid) -> std::vector<std::pair<ll,ll>> {
    auto S = find_char('S', grid);
    auto E = find_char('E', grid);

    auto stk = std::stack<std::pair<ll,ll>>{};
    auto visited = std::unordered_map<std::pair<ll,ll>, ll, hash>{};;

    ll time = 0;
    stk.push({S});
    while (!stk.empty()) {
        auto current = stk.top();
        stk.pop();

        if (visited.contains(current)) continue;
        visited[current]++;
        grid.at(current.first).at(current.second) = time;

        time++;
        std::println("time:{}",time);
        if (current.first == E.first && current.second == E.second) return {};
        if (grid.at(current.first).at(current.second) == hashtag) continue;
        else {
            auto up = std::make_pair(current.first-1, current.second);
            auto right = std::make_pair(current.first, current.second+1);
            auto down = std::make_pair(current.first+1, current.second);
            auto left = std::make_pair(current.first, current.second-1);
            if (grid.at(up.first).at(up.second) != hashtag) stk.push(up);
            if (grid.at(right.first).at(right.second) != hashtag) stk.push(right);
            if (grid.at(down.first).at(down.second) != hashtag) stk.push(down);
            if (grid.at(left.first).at(left.second) != hashtag) stk.push(left);
        }
    }
    return {};
}

bool check_bounds(int y, int x, const auto& grid) { return !((y < 0 || grid.size() <= y) || (x < 0 || grid.at(0).size() <= x)); }

auto advance(std::pair<ll,ll>& pos, const std::vector<std::vector<ll>>& grid) {
    auto val = grid.at(pos.first).at(pos.second);
    auto up = std::make_pair(pos.first-1, pos.second);
    auto right = std::make_pair(pos.first, pos.second+1);
    auto down = std::make_pair(pos.first+1, pos.second);
    auto left = std::make_pair(pos.first, pos.second-1);
    if (grid.at(up.first).at(up.second) == (val+1)) {
        pos = up;
        return true;
    }
    if (grid.at(right.first).at(right.second) == (val+1)) {
        pos = right;
        return true;
    }
    if (grid.at(down.first).at(down.second) == (val+1)) {
        pos = down;
        return true;
    }
    if (grid.at(left.first).at(left.second) == (val+1)) {
        pos = left;
        return true;
    }
    return false;
}

auto part1(const std::vector<std::vector<ll>>& grid) {
    // picoseconds -> freq
    auto cheats = std::unordered_map<ll, ll>{};
    auto pos = find_char(0, grid);
    auto curtime = 0;

    do {
        //std::println("advanced to cell with value {}", grid.at(pos.first).at(pos.second));

        for (ll dy = -2; dy <= 2; dy++) {
            for (ll dx = -2; dx <= 2; dx++) {
                auto jump = std::make_pair(pos.first+dy, pos.second+dx);
                //std::println("created jump to {},{}", jump.first, jump.second);
                if (std::abs(dy)+std::abs(dx) == 2 
                    && check_bounds(jump.first, jump.second, grid) 
                    && (grid.at(jump.first).at(jump.second) != hashtag)) {
                        //std::println("checking {},{} ({}) to {},{} ({})",
                        //             pos.first, pos.second, grid.at(pos.first).at(pos.second),
                        //             jump.first, jump.second, grid.at(jump.first).at(jump.second));
                    if (auto diff = grid.at(jump.first).at(jump.second) - grid.at(pos.first).at(pos.second);
                        2 < diff) {
                        // subtract 2 picoseconds for noclip traversal
                        cheats[diff-2]++;

                        //std::println("found cheat from {},{} ({}) to {},{} ({}) diff {}",
                        //             pos.first, pos.second, grid.at(pos.first).at(pos.second),
                        //             jump.first, jump.second, grid.at(jump.first).at(jump.second),
                        //             diff
                        //             );
                    }
                }
            }
        }

    } while (advance(pos, grid));

    auto cheatvec = std::vector<std::pair<ll,ll>>(cheats.begin(), cheats.end());
    std::sort(cheatvec.begin(), cheatvec.end());

    auto res{0ll};
    for (auto& p : cheatvec) {
        std::println("There are {} cheats that save {} picoseconds", p.second, p.first);
        res += p.first >= 100 ? p.second : 0;
    }
    return res;
}

auto part2(const std::vector<std::vector<ll>>& grid) {
    // picoseconds -> freq
    auto cheats = std::unordered_map<ll, ll>{};
    auto pos = find_char(0, grid);
    auto curtime = 0;

    do {
        //std::println("advanced to cell with value {}", grid.at(pos.first).at(pos.second));

        for (ll dy = -20; dy <= 20; dy++) {
            for (ll dx = -20; dx <= 20; dx++) {
                auto jump = std::make_pair(pos.first+dy, pos.second+dx);
                //std::println("created jump to {},{}", jump.first, jump.second);
                auto absum = std::abs(dy)+std::abs(dx);
                if (2 <= absum
                    && absum <= 20
                    && check_bounds(jump.first, jump.second, grid)
                    && (grid.at(jump.first).at(jump.second) != hashtag)) {
                    // std::println("checking {},{} ({}) to {},{} ({})",
                    //              pos.first, pos.second, grid.at(pos.first).at(pos.second),
                    //              jump.first, jump.second, grid.at(jump.first).at(jump.second));
                    if (auto diff = grid.at(jump.first).at(jump.second) - grid.at(pos.first).at(pos.second);
                        absum < diff) {
                        // subtract 2 picoseconds for noclip traversal
                        cheats[diff-absum]++;

                        //std::println("found cheat from {},{} ({}) to {},{} ({}) diff {}",
                        //             pos.first, pos.second, grid.at(pos.first).at(pos.second),
                        //             jump.first, jump.second, grid.at(jump.first).at(jump.second),
                        //             diff
                        //             );
                    }
                }
            }
        }

    } while (advance(pos, grid));

    auto cheatvec = std::vector<std::pair<ll,ll>>(cheats.begin(), cheats.end());
    std::sort(cheatvec.begin(), cheatvec.end());

    auto res{0ll};
    for (auto& p : cheatvec) {
        std::println("There are {} cheats that save {} picoseconds", p.second, p.first);
        res += p.first >= 100 ? p.second : 0;
    }
    return res;
}

int main() {

    auto f = std::ifstream("input.txt");
    auto grid = std::vector<std::vector<ll>>{};

    for (std::string line {}; std::getline(f, line);) {
        grid.emplace_back(line.begin(), line.end());
    }

    for (auto& row : grid) {
        for (auto& val : row) {
            if (val == '#') {
                val = hashtag;
            }
        }
    }

    auto path = get_path(grid);

    for (auto& row : grid) {
        for (auto& val : row) {
            if (val != -1) {
                std::print("{:>4}", val);
            }
            else {
                std::print("   #");
            }
        }
        std::println("");
    }

    auto ans1 = part1(grid);
    std::println("Ans1 {}", ans1);

    auto ans2 = part2(grid);
    std::println("Ans2 {}", ans2);

    return 0;
}
