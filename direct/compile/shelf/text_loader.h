#ifndef TEXT_LOADER_H
#define TEXT_LOADER_H

typedef struct {
    int width;
    int height;
    int num_chars;
    unsigned char* reserved;

} FontMetaData;

unsigned char* load_font(const char* font_bin, FontMetaData* meta_data);
unsigned char* get_char(unsigned char* font_data, FontMetaData* meta, char ascii);
void print_single(char ascii, int grid_x, int grid_y, unsigned char* font_data, FontMetaData* meta);


#endif
