from VLC_utils import *

data = ORIGINAL_DATA


if __name__ == "__main__":
    transmitted_data = add_frame_number(data)
    print(transmitted_data)
    print("original data length: ", len(data))
    print("data length with frame number: ",len(transmitted_data))

