file = open("input.txt", "r")
lines = file.read().splitlines()

class Command:
    def __init__(self, Command, Args, Result):
        self.command = Command
        self.args = Args
        self.result = Result

line_idx = 0
line_count = len(lines)
commands = []
current_command = None
current_args = None
current_result = None
while line_idx < line_count:
    line = lines[line_idx]
    if len(line) == 0:
        if current_command is not None:
            commands.append(Command(current_command, current_args, current_result))
            current_command = None
        break
    elif line[0] == '$':
        if current_command is not None:
            commands.append(Command(current_command, current_args, current_result))
            current_command = None
        splt = line[2:].split(' ')
        current_command = splt[0]
        current_args = splt[1:] if len(splt) > 1 else None
        current_result = None
    else:
        if current_result is None:
            current_result = []
        current_result.append(line)
    line_idx += 1
if current_command is not None:
    commands.append(Command(current_command, current_args, current_result))
    current_command = None

class TreeElem:
    def __init__(self, Name, IsDirectory, Size):
        self.IsDirectory = IsDirectory
        self.Size = Size
        self.Name = Name
        self.Children = {} if self.IsDirectory else None

    def add_child(self, Child):
        self.Children[Child.Name] = Child

    def has_child(self, Name):
        return Name in self.Children

    def get_child(self, Name):
        return self.Children[Name]

    def compute_size(self):
        if not self.IsDirectory:
            return self.Size
        else:
            if self.Size != 0:
                return self.Size
            Size = 0
            for c in self.Children.values():
                Size += c.compute_size()
            self.Size = Size
            return Size

    def __str__(self):
        return f'{self.Name} {self.Size}'

cwd = []
for command in commands:
    if command.command == 'cd':
        if command.args[0] == '..':
            cwd.pop()
        else:
            if command.args[0] == '/':
                cwd.append(TreeElem(command.args[0], True, 0))
            else:
                cwd.append(cwd[-1].get_child(command.args[0]))
    elif command.command == 'ls':
        for res in command.result:
            splt = res.split(' ')
            if splt[0] == 'dir' and not cwd[-1].has_child(splt[1]):
                cwd[-1].add_child(TreeElem(splt[1], True, 0))
            else:
                cwd[-1].add_child(TreeElem(splt[1], False, int(splt[0])))

def find_size_part1(s):
    sum = 0
    size = s.compute_size()
    is_dir = s.IsDirectory
    if is_dir and size <= 100000:
        sum += size
    if is_dir:
        for c in s.Children.values():
            sum += find_size_part1(c)
    return sum

print(find_size_part1(cwd[0]))

size_taken = cwd[0].compute_size()
total_disk_space = 70000000
size_needed = 30000000
size_to_free = size_needed - (total_disk_space - size_taken)
print(f'{total_disk_space} {size_taken} {size_needed} {size_to_free}')

dirs = []
def find_all_dir_at_least_this_size(s, size):
    ds = s.compute_size()
    is_dir = s.IsDirectory
    if is_dir:
        if ds >= size:
            dirs.append(s)
        for c in s.Children.values():
            find_all_dir_at_least_this_size(c, size)
find_all_dir_at_least_this_size(cwd[0], size_to_free)
dirs.sort(key=lambda d: d.Size)
print([str(d) for d in dirs])
print(dirs[0])