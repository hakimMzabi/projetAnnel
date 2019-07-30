#
# Created by Baptiste Vasseur on 2019-05-27.
#
import json
import os
import sys
from ctypes import *


def get_platform():
    platforms = {
        'linux1': 'Linux',
        'linux2': 'Linux',
        'darwin': 'OSX',
        'win32': 'Windows'
    }
    if sys.platform not in platforms:
        return sys.platform

    return platforms[sys.platform]


dirname = os.path.dirname(__file__)
if get_platform() == "OSX":
    myDll = cdll.LoadLibrary(os.path.join(dirname, 'Librairie/Mac/MLP_Mac.so'))  # For Mac

elif get_platform() == "Linux":
    myDll = cdll.LoadLibrary(os.path.join(dirname, 'Librairie/Linux/MLP_Linux.so'))  # For Linux

elif get_platform() == "Windows":
    myDll = cdll.LoadLibrary(os.path.join(dirname, 'Librairie/Windows/MLP_Windows.so'))  # For Windows


def init(neurons):
    neuronSize = len(neurons)
    neuronPointer = (c_int32 * neuronSize)(*neurons)

    myDll.init.argtypes = [
        POINTER(ARRAY(c_int32, neuronSize)),
        c_int32
    ]

    myDll.init.restype = c_void_p
    return myDll.init(neuronPointer, neuronSize)


def fit_classification(mlp, XTrain, YTrain, sampleCount, epochs, alpha):
    XTrainPointer = (c_double * len(XTrain))(*XTrain)
    YTrainPointer = (c_double * len(YTrain))(*YTrain)

    myDll.fit_classification.argtypes = [
        c_void_p,
        POINTER(ARRAY(c_double, len(XTrain))),
        POINTER(ARRAY(c_double, len(YTrain))),
        c_int32,
        c_int32,
        c_double
    ]

    myDll.fit_classification.restype = c_void_p
    return myDll.fit_classification(mlp, XTrainPointer, YTrainPointer, sampleCount, epochs, alpha)


def fit_regression(mlp, XTrain, YTrain, sampleCount, epochs, alpha):
    lenX = len(XTrain)
    lenY = len(YTrain)
    XTrainFinal = (c_double * lenX)(*XTrain)
    YTrainFinal = (c_double * lenY)(*YTrain)

    myDll.fit_regression.argtypes = [
        c_void_p,
        POINTER(ARRAY(c_double, lenX)),
        POINTER(ARRAY(c_double, lenY)),
        c_int32,
        c_int32,
        c_double
    ]

    myDll.fit_regression.restype = c_void_p
    return myDll.fit_regression(mlp, XTrainFinal, YTrainFinal, sampleCount, epochs, alpha)


def predict(mlp, xToPredict, isClassif=False):
    pointr = (c_double * len(xToPredict))(*xToPredict)

    myDll.predict.argtypes = [
        c_void_p,
        POINTER(ARRAY(c_double, len(xToPredict))),
        c_bool
    ]

    myDll.predict.restype = POINTER(c_double)
    predictions = myDll.predict(mlp, pointr, isClassif)

    myDll.getPredictLength.argtypes = [c_void_p]
    myDll.getPredictLength.restype = c_int
    size = myDll.getPredictLength(mlp)

    result = []
    for i in range(size):
        result.append(predictions[i])

    return result


def export(mlp):
    myDll.getWSize.argtypes = [c_void_p]
    myDll.getWSize.restype = c_int
    WSize = myDll.getWSize(mlp)

    myDll.getAllWValues.argtypes = [c_void_p]
    myDll.getAllWValues.restype = POINTER(c_double)
    WValues = myDll.getAllWValues(mlp)

    myDll.getNeuronSize.argtypes = [c_void_p]
    myDll.getNeuronSize.restype = c_int
    neuronSize = myDll.getNeuronSize(mlp)

    myDll.getAllNeuronValues.argtypes = [c_void_p]
    myDll.getAllNeuronValues.restype = POINTER(c_int)
    neuronValues = myDll.getAllNeuronValues(mlp)

    WValuesArray = []
    neuronValuesArray = []

    for i in range(WSize):
        WValuesArray.append(WValues[i])
    for i in range(neuronSize):
        neuronValuesArray.append(neuronValues[i])

    return {"W": WValuesArray, "npl": neuronValuesArray, 'type': 'mlp'}


def create(content):

    npl = content['npl']
    layer_count = len(npl)

    W = content['W']
    WSize = len(W)

    nplPointer = (c_int * len(npl))(*npl)
    WPointer = (c_double * len(W))(*W)

    myDll.create.argtypes = [
        POINTER(ARRAY(c_int, layer_count)),
        c_int,
        POINTER(ARRAY(c_double, WSize)),
    ]

    myDll.create.restype = c_void_p
    return myDll.create(nplPointer, layer_count, WPointer)
