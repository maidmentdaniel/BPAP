"""
Thu Nov  9 16:10:27 2017
Daniel Maidment
maidment.daniel@gmail.com
13271793
"""
import numpy as np
import matplotlib.pyplot as plt
import serial
from datetime import date
import winsound
import plotting_functions as pf

Freq = 2500 # Set Frequency To 2500 Hertz
Dur = 1000 # Set Duration To 1000 ms == 1 second
now = date.today()
filename = "20{}_raw_data.txt".format(now.strftime('%y_%m_%d'))

def acquireSamples(size = 1024):
    counter = 0
    #statedefinition
    s0 = 0
    s1 = 1
    s2 = 2
    s3 = 3
    s4 = 4
    #current state intialisation
    cs = s0
    #variables
    done = False	
    #begin serial com
    ser = serial.Serial('COM23', 230400) #230400
    file = open(filename, 'w')
	#begin fsm loop
    while(done  == False): #counter value to run loop twice, i.e.full sweep
        if(cs == s0):
            if(ser.in_waiting >=0):
                serCheck = ser.readline().decode('ascii').strip()
                if(serCheck == "s0"):
                    cs = s1
                else:
                    cs = s0
            else:
                cs = s0		
        elif(cs == s1):#tell the arduino to start sweep
            ser.write(b'1')
            print('ready')
            cs = s2
			
        elif(cs == s2):#read data from the arduino, loop until data exists to be read
            
            if(ser.in_waiting >=0):
                serData = ser.readline().decode('ascii').strip()
                if(serData == "s0"):
                    cs = s1
                else:		
                    cs = s3
            else:
                cs = s2
				
        elif(cs == s3):#add data to arrays to be plotted, if the last value in the array >= 5 then all the sweep has been completed
            line = [float(f) for f in serData.split()]
            file.write('{:6.5}, '.format(line[0]*5/255))
            counter = counter+1
            cs = s2
            if(counter >= size-1):
                cs = s4
        
        elif(cs == s4):#escape loop
            if(ser.in_waiting >=0):
                serData = ser.readline().decode('ascii').strip()
                if(serData == "s0"):
                    cs = s1
                else:		
                    line = [float(f) for f in serData.split()]
                    file.write('{:6.5}\n'.format(line[0]*5/255))
                    done = True
            
            
        else:#default reverts to unkown state
            winsound.Beep(Freq,Dur)
            print("Error: voltageSweep() default state.\nAttemoting to go to state s1.\n")
            cs = s1
		
    ser.close()#close serial communications opening port for other use
    file.close()
    print('done')

def readData(filename = 'default'):
    file = open(filename, 'r')
    line = file.readline().strip()
    data = [float(x) for x in line.split(', ')]
    file.close()
    return data
    
if __name__ == "__main__"   :
    N = 1024
    f_s = 9600
    f_d = 2400
    scale = 8
    acquireSamples(N)
    data = readData(filename)
    sp = np.fft.fft(data)*2
    
    t = np.linspace(0, 0.10667, 1024)
    f_axis = np.arange(0, f_d, f_s/N)
    
    plt.ion()
    
    f, axarr = plt.subplots(3)
    axarr[0].set_ylim((0, 5))
    line1, = axarr[0].plot(t[1:int(N/scale)], data[1:int(N/scale)], color = 'black')
    line2, = axarr[1].plot(f_axis, np.log10(np.abs(sp))[0:len(f_axis)], color = 'black')
    line3, = axarr[2].plot(f_axis, np.angle(sp)[0:len(f_axis)], color = 'black')
    
    axarr[0].set_ylabel('Volts [V]')
    axarr[0].set_xlabel('time (s)')
    axarr[1].set_ylabel('20log(V)')
    axarr[2].set_xlabel('Frequency [Hz]')
    axarr[2].set_ylabel('Phase')
    plt.tight_layout()
    
    count = 0;
    while(count<10):
        acquireSamples(N)
        data = readData(filename)
        sp = np.fft.fft(data)*2
        
        line1.set_ydata(data[1:int(N/scale)])
        line2.set_ydata(np.log10(np.abs(sp))[0:len(f_axis)])
        line3.set_ydata(np.angle(sp)[0:len(f_axis)])
        f.canvas.draw()
        plt.pause(0.005)
        count +=1
    plt.ioff        

