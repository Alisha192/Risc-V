#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <math.h>
#pragma pack(push, 1) // устанавливаем выравнивание в 1 байт

// BMP заголовок файла
typedef struct {
    unsigned short signature; // сигнатура файла
    unsigned int size; // размер файла в битах
    unsigned short reserved1, reserved2; // зарезервированные поля
    unsigned int offset; // смещение пос-ти пикселей от начала файла в битах
    unsigned int headersize; // размер структуры заголовка в байтах
    int width; // ширина изображения в пикселях
    int height; // высота изображения в пикселях
    unsigned short planes; // количество плоскостей изображения
    unsigned short bitsperpixel; // количество бит на пиксель
    unsigned int compression; // метод сжатия
    unsigned int bitmapsize; // размер области изображения в битах
    int horizontalres, verticalres; // горизонтальное и вертикальное разрешение пикселей на метр в битах
    unsigned int numcolors; // количество цветов изображения
    unsigned int importantcolors; // количество значимых цветов
} BMPHEADER;

// структура пикселя
typedef struct {
    unsigned char b, g, r;
} PIXEL;

typedef struct{
  BMPHEADER bm;
  PIXEL** arr;
} BMP;

#pragma pack(pop) // сбрасываем выравнивание в стандартное значение
struct arguments {
    char *input_file;
    char *output_file;
    int dorect;
    int color_r;
    int color_g;
    int color_b;
    int line_color_r;
    int line_color_g;
    int line_color_b;
    int line_thickness;
    int circle;
    int fill_circle;
    int fill_circle_color_r;
    int fill_circle_color_g;
    int fill_circle_color_b;
    int circle_x1;
    int circle_y1;
    int circle_x2;
    int circle_y2;
    int circle_X;
    int circle_Y;
    int circle_radius;
    int filter_rgb;
    char component;
    int value;
    int divide_image;
    int n;
    int m;
};



//struct arguments* parse_opt(int key, char *arg, struct argp_state *state);
void make_ramke(BMP* bitmap, int new_h, int new_w, PIXEL color);
void get_rectangles(const char* filename, PIXEL color, PIXEL linecolor, int linewidth);
void draw_circle_r(const char* filename, int x2, int y2, int r,int linewidth, PIXEL linecolor, struct arguments args);
void draw_circle(const char* filename, int x1, int y1, int x2, int y2, int linewidth, PIXEL linecolor,  struct arguments args);
void filter_rgb(const char* filename, char component, char value);
void split_image(const char* filename, int num_rows, int num_cols, PIXEL linecolor, int linewidth, const char* path);
