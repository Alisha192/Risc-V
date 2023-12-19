#include "stdio.h"
#include "stdlib.h"
#include "bmp_reader.h"
#include <argp.h>
#include <math.h>
void fill_row_with_zeroes(FILE* file, int num_zeroes) {
    for (int i = 0; i < num_zeroes; i++) {
        fputc(0, file);
    }
}



void get_rectangles(const char* filename, PIXEL color, PIXEL linecolor, int linewidth) {
FILE* fp = fopen(filename, "rb");
BMPHEADER header;
// Считываем заголовок файла
fread(&header, sizeof(BMPHEADER), 1, fp);

// Вычисляем размер массива пикселей
int padding = (4 - (header.width * sizeof(PIXEL)) % 4) % 4; // количество "мусорных" байт в конце каждой строки
int pixel_array_size = (header.width * sizeof(PIXEL) + padding) * header.height;

// Выделяем память под массив пикселей
PIXEL* pixels = (PIXEL*) malloc(pixel_array_size);
if (pixels == NULL) {
printf("Error: unable to allocate memory\n");
return;
}

// Считываем массив пикселей
fseek(fp, header.offset, SEEK_SET);
fread(pixels, sizeof(PIXEL), header.width * header.height, fp);

// Поиск прямоугольников
int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
int is_rect = 0;
for (int y = 0; y < header.height; y++) {
for (int x = 0; x < header.width; x++) {
if (pixels[y * header.width + x].r == color.r &&
pixels[y * header.width + x].g == color.g &&
pixels[y * header.width + x].b == color.b) {
if (!is_rect) {
x1 = x;
y1 = y;
x2 = x;
y2 = y;
}
else {
if (x > x2) x2 = x;
if (y > y2) y2 = y;
}
is_rect = 1;
}
else {
if (is_rect) {
// Рисуем прямоугольник
for (int i = 0; i < linewidth; i++){
pixels[y1 * header.width + x1 + i] = linecolor;
pixels[y2 * header.width + x1 + i] = linecolor;
pixels[y1 * header.width + x2 + i] = linecolor;
pixels[y2 * header.width + x2 + i] = linecolor;
}
is_rect = 0;
}
}
}
if (is_rect) {
// Рисуем прямоугольник
for (int i = 0; i < linewidth; i++){
pixels[y1 * header.width + x1 + i] = linecolor;
pixels[y2 * header.width + x1 + i] = linecolor;
pixels[y1 * header.width + x2 + i] = linecolor;
pixels[y2 * header.width + x2 + i] = linecolor;
}
is_rect = 0;
}
}

// Сохраняем модифицированное изображение
const char* new_filename = "rectangles.bmp";
FILE* new_fp = fopen(new_filename, "wb");
fwrite(&header, sizeof(BMPHEADER), 1, new_fp);
fseek(new_fp, header.offset, SEEK_SET);
fwrite(pixels, sizeof(PIXEL), header.width * header.height, new_fp);

// Освобождаем память и закрываем файлы
fclose(fp);
fclose(new_fp);
free(pixels);
}
void readImage(const char* path, BMP* bitmap){
    FILE *f = fopen(path, "rb");
	if(!f){
		printf("Не удалось открыть файл\n");
		return;
	}
	fread(&(bitmap->bm),1,sizeof(BMPHEADER),f);


	bitmap->arr = malloc(bitmap->bm.height*sizeof(PIXEL*));
	for(int i=0; i<bitmap->bm.height; i++){
		bitmap->arr[i] = malloc(bitmap->bm.width * sizeof(PIXEL) + (bitmap->bm.width*3)%4);
		fread(bitmap->arr[i],1,bitmap->bm.width * sizeof(PIXEL) + (bitmap->bm.width*3)%4,f);
	}
	fclose(f);
}
void writeImage(const char* path, BMP* bitmap){
	int H = bitmap->bm.height;
	int W = bitmap->bm.width;
	FILE *ff = fopen(path, "wb");
	fwrite(&(bitmap->bm), 1, sizeof(BMPHEADER),ff);
	for(int i=0; i<H; i++){
		fwrite(bitmap->arr[i],1,W * sizeof(PIXEL) + (W*3)%4,ff);
	}
	fclose(ff);
}

void draw_circle(const char* filename, int x1, int y1, int x2, int y2, int linewidth, PIXEL linecolor, struct arguments args){
 int x = (x1+x2)/2;
 int y = (y1+y2)/2;
 int r = (y2-y1)/2;
 draw_circle_r(filename, x, y, r, linewidth, linecolor, args );
}

void draw_circle_r(const char* filename, int x, int y, int radius, int thickness, PIXEL linecolor, struct arguments args) {
    FILE* in_file = fopen(filename, "rb");
    BMPHEADER header;
    PIXEL fillcolor = {args.fill_circle_color_b, args.fill_circle_color_g, args.fill_circle_color_r};
    // Считываем заголовок файла
    fread(&header, sizeof(BMPHEADER), 1, in_file);

    int fill = args.fill_circle;
    int width = header.width;
    int height = header.height;
    int row_size = width * 3;
    if ((row_size % 4) != 0) {
        row_size += (4 - (row_size % 4));
    }
    int row_padding = row_size - (width * 3);

    PIXEL* pixels = malloc(width * height * sizeof(PIXEL));
    fseek(in_file, header.offset, SEEK_SET);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            PIXEL pixel;
            fread(&pixel, sizeof(PIXEL), 1, in_file);
            pixels[((height - i - 1) * width) + j] = pixel;
        }
        fill_row_with_zeroes(in_file, row_padding);
    }
    fclose(in_file);

    int cx = x;
    int cy = height - y;

    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            int dx = j - cx;
            int dy = i - cy;
            int distance = (int)round(sqrt((dx*dx) + (dy*dy)));
            if (fill && distance <= radius) {
                pixels[(i * width) + j] = fillcolor;
            } else if (distance >= radius - thickness && distance <= radius + thickness) {
                pixels[(i * width) + j] = linecolor;
            }
        }
    }
    
    const char* out_filename = "circle.bmp";
    FILE* out_file = fopen(out_filename, "wb");
    if (out_file == NULL) {
        printf("Could not open output file: %s", out_filename);
        free(pixels);
        return;
    }
    fwrite(&header, sizeof(BMPHEADER), 1, out_file);
    fseek(out_file, header.offset, SEEK_SET);
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            PIXEL pixel = pixels[(i * width) + j];
            fwrite(&pixel, sizeof(PIXEL), 1, out_file);
        }
        fill_row_with_zeroes(out_file, row_padding);
    }
    fclose(out_file);
    free(pixels);
}
void make_ramke(BMP* bitmap, int new_h, int new_w, PIXEL color) {
int h = bitmap->bm.height;
int w = bitmap->bm.width;
int size = bitmap->bm.bitmapsize;
int new_size = new_h * new_w * 3;
PIXEL **old_arr = bitmap->arr;
bitmap->bm.width = new_w;
bitmap->bm.height = new_h;
bitmap->bm.bitmapsize = new_size + bitmap->bm.offset;
bitmap->arr = (PIXEL **) malloc(sizeof(PIXEL *) * new_h);
for (int i = 0; i < new_h; i++) {
bitmap->arr[i] = (PIXEL *) malloc(sizeof(PIXEL) * new_w);
//memset(bitmap->arr[i], color, sizeof(PIXEL)*new_w);
}
for (int i = 0; i < new_h; i++) {
for (int j = 0; j < new_w; j++) {
bitmap->arr[i][j] = color;
}
}
for (int y = 0; y < h; y++) {
for (int x = 0; x < w; x++) {
int new_x = x + (new_w - w) / 2;
int new_y = y + (new_h - h) / 2;
bitmap->arr[new_y][new_x] = old_arr[y][x];
}
}
for (int i = 0; i < h; i++) {
free(old_arr[i]);
}
free(old_arr);
}


void filter_rgb(const char* filename, char component, char value){
    FILE* fp = fopen(filename, "rb");
    BMPHEADER header;

    // Считываем заголовок файла
    fread(&header, sizeof(BMPHEADER), 1, fp);

    // Вычисляем размер массива пикселей
    int padding = (4 - (header.width * sizeof(PIXEL)) % 4) % 4; // количество "мусорных" байт в конце каждой строки
    int pixel_array_size = (header.width * sizeof(PIXEL) + padding) * header.height;

    // Выделяем память под массив пикселей
    PIXEL* pixels = (PIXEL*) malloc(pixel_array_size);
    if (pixels == NULL) {
        printf("Ошибка выделения памяти");
        exit(0);
    }
    fseek(fp, header.offset, SEEK_SET);
    fread(pixels, sizeof(PIXEL), header.width * header.height, fp);
    switch (component) {
        case 'r':
            for (int i = 0; i < header.width * header.height; i++) {
                pixels[i].r = value;
            }
            break;
        case 'g':
            for (int i = 0; i < header.width * header.height; i++) {
                pixels[i].g = value;
            }
            break;
        case 'b':
            for (int i = 0; i < header.width * header.height; i++) {
                pixels[i].b = value;
            }
            break;
        default:
            exit(0);
    }
    const char* new_filename = "filter.bmp";
    FILE* new_fp = fopen(new_filename, "wb");
    fwrite(&header, sizeof(BMPHEADER), 1, new_fp);
    fseek(new_fp, header.offset, SEEK_SET);
    fwrite(pixels, sizeof(PIXEL), header.width * header.height, new_fp);

    fclose(fp);
    fclose(new_fp);
    free(pixels);
}
void split_image(const char* filename, int num_rows, int num_cols, PIXEL linecolor, int linewidth, const char* path){
    FILE* fp = fopen(filename, "rb");
    BMPHEADER header;

    // Считываем заголовок файла
    fread(&header, sizeof(BMPHEADER), 1, fp);

    // Вычисляем размер массива пикселей
    int padding = (4 - (header.width * sizeof(PIXEL)) % 4) % 4; // количество "мусорных" байт в конце каждой строки
    int pixel_array_size = (header.width * sizeof(PIXEL) + padding) * header.height;

    // Выделяем память под массив пикселей
    PIXEL* pixels = (PIXEL*) malloc(pixel_array_size);
    if (pixels == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(0);
    }

    // Считываем массив пикселей
    fseek(fp, header.offset, SEEK_SET);
    fread(pixels, sizeof(PIXEL), header.width * header.height, fp);

    int part_width = header.width / num_cols;
    int part_height = header.height / num_rows;

    for(int y = part_height; y < header.height; y += part_height){
        if(linewidth > 1){
            for( int i = y - linewidth / 2 ; i <= y + linewidth / 2; i++){
                for(int x = 0; x < header.width; x++){
                    pixels[y * header.width + x] = linecolor;
                }
            }
        }
        else{
            for (int x = 0; x < header.width; x++) {
                pixels[y * header.width + x] = linecolor;
            }
        }
    }
    for(int x = part_width; x < header.width; x += part_width) {
        if (linewidth > 1) {
            for (int i = 0; i < header.height; i++) {
                for (int j = x - linewidth / 2; j <= x + linewidth / 2; j++) {
                    if (j >= 0 && j < header.width) {
                        pixels[i * header.width + j] = linecolor;
                    }
                }
            }
        }
        else {
            for (int i = 0; i < header.height; i++) {
                pixels[i * header.width + x] = linecolor;
            }
        }
    }
    const char* new_filename = "split.bmp";
    FILE* new_fp = fopen(new_filename, "wb");
    fwrite(&header, sizeof(BMPHEADER), 1, new_fp);
    fseek(new_fp, header.offset, SEEK_SET);
    fwrite(pixels, sizeof(PIXEL), header.width * header.height, new_fp);

    // Освобождаем память и закрываем файлы
    fclose(fp);
    fclose(new_fp);
    free(pixels);
}
