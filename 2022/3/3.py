from math import floor
from itertools import islice

file = open("3/input.txt", "r")

rucksacks = []

def priority(val):
    if val >= 'a' and val <= 'z':
        return (ord(val) - ord('a')) + 1
    elif val >= 'A' and val <= 'Z':
        return (ord(val) - ord('A')) + 27
    return 0

#Part 1
def part1():
    score = 0
    for line in file.readlines():
        if len(line) == 0:
            break
        line_strip = line.strip()
        half_len = floor(len(line_strip)/2)
        part1 = line_strip[0:half_len]
        part2 = line_strip[half_len:]
        d = {}
        for c in part1:
            if c in part2:
                d[c] = True
        for c in d.keys():
            print(part1 + ' and ' + part2 + ' have ' + c + ' in common')
            score += priority(c)
    print(score)

def part2():
    score = 0
    group = []
    for line in file.readlines():
        if len(line) == 0:
            break
        line_strip = line.strip()
        group.append(line_strip)

        if(len(group) == 3):
            dict = {}
            # initialize
            for c in group[0]:
                dict[c] = True

            # remove elements not shared by each group
            kees = list(dict.keys())
            for c in kees:
                if c not in group[1] or c not in group[2]:
                    dict.pop(c)

            # list priorities
            for c in dict.keys():
                print(group[0] + ' and ' + group[1] + ' and ' + group[2] + ' have ' + c + ' in common')
                score += priority(c)
            group.clear()
    print(score)

def window(seq, n=2):
    "Returns a sliding window (of width n) over data from the iterable"
    "   s -> (s0,s1,...s[n-1]), (s1,s2,...,sn), ...                   "
    result = tuple(islice(it, n))
    if len(result) == n:
        yield result
    for elem in it:
        result = result[1:] + (elem,)
        yield result

def part2_but_better():
    score = 0
    lines = [x.strip() for x in file.readlines()]
    for i in range(floor(len(lines) / 3)):
        l1 = lines[i*3 + 0]
        l2 = lines[i*3 + 1]
        l3 = lines[i*3 + 2]
        itc = set(l1) & set(l2) & set(l3)
        score += priority(itc.pop())
    print(score)


part2_but_better()