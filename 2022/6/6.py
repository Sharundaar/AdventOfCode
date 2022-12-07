file = open("input.txt", "r")
lines = file.read().splitlines()


def all_unique(item):
    return len(set(item)) == len(item)


m = lines[0]
l = []
for i in range(13, len(m)):
    l.clear()
    for j in range(i-13, i):
        l.append(m[j])
    if all_unique(l):
        print(i+1)
        break