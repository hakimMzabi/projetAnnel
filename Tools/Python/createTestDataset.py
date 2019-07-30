#!/usr/bin/env python

#
# Created by Baptiste Vasseur | May 8, 2019
#

import os, sys
from PIL import Image, ImageEnhance
import random

filepath = input("Dataset : ")
filepath = filepath.rstrip(' ') + '/'.replace("\\ ", ' ')

files = os.listdir(filepath)
length = len(files)

percent = int(input("Pourcentage d'images test (" + str(length) + ") : "))

newFilePath = filepath + "../Test/"
nbFilesToMove = round(percent * length / 100)
randomList = random.sample(range(1, length), nbFilesToMove)


counter = 1
generalCounter = 1

os.makedirs(newFilePath, mode=493, exist_ok=True)
for filename in files:
    if generalCounter in randomList:
        try:
            os.rename(filepath + filename, newFilePath + filename)

            print('> #' + str(counter) + ' moved to dataset test ' + filename + ' : ok')

        except Exception:
            print('> #' + str(counter) + ' moved to dataset test ' + filename + ' : error')

        finally:
            counter += 1

    generalCounter += 1
