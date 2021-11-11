from hashlib import sha256
def encrypt(inp,key):
    x = 0
    y = 0
    ph=""
    ret = ""
    while x < len(inp):
        if y == 0:
            y = 256
            ph += key
            ph = sha256(ph.encode('utf-8')).hexdigest()
        elt = hex(ord(inp[x])^ord(ph[64-y]))[2:]
        if len(elt) == 1:
            elt = '0'+elt
        # print(x,y,elt)
        ret += elt
        x += 1
        y-=1
    return ret

def decrypt(inp,key):
    assert(len(inp)%2==0)
    f_inp = []
    i = 0
    while i < len(inp):
        f_inp.append(int(inp[i:i+2],16))
        i+=2
    inp = f_inp
    x = 0
    y = 0
    ph=""
    ret = ""
    while x < len(inp):
        if y == 0:
            y = 64
            ph += key
            ph = sha256(ph.encode('utf-8')).hexdigest()
        elt = chr(inp[x]^ord(ph[64-y]))
        ret += elt
        x += 1
        y-=1
    return ret

# print(decrypt("5a060a5e5b"))
