

import os
import sys
import numpy as np
import pprint
from ctypes import *


def get_platform():
    platforms = {
        'linux': 'Linux',
        'darwin': 'OSX',
        'win32': 'Windows'
    }
    if sys.platform not in platforms:
        return sys.platform
    return platforms[sys.platform]


dirname = os.path.dirname(__file__)
if get_platform() == "OSX":
    myDll = cdll.LoadLibrary(os.path.join(dirname, 'Librairie/Mac/RBF_Mac.so'))  # For Mac

elif get_platform() == "Linux":
    myDll = cdll.LoadLibrary(os.path.join(dirname, 'Librairie/Linux/RBF_Linux.so'))  # For Linux

elif get_platform() == "Windows":
    myDll = cdll.LoadLibrary(os.path.join(dirname, 'Librairie/Windows/RBF_Linux.dll'))  # For Windows


def rbf_predict(model, sample):
    myDll.rbf_predict.argtypes = [
        c_void_p,
        POINTER(c_double)
    ]
    myDll.rbf_predict.restype = POINTER(c_double)
    predict_value = myDll.rbf_predict(model, sample)
    return predict_value


def rbf_train(x, y, input_count_per_sample, sample_count, epochs=100, k=2, use_bias=False):
    myDll.rbf_train.argtypes = [
        POINTER(c_double, len(x)),
        POINTER(c_double, len(y)),
        POINTER(c_int32),
        POINTER(c_int32),
        POINTER(c_int32),
        POINTER(c_int32),
        POINTER(c_double),
        POINTER(c_bool)
    ]
    myDll.rbf_predict.restype = c_void_p
    model = myDll.rbf_train(x, y, input_count_per_sample, sample_count, epochs, k, use_bias)
    return model


def naive_rbf_train(x, y, input_count_per_sample, sample_count, gamma=100):
    c_x_double = (c_double * len(x))(*x)
    c_y_double = (c_double * len(y))(*y)

    myDll.naive_rbf_train.argtypes = [
        POINTER(c_double),
        POINTER(c_double),
        c_int32,
        c_int32,
        c_double
    ]
    myDll.naive_rbf_train.restype = c_void_p
    model = myDll.naive_rbf_train(
        c_x_double,
        c_y_double,
        input_count_per_sample,
        sample_count,
        gamma,
    )
    return model


def naive_rbf_regression_predict(model, sample):
    c_sample_double = (c_double * len(sample))(*sample)

    myDll.naive_rbf_regression_predict.argtypes = [
        c_void_p,
        POINTER(c_double)
    ]
    myDll.naive_rbf_regression_predict.restype = c_double
    predict_value = myDll.naive_rbf_regression_predict(model, c_sample_double)
    return predict_value


def naive_rbf_classification_predict(model, sample):
    c_sample_double = (c_double * len(sample))(*sample)

    myDll.naive_rbf_classification_predict.argtypes = [
        c_void_p,
        POINTER(c_double)
    ]
    myDll.naive_rbf_classification_predict.restype = c_int
    predict_value = myDll.naive_rbf_classification_predict(model, c_sample_double)
    return predict_value


def export(rbf):
    myDll.getWSize.argtypes = [c_void_p]
    myDll.getWSize.restype = c_int
    WSize = myDll.getWSize(rbf)

    myDll.getGamma.argtypes = [c_void_p]
    myDll.getGamma.restype = c_double
    Gamma = myDll.getGamma(rbf)

    myDll.getAllWValues.argtypes = [c_void_p]
    myDll.getAllWValues.restype = POINTER(c_double)
    WValues = myDll.getAllWValues(rbf)

    WValuesArray = []

    for i in range(WSize):
        WValuesArray.append(WValues[i])

    print(WValuesArray)

    return {"W": WValuesArray, 'gamma': Gamma, 'type': 'rbf'}


def create(content):
    W = content['W']
    WSize = len(W)

    WPointer = (c_double * len(W))(*W)

    myDll.create.argtypes = [
        POINTER(c_double),
        c_int,
    ]

    myDll.create.restype = c_void_p
    return myDll.create(WPointer, WSize)

