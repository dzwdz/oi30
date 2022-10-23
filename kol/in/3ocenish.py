def rprint(amt, *args, **kwargs):
    for i in range(amt):
        print(*args, **kwargs)

p = 1000000 # food amt
c = 1000000 # cmds

print(2000, p, c)

print(1, 2000, 11)
print(2000, 2000, 12)
print(2000, 1, 13)
p -= 3

if True:
    # fill food
    x = 2
    y = 10
    while p > 0:
        print(x, y, 3999999)
        x += 1
        if x > 1990:
            x = 2
            y += 1
        p -= 1

rprint(1998, 'P')
c -= 1998
# 1 1999
print('Z 1 1') # -1
print('Z 1 1998') # -1
print('Z 1 1999') # 0
print('Z 1 2000') # -1
c -= 4

print('P')
c -= 1
# 1 2000
print('Z 1 1') # -1
print('Z 1 1998') # -1
print('Z 1 1999') # 0
print('Z 1 2000') # 11
print('Z 2 2000') # -1
c -= 5

rprint(1998, 'D')
c-=1998
# 1999 2000
print('Z 1 1') # -1
print('Z 1 2000') # -1
print('Z 2000 2000') # -1
print('Z 1999 2000') # 11
print('Z 1998 2000') # 0
print('Z 1997 2000') # -1
c-=6

print('D')
c-=1
# 2000 2000
print('Z 1 1') # -1
print('Z 1 2000') # -1
print('Z 2000 2000') # 12
print('Z 1999 2000') # 11
print('Z 1998 2000') # 0
print('Z 1997 2000') # -1
c-=6

rprint(1999, 'L')
c-=1999
# 2000 1
print('Z 1 1') # -1
print('Z 1 2000') # -1
print('Z 2000 2000') # -1
print('Z 2000 1') # 13
print('Z 2000 2') # 12
print('Z 2000 3') # 11
print('Z 2000 4') # 0
c-=7

rprint(1999, 'G')
c-=1999
print('Z 1 1') # 13
print('Z 1 2000') # -1
print('Z 2000 2000') # -1
print('Z 2000 1') # -1
c-=4

print('Z 1 1')
print('Z 1 2')
print('Z 2 2')
print('Z 2 1')
# 13 -1 -1 12
c-=4

while c > 0:
    print('P')
    print('Z 1 1')
    print('Z 1 2')
    print('Z 2 2')
    print('Z 2 1')
    # 12 13 -1 11
    c-=5

    print('D')
    print('Z 1 1')
    print('Z 1 2')
    print('Z 2 2')
    print('Z 2 1')
    # 11 12 13 0
    c-=5

    print('L')
    print('Z 1 1')
    print('Z 1 2')
    print('Z 2 2')
    print('Z 2 1')
    # 0 11 12 13
    c-=5

    print('G')
    print('Z 1 1')
    print('Z 1 2')
    print('Z 2 2')
    print('Z 2 1')
    # 13 0 11 12
    c-=5
