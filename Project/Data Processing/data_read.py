
import numpy as np
def readfile(name):
    (count,l2)=(0,[])
    
    f=open(name,"r")
    f.readline()
    
    for i in f.readlines():
        s=i.replace('\x00','').strip()
        l=s.split(',')
        l=l[1:-1]
        l=[int(x) for x in l]
        count=count+1
        l2.append(l)
    #print(count)
    #print(l2)
    l2=np.array(l2)
    #print(type(l2))
    f.close()
    return l2
    
    
if __name__=="__main__":
       readfile("GEST_sample_1.txt") 
