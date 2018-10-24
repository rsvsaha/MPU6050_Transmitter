

import numpy as np
from sklearn.preprocessing import scale
from scipy.interpolate import interp1d

class Sample:
    def __init__(self,accx,accy,accz,gyx,gyy,gyz):
        self.accx=accx
        self.accy=accy
        self.accz=accz
        self.gyx=gyx
        self.gyy=gyy
        self.gyz=gyz
        
    def load_from_file(filename,size_fit=50):
            data_raw=[]
            acc_x=[]
            acc_y=acc_z=gyro_x=gyro_y=gyro_z=[]
            acc_x_stretch=acc_y_stretch=acc_z_stretch=gyro_x_stretch=gyro_y_stretch=gyro_z_stretch=[]
            (facc_x,facc_y,facc_z,fgyro_x,fgyro_y,fgyro_z)=([],[],[],[],[],[])
            j=0
            
            f=open(filename,"r")
            f.readline()
            
            for i in f.readlines():
                s=i.replace('\x00','').strip()
                xsd=([*map(lambda x: int(x), s.split(",")[1:-1])])
                data_raw.append(xsd)
            
            data=np.array(data_raw).astype(float)
            
            data_norm=scale(data)
           
            for i in range(0,5):
                acc_x.append(data_norm[:,j])
                acc_y.append(data_norm[:,j+1])
                acc_z.append(data_norm[:,j+2])
                gyro_x.append(data_norm[:,j+3])
                gyro_y.append(data_norm[:,j+4])
                gyro_z.append(data_norm[:,j+5])
                j=j+6
            
            length_along_x_axis=np.linspace(0,data.shape[0],data.shape[0])
           
            for i in range(0,5):
                facc_x1=interp1d(length_along_x_axis,acc_x[i])
                facc_x.append(facc_x1)
                facc_y1=interp1d(length_along_x_axis,acc_y[i])
                facc_y.append(facc_y1)
                facc_z1=interp1d(length_along_x_axis,acc_z[i])
                facc_z.append(facc_z1)
                fgyro_x1=interp1d(length_along_x_axis,gyro_x[i])
                fgyro_x.append(fgyro_x1)
                fgyro_y1=interp1d(length_along_x_axis,gyro_y[i])
                fgyro_y.append(fgyro_y1)
                fgyro_z1=interp1d(length_along_x_axis,gyro_z)
                fgyro_z.append(fgyro_z1)
            
            new_length_along_x_axis=np.linspace(0,data.shape[0],size_fit)
            
            for i in range(0,5):
                acc_x_stretch.append(facc_x[i](new_length_along_x_axis))
                acc_y_stretch.append(facc_y[i](new_length_along_x_axis))
                acc_z_stretch.append(facc_z[i](new_length_along_x_axis))
                gyro_x_stretch.append(fgyro_x[i](new_length_along_x_axis))
                gyro_y_stretch.append(fgyro_y[i](new_length_along_x_axis))
                gyro_z_stretch.append(fgyro_z[i](new_length_along_x_axis))
            #print(len(acc_x_stretch[0]))
            
            
            print(acc_x_stretch[0])
            return Sample(acc_x_stretch,acc_y_stretch,acc_z_stretch,gyro_x_stretch,gyro_y_stretch,gyro_z_stretch)
        
#if __name__=="__main__":
#       Sample.load_from_file("NEW_TEST_sample_1.txt") 
