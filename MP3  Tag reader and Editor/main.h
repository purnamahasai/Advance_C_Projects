#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define FRAME_DATA 10
#define FRAME 4
#define FRAME_DATA_WITH_ENCODING_BYTE 11
struct file_info
{

    char *file_name;
    FILE *fptr;
    int tag_size;
    char *title;
    char *artist_name;
    char *album;
    char *year;
    char *content_type;
    char *comment;
};
typedef enum
{
    success,
    failure
} status;

void display_help_menu(void);

void display_data(struct file_info file);

void error_message(void);

status view_data(struct file_info file1, FILE *fptr);

status read_data(struct file_info file, char *frame, char **dest);

status write_data(char operation, char *data, struct file_info file);

status update(char *frame, struct file_info file1, char *data);

//sub_functions

status validate_tag_version_read_tag_size(struct file_info *file);

status openfile(struct file_info *file, char *mode);

int string_to_size(char *frame_details);

void size_to_string(char *updated_size, int size);

status copy_file_content(FILE *src, FILE *dest);