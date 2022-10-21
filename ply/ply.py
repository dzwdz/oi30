# BARELY tested, think this through again, etc

def depthvis(a):
    r = ""
    depth = 0
    for c in a:
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        r += str(depth)
    return r

def iter(s, limit, smart=False):
    verbose = 1
    depth = 0
    swaps = 0
    debt = 0
    opens = []
    for pos in range(len(s)):
        preview = [' '] * pos + s[pos:]
        for l in opens:
            assert l < pos
            assert preview[l] == ' '
            preview[l] = "."
        if verbose >= 2:
            print("".join(preview), depth, swaps, opens)

        if depth > limit:
            assert depth == len(opens)
            assert len(opens) >= 2
            preview[opens[1]] = ']'
            s[opens[1]] = ')'
            swaps += 1
            opens = opens[2:]
            depth -= 2

            if smart:
                debt += 1
            else:
                # find spot for (
                depth2 = depth
                for pos2 in range(pos, len(s)):
                    c = s[pos2]
                    if c == "(":
                        depth2 += 1
                    elif c == ")":
                        depth2 -= 1
                        if depth2 < 0:
                            preview[pos2] = '['
                            s[pos2] = '('
                            swaps += 1
                            break
                else:
                    raise ValueError # valid, somehow
            if verbose >= 1:
                print("".join(preview))
                print(depthvis(s))
        
        c = s[pos]
        if c == "(":
            depth += 1
            opens.append(pos)
        elif c == ")":
            depth -= 1
            if depth < 0:
                assert debt > 0
                debt -= 1
                depth += 2
                swaps += 1
                s[pos] = "("
                opens.append(pos)
                preview[pos] = "["
                if verbose >= 1:
                    print("".join(preview))
                    print(depthvis(s))
            else:
                opens.pop()
        assert depth == len(opens)
    assert debt == 0
    assert depth == 0
    return "".join(s), swaps

print(iter(list("(()(()))"), 3))
print(iter(list("(()(()))"), 2))
print(iter(list("(()(()))"), 1))


for i in [4, 3, 2, 1]:
    for smart in [False]:
        print()
        print(i, smart)
        a = list("(()(()())(()(()())))()")
        print("".join(a))
        print(depthvis(a))
        a, swaps = iter(a, i, smart)
        print(a, swaps)
