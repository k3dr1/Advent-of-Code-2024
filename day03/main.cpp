#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

using i64 = std::uint64_t;

auto parse_num(std::string_view sv, i64& idx) -> i64 {
    i64 res{};
    for (; idx < sv.size() && std::isdigit(sv[idx]); idx++) {
        res = 10*res + (sv[idx] - '0');
    }
    return res;
}

auto main() -> int {
    i64 answer1{0};
    i64 answer2{0};

    std::string content{};
    std::string line{};
    auto f = std::ifstream("message.txt");
    while (std::getline(f, line))
        content += line;

    i64 idx{0};
    auto enabled{true};
    while (idx < content.size()) {
        auto mul = content.find("mul(", idx);
        auto _do = content.find("do()", idx);
        auto _dont  = content.find("don't()", idx);

        if (const auto np = content.npos; 
                np == mul 
                && np == _do 
                && np == _dont)
            break;

        if (mul < std::min(_do, _dont)) {
            idx = mul + 4;
            auto m1 = parse_num(content, idx);
            idx++;
            auto m2 = parse_num(content, idx);
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
