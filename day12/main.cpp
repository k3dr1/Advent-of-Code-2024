#include <array>
#include <cstdint>
#include <fstream>
#include <print>
#include <ranges>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using ll = std::int64_t;
using u64 = std::uint64_t;

constexpr std::array<std::pair<ll, ll>, 4> directions = {
    std::make_pair(-1, 0),
    std::make_pair(0, 1),
    std::make_pair(1, 0),
    std::make_pair(0, -1),
};


struct p_hash {
    auto operator()(const std::pair<ll,ll>& p) const {
        return ((((u64)p.first) << (32UL)) | ((u64)p.second)); 
    }
};

struct pp_hash {
    auto operator()(const std::pair<std::pair<ll,ll>,std::pair<ll,ll>>& p) const {
        return p.first.first ^ p.first.second ^ p.second.first ^ p.second.second;
    }
};

auto add(const std::pair<ll, ll>& a, const std::pair<ll, ll>& b) -> std::pair<ll, ll> { return { a.first + b.first, a.second + b.second }; }

bool check_bounds(int y, int x, const auto& grid) { return !((y < 0 || grid.size() <= y) || (x < 0 || grid.at(0).size() <= x)); }

std::unordered_map<std::pair<ll,ll>, bool, p_hash> global_visited1{};
auto find_cost1(int y, int x, const std::vector<std::vector<char>>& grid) {
    const auto type{grid.at(y).at(x)};
    ll perimeter{};
    ll area{};
    std::unordered_map<std::pair<ll,ll>,bool, p_hash> visited{};
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
    auto left = std::make_pair(add(across.second, directions[diridx]), across.second);
    auto go_on = std::make_pair(add(across.first, directions[diridx]), add(across.second, directions[diridx]));
    auto right = std::make_pair(across.first, add(across.first, directions[diridx]));
    return std::array<std::pair<decltype(go_on),ll>, 3> { 
        std::make_pair(left,  (diridx+3)%4), // minus 1
        std::make_pair(go_on, (diridx)),
        std::make_pair(right, (diridx+1)%4) };
}

bool is_horizontal(std::pair<std::pair<ll,ll>, std::pair<ll,ll>> fence) { return (fence.first.second == fence.second.second); }

std::unordered_map<std::pair<ll,ll>, bool, p_hash> global_visited2{};
auto find_cost2(int y, int x, const std::vector<std::vector<char>>& grid) {
    const auto type{grid.at(y).at(x)};
    ll area{};
    std::unordered_map<std::pair<ll,ll>,bool, p_hash> visited{};
    std::unordered_map<std::pair<std::pair<ll, ll>, std::pair<ll, ll>>, ll, pp_hash> fences{};
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
    ll num_sides{0};
    while (true) {
        std::unordered_map<std::pair<std::pair<ll, ll>, std::pair<ll, ll>>, ll, pp_hash> fences2{};
        for (auto& [fence, count] : fences) {
            if (count == true)
                fences2[fence] = true;
        }
        std::swap(fences, fences2);
        auto num_sides2 { 0 };
        if (fences.empty()) break;
        if (!fences.empty()) {
            auto start_fence = (*std::ranges::begin(fences | std::views::take(1))).first;
            auto diridx = is_horizontal(start_fence) ? 1 : 2;
            auto current_fence = start_fence;
            while ((current_fence != start_fence) || (num_sides2 < 3)) {
                fences[current_fence] = false;
                auto turned { false };
                for (auto& [fence, d] : generate_next_fences(current_fence, diridx)) {
                    if (fences.contains(fence)) {
                        current_fence = fence;
                        turned = (d != diridx);
                        diridx = d;
                        break;
                    }
                }
                num_sides2 += turned;
                // reversing
                if (current_fence == start_fence && (num_sides2) == 2) {
                    num_sides2 = 0;
                }
            }
        }
        num_sides += num_sides2;
    }
    return num_sides*area;
}

int main() {
    auto f = std::ifstream("input.txt");

    auto grid = std::vector<std::vector<char>>{};
    for (std::string line{}; std::getline(f, line); ) {
        grid.emplace_back(line.begin(), line.end());
    }

    ll ans1{0};
    ll ans2{0};

    for (ll y = 0; y < grid.size(); y++) {
        for (ll x = 0; x < grid.at(0).size(); x++) {
            if (!global_visited1.contains({ y, x })) {
                ans1 += find_cost1(y, x, grid);
            }
            if (!global_visited2.contains({ y, x })) {
                ans2 += find_cost2(y, x, grid);
            }
        }
    }

    std::println("Answer1 {}", ans1);
    std::println("Answer2 {}", ans2);

    return 0;
}
