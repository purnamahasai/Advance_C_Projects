#ifndef MAIN_H
#include "main.h"

void display_help_menu(void)
{
    printf("--------------------------------HELP----------------------------------\n\n"
    "1. To view please pass like : ./a.out -v <mp3filename>\n\n"
    "2. To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text <mp3filename>\n\n"
    "\t2.1. -t -> to edit song title\n"
    "\t2.2. -a -> to edit artist name\n"
    "\t2.3. -A -> to edit album name\n"
    "\t2.4. -y -> to edit year\n"
    "\t2.5. -m -> to edit content\n"
    "\t2.6. -c -> to edit comment\n\n");
    printf("----------------------------------------------------------------------\n");
}

void display_data(struct file_info file)
{
    
    printf("-----------------------MP3 TAG READER AND EDITOR FOR ID3V2-----------------------\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("%-20.20s : \t\t%-50.50s\n", "TITLE", file.title);
    printf("%-20.20s : \t\t%-50.50s\n", "ARTIST", file.artist_name);
    printf("%-20.20s : \t\t%-50.50s\n", "ALBUM", file.album);
    printf("%-20.20s : \t\t%-50.50s\n", "YEAR", file.year);
    printf("%-20.20s : \t\t%-50.50s\n", "MUSIC", file.content_type);
    printf("%-20.20s : \t\t%-50.50s\n", "COMMENT", file.comment);
    printf("--------------------------------------------------------------------------------\n\n");
    printf("---------------------------DATA DISPLAYED SUCCESSFULLY--------------------------\n");

}

status read_data(struct file_info file, char *frame, char **dest)
{ 
    
    fseek(file.fptr, FRAME_DATA, SEEK_SET);
    int frame_found = 1, size;
    while(frame_found)
    {
        //take local buffer to read one frame details
        char *frame_details = malloc(FRAME_DATA * sizeof(char));
        fread(frame_details,sizeof(char), FRAME_DATA, file.fptr);
        //read the frame size  
        size = string_to_size(frame_details + FRAME);
        //if frame is found or not 
        if(strncmp(frame_details, frame, FRAME) == 0)
        {   
            fseek(file.fptr, 1, SEEK_CUR); //skip encoding byte
            if(strcmp(frame, "COMM")==0)
            {
                fseek(file.fptr, FRAME, SEEK_CUR); //to skip language
                size = size - FRAME;
            }
            //allocate memory to store frame data
            *dest = malloc(size);
            if(*dest == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                return failure;
            }
            fread(*dest, sizeof(char), size-1, file.fptr);
            (*dest)[size] = '\0';
            frame_found = 0;
            free(frame_details);
            return success;
        }
        //if frame not found skip the size number of bytes
        else
        {
            fseek(file.fptr, size, SEEK_CUR);
        }
    }
    
    return failure;
}
status view_data(struct file_info file1, FILE *fptr)
{
    //reading all frames data and store it in respective structure members
    if(read_data(file1, "TIT2", &file1.title) != success)
        return failure;
    if(read_data(file1, "TPE1", &file1.artist_name) != success)
        return failure;
    if(read_data(file1, "TALB", &file1.album) != success)
        return failure;
    if(read_data(file1, "TYER", &file1.year) != success)
        return failure;
    if(read_data(file1, "TCON", &file1.content_type) != success)
        return failure;
    if(read_data(file1, "COMM", &file1.comment) != success)
        return failure;

    //display the data
    display_data(file1);

    //freeing all structure members that will store frame details
    free(file1.title);
    free(file1.artist_name);
    free(file1.album);
    free(file1.year);
    free(file1.content_type);
    free(file1.comment);
    return success;
}



#endif