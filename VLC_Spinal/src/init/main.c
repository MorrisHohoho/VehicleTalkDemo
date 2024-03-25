#include "encoder/encoder.h"
#include "decoder/decoder.h"

#include <stdio.h>
#include <time.h>

int main(int argc, char**argv)
{
    char *message = "SCUNO1MsA7yYslHg2fXYO0oolrtsYQc8VeRF0JxFE8Zwrhk47KaJQ1ZnRKzmEC54PcRoTpmQWWPo1urzixZdvlmYtMnTx1mPWwShSD9pJ2HDGgBjC8yeJyqd8QPrUwl6";
    printf("origin:%s\n",message);
    clock_t start_t,end_t;

    start_t=clock();
    uint8_t *symbols=SpinalEncode(message);
    end_t=clock();
    printf("Spinal encode time:%fencode_time_end\n",(double)(end_t-start_t)/CLOCKS_PER_SEC);

    printf("Spinal code symbols:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d,",symbols[i]);
    }
    printf("\n");

    int  total_symbols = SPINE_LENGTH*PASS;
    printf("total_symbols: %d\n",total_symbols);


    uint8_t decoded_message[MES_LENGTH+1];
    decoded_message[MES_LENGTH]='\0';

    uint8_t received_symbols[total_symbols];
    for(int i =1 ;i<argc;i++)
    {
        received_symbols[i-1] = (uint8_t)argv[i];
    }


    start_t=clock();
    SpinalDecode(received_symbols,decoded_message);
    end_t=clock();


    printf("Spinal decode time:%fdecode_time_end\n",(double)(end_t-start_t)/CLOCKS_PER_SEC);
    free(symbols);

    for(int i=0;i<strlen(message);i++)
    {
        if(message[i]!=decoded_message[i])
        {
            printf("Decode failed\n");
            return 1;
        }

    }
    return 0;
}
