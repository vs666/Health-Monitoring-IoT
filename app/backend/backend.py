from hashlib import sha256
import json
from typing import Protocol
from flask import *
from flask_cors import CORS
from Crypto.Cipher import AES
from script import decrypt, encrypt
from get_data import get_all_data
from flask_talisman import Talisman
from addData import readJson,writeJson
import json
from ml import predML
import numpy as np
from datetime import datetime
# creating a flask app

app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})
Talisman(app,content_security_policy=None)


@app.route("/authenticate", methods=["GET", "POST"])
def authenticate():
    return {"status": "ok"}


@app.route("/getData", methods=["GET", "POST"])
def get_data():
    # try:
    if True:
        param = request.get_json()
        print(param)
        # data = get_all_data(param['username'])  # Returns dictionary {'message':...,'hash':...}
        op_ = get_all_data()[-1]["con"]
        dec_key = param["key"]
        print(op_)
        op = ""
        for i in range(len(op_)):
            if op_[i] == "'":
                op += '"'
            else:
                op += op_[i]
        data = json.loads(op)  # Returns dictionary {'message':...,'hash':...}
        print('data',data,type(data),'key',dec_key)
        dec_msg = decrypt(data['message'],dec_key)
        if True or sha256(encrypt(dec_msg,dec_key)).hexdigest() == data['hash']:
            update_data(json.loads(dec_msg))
            response = {'status':'ok','message':dec_msg,'covidstat':getMLPrediction(dec_msg)}
            print('RESPONSE : ',response)
            return response
        else:
            return {'status':'error','log':'checksum failed'}
    # except:
        # return {'status':'error','log':'server error or invalid key'}


@app.route("/knock")
def knock():
    return """
        <h1>Hey, I am a messenger from the server</h1>
        <p>Don't worry, I am up and running</p>
    """
@app.route("/updateInfo",methods=["GET","POST"])
def updateInfo():
    param = request.get_json()
    print(param)
    di = {}
    with open('./databases/user_info.json','r') as f:
        di = json.load(f)
    for i in param.keys():
        di[i] = param[i]
    with open('./databases/user_info.json','w') as f:
        json.dump(di,f)

    return {"status":"ok"}


def update_data(record):
    jsD = readJson()
    if 'data' not in jsD.keys():
        jsD['data'] = []
    jsD['data'].append(record)
    writeJson(jsD)    




def getMLPrediction(info):
    di = {}
    info = json.loads(info)
    with open('./databases/user_info.json','r') as f:
        di = json.load(f)
    dataPoint = []
    dataPoint.append(int(di['gender']))
    dataPoint.append(0) # race_White
    dataPoint.append(0) # race_AA
    dataPoint.append(0) # race_Other
    dataPoint.append(0) # ethnicity_Hispanic
    dataPoint.append(int(di['age']))
    dataPoint.append(4) # patientClass
    dataPoint.append(0) # encountered type
    dataPoint.append(999) # reason for visit
    dataPoint.append(int(info["bpsys"]))
    dataPoint.append(int(info["bpdia"]))
    dataPoint.append(float(info["temp"]))
    dataPoint.append(int(info["hrate"]))
    dataPoint.append(22)    # RR
    dataPoint.append(float(info["spo2"]))
    dataPoint.append(float(di["BMI"]))
    dataPoint.append(1.85)  # BSA
    dataPoint.append(datetime.now().month-1)
    print(np.array(dataPoint))
    try:
        return predML(dataPoint)
    except:
        return 0 
if __name__ == "__main__":
    # run the app
    app.run(debug=True, port=5000)
