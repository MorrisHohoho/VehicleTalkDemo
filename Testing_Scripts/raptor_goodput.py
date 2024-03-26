from VLC_utils import *
from crc import Calculator,Crc8

# Set the working directory to the python file location
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

testing_files_path = dname + "/Raptor"
fec_exe = "../VLC_Raptor/cmake-build-debug/VLC_Raptor"
data = RAPTOR_DATA
calculator = Calculator(Crc8.CCITT, optimized=True)
# 22 original symbols + 10 repaired symbols + 4 symbols are padding zero (we don't need to receive them)
total_symbols = 36 
symbol_size_in_frame = 6 + 1 +1 # frame number + data + crc
decoding_threshold = 0.8 


def generate_raptor_block(df):
    this_latency = 0

    symbols_collector = [None] * total_symbols
    symbols_collector_num = 0
    for row in df.itertuples():
        # Read each row, add one frame latency
        this_latency += RECEIVING_ONE_FRAME_LATENCY

        # Get each symbol
        this_frame = list(row)[1:]
        each_symbol_arr = np.array_split(this_frame, len(this_frame)/symbol_size_in_frame)

        # Check CRC
        for symbol in each_symbol_arr:
            data = symbol.tolist()[:-1]
            data_bytes = bytes(data)
            if calculator.verify(data_bytes, symbol[-1]) and symbol[0]<total_symbols and symbols_collector[symbol[0]] == None:
                symbols_collector[symbol[0]] = data
                symbols_collector_num+=1

        # Check whether we have enough symbols
        if symbols_collector_num >= int(total_symbols*decoding_threshold):
            ret = []
            for i in symbols_collector:
                if i!=None:
                    ret.extend(i)
            yield [ret,this_latency]
            this_latency = 0
            symbols_collector_num = 0
            symbols_collector = [None] * total_symbols
    return [[],0]

def raptor_decode(block):
    pass_str  = [fec_exe]
    for i in block:
        pass_str.append(str(i))    

    ret = runFEC(pass_str)
    ret_code = ret[0]
    fec_time = ret[1]
 
    if ret_code == 0:
        return (len(ORIGINAL_DATA),fec_time)
    else:
        return (0, fec_time)

if __name__ == "__main__":

    total_correct_packets = 0
    total_latency = 0


    for file in findAllFile(testing_files_path):

        df = readDataFrame(file)

        # Get original block
        for block in generate_raptor_block(df):
            this_block = block[0]
            this_latency = block[1]

            print(this_block)
            # original decode
            ret = raptor_decode(this_block)
            total_correct_packets += ret[0]
            total_latency += ret[1] + this_latency

        if total_latency == 0:
            print(file, " Can't even find a vaild block!")
            continue
        goodput = (total_correct_packets*8)/(total_latency/1000000)
        print(file.split('/')[-1], "original ","goodput: ",goodput,"bps")
        total_correct_packets = 0
        total_latency = 0
        