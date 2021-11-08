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
    try:
        param = request.get_json()
        data = get_all_data(param['username'])  # Returns dictionary {'message':...,'hash':...}
        dec_key = param['key']
        decipher = AES.new((int(sha256(dec_key).hexdigest(),16)).to_bytes(32,'big'), AES.MODE_ECB)
        data_out = json.loads(decipher.decrypt(data['message']).decode())
        if data_out['hash'] == sha256(data['message']).hexdigest():
            return {'status':'ok','message':data_out}
        else:
            return {'status':'error','log':'checksum failed'}
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
