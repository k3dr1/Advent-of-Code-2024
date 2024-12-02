def is_safe(line):
    changes = []
    for i in range(1, len(line)):
        changes.append(line[i] - line[i-1])

    all_pos = True
    all_neg = True
    conforming = True
    for c in changes:
        if (c < 0):
            all_pos = False
        if (c > 0):
            all_neg = False
        if (abs(c) < 1) or (abs(c) > 3):
            conforming = False

    return (all_pos or all_neg) and (conforming)

with open("input.txt") as fp:
    lines = [[int(x) for x in line.split(' ')] for line in fp.readlines()]
    silver = 0
    gold = 0
    for line in lines:
        pos_safe = False
        for i in range(len(line)):
            a = [line[x] for x in range(len(line)) if x != i]
            if (is_safe(a)):
                pos_safe = True
        pos_safe = pos_safe or is_safe(line)
        silver += is_safe(line)
        gold += pos_safe

    print(silver)
    print(gold)
