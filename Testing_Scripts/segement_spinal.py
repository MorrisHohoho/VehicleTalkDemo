from VLC_utils import *

data = SPINAL_DATA
spinal_symbol_size = 5


def segment_data(this_data):
    symbols_str = ''
    for symbol in this_data:
        bin_str = bin(symbol)[2:].zfill(spinal_symbol_size)
        symbols_str+=bin_str
        
    # Spilt them into 8 bits per symbol
    symbols_arr = cut_text(symbols_str,8)
    for i in range(len(symbols_arr)):
        if(len(symbols_arr[i])<8):
            symbols_arr[i] = symbols_arr[i].ljust(8,'0')
    symbols_arr = list(map(lambda x: "0b"+x,symbols_arr))
    symbols_arr = list(map(lambda x: int(x,2),symbols_arr))

    # Add PSN
    return add_frame_number(symbols_arr)


if __name__ == "__main__":
    transmitted_data = segment_data(data)
    print(transmitted_data)
    print("original data length: ", len(data))
    print("data length with frame number: ",len(transmitted_data))

