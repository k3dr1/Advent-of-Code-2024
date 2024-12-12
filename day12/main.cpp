#include <array>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <ranges>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <print>

// backup
    //switch (diridx) {
    //case 0: {
    //        auto go_on = std::make_pair(add(across.first, UP), add(across.second, UP));
    //        auto left = std::make_pair(add(across.second, UP), across.second);
    //        auto right = std::make_pair(across.first, add(across.first, UP));
    //        return {go_on, left, right};
    //    break;
    //}
    //case 1: {
    //        auto go_on = std::make_pair(add(across.first, RIGHT), add(across.second, RIGHT));
    //        auto left = std::make_pair(add(across.second, RIGHT), across.second);
    //        auto right = std::make_pair(across.first, add(across.first, RIGHT));
    //        return {go_on, left, right};
    //    break;
    //}
    //case 2: {
    //        auto go_on = std::make_pair(add(across.first, DOWN), add(across.second, DOWN));
    //        auto left = std::make_pair(add(across.second, DOWN), across.second);
    //        auto right = std::make_pair(across.first, add(across.first, DOWN));
    //        return {go_on, left, right};
    //    break;
    //}
    //case 3: {
    //        auto go_on = std::make_pair(add(across.first, LEFT), add(across.second, LEFT));
    //        auto left = std::make_pair(add(across.second, LEFT), across.second);
    //        auto right = std::make_pair(across.first, add(across.first, LEFT));
    //        return {go_on, left, right};
    //    break;
    //}
    //}

using ll = std::int64_t;
using u64 = std::uint64_t;

constexpr std::array<std::pair<ll, ll>, 4> directions = {
    std::make_pair(-1, 0),
    std::make_pair(0, 1),
    std::make_pair(1, 0),
    std::make_pair(0, -1),
};

constexpr std::array<std::string, 4> direction_names = {
    "UP",
    "RIGHT",
    "DOWN",
    "LEFT",
};

constexpr auto& UP    = directions[0];
constexpr auto& RIGHT = directions[1];
constexpr auto& DOWN  = directions[2];
constexpr auto& LEFT  = directions[3];

struct hash {
    auto operator()(const std::pair<ll,ll>& p) const {
        return ((((u64)p.first) << (32UL)) | ((u64)p.second)); 
    }
};

struct hash2 {
    auto operator()(const std::pair<std::pair<ll,ll>,std::pair<ll,ll>>& p) const {
        return p.first.first ^ p.first.second ^ p.second.first ^ p.second.second;
    }
};

auto add(const std::pair<ll, ll>& a, const std::pair<ll, ll>& b) -> std::pair<ll, ll> {
    return { a.first + b.first, a.second + b.second };
}

bool check_bounds(int y, int x, const auto& grid) { 
    return !((y < 0 || grid.size() <= y) || (x < 0 || grid.at(0).size() <= x)); 
}

std::unordered_map<std::pair<ll,ll>, bool, hash> global_visited1{};
auto find_cost1(int y, int x, const std::vector<std::vector<char>>& grid) {
    const auto type{grid.at(y).at(x)};
    ll perimeter{};
    ll area{};
    std::unordered_map<std::pair<ll,ll>,bool, hash> visited{};
    std::stack<std::pair<ll,ll>> stk{};
    stk.push(std::make_pair(y,x));
    while (!stk.empty()) {
        auto current = stk.top();
        stk.pop();
        if (visited.contains(current)) continue;
        area++;
        visited[current] = true;
        for (const auto& dir : directions) {
            auto nb = add(dir, current);
            if (visited.contains(nb)) continue;
            if (!check_bounds(nb.first, nb.second, grid)) {
                perimeter++;
                continue;
            }
            if (grid.at(nb.first).at(nb.second) != type) {
                perimeter++;
            } else {
                if (!visited.contains(nb)) stk.push(nb);
            }
        }
    }
    for (const auto& p : visited) {
        global_visited1[p.first] = true;
    }
    return perimeter*area;
}

auto generate_next_fences(std::pair<std::pair<ll, ll>, std::pair<ll, ll>> across, ll diridx) {
    // 0 up
    // 1 right
    // 2 down
    // 3 left
    // first - inner side
    // second - outer side

    auto left = std::make_pair(add(across.second, directions[diridx]), across.second);
    auto go_on = std::make_pair(add(across.first, directions[diridx]), add(across.second, directions[diridx]));
    auto right = std::make_pair(across.first, add(across.first, directions[diridx]));
    return std::array<std::pair<decltype(go_on),ll>, 3> { 
        std::make_pair(left,  (diridx+3)%4), // minus 1
        std::make_pair(go_on, (diridx)),
        std::make_pair(right, (diridx+1)%4) };
}

bool is_horizontal(std::pair<std::pair<ll,ll>, std::pair<ll,ll>> fence) {
    // compare x-coordinates, should be the same if horizontal
    return (fence.first.second == fence.second.second);
}

std::unordered_map<std::pair<ll,ll>, bool, hash> global_visited2{};
auto find_cost2(int y, int x, const std::vector<std::vector<char>>& grid) {
    const auto type{grid.at(y).at(x)};
    ll area{};
    std::unordered_map<std::pair<ll,ll>,bool, hash> visited{};
    std::unordered_map<std::pair<std::pair<ll, ll>, std::pair<ll, ll>>, ll, hash2> fences{};
    std::stack<std::pair<ll,ll>> stk{};
    stk.push(std::make_pair(y,x));
    while (!stk.empty()) {
        auto current = stk.top();
        stk.pop();
        if (visited.contains(current)) continue;
        area++;
        visited[current] = true;
        for (const auto& dir : directions) {
            auto nb = add(dir, current);
            if (visited.contains(nb)) continue;
            if (!check_bounds(nb.first, nb.second, grid)) {
                fences[{current,nb}]++;
                continue;
            }
            if (grid.at(nb.first).at(nb.second) != type) {
                fences[{current,nb}]++;
            } else {
                if (!visited.contains(nb)) stk.push(nb);
            }
        }
    }
    for (const auto& p : visited) {
        global_visited2[p.first] = true;
    }

    // Calculating the sides
    std::println("Found fences");
    for (const auto& [fence, count] : fences) {
        std::println("fence: [{} {}, {} {}]",
            fence.first.first,
            fence.first.second,
            fence.second.first,
            fence.second.second);
    }

    ll num_sides{0};
    auto start_fence = (*std::ranges::begin(fences | std::views::take(1))).first;
    // go right if horizontal, down if vertical
    auto diridx = is_horizontal(start_fence) ? 1 : 2;
    std::println("start_fence: [{} {}, {} {}] dir={}",
        start_fence.first.first,
        start_fence.first.second,
        start_fence.second.first,
        start_fence.second.second,
        diridx);
    auto current_fence = start_fence;

    for (auto& [fence, d] : generate_next_fences(start_fence, diridx)) {
        std::println(" checking_fence: [{} {}, {} {}] dir={}",
            fence.first.first,
            fence.first.second,
            fence.second.first,
            fence.second.second,
            d);
        if (fences.contains(fence)) {
            current_fence = fence;
            num_sides += (diridx != d);
            diridx = d;
            break;
        }
    }

    if (current_fence == start_fence) {
        std::println("Something went terribly wrong, couldn't advance from the start_fence");
        std::exit(-1);
    }

    auto loop_complete{false};
    while (((current_fence != start_fence) && (!loop_complete)) || (num_sides < 4)) {
        std::println("current_fence: [{} {}, {} {}] dir={}",
                     current_fence.first.first,
                     current_fence.first.second,
                     current_fence.second.first,
                     current_fence.second.second,
                     diridx
                     );
        auto advance_forth = [](auto&& fence, ll didx) {
            return std::make_pair(add(fence.first, directions[didx]),
                                  add(fence.second, directions[didx]));};
        while (fences.contains(advance_forth(current_fence, diridx))) {
            current_fence = advance_forth(current_fence, diridx);
            if (current_fence == start_fence) loop_complete = true;
        }
        if (loop_complete) {
            std::println("Loop complete!");
            break;
        }

        auto next_found{false};
        for (auto& [fence, d] : generate_next_fences(current_fence, diridx)) {
            std::println(" checking_fence: [{} {}, {} {}] dir={}",
                fence.first.first,
                fence.first.second,
                fence.second.first,
                fence.second.second,
                d);
            if (fences.contains(fence)) {
                current_fence = fence;
                num_sides++;
                diridx = d;
                next_found = true;
                break;
            }
        }
        if (!next_found) {
            std::println("Next not found");
            std::exit(-1);
        }
        if (current_fence == start_fence && (num_sides) == 2) {
            num_sides = 0;
            //reversed = true;
        }
    }
    std::println("Type: {}", type);
    std::println("Num sides: {}", num_sides);
    std::println("Area: {}", area);
    std::println("");
    return num_sides*area;
}

int main() {
    auto f = std::ifstream("ex4.txt");

    auto grid = std::vector<std::vector<char>>{};
    for (std::string line{}; std::getline(f, line); ) {
        grid.emplace_back(line.begin(), line.end());
    }

    ll ans1{0};

    for (ll y = 0; y < grid.size(); y++) {
        for (ll x = 0; x < grid.at(0).size(); x++) {
            if (global_visited2.contains({y,x})) continue;
            ans1 += find_cost2(y, x, grid);
        }
    }

    std::println("Answer1 {}", ans1);

    return 0;
}
