#ifndef MAIN_H
#include "main.h"

int main(int argc, char *argv[])
{
    //validating command line arguments
    if(argc > 1)
    {
        if(strcmp(argv[1], "--h") == 0)
        {
            display_help_menu();
        }
        else if(strcmp(argv[1], "-v") == 0)
        {
            //validating file name
            if(strcmp(strrchr(argv[2], '.'), ".mp3") == 0 && argc < 4)
            {
                struct file_info file1;
                file1.file_name = argv[2];
                
                //file opening
                if(openfile(&file1, "rb") == failure){
                    printf("File not opening\n");
                }                
                             
                if(validate_tag_version_read_tag_size(&file1)==success)
                {
                    if(view_data(file1, file1.fptr) == success)
                    {
                        printf("SUCCESS : Tag viewing\n");
                        
                    }
                    else
                    {
                        printf("Failure : Tag viewing\n");
                    }  
                    fclose(file1.fptr);                      

                }
                else
                {
                    printf("%s is not ID3v2.3 file\nPlease pass valid file\n", file1.file_name);
                }
            }
            else
            {
                error_message();
            }
        }   

        else if(strcmp(argv[1], "-e") == 0)
        {
            //edit_data();
            
            if(argc == 5 && strcmp(strrchr(argv[4], '.'), ".mp3") == 0 )
            {
                struct file_info file1;
                file1.file_name = argv[4];
               

                //file opening
                if(openfile(&file1, "rb+") == failure){
                    printf("File not opening\n");
                }
                if(validate_tag_version_read_tag_size(&file1)==success)
                {
                    if(write_data(argv[2][1], argv[3], file1) == success)
                    {
                        printf("updation done\n");
                        rewind(file1.fptr);
                        view_data(file1, file1.fptr);

                    }
                    else
                    {
                        printf("updation failed\n");
                        return -1;
                    }
                    fclose(file1.fptr);
                }
                else
                {
                    error_message();
                }
              
            }
            else
            {
                error_message();
            }
                
        }           
        
        else
        {
            error_message();
        }
    }
    else
    {
        error_message();
    }
    return 0;
}

void error_message()
{
    printf("ERROR : ./a.out : INVALID ARGUMENTS\n"
        "USAGE :\n"
        "\tTo view please pass like : ./a.out -v <mp3filename>\n"
        "\tTo edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text <mp3filename>\n"
        "\tTo get help please pass : ./a.out --h\n");
}
#endif