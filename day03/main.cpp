#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <ranges>

using i64 = std::uint64_t;

auto parse_num(std::string_view sv, i64& idx) -> i64 {
    return std::ranges::fold_left(
        sv.substr(idx)
            | std::views::take_while([&idx](const auto& ch) { return std::isdigit(ch) ? idx++ : false; })
            | std::views::transform([](const auto& ch) { return ch - '0'; }),
        i64{},
        [](const auto& l, const auto& r) { return 10*l + r; });
}

auto main() -> int {
    i64 answer1{0};
    i64 answer2{0};
    std::string content{};
    std::string line{};
    auto f = std::ifstream("input.txt");
    while (std::getline(f, line)) content += line + '\n';

    i64 idx{0};
    auto enabled{true};
    while (idx < content.size()) {
        auto mul = content.find("mul(", idx);
        auto _do = content.find("do()", idx);
        auto _dont  = content.find("don't()", idx);
        if (mul == content.npos && _do == content.npos && _dont == content.npos) break;
        if (mul < std::min(_do, _dont)) {
            idx = mul + 4;
            auto m1 = parse_num(content, idx);
            auto m2 = parse_num(content, ++idx);
            if (idx < content.size() && content.at(idx) == ')') {
                answer1 += m1*m2;
                answer2 += enabled ? m1*m2 : 0;
                idx++;
            }
        }
        else if (_do < _dont) {
            enabled = true;
            idx = _do + 4;
        } else {
            enabled = false;
            idx = _dont + 7;
        }
    }

    std::cout << "Answer1=" << answer1 << '\n';
    std::cout << "Answer2=" << answer2 << '\n';

    return 0;
}
