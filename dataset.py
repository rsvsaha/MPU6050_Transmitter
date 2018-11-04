

import os,process
import numpy as np
from sklearn.externals import joblib
def create(SAVE=False):
    x_data=[]
    y_data=[]
    count=0
    d={}
    root="Samples"
    
    
    for path,subdirs,files in os.walk(root):
        for name in files:
            filename=os.path.join(path,name)
            #print(filename)
            
            Samp=process.Sample.load_from_file(filename)
            Sample_after_concat=np.concatenate((Samp.accx,Samp.accy,Samp.accz,Samp.gyx,Samp.gyy,Samp.gyz),axis=0)
            
            x_data.append(Sample_after_concat)
            
            reqd_name=name.split('_')[0]
            
            if reqd_name not in d.keys():
                d[reqd_name]=count
                count=count+1
                
            y_data.append(d[reqd_name])
    DATA=(x_data,y_data)
    if SAVE==True:
        joblib.dump(DATA,"Dataset.pkl")
        joblib.dump(d,"Word_to_Int_Dictionary.pkl")
    return DATA
    
    #print(d)
    #print(len(x_data[0]))   
    #print(len(y_data))         

create() 
