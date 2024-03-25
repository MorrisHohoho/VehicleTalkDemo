import os
import pandas as pd
import numpy as np 
import subprocess
import re
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

ORIGINAL_128bytes_data = "SCUNO1MsA7yYslHg2fXYO0oolrtsYQc8VeRF0JxFE8Zwrhk47KaJQ1ZnRKzmEC54PcRoTpmQWWPo1urzixZdvlmYtMnTx1mPWwShSD9pJ2HDGgBjC8yeJyqd8QPrUwl6"

# RS
RS_160_32_encoded_data = [83,67,85,78,79,49,77,115,65,55,121,89,115,108,72,103,50,102,88,89,79,48,111,111,108,114,116,115,89,81,99,56,86,101,82,70,48,74,120,70,69,56,90,119,114,104,107,52,55,75,97,74,81,49,90,110,82,75,122,109,69,67,53,52,80,99,82,111,84,112,109,81,87,87,80,111,49,117,114,122,105,120,90,100,118,108,109,89,116,77,110,84,120,49,109,80,87,119,83,104,83,68,57,112,74,50,72,68,71,103,66,106,67,56,121,101,74,121,113,100,56,81,80,114,85,119,108,54,85,235,49,170,121,233,186,54,133,116,231,1,40,58,237,4,139,210,243,98,135,152,99,180,26,67,30,63,172,180,237,6]
# Raptor
RAPTOR_160_6_symbol_size = 6 + 1 +1 # Symbol + id + CRC
RAPTOR_160_6_total_symbols = 32
RAPTOR_160_6_decode_threshold = 0.8
#Spinal
SPINAL_symbol_size = 5
SPINAL_encoded_data = [0, 218, 150, 134, 159, 238, 229, 70, 81, 58, 102, 2, 105, 81, 23, 166, 77, 72, 192, 230, 108, 150, 205, 101, 245, 200, 114, 219, 170, 60, 217, 174, 1, 159, 45, 212, 61, 110, 171, 115, 211, 33, 10, 252, 117, 21, 19, 45, 61, 235, 85, 15, 111, 177, 136, 40, 19, 96, 121, 37, 187, 113, 52, 244, 2, 187, 98, 118, 8, 222, 196, 71, 86, 54, 147, 183, 141, 74, 26, 51, 76, 153, 118, 2, 14, 178, 64, 170, 180, 67, 87, 8, 135, 62, 129, 250, 3, 30, 132, 20, 34, 81, 115, 81, 235, 76, 133, 247, 143, 51, 241, 6, 237, 137, 186, 182, 245, 179, 251, 43, 58, 236, 66, 250, 243, 137, 248, 74, 4, 254, 178, 233, 132, 189, 180, 133, 31, 107, 141, 127, 195, 122, 109, 205, 37, 155, 113, 57, 222, 174, 197, 93, 72, 192, 49, 231, 9, 197, 194, 37, 5, 53, 45, 54, 24, 249, 61, 237, 93, 45, 66, 106, 247, 159, 229, 168, 187, 237, 206, 101, 207, 119, 239, 35, 192, 231, 244, 33, 108, 160, 151, 208, 6, 245, 12, 93, 93, 231, 75, 171, 211, 180, 252, 228, 178, 63, 140, 6, 82, 23, 105, 122, 214, 101, 64, 78, 250, 117, 12, 52, 186, 12, 37, 209, 7, 101, 66, 14, 109, 107, 57, 77, 112, 163, 56, 243, 216, 75, 165, 218, 168, 173, 48, 85, 184, 124, 100, 184, 21, 107, 139, 138, 115, 56, 191, 194, 8, 188, 221, 52, 32, 179, 44, 80, 91, 94, 36, 35, 230, 66, 165, 46, 36, 75, 194, 90, 64, 233, 138, 234, 126, 218, 106, 217, 46, 15, 146, 81, 9, 135, 197, 55, 208, 201, 33, 184, 206, 70, 199, 20, 179, 146, 109, 181, 64, 32, 236, 142, 180, 206, 10, 209, 182, 96, 112, 8, 74, 59, 39, 117, 10, 162, 107, 197, 213, 85, 58, 39, 200, 30, 46, 128, 169, 114, 120, 31, 144, 94, 152, 213, 174, 198, 186, 0, 101, 239, 121, 144, 188, 40, 249, 147, 11, 16, 90, 170, 246, 176, 92, 99, 8, 242, 242, 23, 254, 139, 232, 73, 17, 85, 157, 15, 205, 213, 194, 144, 70, 210, 105, 39, 88, 212, 212, 49, 12, 113, 29, 255, 158, 197, 111, 228, 60, 213, 5, 131, 131, 106, 104, 148, 17, 36, 43, 239, 27, 96, 243, 245, 4, 198, 19, 161, 219, 71, 223, 11, 13, 78, 131, 11, 183, 147, 163, 228, 111, 199, 215, 60, 111, 210, 218, 235, 89, 125, 8, 115, 13, 35, 86, 64, 158]
# FEC parameters ends

# FEC executable programs location starts
RS_exe = "../VLC_RS/cmake-build-debug/VLC_RS"
RS_testing_files_path = dname + "/RS"

RAPTOR_exe ="../VLC_Raptor/cmake-build-debug/VLC_Raptor"
RAPTOR_testing_files_path = dname + "/Raptor"

SPINAL_exe = "../VLC_Spinal/bin/Test"
SPINAL_testing_files_path = dname + "/Spinal"

BASELINE_testing_files_path = dname + "/baseline"

# FEC executable programs location ends

# This testing setting
which_data = SPINAL_encoded_data
testing_files_path = SPINAL_testing_files_path
which_fec = "spinal" # "rs", "raptor", "spinal" "none"

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
    
    elif which_fec == "spinal":
        fec_exe = SPINAL_exe
    
    pass_str  = [fec_exe]
    for i in block:
        pass_str.append(str(i))    
    p = subprocess.Popen(pass_str,stdout = subprocess.PIPE, stdin = subprocess.PIPE)

    stdout, stderr= p.communicate()

    # Get the decode time
    temp = stdout.decode('utf-8')
    out = temp.split('\n')
    print(out)
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
    if p.returncode:
        return (0,time)
    else:
        return (len(ORIGINAL_128bytes_data), time)
        
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
        if symbols_collector_num >= RAPTOR_160_6_total_symbols*RAPTOR_160_6_decode_threshold:
            ret = []
            for i in symbols_collector:
                if i!=None:
                    ret.extend(i)
            yield [ret,this_latency]
            this_latency = 0
            symbols_collector_num = 0
            symbols_collector = [None] * RAPTOR_160_6_total_symbols
    return [[],0]


def helper_spinal_generator(frame):
    symbols_str = ''
    for symbol in frame:
        bin_str = bin(symbol)[2:].zfill(8)
        symbols_str+=bin_str
        
    # Spilt them into 8 bits per symbol
    symbols_arr = re.findall(r'.{5}',symbols_str)
    symbols_arr = list(map(lambda x: "0b"+x,symbols_arr))
    symbols_arr = list(map(lambda x: int(x,2),symbols_arr))

    return symbols_arr

def generate_Spinal_FEC_block(df):
    ret = [[],0]
    this_latency = 0

    # This time we need to add PSN
    total_frames_in_block = len(which_data)//(VLC_FRAME_LENGHT)
    if(len(which_data)%(VLC_FRAME_LENGHT) != 0):
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
            ret[0] = helper_spinal_generator(ret[0])
            ret[1] = this_latency
            yield ret
            ret = [[],0]
            this_latency = 0
            frame_collector = [None] * total_frames_in_block
            collect_frame_num = 0
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
    
    if which_fec == "spinal":
        yield from generate_Spinal_FEC_block(df)






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
    print(which_fec," goodput ",goodput,"bps")