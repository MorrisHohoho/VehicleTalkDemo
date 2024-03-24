import os
import pandas as pd
import numpy as np 
import subprocess


# Set the working directory to the python file location
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)


# Manchester code + UART latency 
RECEIVING_ONE_FRAME_LATENCY = 5133 # us
VLC_FRAME_LENGHT = 32 

# FEC parameters
RS_160_32_orignal_data = "SCUNO1MsA7yYslHg2fXYO0oolrtsYQc8VeRF0JxFE8Zwrhk47KaJQ1ZnRKzmEC54PcRoTpmQWWPo1urzixZdvlmYtMnTx1mPWwShSD9pJ2HDGgBjC8yeJyqd8QPrUwl6"
RS_160_32_block_len = 160 # The block num without PSN
RS_160_32_block_with_PSN_len = 166 # The block num with PSN
RS_160_32_original_len = len(RS_160_32_orignal_data)
RS_160_32_original_with_PSN_len = 133

# FEC executable programs location
ORI_exe = "NULL"
RS_exe = "../VLC_RS/cmake-build-debug/VLC_RS"
RS_testing_files_path = dname + "/RS"


# This testing setting
original_data = RS_160_32_orignal_data
block_len = RS_160_32_original_len
block_with_PSN_len = RS_160_32_original_with_PSN_len
fec_exe = ORI_exe
testing_files_path = RS_testing_files_path

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

def FEC_decode(which_fec,block):
    #Non encoding
    if which_fec == ORI_exe:
        received_str = ''
        for i in block:
            received_str += chr(i)
        if received_str == original_data:
            return(len(original_data),0)
        else:
            return (0,0)


    pass_str  = [which_fec]
    for i in block:
        pass_str.append(str(i))    
    p = subprocess.Popen(pass_str,stdout = subprocess.PIPE, stdin = subprocess.PIPE)

    stdout, stderr= p.communicate()

    # Get the decode time
    temp = stdout.decode('utf-8')
    out = temp.split('\n')
    for i in out:
        index = i.find("decode time:")
        if index != -1:
            decode_time = i[index+len("decode time:"):]
            decode_time = float(decode_time)
        
    if p.returncode:
        return (0,decode_time)
    else:
        return (len(original_data), decode_time)
        



def generate_FEC_block(df,bl,bpl):
    '''
    Generate a FEC block from the dataframe.
    
    Args:
        df (pandas_dataframe): The dataframe containing the received data.
        bl (int): The block length.
        bpl (int): The block length with PSN (block length + PSN length).
    
    Returns:
        (list): The FEC block. This first element is the block, the second element is the receiving latency.
    '''
    ret = [[],0]

    # Calculate total frames in one FEC block
    total_frames_in_block = bpl // VLC_FRAME_LENGHT
    if bpl%VLC_FRAME_LENGHT != 0:
        total_frames_in_block +=1

    this_latency = 0

    # Init the frame_collector
    frame_collector = []
    collect_frame_num = 0
    for i in range(total_frames_in_block):
        frame_collector.append([])

    for row in df.itertuples():

        # Read each row, add one frame latency
        this_latency += RECEIVING_ONE_FRAME_LATENCY

        # The first column is the panda dataframe index
        # The second column is our pass number
        this_frame_num = row[1]
        if this_frame_num > total_frames_in_block or len(frame_collector[this_frame_num])!=0:
            continue
        temp =  list(row)[2:]
        frame_collector[this_frame_num].extend(temp)
        collect_frame_num+=1
        if(collect_frame_num>=total_frames_in_block):
            for i in range(total_frames_in_block):
                ret[0].extend(frame_collector[i])
            ret[1] = this_latency
            yield ret
            ret = [[],0]
            this_latency = 0
            frame_collector = []
            collect_frame_num = 0
            for i in range(total_frames_in_block):
                frame_collector.append([])
    
    return ret





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
        for block in generate_FEC_block(df,block_len,block_with_PSN_len):
            this_block = block[0][:block_len]
            this_latency = block[1]

            # FEC decode
            ret = FEC_decode(fec_exe,this_block)
            total_correct_packets +=ret[0]
            total_latency += ret[1] + this_latency

    print("total correct packets in bytes:",total_correct_packets)
    print("total latency:", total_latency)
    goodput = (total_correct_packets*8)/(total_latency/1000000)
    print("goodput",goodput,"bps")