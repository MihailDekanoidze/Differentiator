#include <stdio.h>
#include <malloc.h>
#include "../include/InputText.h"


size_t fsize(FILE* file)
{
    size_t begin = 0;
    size_t end = 0;

    begin = (size_t)ftell(file);
    fseek(file, 0, SEEK_END);
    end = (size_t)ftell(file);

    return end - begin;
}


struct TextInfo* InputText(TextInfo* text, FILE* file)
{
    size_t size = 0;

    size = fsize(file);

    rewind(file);

    //printf("size = %d\n", size);

    char* buffer = (char*) calloc(size + 1, sizeof(char));

    size_t elemcount = 0;
    elemcount = fread(buffer, sizeof(char), size, file);

    /*if (elemcount != size)          ///  Спросить
    {
        text.buffer = NULL;
        return text;
    }*/

    buffer[elemcount] = '\0';

    text->buffer = buffer;
    text->elemcount = elemcount;


    //printf("char_read = %d\n", elemcount);
    //charprint(text);

    return text;
}


void charprint(struct TextInfo text)
{
    size_t size = text.elemcount;
    char* ptr = (char*) text.buffer;

    for (size_t i = 0; i < size; i++)
    {
        printf("%c (%d)\n", ptr[i], ptr[i]);
    }
}

void TextInfoDtor(TextInfo* text)
{
    text->elemcount = 0;

    free(text->buffer);
    free(text);
}




