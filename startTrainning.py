#!/usr/bin/env python
# coding=utf-8

#
# Created by Baptiste Vasseur | May 8, 2019
#

import sys
import cv2
import numpy as np
import os
import time
import random

import Implementation.MLP.MLP as MLP
import Implementation.Linear.Linear as LINEAR
import Utils


def image_to_array(image):
    list_pixel = []
    for i in range(0, np.size(image, axis=0)):
        for j in range(0, np.size(image, axis=1)):
            for rgb in image[i, j]:
                list_pixel.append(rgb)

    return list_pixel


def start(path, _size):
    valid_extension = {'jpg', 'png', 'jpeg'}
    _XTrain = []
    _YTrain = []
    classe = 0

    _sampleCount = 0
    _inputCountPerSample = _size[0] * _size[1] * 3

    for root, dirList, _files in os.walk(path):

        if 0 < len(dirList) < 2:
            print("La dataset doit être composé de 2 classes au minimum !")
            return

        if 0 < len(dirList):

            print()

            for singleDir in dirList:

                list_possibily = [-1] * len(dirList)
                list_possibily[classe] = 1

                print("> Chargement du dossier " + str(singleDir))

                fullpath = os.path.join(root, singleDir)
                filelist = os.listdir(fullpath)

                counter = 0
                for i, filename in enumerate(filelist):
                    if filename.split(".")[-1].lower() in valid_extension:
                        filename = os.path.join(fullpath, filename)

                        _XTrain.append(filename)
                        _YTrain.append(list_possibily)
                        _sampleCount += 1
                        counter += 1

                print("> Images dans le dossier : " + str(counter))
                print("> Dossier " + str(singleDir) + " chargé !", '->', list_possibily)

                print()
                classe += 1

    c = list(zip(_XTrain, _YTrain))
    random.shuffle(c)
    _XTrain, _YTrain = zip(*c)

    return _XTrain, _YTrain, _sampleCount, _inputCountPerSample


def launchTraitement(_XTrain, _YTrain, _algo_name, _size):

    _xTrainFinal = []
    _yTrainFinal = []

    print("> Convertion des images en pixel")
    _XTrain_size = len(_XTrain)
    _XTrain_size_percent = int(_XTrain_size / 10)

    for i, filename in enumerate(_XTrain):
        try:
            image = cv2.imread(filename)
            image = cv2.resize(image, _size)
            pixel = image_to_array(image)
            _xTrainFinal += pixel

            if algo_name == "MLP":
                _yTrainFinal += _YTrain[i]

            elif algo_name == "LINEAR":

                _attemp = False
                if _YTrain[i] == [1, -1]:
                    _attemp = 1
                elif _YTrain[i] == [-1, 1]:
                    _attemp = -1

                _yTrainFinal.append(_attemp)

            elif algo_name == "RBF":
                _yTrainFinal += _YTrain[i]

            if i % _XTrain_size_percent == 0:
                print("- Conversion : " + str(i) + '/' + str(_XTrain_size) + " images")

        except Exception as e:
            print("L'image : " + filename + " est inutilisable." + str(e))
            continue

    return _xTrainFinal, _yTrainFinal


def menu():
    _inputVal = input("Dataset à entrainer : ")
    _filepath = _inputVal.rstrip(' ') + '/'.replace("\\ ", ' ')

    _largeur = input("Largeur de l'image ? (par défaut 32) : ")
    _largeur = 32 if len(_largeur) == 0 else int(_largeur)
    _size = (_largeur, _largeur)

    _epochs = input("Epochs ? (par défaut 1000) : ")
    _epochs = 1000 if len(_epochs) == 0 else int(_epochs)

    _alpha = input("Alpha ? (par défaut 0.01) : ")
    _alpha = 0.01 if len(_alpha) == 0 else float(_alpha)

    _repeat = True
    _algo_name = ''
    while _repeat:
        _algo_name = input("Algo ? LINEAR, MLP or RBF ? : ")
        _algo_name = _algo_name.upper()
        if _algo_name == "LINEAR" or _algo_name == "MLP" or _algo_name == "RBF":
            _repeat = False

    return _filepath, _size, _epochs, _alpha, _algo_name


if __name__ == "__main__":

    filepath, size, epochs, alpha, algo_name = menu()

    # Traitement

    files, possibleResults, sampleCount, inputCountPerSample = start(filepath, size)

    xTrain, yTrain = launchTraitement(files, possibleResults, algo_name, size)

    # print(yTrain)

    print("")
    print("- Epochs : " + str(epochs))
    print("- Alpha : " + str(alpha))
    print("- Taille d'une image : " + str(size[0]) + "x" + str(size[1]) + 'x3 -> ' + str(inputCountPerSample))
    print("- Nombre d'images : " + str(sampleCount))

    print("")

    start_time = time.time()

    if algo_name == "MLP":

        N = [inputCountPerSample, 64, 64, 2]
        mlpClassif = MLP.init(N)
        mlpClassif = MLP.fit_classification(mlpClassif, xTrain, yTrain, sampleCount, epochs, alpha)
        Utils.save(mlpClassif, 'MLP', 'classification_E'+str(epochs)+'_A'+str(alpha)+'_N'+str(N))

    elif algo_name == "LINEAR":

        linearClassif = LINEAR.create_linear_model(inputCountPerSample)
        linearClassif = LINEAR.fit_classification(linearClassif, xTrain, yTrain, alpha, epochs, inputCountPerSample)
        Utils.save(linearClassif, 'LINEAR', 'classification_E'+str(epochs)+'_A'+str(alpha))

    elif algo_name == "RBF":
        rbfClassif = RBF.naive_rbf_train(xTrain, yTrain, inputCountPerSample, sampleCount, gamma)
        Utils.save(rbfClassif, 'RBF', 'classification_E' + str(epochs) + '_A' + str(alpha) + '_G' + str(gamma))

    end_time = time.time()

    print("")
    print("- Durée : %s secondes !" % (end_time - start_time))
    print("")
