from hashlib import sha256
import json
import random
from Crypto.Cipher import AES
from oneM2M_functions import *


server = "https://esw-onem2m.iiit.ac.in"
cse = "/~/in-cse/in-name"
ae = "/Team-28/Node-1"
container_name = "/Data"


def encrypt(msg: str):
    """
    PADDING
    """
    jstr = json.dumps(msg)
    while len(jstr) % 16 != 0:
        jstr += "="
    key = sha256(b"hello").hexdigest()
    cipher = AES.new((int(key, 16)).to_bytes(32, "big"), AES.MODE_ECB)
    return json.dumps(
        {
            "hash": sha256(cipher.encrypt(jstr)).hexdigest(),
            "message": cipher.encrypt(jstr).hex(),
        }
    )


def toStr(temp: float, gsr: int, hr: int):
    return encrypt(json.dumps({"temp": temp, "gsr": gsr, "hr": hr}))


fhand = open("test.txt", "w+")

for _ in range(10):
    temp = random.randint(0, 100)
    gsr = random.randint(0, 100)
    hr = random.randint(0, 100)
    fhand.write(f"{temp} {gsr} {hr}\n")
    create_data_cin(server + cse + ae + container_name, toStr(temp, gsr, hr))
