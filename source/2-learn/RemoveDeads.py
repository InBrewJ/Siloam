#!/usr/local/bin/python3
#
# RemoveDeads.py
#
# Using Pillow, recursively find all PNG images that
# contain all black pixels in a folder and
# remove them
#
# Written for Python 3.x
#
# Jason Brewer 2017
#

import argparse
import os
import PIL
from PIL import Image

# Walk through the directory defined on the command line
# and delete all images that are entirely black

parser = argparse.ArgumentParser()
parser.add_argument("trimble_renders_root_path")
args = parser.parse_args()
print("Removing all pngs in path: " + args.trimble_renders_root_path + "\n")

for root, dirs, files in os.walk(args.trimble_renders_root_path):
    for name in files:
        current_file = os.path.join(root, name)

        # find returns -1 if the string is not found:
        # this ignores all other files apart from those
        # that have .png. OS requirements state that .png
        # should only be found as a suffix so there is no
        # need to do anything regex
        
        if current_file.find(".png") != -1:
            img = Image.open(current_file)
            if not img.getbbox():
                os.remove(current_file)
                print(current_file + " removed!")
        else:
            print(current_file + " ignored!")
                
