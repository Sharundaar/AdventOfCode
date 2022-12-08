file = open("input.txt", "r")
lines = file.read().splitlines()

heightmap = []
scenic_score = []
visible = []
for l in lines:
    if len(l) == 0:
        break

    for c in l:
        i = int(c)
        heightmap.append(i)
        scenic_score.append(0)
        visible.append(False)

def get_tree_height(x, y):
    return heightmap[len(lines[0])*y + x]

w = len(lines[0])
def is_visible(x, y):
    if x == 0 or x == w-1 or y == 0 or y == w-1:
        return True
    h = get_tree_height(x, y)
    v1, v2, v3, v4 = True, True, True, True
    scenic_score_c = 0
    scenic_score_total = 1
    for nx in range(x-1, -1, -1):
        scenic_score_c += 1
        if get_tree_height(nx, y) >= h:
            v1 = False
            break
    scenic_score_total *= scenic_score_c
    scenic_score_c = 0
    for nx in range(x + 1, w, 1):
        scenic_score_c += 1
        if get_tree_height(nx, y) >= h:
            v2 = False
            break
    scenic_score_total *= scenic_score_c
    scenic_score_c = 0
    for ny in range(y - 1, -1, -1):
        scenic_score_c += 1
        if get_tree_height(x, ny) >= h:
            v3 = False
            break
    scenic_score_total *= scenic_score_c
    scenic_score_c = 0
    for ny in range(y + 1, w, 1):
        scenic_score_c += 1
        if get_tree_height(x, ny) >= h:
            v4 = False
            break
    scenic_score_total *= scenic_score_c
    scenic_score_c = 0
    scenic_score[y*w+x] = scenic_score_total
    return v1 or v2 or v3 or v4


# print(get_tree_height(1, 2))

for y in range(w):
    for x in range(w):
        visible[y*w+x] = is_visible(x, y)

str = []
c = 0
for p, i in enumerate(visible):
    if p > 0 and p % w == 0:
        str.append('\n')
    str.append('1' if i else '0')
    if i is True:
        c += 1
print(''.join(str))
print(c)
scenic_score.sort(reverse=True)
print(scenic_score[0])