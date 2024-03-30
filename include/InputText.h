#ifndef INPUT_TEXT_H
#define INPUT_TEXT_H

struct TextInfo {
                    void* buffer;
                    size_t elemcount;
                };

size_t fsize(FILE* file);

struct TextInfo* InputText(TextInfo*, FILE* file);

void charprint(struct TextInfo text);

void TextInfoDtor(TextInfo*);

#endif //INPUT_TEXT_H
