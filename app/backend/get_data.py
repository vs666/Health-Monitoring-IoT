"""
    This code gets data from Om2m and/or ThingSpeak for display.
    Parameterized by the username (same for all data)
"""
from oneM2M_functions import *

server = "http://127.0.0.1:8080"
cse = "/~/in-cse/in-name"
ae = "/Test-AE"
container_name = "/Data-1"

# [0: temperature, 1: blood pressure, 2: pulse rate]


def get_temperature():
    ret_code, latest_data = get_data(server + cse + ae + container_name + "/la")
    latest_data = eval(latest_data)
    return latest_data[0]


def get_blood_pressure():
    ret_code, latest_data = get_data(server + cse + ae + container_name + "/la")
    latest_data = eval(latest_data)
    return latest_data[1]


def get_pulse_rate():
    ret_code, latest_data = get_data(server + cse + ae + container_name + "/la")
    latest_data = eval(latest_data)
    return latest_data[2]


# Can also change to get-all-data and return everything together
def get_all_data():
    ret_code, all_data = get_whole_data(server + cse + ae + container_name + "?rcn=4")
    return all_data