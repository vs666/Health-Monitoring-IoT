from hashlib import sha256
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
    try:
        param = request.get_json()
        data = get_all_data(param['username'])
        dec_key = param['key']
        decipher = AES.new(dec_key, AES.MODE_ECB)
        return {'status':'ok','message':str(decipher.decrypt(data))}
    except:
        return {'status':'error','log':'server error'}


@app.route('/knock')
def knock():
    return '''
        <h1> Hey, I am a messenger from the server</h1>
        <p> Don't worry, I am up and running</p>
    '''        

if __name__ == '__main__':
    # run the app
    app.run(debug=True,port=PORT)
