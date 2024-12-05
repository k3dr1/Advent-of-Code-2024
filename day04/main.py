with open("input.txt") as fp:
    ans1 = 0
    ans2 = 0
    lines = fp.readlines()
    text = "".join(lines)

    w = len(lines[0])

    for i in range(len(text) - len("XMAS")):
        if (text[i] == "X") and (text[i+1] == "M") and (text[i+2] == "A") and (text[i+3] == "S"):
            ans1+= 1
    for i in range(len(text) - len("XMAS")):
        if (text[i] == "S") and (text[i+1] == "A") and (text[i+2] == "M") and (text[i+3] == "X"):
            ans1+= 1
    for i in range(len(text) - 3*w):
        if (text[i] == "X") and (text[i+w] == "M") and (text[i+w*2] == "A") and (text[i+w*3] == "S"):
            ans1+= 1
    for i in range(len(text) - 3*w):
        if (text[i] == "S") and (text[i+w] == "A") and (text[i+w*2] == "M") and (text[i+w*3] == "X"):
            ans1+= 1
    for i in range(len(text) - (3*w+3)):
        if (text[i] == "X") and(text[i+w+1] == "M") and (text[i+2*w+2] == "A") and (text[i+3*w+3] == "S"):
            ans1 += 1
    for i in range(len(text) - (3*w+3)):
        if (text[i] == "S") and(text[i+w+1] == "A") and (text[i+2*w+2] == "M") and (text[i+3*w+3] == "X"):
            ans1 += 1
    for i in range(len(text) - (3*w-3)):
        if (text[i] == "X") and(text[i+(w-1)] == "M") and (text[i+(2*w-2)] == "A") and (text[i+(3*w-3)] == "S"):
            ans1 += 1
    for i in range(len(text) - (3*w-3)):
        if (text[i] == "S") and(text[i+(w-1)] == "A") and (text[i+(2*w-2)] == "M") and (text[i+(3*w-3)] == "X"):
            ans1 += 1
    for i in range(len(text) - (2*w+2)):
        downleft = False
        downright = False
        if (text[i] == "M") and (text[i+w+1] == "A") and (text[i+2*w+2] == "S"):
            downright = True
        if (text[i] == "S") and (text[i+w+1] == "A") and (text[i+2*w+2] == "M"):
            downright = True
        if (text[i+2] == "M") and (text[i+1+w] == "A") and (text[i+2*w] == "S"):
            downleft = True
        if (text[i+2] == "S") and (text[i+1+w] == "A") and (text[i+2*w] == "M"):
            downleft = True
        ans2 += downright and downleft 

    print("ans1=", ans1)
    print("ans2", ans2)
