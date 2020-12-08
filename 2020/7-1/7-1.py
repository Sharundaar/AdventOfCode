import re

bags_data = {}

with open( '2020/7-1/input.txt' ) as file:
    for line in file.readlines():
        m = re.match( '(\w+ \w+) bags contain (.+)', line )
        bag_type = m.group( 1 )
        bag_content = []
        for content in m.group( 2 ).split( ',' ):
            content_stripped = content.strip()
            m2 = re.match( '(\d+) (\w+ \w+) bags*\.*', content_stripped )
            if m2:
                number = int( m2.group( 1 ) )
                color = m2.group( 2 )
                obj = {
                    "number": number,
                    "color": color
                }
                bag_content.append( obj )
        bags_data[bag_type] = bag_content

count = 0

def can_hold_bag( color_to_find, bag_color ):
    if bag_color == color_to_find:
        return True
    bag_content = bags_data[bag_color]
    for content in bag_content:
        if( can_hold_bag( color_to_find, content['color'] ) ):
            return True
    return False


for k, v in bags_data.items():
    for content in v:
        if can_hold_bag( 'shiny gold', content['color'] ):
            count = count + 1
            break

print( count )

shiny_bag_content = bags_data['shiny gold']
count = 0

def count_bags( bag_content ):
    content_count = 1
    for content in bag_content:
        content_count = content_count + content['number'] * count_bags( bags_data[content['color']] )
    return content_count

count = count_bags( shiny_bag_content ) - 1
print( count )