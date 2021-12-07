import json
FILENAME = './databases/user_data.json'

def writeJson(json_data:dict):
    with open(FILENAME, 'w') as f:
        json.dump(json_data, f)

def readJson():
    with open(FILENAME,'r') as f:
        return json.load(f)
