from hashlib import sha256
import json
from Crypto.Cipher import AES

def encrypt(msg:str):
    '''
        PADDING
    '''
    jstr = json.dumps(msg)
    while len(jstr)%16!=0:
        jstr+="="
    key = sha256(b'hello').hexdigest()
    cipher = AES.new((int(key,16)).to_bytes(32,'big'), AES.MODE_ECB)
    # print(len(jstr.encode()),jstr.encode(),key,len(key),cipher.encrypt(jstr).hex())
    return json.dumps({"hash":sha256(cipher.encrypt(jstr)).hexdigest(),"message":cipher.encrypt(jstr).hex()})

def toStr(temp:float,gsr:int,hr:int):
    return encrypt(json.dumps({"temp":temp,"gsr":gsr,"hr":hr}))
