file = open("2/input.txt", "r")


beat_map = {
    'X': 'Z',
    'Z': 'Y',
    'Y': 'X'
}
lose_map = {
    'Z': 'X',
    'Y': 'Z',
    'X': 'Y'
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
    if mine == 'Y':
        score += 3
        score += score_map[opponent]
    elif mine == 'Z':
        score += 6
        score += score_map[lose_map[opponent]]
    elif mine == 'X':
        score += 0
        score += score_map[beat_map[opponent]]

print(score)


