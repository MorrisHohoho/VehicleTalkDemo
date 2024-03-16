import os
import pandas as pd


def check_BER(df,original_data):
    for idx,data in df.iterrows():
        # iterrate each data and checks per bit.
        print(data)
    return 0
    

# Set the working directory to the python file location
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

# Read the input testing data file
df = pd.read_csv('data.txt',header=None)
# Discard the first and the last line
df = df.drop([0])
df = df.drop([len(df)])
check_BER(df,"123")