from sys import stdin, stderr

def lmap(f,i):
    return list(map(f,i))

boxes = []

for line in stdin:
    if not line.startswith("svg/box"):
        print(line, file=stderr, end="")
        continue
    line = line.removeprefix("svg/box").strip()
    d = line.split(None, 5)
    for i in range(4):
        d[i] = float(d[i])
    if len(d) == 4:
        d.append(None)
    boxes.append(d)

bbox1 = [0, 0]
bbox2 = [0, 0]
def inbbox(x, y):
    bbox1[0] = min(bbox1[0], x)
    bbox1[1] = min(bbox1[1], y)
    bbox2[0] = max(bbox2[0], x)
    bbox2[1] = max(bbox2[1], y)

for box in boxes:
    inbbox(box[0], box[1])
    inbbox(box[0] + box[2], box[1] + box[3])

def tag(name, **attrs):
    s = '<' + name
    for k, v in attrs.items():
        k = k.replace('_', '-')
        s += f' {k}="{v}"'
    s += '>'
    print(s)
def untag(name):
    print(f'</{name}>')

dims = lmap(lambda a: a[0]-a[1], zip(bbox2, bbox1))
s = 40 # scale

viewBox = lmap(str, [(bbox1[0]-1)*s, (bbox1[1]-1)*s, (dims[0]+2)*s, (dims[1]+2)*s])
tag('svg', width=dims[0]*s, height=dims[1]*s, viewBox=" ".join(viewBox))

tag('rect', x=viewBox[0], y=viewBox[1], width="100%", height="100%", fill="white")
untag('rect')

for box in boxes:
    tag('rect', x=box[0]*s, y=box[1]*s, width=box[2]*s, height=box[3]*s, fill="#0002", stroke="black", stroke_width="2")
    untag('rect')
    if box[4]:
        tag('text', x=(box[0] * 2 + box[2])/2*s, y=(box[1] * 2 + box[3])/2*s, font_size=s/2, text_anchor="middle", dy=".4em")
        print(box[4])
        untag('text')

untag('svg')
