#ifndef TEXT_H
#define TEXT_H

typedef struct {} Text;

Text* construct_text();
void text_remove_unwanted(Text* text);

#endif
