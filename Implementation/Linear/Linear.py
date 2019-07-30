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
    myDll = cdll.LoadLibrary(os.path.join(dirname, 'Librairie/Mac/Linear_Mac.so'))  # For Mac

elif get_platform() == "Linux":
    myDll = cdll.LoadLibrary(os.path.join(dirname, 'Librairie/Linux/Linear_Linux.so'))  # For Linux

elif get_platform() == "Windows":
    myDll = cdll.LoadLibrary(os.path.join(dirname, 'Librairie/Windows/Linear_Windows.so'))  # For Windows


def create_linear_model(inputCountPerSample):
    myDll.create_linear_model.argtypes = [c_int32]
    myDll.create_linear_model.restype = POINTER(c_double)
    result = myDll.create_linear_model(inputCountPerSample)

    values = []
    for i in range(inputCountPerSample + 1):
        values.append(result[i])

    return values


def fit_classification(W, XTrain, YTrain, alpha, epochs, inputCountPerSample=False):
    WPointer = (c_double * len(W))(*W)
    XTrainPointer = (c_double * len(XTrain))(*XTrain)
    YTrainPointer = (c_double * len(YTrain))(*YTrain)

    if not inputCountPerSample:
        inputCountPerSample = int(len(XTrain) / len(YTrain))

    sampleCount = int(len(XTrain) / inputCountPerSample)

    myDll.fit_classification.argtypes = [
        POINTER(ARRAY(c_double, inputCountPerSample + 1)),
        POINTER(ARRAY(c_double, len(XTrain))),
        POINTER(ARRAY(c_double, len(YTrain))),
        c_int32,
        c_int32,
        c_double,
        c_int32
    ]

    myDll.fit_classification.restype = POINTER(c_double)
    result = myDll.fit_classification(WPointer, XTrainPointer, YTrainPointer, sampleCount, inputCountPerSample, alpha, epochs)

    values = []
    for i in range(inputCountPerSample + 1):
        values.append(result[i])

    return {"model": values, "inputCountPerSample": inputCountPerSample, "sampleCount": sampleCount,
            'type': 'linear', 'liner_type': 'classification', "alpha": alpha, 'epochs': epochs}


def fit_regression(W, XTrain, YTrain, inputCountPerSample=False):
    WPointer = (c_double * len(W))(*W)
    XTrainPointer = (c_double * len(XTrain))(*XTrain)
    YTrainPointer = (c_double * len(YTrain))(*YTrain)

    if not inputCountPerSample:
        inputCountPerSample = int(len(XTrain) / len(YTrain))

    sampleCount = int(len(XTrain) / inputCountPerSample)

    myDll.fit_regression.argtypes = [
        POINTER(ARRAY(c_double, inputCountPerSample + 1)),
        POINTER(ARRAY(c_double, len(XTrain))),
        POINTER(ARRAY(c_double, len(YTrain))),
        c_int32,
        c_int32
    ]

    myDll.fit_regression.restype = POINTER(c_double)
    result = myDll.fit_regression(WPointer, XTrainPointer, YTrainPointer, sampleCount, inputCountPerSample)

    values = []
    for i in range(inputCountPerSample + 1):
        values.append(result[i])

    return {"model": values, "inputCountPerSample": inputCountPerSample, "sampleCount": sampleCount,
            'type': 'linear', 'liner_type': 'regression'}


def predict_classification(model, value):
    inputCountPerSample = model['inputCountPerSample']
    W = model['model']

    valuePointer = (c_double * len(value))(*value)
    WPointer = (c_double * len(W))(*W)

    myDll.predict_regression.argtypes = [
        POINTER(ARRAY(c_double, len(W))),
        POINTER(ARRAY(c_double, len(value))),
        c_int32,
        c_bool
    ]

    myDll.predict_classification.restype = c_double
    return myDll.predict_classification(WPointer, valuePointer, inputCountPerSample, True)


def predict_regression(model, value):
    inputCountPerSample = model['inputCountPerSample']
    W = model['model']

    valuePointer = (c_double * len(value))(*value)
    WPointer = (c_double * len(W))(*W)

    myDll.predict_regression.argtypes = [
        POINTER(ARRAY(c_double, len(W))),
        POINTER(ARRAY(c_double, len(value))),
        c_int32,
        c_bool
    ]

    myDll.predict_regression.restype = c_double
    return myDll.predict_regression(WPointer, valuePointer, inputCountPerSample, True)


def export(model):
    return model


def create(content):
    return content
