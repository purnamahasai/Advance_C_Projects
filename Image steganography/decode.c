#include "decode.h"
#include "types.h"
#include "common.h"


 /*
 DECODING : 
    Magic string
    Secret file extension size
    Secret file extension
    Secret file size
    Secret file data
 */

 /*
 * Read and validating decoding arguments
 * INPUT : *argv[]
 * OUTPUT : storing file names
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //validating argv[2]  .bmp
    if(argv[2]!= NULL && strcmp(strrchr(argv[2], '.'), ".bmp") == 0)
    {
        decInfo->bmp_file_name = argv[2];
    }
    else
    {
        return e_failure;
    }

    //validating argv[3] output 
    if(argv[3] == NULL)
    {
        strcpy(decInfo->output_file_name , "output");
    }
    else
    {        
        if(strrchr(argv[3], '.')!= NULL)
        {
            char *temp = strrchr(argv[3], '.');
            *temp = '\0';
        }
        strcpy(decInfo->output_file_name , argv[3]);

        //validating any extra argument passed or not
        if(argv[4] != NULL)
        {
            return e_failure;
        }
    }
    return e_success;
}

/*
 * Open files, decoding data, and validating it
 * INPUT : DecodeInfo structure
 * OUTPUT : getting decoded magic string, secret file extension length and extension, secret file size and data
 * Return Value: e_success or e_failure, on all decoding operations
*/

Status do_decoding(DecodeInfo *decInfo)
{
    printf("  #Opening decoding files\n");
    if(open_decoding_files(decInfo) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }
     
    //set bmp file pointer to 55th byte to pass header
    fseek(decInfo->fptr_stego, 54, SEEK_SET);

    //Decoding magic string
    printf("  #Decoding magic string\n");
    if(decode_magic_string(decInfo)==e_success)
    {
        printf("\tSUCCESS\n");
        
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }

    printf("  #Decoding secret file extension size\n");
    if(decode_secret_file_extn_size(&(decInfo->secret_file_extn_size), decInfo) == e_success)
    {
        printf("\tSUCCESS\n");
        //printf("%d", decInfo->secret_file_extn_size);
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }

    printf("  #Decoding secret file extension\n");
    if(decode_secret_file_extn(decInfo)==e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {       
        printf("\tFAILURE\n");
        return e_failure;
    }

    printf("  #Opening output file\n");
    if(open_output_file(decInfo) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }

    printf("  #Decoding secret file size\n");
    if(decode_secret_file_size(&(decInfo->secret_file_size), decInfo) == e_success)
    {
        printf("\tSUCCESS\n");
        //printf("%d\n", decInfo->secret_file_size);
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }

    printf("  #Decoding secret file data and store it on output file\n");
    if(decode_secret_file_data(decInfo) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
    }
    return e_success;
}

/* 
 * Get File pointers for i/p file (stego)
 * Inputs: Stego Image file
 * Output: FILE pointer for .bmp file
 * Return Value: e_success or e_failure, on file errors
 */

Status open_decoding_files(DecodeInfo *decInfo)
{
    // BMP Image file
    decInfo->fptr_stego = fopen(decInfo->bmp_file_name, "r");
    // Do Error handling
    if (decInfo->fptr_stego == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->bmp_file_name);

    	return e_failure;
    }
    return e_success;
}

/*
 * Decoding 1 byte data from image LSB(stego)
 * INPUT : data(1 Byte) and image buffer(8 Bytes)
 * OUTPUT : getting data from image buffer LSB and store it on data
 * Return Value : e_success based on getting data
*/

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    *data = 0;
    for(int i=0; i<8; i++)
    {
        *data |= (image_buffer[i] & 0x01) << i;
    }
    //putchar(*data);
    return e_success;
}

/*
 * Decoding 4 byte data from image LSB(stego)
 * INPUT : data(4 Byte) and image buffer(32 Bytes)
 * OUTPUT : getting size from image buffer LSB and store it on data
 * Return Value : e_success based on getting size 
*/

Status decode_size_from_lsb(int *data, char *image_buffer)
{
    for(int i=0; i<32; i++)
    {
        *data |= (image_buffer[i] & 0x01) << i;
    }
    return e_success;
}

/*
 * Decoding magic string and compare it with Original magic string
 * INPUT : magic string (local buffer), magic string(original), file pointer of .bmp file (strgo)
 * OUTPUT : Call decode_data_from_image() to decode magic string
 * Return Value : e_success or e_failure based on encode_data_to_image() 
*/

Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_buffer[strlen(MAGIC_STRING)+1]; 
    if(decode_data_from_image(magic_buffer, strlen(MAGIC_STRING), decInfo->fptr_stego)!= e_success)
    {
        
        return e_failure;
    }

    printf("\t  Magic string matching\n");
    if(strcmp(MAGIC_STRING, magic_buffer) == 0)
    {
        //puts(magic_buffer);
        printf("\t  SUCCESS\n");
    }
    else
    {
        printf("\t  FAILURE\n");
        return e_failure;
    } 
    return e_success;
}

/*
 * Decoding data to image (stego)
 * INPUT : magic_string(local), size of magic_string(local), file pointer of source bmp file (strgo)
 * OUTPUT : Call decode_byte_from_lsb() to decode  byte and DO it on size times -->> 
            store decoded data in magic buffer it will store decoded magic string
 * Return Value : e_success or e_failure based on storing data to stego bmp file 
*/

Status decode_data_from_image(char magic_buffer[], int len, FILE *fptr_stego)
{
      
    char buffer[8];
    int i;
    for(i=0; i<len; i++)
    {
        fread(buffer, 8, 1, fptr_stego);
        decode_byte_from_lsb(&magic_buffer[i], buffer);
        //putchar(magic_buffer[i]);        
    }
    magic_buffer[i] = '\0';
    
    return e_success;
}

/*
 * Decoding secret file extension size
 * INPUT : file_extension size, file pointer of source bmp file (strgo)
 * OUTPUT : decoding secret file extension size by calling decode_size_from_lsb()
 * Return Value : e_success if decoding secret file extension size is done
*/

Status decode_secret_file_extn_size(int *file_extn_size, DecodeInfo *decInfo)
{
    char buffer[32];
    *file_extn_size = 0;
    fread(buffer, 32, 1, decInfo->fptr_stego);
    decode_size_from_lsb(file_extn_size, buffer);
    return e_success;
}

/*
 * Decoding secret file extension
 * INPUT : extensionbuffer (local), file pointer of source bmp file (strgo)
 * OUTPUT : Call decode_data_from_image() to decode secret file extension
            and conactinate secret file extension to output file
 * Return Value : e_success if decoding secret file extension is done else return e_failure
*/

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char extn_buffer[(decInfo->secret_file_extn_size)+1];
    
    if(decode_data_from_image(extn_buffer, decInfo->secret_file_extn_size, decInfo->fptr_stego)!= e_success)
    {
      
        return e_failure;
    }
    strcat(decInfo->output_file_name, extn_buffer);
    //puts(decInfo->output_file_name);
    return e_success;
}

/* 
 * Get File pointers for i/p file (stego)
 * Inputs: Decinfo structure
 * Output: FILE pointer for output file
 * Return Value: e_success or e_failure, on file errors
 */

Status open_output_file(DecodeInfo *decInfo)
{
    decInfo->fptr_output = fopen(decInfo->output_file_name, "w");
    if(decInfo->fptr_output == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_file_name);

    	return e_failure;
    }
    return e_success;
}

/*
 * Decoding secret file size
 * INPUT : file size(loacl), file pointer of source bmp file (strgo)
 * OUTPUT : call decode_size_from_lsb() to decode size 
 * Return Value : e_success if decoding secret file size is done
*/

Status decode_secret_file_size(int *file_size, DecodeInfo *decInfo)
{
    char buffer[32];
    *file_size = 0;
    fread(buffer, 32, 1, decInfo->fptr_stego);
    decode_size_from_lsb(file_size, buffer);
    return e_success;
}

/*
 * Decoding secret file data
 * INPUT : secret file size (get from decode_secret_file_size()),  file pointer of source bmp file (strgo)
 * OUTPUT : Call decode_data_from_image() to decode secret file data
 * Return Value : e_success if decoding secret file data is done else return e_failure
*/


Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char output[decInfo->secret_file_size];
    char buffer[8];
    int i;
    for(i=0; i<(decInfo->secret_file_size); i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego);
        decode_byte_from_lsb(&output[i], buffer);
    }
    fwrite(&output, decInfo->secret_file_size, 1, decInfo->fptr_output);
    //printf("%s output\n", output);

    return e_success;
}

