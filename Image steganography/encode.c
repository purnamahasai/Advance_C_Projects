#include "encode.h"
#include "types.h"
#include "common.h"


 /*
 ENCODING : 
    Magic string
    Secret file extension size
    Secret file extension
    Secret file size
    Secret file data
 COPYING REMAINIG RGB DATA
 */


 /* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("\t INFO : width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("\t INFO : height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/*
 * Read and validating encoding arguments
 * INPUT : *argv[]
 * OUTPUT : storing file names
 * Return Value: e_success or e_failure, on file errors
 */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    //validating argv[2] .bmp
    if((argv[2] != NULL) && (strcmp(strrchr(argv[2], '.'), ".bmp") == 0))
    {
        encInfo->src_image_fname = argv[2];
        
    }
    else
    {
        return e_failure;
    }

    //validating argv[3] secret file and file is empty or not
    if(argv[3] != NULL && strchr(argv[3], '.')!= NULL)
    {
        // validating file is empty or not
        FILE *temp_fptr = fopen(argv[3], "r");
        if(temp_fptr == NULL)
        {
            printf("FAILURE : SECRET FILE IS EMPTY\n");
            return e_failure;
        }
        fclose(temp_fptr);

        encInfo->secret_fname = argv[3];
        
        //storing secret file extension
        strcpy(encInfo->extn_secret_file, strchr(argv[3], '.'));
    }
    else
    {
        return e_failure;
    }

    //validating argv[4] .bmp
    if(argv[4] == NULL)
    {
        
        encInfo->stego_image_fname = "stego.bmp";
                
    }
    else
    {
        if(strcmp(strrchr(argv[4], '.'), ".bmp") == 0)
        {
            encInfo->stego_image_fname = argv[4];
            
        }
        else
        {
            return e_failure;            
        }
    }

    //validating argv[5] NULL
    if(argv[4] != NULL && argv[5] != NULL)
    {
        return e_failure;
    }    
    return e_success;
}

/*
 * Open files, check capacity of files, copying bmp header, encoding data, copy remaining data
 * INPUT : Magic string, EncodeInfo structure
 * OUTPUT : storing  header, encoded data and remaining data to output stego file
 * Return Value: e_success or e_failure, on file errors
*/

Status do_encoding(EncodeInfo *encInfo)
{
    printf("  #Opening the files");
    if(open_files(encInfo) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }
    printf("  #Checking capacity of File\n");
    if(check_capacity(encInfo) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }

    printf("  #Copying bmp header\n");
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
    }

    printf("  #Encoding Magic string\n");
    if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }

    printf("  #Encoding secret file extension size\n");
    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }

    printf("  #Encoding secret file extension\n");
    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }

    printf("  #Encoding size of secret file\n");
    if(encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
    }

    printf("  #Encoding Secret file data\n");
    if(encode_secret_file_data(encInfo) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }

    printf("  #Copying remaining rgb data\n");
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        printf("\tSUCCESS\n");
    }
    else
    {
        printf("\tFAILURE\n");
        return e_failure;
    }
    return e_success;
}

/*
 * checking capacity of .bmp  file
 * INPUT : Magic string, EncodeInfo structure
 * OUTPUT : image capacity, size of secret file
 * Return Value: e_success or e_failure, on capacity of .bmp file
*/

Status check_capacity(EncodeInfo *encInfo)
{
    //Size of rgb data
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    //find size of secret file
    encInfo->size_secret_file =  get_file_size(encInfo->fptr_secret);
    printf("\t INFO : Size of secrete file : %d\n", encInfo->size_secret_file);

    if (encInfo->image_capacity  >  ((strlen(MAGIC_STRING) * 8) + 32 +((strlen(encInfo->extn_secret_file) * 8)) + 32 + ((encInfo->size_secret_file) * 8) ))
    {
        return e_success;
    }
    return e_failure;    
}

/*
 * Getting file size
 * INPUT : file pointer
 * OUTPUT : size of file
 * Return Value : size of file
*/
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

/*
 * Copying BMP header
 * INPUT : file pointer of source bmp file, output bmp file (strgo)
 * OUTPUT : Copying BMP header ( 54 bytes ) to output bmp file
 * Return Value : e_success if copying success 
*/

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);
    char buffer[54];
    fread(buffer, 54, 1, fptr_src_image);
    fwrite(buffer, 54, 1, fptr_dest_image);
    return e_success;
}

/*
 * Encoding magic string
 * INPUT : magic string, file pointer of source bmp file, output bmp file (strgo)
 * OUTPUT : Call encode_data_to_image() to encode magic string
 * Return Value : e_success or e_failure based on encode_data_to_image() 
*/

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    return e_success;
    else
    return e_failure;
}

/*
 * Encoding data to image (stego)
 * INPUT : data, size of data, file pointer of source bmp file, output bmp file (strgo)
 * OUTPUT : Call encode_byte_to_lsb() to encode  byte and DO it on size times -->> store encoded data in stego image
 * Return Value : e_success or e_failure based on storing data to stego bmp file 
*/

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    for(int i=0; i<size; i++)
    {
        fread(buffer, 8, 1, fptr_src_image);
        encode_byte_to_lsb(data[i], buffer);
        fwrite(buffer, 8, 1, fptr_stego_image);
    }
    return e_success;
}

/*
 * Encoding 1 byte data to image LSB(stego)
 * INPUT : data(1 Byte) and image buffer(8 Bytes)
 * OUTPUT : storing data to image buffer LSB
 * Return Value : e_success or e_failure based on storing data to buffer 
*/

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0; i<8; i++)
    {
        image_buffer[i] = ((image_buffer[i] & 0xFE) | ((data & (1<<i)) >> i));
    }
    return e_success;
}

/*
 * Encoding 4 byte data to image LSB(stego)
 * INPUT : data(4 Byte) and image buffer(32 Bytes)
 * OUTPUT : storing data to image buffer LSB
 * Return Value : e_success or e_failure based on storing data to buffer 
*/

Status encode_size_to_lsb(int data, char *image_buffer)
{
    for(int i=0; i<32; i++)
    {
        image_buffer[i] = ((image_buffer[i] & 0xFE) | ((data & (1<<i)) >> i));
    }
    return e_success;
}

/*
 * Encoding secret file extension size
 * INPUT : file pointer of source bmp file, output bmp file (strgo)
 * OUTPUT : storing secret file extension size and encoding secret file extension size
 * Return Value : e_success if encoding secret file extension size is done
*/

Status encode_secret_file_extn_size(int file_extn_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, 32,1, encInfo->fptr_src_image);
    encode_size_to_lsb(file_extn_size, buffer);
    fwrite(buffer, 32,1, encInfo->fptr_stego_image);
    return e_success;
}

/*
 * Encoding secret file extension
 * INPUT : secret file extension, file pointer of source bmp file, output bmp file (strgo)
 * OUTPUT : Call encode_data_to_image() to encode secret file extension
 * Return Value : e_success if encoding secret file extension is done else return e_failure
*/

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    if(encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    return e_success;
    else
    return e_failure;
}

/*
 * Encoding secret file size
 * INPUT : file pointer of source bmp file, output bmp file (strgo)
 * OUTPUT : storing secret file size and encoding secret file size
 * Return Value : e_success if encoding secret file size is done
*/

Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, 32,1, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, buffer);
    fwrite(buffer, 32,1, encInfo->fptr_stego_image);
    return e_success;
}

/*
 * Encoding secret file data
 * INPUT : secret file size,  file pointer of source bmp file, output bmp file (strgo)
 * OUTPUT : Call encode_data_to_image() to encode secret file data
 * Return Value : e_success if encoding secret file data is done else return e_failure
*/

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[encInfo->size_secret_file + 1];
    rewind(encInfo->fptr_secret);
    fread(buffer, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    buffer[encInfo->size_secret_file] = '\0';
    //printf("\t %s\n", buffer);
    if(encode_data_to_image(buffer, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }    
}

/*
 * Copying remaining bytes
 * INPUT : file pointer of source bmp file, output bmp file (strgo)
 * OUTPUT : data copied to stego bmp  file
 * Return Value : e_success if copying  remaining data is done.
*/

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch, 1, 1, fptr_src) != 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}

