import os,process
import numpy as np
from sklearn.externals import joblib
def create(SAVE=True):
    x_data=[]
    y_data=[]
    count=0
    Encoder={}
    Decoder={}
    root="Samples"
    
    
    for path,subdirs,files in os.walk(root):
        for name in files:
            filename=os.path.join(path,name)
            #print(filename)
            
            Samp=process.Sample.load_from_file(filename)
            Sample_after_concat=np.concatenate((Samp.accx,Samp.accy,Samp.accz,Samp.gyx,Samp.gyy,Samp.gyz),axis=0)
            
            x_data.append(Sample_after_concat)
            
            reqd_name=name.split('_')[0]
            
            if reqd_name not in Encoder.keys():
                Encoder[reqd_name]=count
                count=count+1
                
            y_data.append(Encoder[reqd_name])
    
    
    
    x_data=np.array(x_data)
    y_data=np.array(y_data)
    
    
    
    for key in Encoder.keys():
        Decoder[Encoder[key]]=key
    
    
    Data=(x_data,y_data)
    
    if SAVE==True:
        joblib.dump(Data,"Dataset.pkl")
        joblib.dump(Encoder,"Word_to_Int_Dictionary.pkl")
        joblib.dump(Decoder,"Int_to_Word_Dictionary.pkl")
    
    return (x_data,y_data,Decoder)
    
    #print(d)
    #print(len(x_data[0]))   
    #print(len(y_data))         
