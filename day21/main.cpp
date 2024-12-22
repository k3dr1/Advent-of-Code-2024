#include <algorithm>
#include <cstdint>
#include <array>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <limits>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <print>

using ll = std::int64_t;

struct vec2 {
    ll x;
    ll y;
    auto operator+(const auto& other) const { return vec2 { this->x + other.x, this->y + other.y }; }
    auto operator-(const auto& other) const { return vec2 { this->x - other.x, this->y - other.y }; }
    auto operator*(const ll& scalar) const { return vec2 { scalar * (this->x), scalar * (this->y) }; }
    bool operator==(const auto& other) const { return (this->x == other.x) && (this->y == other.y); }
};

struct vhash {
    bool operator()(const vec2& v) const {
        return (v.x<<32) ^ (v.y); 
    }
};

constexpr char bad = '#';

const auto numpad = std::vector<std::vector<char>> {
    { '7', '8', '9' },
    { '4', '5', '6' },
    { '1', '2', '3' },
    { '#', '0', 'A' },
};

const auto dirpad = std::vector<std::vector<char>> {
    { '#', '^', 'A' },
    { '<', 'v', '>' },
};

constexpr std::array<vec2, 4> dirs = {
    vec2 { 0, -1 },
    vec2 { 1,  0 },
    vec2 { 0,  1 },
    vec2 { -1, 0 }
};

auto dir2char(vec2 dir) -> char {
    if (dir == vec2{ 0, -1 }) return '^';
    if (dir == vec2{ 1,  0 }) return '>';
    if (dir == vec2{ 0,  1 }) return 'v';
    if (dir == vec2{ -1, 0 }) return '<';
    std::unreachable();
    std::println("ERROR in dir2char");
    std::exit(-1);
}

auto char2dir(const char ch) -> vec2 {
    if (ch == '^') return vec2{ 0, -1 };
    if (ch == '>') return vec2{ 1,  0 };
    if (ch == 'v') return vec2{ 0,  1 };
    if (ch == '<') return vec2{ -1, 0 };
    std::unreachable();
    std::println("ERROR in dir2char");
    std::exit(-1);
}

bool check_bounds(int y, int x, const auto& grid) { return !((y < 0 || grid.size() <= y) || (x < 0 || grid.at(0).size() <= x)); }

auto find_char(char ch, const std::vector<std::vector<char>>& grid) -> vec2 {
    for (ll y = 0; y < grid.size(); y++) 
        for (ll x = 0; x < grid.at(0).size(); x++) 
            if (grid.at(y).at(x) == ch) return {x,y};
    return {-1,-1};
}

auto get_paths(char a, char b, const std::vector<std::vector<char>>& grid) {
    auto apos = find_char(a, grid);
    auto bpos = find_char(b, grid);
    auto taking = bpos - apos;
    const auto manh_dist = std::abs(taking.x) + std::abs(taking.y);
    auto paths = std::vector<std::string>{};

    using T = std::tuple<vec2, ll, std::string>;
    auto comp = [](const T& l, const T& r) {
        const auto& [lpos, ldist, lpath] = l;
        const auto& [rpos, rdist, rpath] = r;
        return ldist > rdist;
    };

    auto pq = std::priority_queue<T, std::vector<T>, decltype(comp)> { comp };
    auto visited = std::unordered_map<vec2, ll, vhash>{};

    pq.push({apos, 0, ""});
    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        const auto& [pos, dist, path] = top;
        if (manh_dist < dist) continue;
        if (!check_bounds(pos.y, pos.x, grid)) continue;
        if (grid.at(pos.y).at(pos.x) == bad) continue;
        if (visited.contains(pos) && visited.at(pos) < dist) continue;
        visited[pos] = dist;
        if (pos == bpos) {
            paths.push_back(path+'A');
        }
        else {
            for (const auto& dir : dirs) {
                pq.push({pos+dir, dist+1, path+dir2char(dir)});
            }
        }
    }
    return paths;
}

auto convert(const std::string& code, const auto& grid) {
    std::vector<std::string> res{};
    auto initial_paths = get_paths('A', code.at(0), grid);
    res.insert(res.end(), initial_paths.begin(), initial_paths.end());
    for (ll i = 1; i < code.size(); i++) {
        auto newres = std::vector<std::string>{};
        auto paths = get_paths(code.at(i-1), code.at(i), grid);
        for (const auto& p : paths) {
            for (const auto& r : res) {
                newres.push_back(r+p);
            }
        }
        res = newres;
    }

    auto min_length = std::numeric_limits<ll>::max();
    for (const auto& r : res) {
        min_length = std::min(min_length, ll(r.size()));
    }
    auto shortest = std::vector<std::string>{};
    for (const auto& r : res) {
        if (r.size() == min_length) shortest.push_back(r);
    }
    return shortest;
}

auto decode_dir(const std::string& code) {
    auto pos = vec2{2,0};
    std::string res{};
    for (char ch : code) {
        //std::println("decoded: {}", res);
        //std::println("pos {},{}", pos.x, pos.y);
        switch (ch) {
            case 'A': {
                res += dirpad.at(pos.y).at(pos.x);
                break;
            }
            case '^': {
                pos = pos + vec2{0, -1};
                break;
            }
            case '>': {
                pos = pos + vec2{1, 0};
                break;
            }
            case 'v': {
                pos = pos + vec2{0, 1};
                break;
            }
            case '<': {
                pos = pos + vec2{-1, 0};
                break;
            }
        }
    }
    return res;
}

auto srt_cmp = [](const std::string& l, const std::string& r) {return l.size() < r.size(); };
auto translate1(const std::string& _code) {
    auto d0s = std::vector<std::string>{};
    auto d1s = std::vector<std::string>{};
    auto d2s = std::vector<std::string>{};

    d0s = convert(_code, numpad);
    for (auto& code : d0s) {
        auto c = convert(code, dirpad);
        d1s.insert(d1s.end(), c.begin(), c.end());
    }
    std::sort(d1s.begin(), d1s.end(), srt_cmp);

    for (auto& code : d1s) {
        auto c = convert(code, dirpad);
        d2s.insert(d2s.end(), c.begin(), c.end());
    }
    std::sort(d2s.begin(), d2s.end(), srt_cmp);

    auto front = d2s.at(0);
    ll length = front.length();
    ll numeric = std::stoll(_code);

    std::println("code {}: {}*{}={}", _code, length, numeric, length*numeric);

    std::println("{}", decode_dir(decode_dir(front)));
    std::println("{}", decode_dir(front));
    std::println("{}", front);

    return length*numeric;
}

auto hits_bad_key(const std::string& code, char a, char b, const auto& grid) {
    if (code.size() == 1 && code.at(0) == 'A') return false;
    //std::println("hits bad key called with {} {} {}", code, a, b);
    auto apos = find_char(a, grid);
    auto bpos = find_char(b, grid);
    auto pos = apos;
    for (const char ch : code ) {
        if (ch == 'A') return false;
        pos = pos + char2dir(ch);
        if (!check_bounds(pos.y, pos.x, grid)) {
            std::println("Out of bounds: {},{}", pos.x, pos.y);
        }
        if (grid.at(pos.y).at(pos.x) == bad) return true;
    }
    return false;
}

auto hits_bad_key2(const std::string& code, char a, char b, const auto& grid) {
    switch (a) {
        case '0': {
            return code.starts_with("<");
            break;
        }
        case 'A': {
            return code.starts_with("<<");
            break;
        }
        case '1': {
            return code.starts_with("v");
            break;
        }
        case '4': {
            return code.starts_with("vv");
            break;
        }
        case '7': {
            return code.starts_with("vvv");
            break;
        }
        case '<': {
            return code.starts_with("^");
            break;
        }
        case '^': {
            return code.starts_with("<");
            break;
        }
    }
    return false;
}

auto dirpath(char a, char b, const auto& grid) -> std::string {

    if (a == 'A' && b == '<') return "v<<A";
    if (a == '^' && b == '<') return "v<A";

    if (a == '<' && b == 'A') return ">>^A";
    if (a == '<' && b == '^') return ">^A";

    std::string res{};
    auto vec = find_char(b, grid) - find_char(a, grid);
    // horizontal
    res += std::string(std::abs(vec.x), vec.x >= 0 ? '>' : '<');
    // vertical
    res += std::string(std::abs(vec.y), vec.y >= 0 ? 'v' : '^');
    // then accept
    res += 'A';
    return res;
}

auto conv = [](const char ch) {
    switch (ch) {
        case '^': return 1;
        case '>': return 2;
        case 'v': return 3;
        case '<': return 4;
    }
    std::unreachable();
};
auto mp = [](const char l, const char r){
    return conv(l) > conv(r);
};
auto amp = [](const char l, const char r){
    return conv(l) < conv(r);
};
auto get_shortest_path(char a, char b, const auto& grid) {
    //std::println("get_shortest_path called with {} {}", a, b);
    //auto paths = get_paths(a, b, grid);
    //auto orig_path = get_paths(a, b, grid).at(paths.size()-1);
    auto path = dirpath(a, b, grid);
    std::sort(path.begin(), path.begin() + path.find('A'), mp);
    if (hits_bad_key2(path, a, b, grid)) {
        std::sort(path.begin(), path.begin() + path.find('A'), amp);
        if (hits_bad_key2(path, a, b, grid)) {
            std::println("MAAAN IT STILL HIT THE BAD KEY {} from {} to {}", path, a, b);
        }
    }
    return path;
}

auto breakdown(const std::string& code, ll degree) {
    //std::println("breakdown called with {} {}", code, degree);
    if (degree == 0) {
        if (code.size() == 2) {
            auto path = get_shortest_path(code.at(0), code.at(1), dirpad);
            return path;
        } else {
            std::println("Something went wrong");
        }
    }
    else {
        std::string res{};
        res += breakdown(std::string("A") + code.at(0), degree - 1);
        for (ll i = 1; i < code.size(); i++) {
            res += breakdown(std::string(1, code.at(i-1)) + code.at(i), degree-1);
        }
        return res;
    }
    std::unreachable();
}

auto breakdown2(const std::string& code, ll degree, const auto& grid) {
    //std::println("breakdown2 called with {} {}", code, degree);
    auto res = code;
    for (ll i = 0; i < degree; i++) {
        //std::println("iteration {}", i);
        std::string newres{};
        newres += get_shortest_path('A', res.at(0), grid);
        for (ll j = 1; j < res.size(); j++) {
            auto shortest_path = get_shortest_path(res.at(j-1), res.at(j), grid);
            newres += shortest_path;
        }
        res = newres;
    }
    return res;
}

struct hash {
    auto operator()(const std::pair<std::string,ll>& p) const {
        return ((std::hash<std::string>()(p.first) << (32UL)) | ((std::uint64_t)p.second)); 
    }
};

auto memo = std::unordered_map<std::pair<std::string, ll>, ll, hash>{};
auto recurse(const std::string& code, ll iterations) -> ll {
    if (iterations == 0) {
        return code.size();
    }
    if (memo.contains(std::make_pair(code, iterations))) return memo[std::make_pair(code, iterations)];
    ll num{};
    const auto padded = std::string("A") + code;
    for (ll i = 1; i < padded.size(); i++) {
        ll min = std::numeric_limits<ll>::max();
        auto paths = get_paths(padded.at(i-1), padded.at(i), dirpad);
        for (const auto& path : paths) {
            min = std::min(min, recurse(path, iterations - 1));
        }
        num += min;
    }
    memo[std::make_pair(code, iterations)] = num;
    return num;
}

auto num2dir(std::string code, ll iterations) {
    auto num{0ll};
    code = std::string("A") + code;
    for (ll i = 1; i < code.size(); i++) {
        ll min = std::numeric_limits<ll>::max();
        auto paths = get_paths(code.at(i-1), code.at(i), numpad);
        for (const auto& path : paths) {
            min = std::min(min, recurse(path, iterations));
        }
        num += min;
    }
    std::println("num = {}", num);
    return num;
}

auto fast_translate(const std::string& code) {
    auto h = convert(code, numpad).at(0);
    std::string d0{};
    std::string d1{};
    std::string d2{};
    std::string d3{};
    std::string d4{};
    std::string d5{};

    d0 = breakdown2(code, 1, numpad);
    //d1 = breakdown2(d0, 1, dirpad);

    //std::println("==========iterative=========");
    //std::println("{}", d0);
    //std::println("{}", d1);
    //std::println("{}", d2);
    //std::println("{}", d3);
    //std::println("{}", d4);
    //std::println("{}", d5);

    std::println("==========recurive==========");
    //auto r1 = recurse(d0, 1);
    //auto r2 = recurse(d0, 2);
    //auto r3 = recurse(d0, 3);
    //auto r4 = recurse(d0, 4);
    auto r25 = recurse(d0, 25);
    auto other = num2dir(code, 25);

    //std::println("{} (same)", d0);
    //std::println("{}", r1);
    //std::println("{}", r2);
    //std::println("{}", r3);
    //std::println("{}", r4);
    //std::println("{}", r5);

    ll length = other;
    ll numeric = std::stoll(code);
    std::println("code {}: {}*{}={}", code, length, numeric, length*numeric);

    return length*numeric;
}


int main() {

    std::uint64_t ans1{0};
    ll ans2{0};

    auto f = std::ifstream("input.txt");
    auto codes = std::vector<std::string>{};
    for (std::string line{}; std::getline(f, line);) {
        codes.emplace_back(line.begin(), line.end());
    }

    for (const auto& code : codes) {
        ans1 += fast_translate(code);
    }

    // not 390330474728830
    std::println("ans1: {}", ans1);

    return 0;

}
