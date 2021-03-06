#include "file_reader.h"
#include "function_shortcuts.h"
#include <stdio.h>
#include <string.h>

/* THIS FILE WILL SUPPORT ALL FILE HANDLING WITHIN THE Jang PROGRAMMING LANGUAGE */

static file_buffer* buffer_file_size(size_t size) {
    file_buffer* f_b = calloc(1,sizeof(*f_b));

    f_b->buffer_size = size*8;
    f_b->ammount_of_character = size;

    return f_b;
}

void* gather_main_jang_file() {
    system("python3 src/gather_main.py");

    FILE* find_main = fopen("main.txt","rb");
    char* a = calloc(500,sizeof(char));

    if(!find_main) RAISE_ERROR("\nNo main file found to execute\n\n",-1);

    fgets(a, 500, find_main);

    if(strcmp(a,"err")==0) RAISE_ERROR("\nNo main.cua file found to run your CUA application\nPlease create a main.cua file to run your project\n\n",-1);
    fclose(find_main);
    
    return a;
}

char* read_file(const char* file_to_read) {
    char* buffer = 0;
    long length;

    int index = 0, length_ = 0;
    char* extension = calloc(1,sizeof(char));
    gather_main_jang_file();

    for(;index < strlen(file_to_read); index++) {
        if(file_to_read[index] == '.') {
            while(file_to_read[index] != '\0') {
                char* value = calloc(1,sizeof(char*));

                value[0] = file_to_read[index];
                value[1] = '\0';

                length_++;
                extension = realloc(extension,length_*sizeof(char*));
                strcat(extension,value);
                index++;
            }
        }
    }
    
    if(strcmp(extension,".cua")==0) {
        FILE* file = fopen(file_to_read,"rb");

        if(!(file)) {
            printf("\n\nErr reading file: %s\n\n",file_to_read);
            exit(1);
        }

        fseek(file, 0, SEEK_END);
        length = ftell(file);
        file_buffer* f_b = buffer_file_size(length);
        fseek(file, 0, SEEK_SET);

        if(ASSERT_FILE_BUFFER_SIZE(f_b->ammount_of_character, f_b->buffer_size)==0) {
            buffer = calloc(f_b->ammount_of_character, f_b->buffer_size);
            if(buffer) {
                fread(buffer,1,length,file);
            }
        } else {
            fprintf(stderr,"\nError matching memory size of file.\nTry again.\n\n");
            fflush(stderr);
            exit(EXIT_FAILURE);
        }

        fclose(file);
        free(f_b);
    } else {
        fprintf(stderr, "\nUnknown file type: %s\n\n",extension);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    return buffer;
}