#include <cstdint>
#include <fstream>
#include <limits>
#include <print>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using ll = std::int64_t;

struct hash {
    auto operator()(const std::pair<ll,ll>& p) const {
        return ((((std::uint64_t)p.first) << (32UL)) | ((std::uint64_t)p.second)); 
    }
};

bool check_bounds(int y, int x, const auto& grid) { return !((y < 0 || grid.size() <= y) || (x < 0 || grid.at(0).size() <= x)); }

auto comp = [](const auto& l, const auto& r) {
    auto [ly, lx, lc] = l;
    auto [ry, rx, rc] = r;
    return lc > rc;
};

auto search(std::pair<ll,ll> start, decltype(start) end, const auto& grid) {
    auto pq = std::priority_queue<std::tuple<ll,ll,ll>, std::vector<std::tuple<ll,ll,ll>>, decltype(comp)>{comp};
    auto visited = std::unordered_map<std::pair<ll,ll>, ll, hash>{};
    pq.push({start.first, start.second, 0});

    auto min = std::numeric_limits<ll>::max();

    while (!pq.empty()) {
        auto [y,x,cost] = pq.top();

        //std::println("at y={} x={} cost={}", y, x, cost);

        pq.pop();
        if (!check_bounds(y, x, grid)) continue;
        if (grid.at(y).at(x) == '#') continue;
        if (std::make_pair(y, x) == end) return cost;
        if (visited.contains(std::make_pair(y, x))) continue;
        visited[std::make_pair(y, x)]++;
        pq.push({ y - 1, x, cost + 1 });
        pq.push({ y, x + 1, cost + 1 });
        pq.push({ y + 1, x, cost + 1 });
        pq.push({ y, x - 1, cost + 1 });
    }
    return -1ll;
}

int main() {
    auto f = std::ifstream("input.txt");
    auto bytes = std::vector<std::pair<ll,ll>>{};

    for (std::string line{}; std::getline(f, line);) {
        auto byte = std::make_pair(std::stoll(line), std::stoll(line.substr(line.find(',')+1)));
        bytes.push_back(byte);
    }

    constexpr auto width = 70ll;
    auto grid = std::vector(width+1, std::vector(width+1, '.'));

    auto ans1{0ll};
    std::string ans2{};

    for (ll i = 0; i < bytes.size(); i++) {
        std::println("i={}", i);
        grid.at(bytes[i].second).at(bytes[i].first) = '#';
        if (i == 1024) {
            ans1 = search({0,0}, {width, width}, grid);
        } else {
            if (search({0,0}, {width, width}, grid) == -1) {
                ans2 = std::to_string(bytes[i].first) + "," + std::to_string(bytes[i].second);
                break;
            }
        }
    }

    std::println("ans1 {}", ans1);
    std::println("ans2 {}", ans2);

    return 0;
}
