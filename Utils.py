import json
import os
import sys

import Implementation.MLP.MLP as MLP
import Implementation.Linear.Linear as Linear
import Implementation.RBF.RBF as RBF


def load(_filename):
    dirname = os.path.dirname(__file__)
    _filename = os.path.join(dirname, "Models/" + _filename)

    with open(_filename, mode='r+') as f:
        content = json.load(f)

    value = "Erreur lors du chargement !"
    algo_name = content['type'].upper()
    
    if algo_name == "MLP":
        value = MLP.create(content)

    elif algo_name == "LINEAR":
        value = Linear.create(content)

    elif algo_name == "RBF":
        value = RBF.create(content)

    return value, algo_name


def predict(model, algo_name, XToPredict):
    if algo_name.upper() == "MLP":
        return MLP.predict(model, XToPredict, True)

    elif algo_name.upper() == "LINEAR":
        return Linear.predict_classification(model, XToPredict)

    elif algo_name.upper() == "RBF":
        return RBF.naive_rbf_classification_predict(model, XToPredict)


def save(model, algo_name, _filename):

    value = {}
    if algo_name.upper() == "MLP":
        value = MLP.export(model)

    elif algo_name.upper() == "LINEAR":
        value = Linear.export(model)

    elif algo_name.upper() == "RBF":
        value = RBF.export(model)
        pass

    json_val = json.dumps(value)

    _fileName = algo_name + "_" + _filename + '.model'
    dirname = os.path.dirname(__file__)
    _fileName = os.path.join(dirname, "Models/" + _fileName)

    file = open(_fileName, "w+")
    file.write(json_val)
    file.close()

    print(_fileName + ' saved')
