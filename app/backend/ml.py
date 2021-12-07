import sys
import pickle
import pandas as pd
import sklearn.metrics as skm


def predML(dataPoint):
    models = ['3Class', 'CPosvCNeg', 'FvOthers', 'FvC']
    model = pickle.load(open(f"models/{models[1]}.pkl", 'rb'))
    return model.predict(dataPoint)
  