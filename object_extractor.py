#!/usr/bin/env python
from PIL import Image
import sys
import os

if len(sys.argv) <= 1:
    print('''Usage: object_extractor.py annotation_file /path/to/extraction/directory

object_extractor is a Python script that reads an image annotation text
file whose format is similar to OpenCV traincascade annotation file and
extracts the objects from their original images to a standalone image.''')
    exit()

if not os.path.exists(sys.argv[2]):
    os.mkdir(sys.argv[2])

f = open(sys.argv[1], 'r')
linha = f.readline()

os.chdir(os.path.dirname(sys.argv[1]))

while linha != '':
    print(linha.split(' '))
    arquivo, n_rec, x, y, w, h = linha.split(' ')
    print(os.path.normpath(os.path.abspath(arquivo)))
    img = Image.open(arquivo)

    x = int(x)
    y = int(y)
    w = int(w)
    h = int(h)
    left = x
    top = y
    right = x + w
    bottom = y + h
    obj = img.crop((left, top, right, bottom))
    obj.save(sys.argv[2] + os.path.basename(arquivo))
    linha = f.readline()
