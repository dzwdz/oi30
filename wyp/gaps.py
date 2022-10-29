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

###

def gaps0(in_me, in_cars):
    from fractions import Fraction as Frac
    class Car:
        def __init__(self, front, size, vel):
            self.front = front
            self.size = size
            self.v = vel
    me = Car(0, in_me[0], Frac(in_me[1], in_me[2]))
    cars = []
    for car in sorted(in_cars):
        v = Frac(car[2], car[3])
        if not v < me.v:
            print("invalid input!", v, me.v)
            assert False
        cars.append(Car(car[0], car[1], v))
    assert len(cars) > 0

    swerve_cnt = 1 # swerves right once at the end, since it's passing at least one trucks
    frontvels = [(0, cars[-1].v)]
    #print('i', 'dv', 'gap/dv', 'init vels', sep='\t')
    for i in range(len(cars) - 1)[::-1]:
        frontc = cars[i+1]
        backc = cars[i]
        front = frontc.front - frontc.size # back of front car
        back = backc.front # front of back car

        backv = backc.v

        def find_hit(gap):
            assert gap >= 0
            #print("| 0 ", gap)
            for j in range(len(frontvels)):
                t, v = frontvels[j]
                if j == len(frontvels) - 1:
                    #print("(last speed)")
                    if backv > v:
                        return t + gap / (backv - v), v
                    else:
                        return None, None
                next_t = frontvels[j+1][0]
                dt = next_t - t
                gap_after_dt = gap - backv * dt + v * dt
                #print("|", next_t, gap_after_dt)
                if gap_after_dt > 0:
                    gap = gap_after_dt
                else:
                    assert backv > v
                    return t + gap / (backv - v), v
            return None, None

        igap = front - back # initial gap
        hit_t, hit_v = find_hit(igap)
        edges = []
        swerves = False

        if True: # find the range of t for which gap >= me.size
            class Period:
                def __init__(self, t, dur, gap, dgap):
                    self.t = t
                    self.dur = dur
                    self.gap = gap
                    self.dgap = dgap
                def dump(self):
                    print('|', self.t, self.dur, self.gap, self.dgap, sep='\t')
            periods = []
            myv = backv
            for t, v in frontvels:
                if hit_t == None or t < hit_t:
                    periods.append(Period(t, None, None, v - backv))
                else:
                    if t > hit_t and periods[-1].t < hit_t:
                        # edge
                        periods.append(Period(hit_t, None, None, 0))
                    periods.append(Period(t, None, None, 0))

            if hit_t != None and periods[-1].t < hit_t:
                periods.append(Period(hit_t, None, None, 0))

            # fill out durations
            for j in range(len(periods) - 1):
                periods[j].dur = periods[j+1].t - periods[j].t
            # fill out gaps
            gap = igap
            for p in periods:
                p.gap = gap
                if p.dur == None: # last one
                    break
                gap = gap + p.dgap * p.dur

            #print('|', 't', 'dur', 'gap', 'dgap', sep='\t')
            #for p in periods:
            #    p.dump()
            if periods[0].gap >= me.size:
                edges.append(0)
            for j in range(len(periods) - 1):
                p1 = periods[j]
                p2 = periods[j+1]
                if (p1.gap >= me.size) != (p2.gap >= me.size):
                    edge = p1.t + (me.size - p1.gap) / p1.dgap
                    assert p1.t <= edge <= p2.t
                    edges.append(edge)
            if True:
                p = periods[-1]
                if (p.gap >= me.size and p.dgap < 0) or (p.gap < me.size and p.dgap > 0):
                    # (gap-me.size) and (gap-me.size)' have different signs - will cross 0
                    edge = p.t + (me.size - p.gap) / p.dgap
                    assert p.t <= edge
                    edges.append(edge)

            if edges:
                for edge in edges:
                    if hit_t != None:
                        assert edge < hit_t
                # when do we pass the edge?
                pass_t = (back + me.size) / (me.v - backv)
                # this can be too late - see asserts later
                # however, the edge is always before the gap closure anyways
                if hit_t != None:
                    assert edges[1] < hit_t

                swerves = None
                if len(edges) == 1:
                    swerves = edges[0] <= pass_t
                elif len(edges) == 2:
                    # the back of the car needs to pass inbetween edges - while gap >= me.size
                    swerves = edges[0] <= pass_t <= edges[1]
                assert swerves != None

                # after the gap closes, the back of it can move closer than backv
                is_too_late = hit_t != None and hit_t < pass_t
                if is_too_late:
                    assert len(edges) == 2 # closes completely
                    assert edges[1] < hit_t # must pass me.size before passes 0
                    assert hit_t < pass_t # only is too late if the gap closes before pass_t
                    # thus
                    assert edges[1] < pass_t
                    # thus
                    assert not (pass_t <= edges[1])
                    # thus
                    assert not swerves
                if swerves:
                    swerve_cnt += 1


        if hit_t != None:
            while frontvels and frontvels[0][0] <= hit_t:
                frontvels = frontvels[1:]
            # only carry over the speeds after we've hit the parent
            if frontvels:
                assert hit_t < frontvels[0][0]

            frontvels = [(hit_t, hit_v)] + frontvels
            if hit_t != 0:
                frontvels = [(0, backc.v)] + frontvels
        else:
            frontvels = [(0, backc.v)]
        # TODO prune frontvels that happen after the car has passed us

        #if edges:
        #    print(i, swerves, len(frontvels), *edges, sep='\t')
    return swerve_cnt


me, trucks = get_testcase()
print(gaps0(me, trucks))
