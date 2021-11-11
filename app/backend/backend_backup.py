from hashlib import sha256
import json
from typing import Protocol
from flask import *
import flask
from flask_cors import CORS
from Crypto.Cipher import AES
from get_data import get_all_data
# creating a flask app
app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})


@app.route('/authenticate',methods=['GET','POST'])
def authenticate():
    return {'status':'ok'}


@app.route('/getData',methods=['GET','POST'])
def get_data():
    # try:
    if True:
        param = request.get_json()
        print(param)
        # data = get_all_data(param['username'])  # Returns dictionary {'message':...,'hash':...}
        op_ = get_all_data()[-1]['con']
        print(op_)
        op = ""
        for i in range(len(op_)):
            if op_[i] == '\'':
                op += '\"'
            else: 
                op += op_[i]
        data = json.loads(op)  # Returns dictionary {'message':...,'hash':...}
        print('data',data,type(data))#,'key',dec_key)
        # dec_key = param['key']
        dec_key = sha256(str(param['key']).encode('utf-8')).hexdigest()
        decipher = AES.new((int(dec_key,16).to_bytes(32,'big')), AES.MODE_CBC)
        decs = decipher.decrypt((int(data['message'],16).to_bytes(64,'big')))
        print(type(decs),decs)
        # count = 0
        # for i in decs:
        #     print(chr(i),'ee',count)
        #     count += 1
        first = 0

        last = len(decs)-1
        while chr(decs[first]) != '\"':
            first+=1
        while chr(decs[last]) != '\"':
            last-=1
        f_new = first
        t_str = ""
        while f_new < len(decs):
            t_str += chr(decs[f_new])
            f_new+=1
        de_new = ""
        # print(first,last)
        while first <= last:
            de_new += chr(decs[first])
            first+=1
        # print(de_new,'final')
        data_out = json.loads(de_new)
        if sha256(decipher.encrypt(t_str)).hexdigest() == data['hash']:
            return {'status':'ok','message':data_out}
        else:
            return {'status':'error','log':'checksum failed'}
    # except:
    #     return {'status':'error','log':'server error or invalid key'}


@app.route('/knock')
def knock():
    return '''
        <h1> Hey, I am a messenger from the server</h1>
        <p> Don't worry, I am up and running</p>
    '''        

if __name__ == '__main__':
    # run the app
    app.run(debug=True,port=5000)
