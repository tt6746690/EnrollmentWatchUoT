
# coding: utf-8

# In[1]:
import pandas as pd 
import numpy as np 

from settings import DATA_201701



def filter_table(t):
    df = pd.read_table(t, sep='\t')
    

filter_table(DATA_201701)


# In[ ]:



