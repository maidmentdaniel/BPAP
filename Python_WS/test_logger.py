import serial
# from datetime import date
import time
import numpy as np


if __name__ == "__main__":

    port = "COM5"
    ser = serial.Serial(port, 9600)
    user_in = 0
    counter = 0

    state = {
        "USER":     0,
        "SEND":     1,
        "RECEIVE":  2,
        "STOP":     3
    }

    cur_state = state["USER"]

    while(user_in != 'q'):
        if(cur_state == state["USER"]):
            #
            counter = 0
            print("USER state \n")
            user_in = input("Enter the value:\n")
            cur_state = state["SEND"]
            #
        elif(cur_state == state["SEND"]):
            #
            user_in_b = user_in.encode('UTF-8')
            ser.write(user_in_b)
            if(ser.out_waiting <= 0):
                cur_state = state["RECEIVE"]
                tic = time.time()
            #
        elif(cur_state == state["RECEIVE"]):
            #
            toc = time.time()
            if(ser.in_waiting > 0):
                serData = ser.read()
                # serData = ser.readline().decode('ascii').strip()
                print(serData, '\n')
                cur_state = state["USER"]
            elif(toc-tic >= 10 and counter <= 2):
                print("Wait for data timeout\n Trying to send again.")
                print("Counter: ", counter, '\n')
                cur_state = state["SEND"]
                counter += 1
            elif(counter > 2):
                print("Done Waiting")
                cur_state = state["USER"]
            else:
                cur_state = state["RECEIVE"]
            #
    print("DONE")
    ser.close()  # close serial communications opening port for other use
