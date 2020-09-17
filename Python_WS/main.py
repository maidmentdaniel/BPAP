import serial
import numpy as np
from datetime import date

now = date.today()
filename = "20{}_raw_data.txt".format(now.strftime('%y_%m_%d'))
port = "COM23"

ser = serial.Serial(port, 9600)

state = {
    "START":   0,
    "FIRST":   1,
    "SECOND":  2,
    "THIRD":   3
}

print(state)
# while()
