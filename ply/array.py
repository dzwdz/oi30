def parse(s):
    stack = [[]]
    for c in s:
        if c == "(":
            stack.append([])
        elif c == ")":
            stack[-2].append(stack.pop())
    if len(stack) != 1:
        raise ValueError
    return stack[0]

def realdepth(a):
    high = 0
    for sub in a:
        high = max(high, realdepth(sub))
    return high + 1

def depth(a):
    return realdepth(a) - 1

s = "(()(()))"
a = parse(s)
print(a)
print(depth(a))
