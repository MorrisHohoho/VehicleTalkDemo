from VLC_utils import *

# Set the working directory to the python file location
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

testing_files_path = dname + "/Spinal"
fec_exe = "../VLC_Spinal/bin/Test"
data = SPINAL_DATA
symbol_size = 5

def helper_spinal_generator(frame):
    symbols_str = ''
    for symbol in frame:
        bin_str = bin(symbol)[2:].zfill(8)
        symbols_str+=bin_str
        
    symbols_arr = cut_text(symbols_str,symbol_size)
    symbols_arr = list(map(lambda x: "0b"+x,symbols_arr))
    symbols_arr = list(map(lambda x: int(x,2),symbols_arr))

    return symbols_arr[:len(data)]

def generate_spinal_block(df):
    ret = [[],0]
    this_latency = 0

    total_frames_in_block = ((len(data) * symbol_size)// 8) // (VLC_FRAME_LENGHT-1)
    if(len(data)%(VLC_FRAME_LENGHT) != 0):
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
            ret[0] = helper_spinal_generator(ret[0])
            ret[1] = this_latency
            yield ret
            ret = [[],0]
            this_latency = 0
            frame_collector = [None] * total_frames_in_block
            collect_frame_num = 0
    return [[],0]

def spinal_decode(block):
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
        for block in generate_spinal_block(df):
            this_block = block[0]
            this_latency = block[1]
            # original decode
            ret = spinal_decode(this_block)
            total_correct_packets += ret[0]
            total_latency += ret[1] + this_latency

        if total_latency == 0:
            print(file, " Can't even find a vaild block!")
            continue
        goodput = (total_correct_packets*8)/(total_latency/1000000)
        print(file.split('/')[-1], "original ","goodput: ",goodput,"bps")
        total_correct_packets = 0
        total_latency = 0
        