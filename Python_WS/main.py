import serial
from datetime import date
import time


if __name__ == "__main__":
    filename = "./BPAP_{}_log.txt".format(date.today().strftime('%y_%m_%d'))
    port = "COM4"
    ser = serial.Serial(port, 9600)
    file = open(filename, 'w')
    N = 4094
    n = 0
    state = {
        "START":   0,
        "S1":   1,
        "S2":  2,
        "STOP":   3
    }

    cur_state = state["START"]
    while(n < N):
        if(ser.in_waiting >= 0):
            serData = ser.readline().decode('ascii').strip()
            file.write('{},'.format(serData))
            n += 1
    print("DONE")

    file.write('\n')
    file.write(time.ctime(time.time()))
    file.write('\n')

    ser.close()  # close serial communications opening port for other use
    file.close()
