#include "encoder/encoder.h"
#include "decoder/decoder.h"

#include <stdio.h>
#include <time.h>

int main(int argc, char**argv)
{
    char *message = "SCUNO1MsA7yYslHg2fXYO0oolrtsYQc8VeRF0JxFE8Zwrhk47KaJQ1ZnRKzmEC54PcRoTpmQWWPo1urzixZdvlmYtMnTx1mPWwShSD9pJ2HDGgBjC8yeJyqd8QPrUwl6";
    uint8_t *symbols=SpinalEncode(message);
    printf("origin:%s\n",message);

    printf("Spinal code symbols:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d,",symbols[i]);
    }
    printf("\n");

    symbols[2] = 5;
    symbols[3] = 5;


    uint8_t decoded_message[MES_LENGTH+1];
    decoded_message[MES_LENGTH]='\0';
    clock_t start_t,end_t;

    start_t=clock();
    SpinalDecode(symbols,decoded_message);
    end_t=clock();


    printf("time:%f\n",(double)(end_t-start_t)/CLOCKS_PER_SEC);
    free(symbols);

    printf("decoded:%s",decoded_message);
    printf("\n");
    return 0;
}
