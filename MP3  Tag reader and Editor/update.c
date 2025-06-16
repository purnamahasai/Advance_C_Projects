#ifndef MAIN_H
#include "main.h"
status write_data(char operation, char *data, struct file_info file)
{    
    switch (operation)
    {
        case 't':
        if(update("TIT2", file, data) != success)
        {
            return failure;
        }
        break;
        case 'a':
        if(update("TPE1", file, data) != success)
        {
            return failure;
        }
        break;
        case 'A':
        if(update("TALB", file, data) != success)
        {
            return failure;
        }
        break;
        case 'y':
        if(update("TYER", file, data) != success)
        {
            return failure;
        }
        break;
        case 'm':
        if(update("TCON", file, data) != success)
        {
            return failure;
        }
        break;
        case 'c':
        if(update("COMM", file, data) != success)
        {
            return failure;
        }
        break;
        default:
        error_message();
    }
    return success;
}

status update(char *frame, struct file_info file, char *data)
{
    rewind(file.fptr);
    /* create sample file open in wb+ mode */
    FILE *temp = fopen("sample_file.mp3", "wb+");
    //reading 10 bytes(ID3 version and tag_size) to local buffer 
    char buffer[FRAME_DATA];
    fread(buffer, FRAME_DATA, sizeof(char), file.fptr);
    //write to sample file
    fwrite(buffer, FRAME_DATA, sizeof(char), temp);
    char frame_details[FRAME_DATA_WITH_ENCODING_BYTE], frame_found = 0;
    unsigned int size = 0, prev_size = 0;
    /* run loop upto frame is found */
    while(frame_found != 1)
    {
        //read 11 bytes of frame and get size from it
        fread(frame_details,sizeof(char), FRAME_DATA_WITH_ENCODING_BYTE, file.fptr);        
        prev_size = string_to_size(frame_details + FRAME); 
        //check if frame is found or not
        if(strncmp(frame_details, frame, FRAME) == 0)
        {
            //get the newdata length + 1(endoding byte)
            size = strlen(data) + 1;
            //if frame is comm            
            if(strcmp(frame, "COMM") == 0)
            {
                //for language 4 bytes present update frame size and frame data with new data content
                char language_buffer[4];            
                file.tag_size = file.tag_size - prev_size + size+4;
                size_to_string(frame_details+4, size+4);
                fwrite(frame_details, FRAME_DATA_WITH_ENCODING_BYTE, sizeof(char), temp);
                fread(language_buffer, sizeof(char), 4, file.fptr);
                fwrite(language_buffer, sizeof(char), 4, temp);
                fwrite(data, strlen(data), 1, temp);
                fseek(file.fptr, prev_size-5, SEEK_CUR);                
            }
            else
            {
                // update frame size and frame data with new data content
                file.tag_size = file.tag_size - prev_size + size;
                size_to_string(frame_details+4, size);
                fwrite(frame_details, FRAME_DATA_WITH_ENCODING_BYTE, sizeof(char), temp);
                fwrite(data, strlen(data), sizeof(char), temp);
                fseek(file.fptr, prev_size-1, SEEK_CUR);
            }
            //set the frame found
            frame_found = 1;
                        
        }
        //if frame not found
        else
        {
            //copy the frame content to sample file 
            fwrite(frame_details, FRAME_DATA_WITH_ENCODING_BYTE, 1, temp);
            char content[prev_size-1];
            fread(content,sizeof(char), prev_size-1, file.fptr);
            fwrite(content,sizeof(char), prev_size-1, temp);            
        }
    }
    //copying remaining data
    if (copy_file_content(file.fptr,temp) != success)
    {
        return failure;
    }
      
   
     // Update frame size in sync-safe format
    char updated_size[4];   

    for (int i = 0; i < 4; i++) {
        updated_size[i] = (file.tag_size >> (3 - i) * 7) & 0x7F;
    }
    //update tag size with new size
    fseek(temp, 6, SEEK_SET);
    fwrite(updated_size, 4, sizeof(char), temp);
    //copy all sample file content to main file
    rewind(file.fptr);
    rewind(temp);
    if (copy_file_content(temp, file.fptr) != success)
    {
        return failure;
    }
    return success;
}
#endif
