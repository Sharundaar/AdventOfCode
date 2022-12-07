file = open("2/input.txt", "r")


beat_map = {
    'X': 'Z',
    'Z': 'Y',
    'Y': 'X'
}
score_map = {
    'X': 1,
    'Y': 2,
    'Z': 3,
}
opponent_map = {
    'A': 'X',
    'B': 'Y',
    'C': 'Z',
}
score = 0
for line in file.readlines():
    if len(line) == 0:
        break
    splt = line.strip().split(' ')
    opponent = opponent_map[splt[0]]
    mine = splt[1]
    print(opponent + ' ' + mine)
    if beat_map[mine] == opponent:
        score += 6
    elif mine == opponent:
        score += 3
    score += score_map[mine]

print(score)


