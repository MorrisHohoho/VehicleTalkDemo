from VLC_utils import *
from crc import Calculator,Crc8

calculator = Calculator(Crc8.CCITT, optimized=True)
data = RAPTOR_DATA
symbol_size = 6 + 1 # symbol + id 


def segment_data(this_data):
    raptor_symbols = np.array_split(this_data, len(this_data)/symbol_size)
    ret = []
    # add CRC
    for i in raptor_symbols:
        data = i.tolist()
        data_bytes = bytes(data)
        crc_checksum = calculator.checksum(data_bytes)
        ret.extend(data)
        ret.append(crc_checksum)
    return ret


if __name__ == "__main__":
    transmitted_data = segment_data(data)
    # temp = np.array_split(transmitted_data,len(transmitted_data)/32)
    # for i in temp:
    #     print(i.tolist())
    print(transmitted_data)
    print("original data length: ", len(data))
    print("data length with frame number: ",len(transmitted_data))

