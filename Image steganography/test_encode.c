#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{  

    //validating argc whether mandotory arguments passed or not
    if(argc > 1)
    {
        //finding opeartion with argv[1]
        switch(check_operation_type(argv[1]))
        {

            case e_encode:

            printf("**************ENCODING****************\n");
            printf("  #Reading and validating Encoding arguments\n");
            EncodeInfo encInfo;            
            //read and validating remaining arguments for encoding
            if(read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                printf("\tSUCCESS\n");
                if(do_encoding(&encInfo) == e_success)
                {
                    printf("**********SUCCESS : Encoding***********\n");
                }
                else
                {
                    printf("**********FAILURE : Encoding***********\n");
                }
            }
            else
            {
                printf("\tFAILURE \n");
            }
            break;


            case e_decode:

            printf("**************DECODING****************\n");
            printf("  #Reading and validating Decoding arguments\n");
            DecodeInfo decInfo;
            //read and validating remaining arguments for decoding
            if(read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                printf("\tSUCCESS \n");
                if(do_decoding(&decInfo) == e_success)
                {
                    printf("**********SUCCESS : Decoding***********\n");                    
                }
                else
                {
                    printf("**********FAILURE : Decoding***********\n");
                }
            }
            else
            {
                printf("\tFAILURE\n");
            }
            break;


            case e_unsupported:
            printf("Invalid operation\npass -e or -d  to select encoding or decoding\n");
            return 1;
        }
    }

    else
    {
        printf("Fail : pass mandotary arguments for encodin or decoding\n");
    }

    return 0;
}


//finding operation type
OperationType check_operation_type(char *argv)
{
    if(!strcmp(argv, "-e"))
    {
        return e_encode;
    }
    else if(!strcmp(argv, "-d"))
    {
        return e_decode;
    }
    return e_unsupported;
}

