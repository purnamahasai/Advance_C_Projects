#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include "encode.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define FILE_SUFFIX 10

typedef struct _DecodeInfo
{
    //.bmp file
    FILE *fptr_stego;
    char *bmp_file_name;
    //magic string
    char (*decoded_magic_String)[3];
    //secret file
    int secret_file_size;
    char *secret_file_extension;
    int secret_file_extn_size;

    //outputfile
    char output_file_name[FILE_SUFFIX];
    FILE *fptr_output;
}DecodeInfo;

//read and validating decoding arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for bmp file */
Status open_decoding_files(DecodeInfo *decInfo);

/* Decode a 4 bytes from LSB of image data array */
Status decode_size_from_lsb(int *data, char *image_buffer);

/* Decode a byte from LSB of image data array */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decoding Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode function, which does the real Decoding */
Status decode_data_from_image(char magic_buffer[], int size, FILE *fptr_stego_image);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(int *file_extn_size, DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Opening the output file */
Status open_output_file(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(int *file_size, DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decoded secret data validation*/
Status validating_decoded_secret_data(FILE *fptr_secret, FILE *fptr_output);

#endif