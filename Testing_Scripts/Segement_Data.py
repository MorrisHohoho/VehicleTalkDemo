from crc import Calculator,Crc8
import numpy as np 

# VLC physical layer configuration starts
VLC_FRAME_LENGHT = 32 
# VLC physical layer configuration ends

#CRC configuration
calculator = Calculator(Crc8.CCITT, optimized=True)
# CRC configuration ends

# FEC encoded data starts
RS_160_32_encoded_data=[83,67,85,78,79,49,77,115,65,55,121,89,115,108,72,103,50,102,88,89,79,48,111,111,108,114,116,115,89,81,99,56,86,101,82,70,48,74,120,70,69,56,90,119,114,104,107,52,55,75,97,74,81,49,90,110,82,75,122,109,69,67,53,52,80,99,82,111,84,112,109,81,87,87,80,111,49,117,114,122,105,120,90,100,118,108,109,89,116,77,110,84,120,49,109,80,87,119,83,104,83,68,57,112,74,50,72,68,71,103,66,106,67,56,121,101,74,121,113,100,56,81,80,114,85,119,108,54,85,235,49,170,121,233,186,54,133,116,231,1,40,58,237,4,139,210,243,98,135,152,99,180,26,67,30,63,172,180,237,6]
Raptor_160_6_encoded_data=[0,6,59,123,67,188,244,1,52,37,229,205,125,35,2,183,193,84,90,29,125,3,175,228,76,131,201,48,4,36,235,39,223,94,28,5,68,201,249,16,160,149,6,85,46,161,27,136,50,7,249,135,238,234,82,232,8,39,242,100,119,122,30,9,57,154,199,174,179,76,10,191,104,185,140,253,217,11,108,227,11,126,180,116,12,51,64,81,166,30,23,13,122,150,4,134,73,80,14,113,20,94,100,184,54,15,36,219,24,221,151,190,16,94,173,252,139,222,59,17,64,58,212,246,7,148,18,80,242,47,87,27,123,19,62,234,10,240,142,70,20,215,37,206,171,209,134,21,101,235,149,48,35,52,22,206,65,93,239,89,253,23,127,242,17,122,28,250,24,124,35,32,47,234,56,25,244,143,209,178,48,13,26,34,143,201,197,0,0,27,0,0,0,0,0,0,28,0,0,0,0,0,0,29,0,0,0,0,0,0,30,1,168,113,9,158,232,31,171,128,108,209,39,231,32,149,174,212,193,237,251,33,191,104,114,217,167,166,34,34,82,156,238,200,31,35,213,40,156,90,46,163]
Raptor_Symbol_Size = 6 + 1 # Symbol + id
# FEC encoded data ends

data = Raptor_160_6_encoded_data
which_fec = "raptor" # "rs", "raptor", "spinal"


def rs_segment_data(this_data):
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

def raptor_segment_data(this_data):
    raptor_symbols = np.array_split(this_data, len(this_data)/Raptor_Symbol_Size)
    ret = []
    # add CRC
    for i in raptor_symbols:
        data = i.tolist()
        data_bytes = bytes(data)
        crc_checksum = calculator.checksum(data_bytes)
        ret.extend(data)
        ret.append(crc_checksum)
    return ret

        


def segment_data(this_data,which_fec):
    if which_fec == "rs" or which_fec == "none":
        return rs_segment_data(this_data)
    elif which_fec == "raptor":
        return raptor_segment_data(this_data)


if __name__ == "__main__":
    transmitted_data = segment_data(data,which_fec)
    print(transmitted_data)
    print(len(transmitted_data))

