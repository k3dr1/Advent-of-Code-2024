#include <algorithm>
#include <atomic>
#include <cstdint>
#include <execution>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <array>
#include <print>

using ll = std::int64_t;

auto next(ll x) -> ll {
    x = (64*x)^x;
    x = x%(1ll<<24);
    x = (x/32)^x;
    x = x%(1ll<<24);
    x = (x*2048)^x;
    x = x%(1ll<<24);
    return x;
}

auto max_bananas_with_kernel(const std::array<ll,4> kernel, 
                 const std::vector<std::vector<ll>>& changes,
                 const std::vector<std::vector<ll>>& money) {
    ll res {};
    for (ll i = 0; i < changes.size(); i++) {
        auto bought { false };
        for (ll j = 3; j < changes.at(i).size() && (!bought); j++) {
            if (   (kernel.at(0) == changes.at(i).at(j - 3))
                && (kernel.at(1) == changes.at(i).at(j - 2))
                && (kernel.at(2) == changes.at(i).at(j - 1))
                && (kernel.at(3) == changes.at(i).at(j - 0))) {
                res += money.at(i).at(j) % 10;
                bought = true;
            }
        }
    }

    return res;
}

struct hash { auto operator()(const std::array<ll, 4>& r) const { return (137 * r[0] ^ 31 * r[1] ^ 7 * r[2] ^ r[1]); } };
struct phash { auto operator()(const std::pair<std::array<ll,4>, ll>& p) const { return hash()(p.first) ^ p.second; } };

auto find_best_kernel(const std::vector<std::vector<ll>>& changes, const std::vector<std::vector<ll>>& money) {
    auto map = std::unordered_map<std::pair<std::array<ll,4>, ll>, ll, phash>{};
    auto other = std::unordered_map<std::array<ll,4>, ll, hash>{};
    for (ll i = 0; i < changes.size(); i++)
        for (ll j = 3; j < changes.at(0).size(); j++) {
            auto kernel = std::array<ll, 4> {
                changes.at(i).at(j - 3),
                changes.at(i).at(j - 2),
                changes.at(i).at(j - 1),
                changes.at(i).at(j - 0),
            };
            if (!map.contains(std::make_pair(kernel, i))) map[std::make_pair(kernel, i)] = money.at(i).at(j) % 10;
        }
    for (const auto& [p, money] : map) other[p.first] += money;
    return *std::ranges::max_element(other | std::views::values);
}

int main() {
    auto buyers = std::vector<ll>{};
    auto f = std::ifstream("input.txt");
    for (std::string line{}; std::getline(f, line);) {
        buyers.push_back(std::stoll(line));
    }

    ll ans1{};
    std::atomic_int64_t ans2{};
    auto money   = std::vector<std::vector<ll>>(buyers.size(), std::vector<ll>{});
    auto changes = std::vector<std::vector<ll>>(buyers.size(), std::vector<ll>{});

    for (auto num : buyers) {
        auto x = num;
        for (ll i = 0; i < 2000; i++) {
            x = next(x);
        }
        ans1+=x;
    }

    for (ll i = 0; i < buyers.size(); i++) {
        auto x = buyers.at(i);
        for (ll j = 0; j < 2000; j++) {
            auto oldx = x;
            x = next(x);
            money.at(i).push_back(x);
            changes.at(i).push_back((x%10) - (oldx%10));
        }
    }

    // Parallelized Bruteforce
    std::array<ll, 4> max_kernel { 0, 0, 0, 0 };
    for (ll i = -10; i <= 10; i++) {
        for (ll j = -10; j <= 10; j++) {
            auto r = std::views::cartesian_product(
                std::views::iota(-10, 11),
                std::views::iota(-10, 11));
            std::for_each(
                std::execution::seq,
                r.begin(),
                r.end(),
                [&](const auto& kl) {
                    const auto [k, l] = kl;
                    auto mb = max_bananas_with_kernel({ i, j, k, l }, changes, money);
                    if (ans2 < mb) {
                        ans2.store(mb);
                        max_kernel = { i, j, k, l };
                    }
                });
            std::println("done with i={} j={}", i, j);
        }
    }

    // One pass sequential with hashmap
    //ans2.store(find_best_kernel(changes, money));

    std::println("ans1 {}", ans1);
    std::println("ans2 {}", ll(ans2));

    return 0;
}
