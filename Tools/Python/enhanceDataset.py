#!/usr/bin/env python

#
# Created by Baptiste Vasseur | May 8, 2019
#

import os, sys
from PIL import Image, ImageEnhance

filepath = input("Dataset à améliorer : ")
filepath = filepath.rstrip(' ') + '/'.replace("\\ ", ' ')
string = str(input('Rotations (séparer par des virgules) : '))
rotation = list(map(int, string.replace(' ', '').split(',')))

newFilePath = filepath + "../Enhanced/"
validExt = {'jpg', 'png', 'gif', 'jpeg'}
counter = 1
imageCounter = 1

os.makedirs(newFilePath, mode=493, exist_ok=True)

for filename in os.listdir(filepath):

    if filename.split(".")[-1].lower() in validExt:

        img = Image.open(filepath + filename)

        for deg in rotation:
            try:
                newImg = img.rotate(deg)
                # newImg = ImageEnhance.Contrast(newImg).enhance(0.3)
                newImg = ImageEnhance.Color(newImg).enhance(0.5)
                newImg.save(newFilePath + str(counter) + '_rotated_' + str(deg) + '.jpg', 'JPEG', quality=90)

                print('> #' + str(counter) + ' Image #' + str(imageCounter) + ' add ' + filename + ' Rotation ' + str(deg) + ' : ok')

            except Exception:
                print('> #' + str(counter) + ' Image #' + str(imageCounter) + ' add ' + filename + ' Rotation ' + str(deg) + ' : error')

            finally:
                counter += 1

        imageCounter += 1
