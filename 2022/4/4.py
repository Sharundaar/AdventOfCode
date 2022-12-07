file = open("input.txt", "r")

lines = file.read().splitlines()


def contained(r1, r2):
    return r1[0] <= r2[0] and r1[1] >= r2[1]


def overlap(r1, r2):
    return not (r1[1] < r2[0] or r1[0] > r2[1])


elves_pairs = list(map(lambda line: [[int(y) for y in x.split('-')] for x in line.split(',')], lines))


def part1():
    count = 0
    for elv0, elv1 in elves_pairs:
        if contained(elv0, elv1) or contained(elv1, elv0):
            count += 1
    print(count)


def part2():
    count = 0
    for elv0, elv1 in elves_pairs:
        if overlap(elv0, elv1) or overlap(elv1, elv0):
            count += 1
    print(count)


part1()
part2()
