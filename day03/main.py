import re

pattern = r"mul\((\d+),(\d+)\)|do\(\)|don't\(\)"

with open("input.txt", "r") as fp:
    contents = "".join(fp.readlines())
    ans = 0
    enabled = True
    for match in re.finditer(pattern, contents):
        print(match.group())
        if (match.group() == "do()"):
            enabled = True
            continue
        elif (match.group() == "don't()"):
            enabled = False
            continue
        else:
            if (enabled):
                t = [int(num) for num in match.group()[4:][:-1].split(',')]
                ans += t[0] * t[1]
    print(ans)

