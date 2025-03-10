#include <stdio.h>
#include <stdlib.h>

#define NUM_CHARS 26
#define FONT_PATH "config/font_generate/font.bin"
typedef struct {
    unsigned char width;
    unsigned char height;
    unsigned char num_chars;
    unsigned char reserved;
} FontMetaData;

unsigned char* load_font(const char* font_bin, FontMetaData* meta) {
    FILE *file = fopen(font_bin, "rb");
    if (!file) {
        perror("file doesnt exist or isnt in the right location (config/font_generate). run the generate file to fix\n");
        exit(1);
    }
    fread(meta, sizeof(FontMetaData), 1, file);
    size_t font_data_size = meta-> num_chars * meta->height * ((meta->width + 7) / 8); // bytes per row in the python file
    unsigned char* font_data = malloc(font_data_size);

    fread(font_data, font_data_size, 1, file);
    fclose(file);
    return font_data;
}

unsigned char* get_char(unsigned char* font_data, FontMetaData* meta, char ascii) { //get these nuts
    size_t char_offset = (ascii) * meta->height * ((meta->width + 7) /8);
    return &font_data[char_offset];
}

void print_single(char ascii, int grid_x, int grid_y, unsigned char* font_data, FontMetaData* meta) {
    unsigned char* bitmap = get_char(font_data, meta, ascii);

    for (int row = 0; row < meta->height; row++) {
        unsigned char byte = bitmap[row];
        for (int col = 0; col < meta->width; col++) {
            if (byte & (1 << (meta->width - 1 - col))) {
                draw_pixel(grid_x + col, grid_y + row);
            }
        }
    }
}

