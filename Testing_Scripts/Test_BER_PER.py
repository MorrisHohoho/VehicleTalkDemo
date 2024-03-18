import os
import pandas as pd

### Global Varibales ###
testing_8bytes_mes = "SCU:No1!"
testing_16bytes_mes = "SCU?No1!no1?vlc!"

cur_test_mes = testing_16bytes_mes
### Global Varibales End ###


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

def get_corrupted_bits(r,t):
    '''
    Check whether each bit is corrupted in r compared to t.

    Args:
        r (int): The received data
        t (int): The transmitted data, which is assumed to be correct.
    
    Returns:
        (int): Total correct bits.
    '''

    binary_r = bin(r)[2:].zfill(8)
    binary_t = bin(t)[2:].zfill(8)
    ret = 0
    for i in range(len(binary_t)):
        if binary_t[i]!=binary_r[i]:
            ret+=1
    return ret

def check_BER_PER(df,original_data):
    '''
    Check bit error ratio, BER = corrupted_bits/all_bits

    Args:
        df (pandas_dataframe): The dataframe containing the received data.
        original_data (string): The original transmitted data.
 
    Returns:
        (BER in double,PER in double): The calculated BER and PER.
    '''

    # Some varibles declaration
    ber = 0.0
    per = 0.0
    corrupted_bits = 0
    all_bits = len(df)*(len(original_data)+1)*8 # all packets * each packet length in byte * 8 bits
    corrupted_packets = 0
    all_packets = len(df)
    packet_corrupted_flag = False
    
    # Convert the original_data to ASCII first
    data_in_ascii =[0] # The SQN
    for ch in original_data:
        data_in_ascii.append(ord(ch))


    for row in df.itertuples():
        packet_corrupted_flag = False
        for i in range(len(row)-1):
            corrupted_bits+=get_corrupted_bits(row[i+1],data_in_ascii[i])
            if row[i+1] != data_in_ascii[i]:
                packet_corrupted_flag = True
        
        if packet_corrupted_flag:
            corrupted_packets+=1

    
    ber = corrupted_bits/all_bits
    per = corrupted_packets/all_packets
            
    return (ber,per)
    

# Set the working directory to the python file location
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

for i in findAllFile(dname+'/baseline'):
    # Read the input testing data file and discard the first and final line
    df = pd.read_csv(i,engine='python',header=None,skiprows=1,skipfooter=1)
    # Drop the last column, which is all NaN
    df = df.drop(df.columns[-1],axis=1)
    # Check BER and PER
    ret = check_BER_PER(df,cur_test_mes)
    print(i,ret)