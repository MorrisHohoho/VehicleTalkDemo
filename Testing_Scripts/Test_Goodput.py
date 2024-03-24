import os
import pandas as pd
import numpy as np 
import subprocess

RS_160_32_original_data = "SCUNO1koNavS4Ts/TsDrR A3<oSvoGt6^{KmWKS0Ld%;I7=a5m*WAPkr(L<!#+Q| y%Wc4Gp^Idj_ky5L[64j+Q9]ApsJrF#Jp-W";
RS_160_32_block_len = 160

receiving_one_frame_latency = 5133 # us

original_data = RS_160_32_original_data
block_len = RS_160_32_block_len

def findAllFile(base):
    '''
    Find all files in the `base` directory, in absolute path.

    Args:
        base (string): The searching directory in absolute path.

    Returns:
        (string): File path in abosulte.
    '''
    for root, ds, fs in os.walk(base):
        for f in fs:
            fullname = os.path.join(root, f)
            yield fullname

def FEC_decode(block,latency):
    p = subprocess.Popen('')


def generate_FEC_block(df,FEC_pass_len):
    '''
    Generate a FEC block from the dataframe.
    
    Args:
        df (pandas_dataframe): The dataframe containing the received data.
        FEC_pass_len (int): The length of each FEC pass.
    
    Returns:
        (list): The FEC block. This first element is the block, the second element is the receiving latency.
    '''
    this_pass = 0
    this_latency = 0
    ret = [[],0]
    for row in df.itertuples():
        # The first column is the panda dataframe index
        # The second column is our pass number
        if(row[1] == this_pass):
            temp =  list(row)[2:]
            ret[0].extend(temp)
            this_pass+=1
            this_latency += receiving_one_frame_latency
            if(len(ret[0])>=FEC_pass_len):
                ret[1] = this_latency
                yield ret
                ret = [[],0]
                this_pass = 0
                this_latency = 0




if __name__ == "__main__":

# Set the working directory to the python file location
    abspath = os.path.abspath(__file__)
    dname = os.path.dirname(abspath)
    os.chdir(dname)

    total_correct_packets = 0
    total_latency = 0

    for i in findAllFile(dname+'/RS'):
        # Read the input testing data file and discard the first and final line
        df = pd.read_csv(i,engine='python',header=None,skiprows=1,skipfooter=1)
        # Drop the last column, which is all NaN
        df = df.drop(df.columns[-1],axis=1)
        # Convert to int64
        df = df.astype(np.int64)

        # Get original block
        for block in generate_FEC_block(df,RS_160_32_block_len):
            this_block = block[0:RS_160_32_block_len]
            this_latency = block[1]
            # FEC decode
            FEC_decode(this_block,this_latency)