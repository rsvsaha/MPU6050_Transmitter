# -*- coding: utf-8 -*-
"""
Created on Fri Aug 31 15:53:42 2018

@author: Rishav
"""

import os,serial
#from sklearn.externals import joblib
import argparse
def add_sample(sample_name,sample_num,SERIAL_PORT="COM4",BAUD_RATE=9600,TIME_OUT=100):
    #word_dictionary=joblib.load('word_dictionary.pkl')
    #alpha_dictionary=joblib.load('spell_classes.pkl')
    #word_sample_id=len(word_dictionary)
    #alpha_sample_id=len(alpha_dictionary)
    ser=serial.Serial(SERIAL_PORT,BAUD_RATE,timeout=TIME_OUT)
    output=[]
    #current_sample=0
    root="Samples"
    recording=False
    try:
        while True:
            line=ser.readline().decode("UTF-8").replace("\r\n","")#This is very important
            print(line)
            if line=="Initialisng Sensors":
                print("Initialisng Sensors")
            elif line=="BATCH STARTED":
                print("Started Recording")
                recording=True
            elif line=="BATCH ENDED" and recording==True:
                print("Done Recording")
                filename="{word}_sample_{sample_no}.txt".format(word=sample_name,sample_no=sample_num)
                path=root+os.sep+filename
                f=open(path,"w")
                f.write('\n'.join(output))
                f.close()
                ser.close()
                break
            elif recording:
                output.append(line)
    except KeyboardInterrupt:
        ser.close()
        print("Closing")
        quit()    

ap=argparse.ArgumentParser()
ap.add_argument("-n","--name",required=True,help="Sample Name")
ap.add_argument("-o","--itr",required=True,help="Sample Number")
ap.add_argument("-c","--com",required=True,help="COM Port")
ap.add_argument("-b","--baud",required=True,help="baudrate")
args=vars(ap.parse_args())
add_sample(args["name"],args["itr"],args["com"],args["baud"])   
        