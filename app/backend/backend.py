from hashlib import sha256
import json
from typing import Protocol
from flask import *
import flask
from flask_cors import CORS
from Crypto.Cipher import AES
from app.backend.script import decrypt, encrypt
from get_data import get_all_data
# creating a flask app
app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})


@app.route('/authenticate',methods=['GET','POST'])
def authenticate():
    return {'status':'ok'}


@app.route('/getData',methods=['GET','POST'])
def get_data():
    try:
    # if True:
        param = request.get_json()
        print(param)
        # data = get_all_data(param['username'])  # Returns dictionary {'message':...,'hash':...}
        op_ = get_all_data()[-1]['con']
        dec_key = param['key']
        print(op_)
        op = ""
        for i in range(len(op_)):
            if op_[i] == '\'':
                op += '\"'
            else: 
                op += op_[i]
        data = json.loads(op)  # Returns dictionary {'message':...,'hash':...}
        print('data',data,type(data),'key',dec_key)
        dec_msg = decrypt(data['message'],dec_key)
        if sha256(encrypt(dec_msg,dec_key)).hexdigest() == data['hash']:
            return {'status':'ok','message':dec_msg}
        else:
            return {'status':'error','log':'checksum failed'}
    except:
        return {'status':'error','log':'server error or invalid key'}


@app.route('/knock')
def knock():
    return '''
        <h1> Hey, I am a messenger from the server</h1>
        <p> Don't worry, I am up and running</p>
    '''        

if __name__ == '__main__':
    # run the app
    app.run(debug=True,port=5000)
