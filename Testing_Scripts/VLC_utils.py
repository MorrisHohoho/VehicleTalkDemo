
import os
import pandas as pd
import numpy as np
import subprocess
import re

# VLC physical layer configuration starts
VLC_FRAME_LENGHT = 32 
RECEIVING_ONE_FRAME_LATENCY = 5133 # us
# VLC physical layer configuration ends

# VLC data starts
ORIGINAL_DATA = "SCUNO1MsA7yYslHg2fXYO0oolrtsYQc8VeRF0JxFE8Zwrhk47KaJQ1ZnRKzmEC54PcRoTpmQWWPo1urzixZdvlmYtMnTx1mPWwShSD9pJ2HDGgBjC8yeJyqd8QPrUwl6"
RS_DATA=[83,67,85,78,79,49,77,115,65,55,121,89,115,108,72,103,50,102,88,89,79,48,111,111,108,114,116,115,89,81,99,56,86,101,82,70,48,74,120,70,69,56,90,119,114,104,107,52,55,75,97,74,81,49,90,110,82,75,122,109,69,67,53,52,80,99,82,111,84,112,109,81,87,87,80,111,49,117,114,122,105,120,90,100,118,108,109,89,116,77,110,84,120,49,109,80,87,119,83,104,83,68,57,112,74,50,72,68,71,103,66,106,67,56,121,101,74,121,113,100,56,81,80,114,85,119,108,54,85,235,49,170,121,233,186,54,133,116,231,1,40,58,237,4,139,210,243,98,135,152,99,180,26,67,30,63,172,180,237,6]
RAPTOR_DATA = [0,83,67,85,78,79,49,1,77,115,65,55,121,89,2,115,108,72,103,50,102,3,88,89,79,48,111,111,4,108,114,116,115,89,81,5,99,56,86,101,82,70,6,48,74,120,70,69,56,7,90,119,114,104,107,52,8,55,75,97,74,81,49,9,90,110,82,75,122,109,10,69,67,53,52,80,99,11,82,111,84,112,109,81,12,87,87,80,111,49,117,13,114,122,105,120,90,100,14,118,108,109,89,116,77,15,110,84,120,49,109,80,16,87,119,83,104,83,68,17,57,112,74,50,72,68,18,71,103,66,106,67,56,19,121,101,74,121,113,100,20,56,81,80,114,85,119,21,108,54,0,0,0,0,26,48,194,75,96,8,27,27,138,248,105,169,193,89,28,246,89,74,190,105,2,29,86,163,80,37,32,21,30,159,36,254,107,120,6,31,214,93,20,213,116,66,32,27,209,219,58,68,175,33,7,205,213,175,97,219,34,35,220,242,12,97,197,35,183,134,248,45,171,98]
SPINAL_DATA = [27,10,11,8,13,7,31,14,28,21,3,5,2,14,19,6,0,9,20,21,2,5,29,6,9,21,4,12,1,25,19,12,18,27,6,22,11,29,14,8,14,11,13,26,20,15,6,25,21,26,15,18,27,21,1,29,13,26,21,23,7,20,25,1,1,11,30,7,10,5,8,19,5,20,30,30,22,21,8,15,13,30,24,24,16,10,0,19,12,1,28,18,11,14,27,17,6,19,26,11,22,24,19,22,1,3,15,12,8,17,26,22,6,26,9,27,15,3,10,10,3,8,25,20,25,6,11,22,0,8,7,11,4,16,5,10,22,17,1,21,14,2,4,7,7,26,0,31,20,7,20,4,2,16,17,5,2,28,26,17,29,13,6,8,11,29,28,15,6,15,24,16,13,27,12,9,23,10,27,15,11,12,31,27,5,12,29,14,24,16,23,26,30,14,4,31,16,18,23,30,22,11,20,24,9,15,13,20,16,20,15,22,23,3,11,31,24,13,29,6,27,19,9,5,19,13,24,19,19,23,21,14,24,21,14,20,17,16,1,17,28,28,4,28,11,16,17,5,6,20,22,19,12,6,7,25,7,23,22,21,26,11,10,2,13,11,27,25,31,25,13,8,23,15,22,28,28,25,14,15,14,31,23,18,7,16,7,7,30,16,16,22,25,8,4,23,26,3,26,16,24,23,10,29,28,29,5,26,23,20,29,20,31,19,18,11,4,15,28,12,0,25,9,1,14,26,11,26,26,25,18,20,0,19,23,26,14,20,6,3,9,14,16,12,4,23,8,22,10,16,16,14,13,21,21,19,18,19,11,16,20,12,28,15,7,22,2,11,20,23,13,10,17,11,9,16,10,22,28,7,24,25,5,24,2,21,21,24,23,2,19,19,7,2,31,28,5,15,6,29,6,16,16,11,6,11,2,16,11,13,15,2,8,8,31,6,8,10,18,18,28,9,2,11,24,9,13,4,1,26,12,10,29,9,31,13,20,26,22,25,5,24,7,25,4,20,12,7,24,20,27,29,1,18,9,1,23,3,7,4,13,17,24,20,22,14,9,6,27,13,10,0,4,3,22,8,29,13,6,14,1,11,8,27,12,24,3,16,1,1,5,3,22,9,27,21,20,9,21,28,11,21,10,21,7,8,19,28,16,7,17,14,16,2,20,23,4,30,0,31,18,1,15,9,17,21,13,14,24,26,29,0,0,25,15,15,15,6,8,11,24,10,7,25,18,12,8,5,21,10,23,22,22,1,14,6,6,2,7,18,30,8,11,31,29,2,31,8,9,4,8,21,11,7,8,15,25,23,10,28,5,4,2,6,26,9,20,18,14,22,6,20,26,16,24,23,2,7,15,31,19,27,2,22,31,25,1,28,26,20,2,24,7,0,27,10,13,2,10,1,2,9,1,11,29,28,13,22,1,28,31,21,0,19,3,1,7,8,14,27,8,31,15,16,22,19,20,3,1,14,27,25,7,8,31,4,13,31,3,29,14,15,3,15,26,11,13,14,22,22,11,29,1,1,25,16,26,8,26,22,8,2,15,14]
# VLC data ends

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



def readDataFrame(file_path):
        '''
        Read the file of the file_path and return the pandas dataframe.

        Args:
            file_path (string): The intended data file path.

        Returns:
            (pandas dataframe): Data in pandas dataframe.
        '''
        # Read the input testing data file and discard the first and final line
        df = pd.read_csv(file_path,engine='python',header=None,skiprows=1,skipfooter=1)
        # Drop the last column, which is all NaN
        df = df.drop(df.columns[-1],axis=1)
        # Convert to int64
        df = df.astype(np.int64)
        return df

def runFEC(args): 
    '''
    Pass the data into the fec and execute.

    Args:
        args (list): fec path + data

    Returns:
        (decode_correct, time).
    '''
    p = subprocess.Popen(args,stdout = subprocess.PIPE, stdin = subprocess.PIPE)

    stdout, stderr= p.communicate()

    # Get the decode time
    temp = stdout.decode('utf-8')
    out = temp.split('\n')
    # print(out)
    decode_time = 0
    encode_time = 0
    for i in out:
        index = i.find("decode time:")
        last_index = i.find("decode_time_end")
        if index != -1:
            decode_time = i[index+len("decode time:"):last_index]
            decode_time = float(decode_time)

    for i in out:
        index = i.find("encode time:")
        last_index = i.find("encode_time_end")
        if index != -1:
            encode_time = i[index+len("encode time:"):last_index]
            encode_time = float(encode_time)
            
        time = encode_time*1000000 + decode_time*1000000

    return (p.returncode,time)

def cut_text(text,lenth): 
    textArr = re.findall('.{'+str(lenth)+'}', text)
    textArr.append(text[(len(textArr)*lenth):]) 
    return textArr 

def add_frame_number(this_data):
    transmitted_data = []
    PSN = 0
    jjj =0;
    for i in this_data:
        if(jjj%(VLC_FRAME_LENGHT-1) == 0):
            transmitted_data.append(PSN)
            PSN = PSN + 1
        transmitted_data.append(i)
        jjj+=1
    return transmitted_data