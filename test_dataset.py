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

import Implementation.MLP.MLP as MLP
import Utils


def image_to_array(image):
    list_pixel = []
    for i in range(0, np.size(image, axis=0)):
        for j in range(0, np.size(image, axis=1)):
            for rgb in image[i, j]:
                list_pixel.append(rgb)

    return list_pixel


def start(_path, _size, _model, _algo_name):
    validExt = {'jpg', 'png', 'jpeg'}
    classe = 0
    _sampleCount = 0
    valid = 0
    invalid = 0
    _inputCountPerSample = _size[0] * _size[1] * 3

    for root, dirList, files in os.walk(_path):

        if 0 < len(dirList) < 2:
            print("La dataset doit être composé de 2 classes au minimum !")
            return

        if 0 < len(dirList):

            for singleDir in dirList:

                YTrain = [-1] * len(dirList)
                YTrain[classe] = 1

                print("Classe :", singleDir, '->', YTrain)

                fullpath = os.path.join(root, singleDir)
                filelist = os.listdir(fullpath)

                for filename in filelist:
                    if filename.split(".")[-1].lower() in validExt:
                        filename = os.path.join(fullpath, filename)
                        # try:
                        image = cv2.imread(filename)
                        image = cv2.resize(image, _size)
                        xToPredict = image_to_array(image)

                        res = Utils.predict(_model, _algo_name, xToPredict)

                        if result_traitment(_sampleCount, _algo_name, filename, res, YTrain):
                            valid += 1
                        else:
                            invalid += 1

                        _sampleCount += 1

                        # except Exception:
                        #     continue

                print("     > Dossier " + str(singleDir) + " chargé !")
                classe += 1

    return _sampleCount, _inputCountPerSample, valid, invalid


def result_traitment(_counter, _algo_name, _filename, _res, _YTrain):
    if _algo_name.upper() == "MLP":

        indexRes = _res.index(max(_res))
        YTrainIndex = _YTrain.index(max(_YTrain))

        if indexRes == YTrainIndex:
            print('#' + str(_counter), '> filename', _filename, '_res:', str(_res), '> indexRes', indexRes, '-',
                  'YTrainIndex', YTrainIndex, '-> GOOD')
            return True

        else:
            print('#' + str(_counter), '> filename', _filename, '_res:', str(_res), '> indexRes', indexRes, '-',
                  'YTrainIndex', YTrainIndex, '-> BAD')
            return False

    elif _algo_name.upper() == "LINEAR":

        _attemp = ""
        if _YTrain == [1, -1]:
            _attemp = "superior"
        elif _YTrain == [-1, 1]:
            _attemp = "inferior"

        isOk = False
        if _attemp == "superior":
            if _res > 0:
                isOk = True

        elif _attemp == "inferior":
            if _res < 0:
                isOk = True

        if isOk:
            print('#' + str(_counter), '> filename', _filename, '> attemp', _attemp, '-',
                  'res', _res, '-> GOOD')
            return True

        else:
            print('#' + str(_counter), '> filename', _filename, '> attemp', _attemp, '-',
                  'res', _res, '-> BAD')
            return False

    elif _algo_name.upper() == "RBF":
        # Todo : Ajout implém RBF
        pass


if __name__ == "__main__":
    inputVal = input("Dataset à tester : ")
    filepath = inputVal.rstrip(' ') + '/'.replace("\\ ", ' ')

    _model_name = input('Modèle à charger (nom.model) ? : ')
    model, algo_name = Utils.load(_model_name)

    largeur = input("Largeur de l'image ? (par défaut 32) ")
    largeur = 32 if len(largeur) == 0 else int(largeur)
    size = (largeur, largeur)

    start_time = time.time()

    sampleCount, inputCountPerSample, nb_valid, nb_invalid = start(filepath, size, model, algo_name)

    print()
    print()
    print("- Taille d'une image : " + str(inputCountPerSample))
    print("- Nombre d'images : " + str(sampleCount))

    print("- Prédiction correct : " + str(nb_valid) + "/" + str(sampleCount) + " soit " + str((100 * nb_valid / sampleCount)) + "%")
    print("- Prédiction incorrect : " + str(nb_invalid) + "/" + str(sampleCount) + " soit " + str((100 * nb_invalid / sampleCount)) + "%")

    end_time = time.time()

    print("")
    print("- Durée : %s secondes !" % (end_time - start_time))
    print("")

