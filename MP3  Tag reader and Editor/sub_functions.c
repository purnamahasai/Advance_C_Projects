#ifndef MAIN_H
#include "main.h"
status validate_tag_version_read_tag_size(struct file_info *file1)
{
    //validating id3 
    // temprary buffer of 10 bytes
    char *header = malloc(10);
    // checking if memory allocated or not
    if(header == NULL)
        return failure;
    //read first 10 bytes from file
    fread(header,sizeof(char), 10, file1->fptr);
    //validating ID3 tag
    if(strncmp(header, "ID3", 3)==0)
    {
        //validating version
        if(header[3] == 0x03 && header[4] == 0x00)
        {
            //reading tag size in sync safe integer format  
            file1->tag_size = ((header[6] & 0x7F) << 21) | ((header[7] & 0x7F) << 14) | 
                                ((header[8] & 0x7F) << 7) | (header[9] & 0x7F);
            free(header);
            return success;
        }
    }
    return failure;
}

status openfile(struct file_info *file1, char *mode)
{
    file1->fptr = fopen(file1->file_name, mode);                
    if(file1->fptr == NULL)
    {
        printf("ERROR : File opening failed\n");
        return failure;
    }
    return success;
}

int string_to_size(char *frame_details)
{
    return (((frame_details[0] & 0xFF) << 24) | ((frame_details[1] & 0xFF) << 16) | 
                         ((frame_details[2] & 0xFF) << 8) | (frame_details[3] & 0xFF));
}

void size_to_string(char *updated_size, int size)
{
    for (int i = 0; i < 4; i++) {
        updated_size[i] = (size >> ((3 - i) * 8)) & 0xFF;
    }
}

status copy_file_content(FILE *src, FILE *dest)
{    
    //copying remaining data
    char ch;
    //read untill EOF of source file write that into dest file
    while(fread(&ch, sizeof(char), 1, src) == 1)
    {
        if(fwrite(&ch, sizeof(char), 1, dest) != 1)
        {
            printf("ERROR : File editing failed");
            return failure;
        }
    }
    //write EOF to dest file explicitely
    ch = EOF;
    fwrite(&ch, sizeof(char), 1, dest);
    return success;
}
#endif