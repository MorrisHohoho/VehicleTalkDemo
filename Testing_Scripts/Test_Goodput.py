import os
import pandas as pd
import numpy as np 
import subprocess
from crc import Calculator,Crc8


# Set the working directory to the python file location
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)


# Manchester code + UART latency 
RECEIVING_ONE_FRAME_LATENCY = 5133 # us


# VLC physical layer configuration starts
VLC_FRAME_LENGHT = 32 
# VLC physical layer configuration ends

#CRC configuration
calculator = Calculator(Crc8.CCITT, optimized=True)
# CRC configuration ends


# FEC parameters starts

# RS
RS_160_32_orignal_data = "SCUNO1MsA7yYslHg2fXYO0oolrtsYQc8VeRF0JxFE8Zwrhk47KaJQ1ZnRKzmEC54PcRoTpmQWWPo1urzixZdvlmYtMnTx1mPWwShSD9pJ2HDGgBjC8yeJyqd8QPrUwl6"
RS_160_32_encoded_data = [83,67,85,78,79,49,77,115,65,55,121,89,115,108,72,103,50,102,88,89,79,48,111,111,108,114,116,115,89,81,99,56,86,101,82,70,48,74,120,70,69,56,90,119,114,104,107,52,55,75,97,74,81,49,90,110,82,75,122,109,69,67,53,52,80,99,82,111,84,112,109,81,87,87,80,111,49,117,114,122,105,120,90,100,118,108,109,89,116,77,110,84,120,49,109,80,87,119,83,104,83,68,57,112,74,50,72,68,71,103,66,106,67,56,121,101,74,121,113,100,56,81,80,114,85,119,108,54,85,235,49,170,121,233,186,54,133,116,231,1,40,58,237,4,139,210,243,98,135,152,99,180,26,67,30,63,172,180,237,6]
# Raptor
RAPTOR_160_6_original_data = [6,59,123,67,188,244,52,37,229,205,125,35,183,193,84,90,29,125,175,228,76,131,201,48,36,235,39,223,94,28,68,201,249,16,160,149,85,46,161,27,136,50,249,135,238,234,82,232,39,242,100,119,122,30,57,154,199,174,179,76,191,104,185,140,253,217,108,227,11,126,180,116,51,64,81,166,30,23,122,150,4,134,73,80,113,20,94,100,184,54,36,219,24,221,151,190,94,173,252,139,222,59,64,58,212,246,7,148,80,242,47,87,27,123,62,234,10,240,142,70,215,37,206,171,209,134,101,235,149,48,35,52,206,65,93,239,89,253,127,242,17,122,28,250,124,35,32,47,234,56,244,143,209,178,48,13,34,143,201,197]
RAPTOR_160_6_symbol_size = 6 + 1 +1 # Symbol + id + CRC
RAPTOR_160_6_total_symbols = 36
RAPTOR_160_6_decode_threhold = 0.8
# FEC parameters ends

# FEC executable programs location starts
RS_exe = "../VLC_RS/cmake-build-debug/VLC_RS"
RS_testing_files_path = dname + "/RS"

RAPTOR_exe ="../VLC_Raptor/cmake-build-debug/VLC_Raptor"
RAPTOR_testing_files_path = dname + "/Raptor"

# FEC executable programs location ends

# This testing setting
which_data = RAPTOR_160_6_original_data
testing_files_path = RAPTOR_testing_files_path
which_fec = "raptor" # "rs", "raptor", "spinal" "none"

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

def non_decode(block):
    received_str = ''
    for i in block:
        received_str += chr(i)
    if received_str == which_data:
        return(len(which_data),0)
    else:
        return (0,0)

def FEC_decode(which_fec,block):
    fec_exe = "NULL"

    #Non decoding
    if which_fec == "none":
        return non_decode(block)
    
    # RS decoding
    elif which_fec == "rs":
        fec_exe = RS_exe
    
    # Raptor decoding
    elif which_fec == "raptor":
        fec_exe =RAPTOR_exe
    
    pass_str  = [fec_exe]
    for i in block:
        pass_str.append(str(i))    
    p = subprocess.Popen(pass_str,stdout = subprocess.PIPE, stdin = subprocess.PIPE)

    stdout, stderr= p.communicate()

    # Get the decode time
    temp = stdout.decode('utf-8')
    out = temp.split('\n')
    decode_time = 0
    for i in out:
        index = i.find("decode time:")
        last_index = i.find("decode_time_end")
        if index != -1:
            decode_time = i[index+len("decode time:"):last_index]
            decode_time = float(decode_time)
        
    if p.returncode:
        return (0,decode_time)
    else:
        return (len(which_data), decode_time)
        
def generte_RS_FEC_block(df):
    ret = [[],0]
    this_latency = 0

    total_frames_in_block = len(which_data)//(VLC_FRAME_LENGHT-1)
    if(len(which_data)%(VLC_FRAME_LENGHT-1) != 0):
        total_frames_in_block +=1

    # Init the frame_collector
    frame_collector = [None]*total_frames_in_block
    collect_frame_num = 0

    for row in df.itertuples():
        # Read each row, add one frame latency
        this_latency += RECEIVING_ONE_FRAME_LATENCY

        # The first column is the panda dataframe index
        # The second column is our pass number

        # Check the frame number
        this_frame_num = row[1]
        if this_frame_num > total_frames_in_block or frame_collector[this_frame_num] != None:
            continue

        # Discard the frame number
        temp =  list(row)[2:]
        frame_collector[this_frame_num] = temp
        collect_frame_num+=1

        # Check whether we have enough frames
        if(collect_frame_num>=total_frames_in_block):
            for i in range(total_frames_in_block):
                ret[0].extend(frame_collector[i])
            ret[0] = ret[0][:len(which_data)]
            ret[1] = this_latency
            yield ret
            ret = [[],0]
            this_latency = 0
            frame_collector = [None] * total_frames_in_block
            collect_frame_num = 0
    return [[],0]

def generate_Raptor_block(df):
    this_latency = 0

    symbols_collector = [None] * RAPTOR_160_6_total_symbols
    symbols_collector_num = 0
    for row in df.itertuples():
        # Read each row, add one frame latency
        this_latency += RECEIVING_ONE_FRAME_LATENCY

        # Get each symbol
        this_frame = list(row)[1:]
        each_symbol_arr = np.array_split(this_frame, len(this_frame)/RAPTOR_160_6_symbol_size)

        # Check CRC
        for symbol in each_symbol_arr:
            data = symbol.tolist()[:-1]
            data_bytes = bytes(data)
            if calculator.verify(data_bytes, symbol[-1]):
                symbols_collector[symbol[0]] = data
                symbols_collector_num+=1
        
        # Check whether we have enough symbols
        if symbols_collector_num >= RAPTOR_160_6_total_symbols*RAPTOR_160_6_decode_threhold:
            ret = []
            for i in symbols_collector:
                if i!=None:
                    ret.extend(i)
            yield [ret,this_latency]
            this_latency = 0
            symbols_collector_num = 0
            symbols_collector = [None] * RAPTOR_160_6_total_symbols
    return [[],0]

def generate_FEC_block(which_fec,df):
    '''
    Generate a FEC block from the dataframe.
    
    Args:
        which_fec (string): The intended fec.
        df (pandas_dataframe): The dataframe containing the received data.
    
    Returns:
        (list): The FEC block. This first element is the block, the second element is the receiving latency.
    '''
    if which_fec == "none" or which_fec == "rs":
        yield from generte_RS_FEC_block(df)

    if which_fec == "raptor":
        yield from generate_Raptor_block(df)






if __name__ == "__main__":

    total_correct_packets = 0
    total_latency = 0


    for i in findAllFile(testing_files_path):
        # Read the input testing data file and discard the first and final line
        df = pd.read_csv(i,engine='python',header=None,skiprows=1,skipfooter=1)
        # Drop the last column, which is all NaN
        df = df.drop(df.columns[-1],axis=1)
        # Convert to int64
        df = df.astype(np.int64)

        # Get original block
        for block in generate_FEC_block(which_fec,df):
            this_block = block[0]
            this_latency = block[1]

            # FEC decode
            ret = FEC_decode(which_fec,this_block)
            total_correct_packets +=ret[0]
            total_latency += ret[1] + this_latency

            

    print("total correct packets in bytes:",total_correct_packets)
    print("total latency:", total_latency)
    goodput = (total_correct_packets*8)/(total_latency/1000000)
    print("goodput",goodput,"bps")