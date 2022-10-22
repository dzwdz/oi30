# fails ex5

def lmap(f, i):
    return list(map(f, i))


def get_testcase():
    ex_me = lmap(int, input().split())
    truck_amt = ex_me[0]
    ex_me = ex_me[1:]
    # length, velfrac

    ex_trucks = []
    for i in range(truck_amt):
        ex_trucks.append(lmap(int, input().split()))
        # front, length, velfrac
    return ex_me, ex_trucks


def fullvis(in_me, in_trucks):
    from fractions import Fraction as Frac
    class Car:
        def __init__(self, front, _len, vel):
            self.front = front
            self.len = _len
            self.vel = vel
        def draw(self, y, msg):
            print("svg/box", float(self.get_back()), y, self.len, 1, msg)
        def get_back(self):
            return self.front - self.len
    me = Car(0, in_me[0], Frac(in_me[1], in_me[2]))
    cars = []
    for t in sorted(in_trucks):
        cars.append(Car(t[0], t[1], Frac(t[2], t[3])))

    time = 0
    side = 0
    cpos = 0
    changes = 0

    def until_hit(d, d2, v, v2):
        if v == v2:
            return -1337
        return (d2 - d) / (v - v2)

    si = 0
    def draw():
        nonlocal si, time, me, side, cpos, changes, cars
        print("svg/box", -4, si, 2, 1, round(float(time), 2))
        print("svg/box", -5, si, .8, 1, changes)
        me.draw(si - side - .1, "me")
        for i in range(len(cars)):
            off = -.1 if i == cpos else 0
            cars[i].draw(si + off, i)
        si += 2.5

    def update(dt):
        nonlocal time, me, side, cpos, changes, cars

        time += dt
        me.front += me.vel * dt
        for c in cars:
            c.front += c.vel * dt
        for i in range(len(cars) - 1)[::-1]:
            first = cars[i]
            second = cars[i+1]
            if first.front == second.get_back() and first.vel > second.vel:
                first.vel = second.vel

        if me.front == cars[cpos].get_back():
            changes += 1
        elif cars[cpos].front <= me.get_back():
            cpos += 1

        side = 0
        if cpos < len(cars):
            assert cars[cpos].front > me.get_back()
            if me.front > cars[cpos].get_back():
                side = 1
            elif me.front == cars[cpos].get_back():
                side = .5 # represent edges

    update(0)
    draw()
    while cpos < len(cars):
        c = cars[cpos]

        dt = None
        def event(t):
            nonlocal dt
            if t > 0 and (dt == None or dt > t):
                dt = t
        event(until_hit(me.front, c.get_back(), me.vel, c.vel))
        event(until_hit(me.get_back(), c.front, me.vel, c.vel))
        for i in range(len(cars) - 1):
            c1 = cars[i]
            c2 = cars[i+1]
            t = until_hit(c1.front, c2.get_back(), c1.vel, c2.vel)
            event(t)
        if not dt:
            break

        update(dt)
        draw()
    return changes

me, trucks = get_testcase()
print(fullvis(me, trucks))
