#!/usr/bin/env python

#
# Created by Baptiste Vasseur | May 8, 2019
#

import os, sys
from PIL import Image

filepath = input("Dossier à parcourir : ")
filepath = filepath.rstrip(' ') + '/'.replace("\\ ", ' ')
imgL = int(input("Largeur de l'image en px : "))
imgH = int(input("Hauteur de l'image en px : "))

newFilePath = filepath + "../resized/"
size = (imgL, imgH)
validExt = {'jpg', 'png', 'gif', 'jpeg'}
counter = 1

os.makedirs(newFilePath, mode=493, exist_ok=True)
for filename in os.listdir(filepath):

    if filename.split(".")[-1].lower() in validExt:

        img = Image.open(filepath + filename)

        try:
            newImg = img.resize(size, Image.ANTIALIAS)
            newImg.save(newFilePath + str(counter) + '_resized.jpg', 'JPEG', quality=90)

            print('> #' + str(counter) + ' convert ' + filename + ' : ok')

        except Exception:
            print('> #' + str(counter) + ' convert ' + filename + ' : error')

        finally:
            counter += 1
