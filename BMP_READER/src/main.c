#include "stdlib.h"
#include "stdio.h"
#include "bmp_reader.h"
#include "string.h"
#include "stdio.h"
#include <argp.h>
#include <math.h>
#include <getopt.h>
/*int main(int argc, char **argv){
struct arguments args = { .input_file = NULL, .output_file = NULL, .color_r = 0, .color_g = 0, .color_b = 0, .line_color_r = 0, .line_color_g = 0, .line_color_b = 0, .line_thickness = 0, .fill_circle = 0, .fill_circle_color_r = 0, .fill_circle_color_g = 0, .fill_circle_color_b = 0, .circle_x1 = 0, .circle_y1 = 0, .circle_x2 = 0, .circle_X=0, .circle_Y=0, .circle_y2 = 0, .circle_radius = 0, .filter_rgb = -1, .component = '\0', .value = 0, .divide_image = 0, .n = 0, .m = 0 };


int option;
while ((option = getopt(argc, argv, "i:o:c:l:t:f:x:y:z:r:g:b:F:D:X:Y:h")) != -1) {
    switch (option) {
        case 'i':
            args.input_file = optarg;
            break;
        case 'o':
            args.output_file = optarg;
            break;
        case 'c':
       		args.dorect = 1;
            sscanf(optarg, "%d,%d,%d", &args.color_r, &args.color_g, &args.color_b);
            if(args.color_r < 0 || args.color_r > 255 || args.color_b<0|| args.color_b > 255 || args.color_g < 0 || args.color_g > 255){
            	printf("Components RGB must be >0 and <255");
            	exit(0);
            }
            break;
        case 'l':
            sscanf(optarg, "%d,%d,%d", &args.line_color_r, &args.line_color_g, &args.line_color_b);
            if(args.line_color_r < 0 || args.line_color_r > 255 || args.line_color_b<0|| args.line_color_b > 255 || args.line_color_g < 0 || args.line_color_g > 255){
            	printf("Components RGB must be >0 and <255");
            	exit(0);
            }
            break;
        case 't':
            args.line_thickness = atoi(optarg);
            break;
        case 'f':
            args.fill_circle = 1;
            sscanf(optarg, "%d,%d,%d", &args.fill_circle_color_r, &args.fill_circle_color_g, &args.fill_circle_color_b);
            if(args.fill_circle_color_r < 0 || args.fill_circle_color_r > 255 || args.fill_circle_color_b<0|| args.fill_circle_color_b > 255 || args.fill_circle_color_g < 0 || args.fill_circle_color_g > 255){
            	printf("Components RGB must be >0 and <255");
            	exit(0);
            }
            break;
        case 'x':
                    args.circle = 1;
            args.circle_x1 = atoi(optarg);
            break;
        case 'y':
                    args.circle = 1;
            args.circle_y1 = atoi(optarg);
            break;
        case 'z':
                    args.circle = 1;
            args.circle_x2 = atoi(optarg);
            break;
        case 'r':
                    args.circle = 1;
            args.circle_y2 = atoi(optarg);
            break;
        case 'g':
                    args.circle = 1;
            args.circle_radius = atoi(optarg);
            break;
        case 'X':
                    args.circle = 1;
            args.circle_X = atoi(optarg);
            break;
        case 'Y':
                    args.circle = 1;
            args.circle_Y = atoi(optarg);
        case 'F':
            args.filter_rgb = 1;
            sscanf(optarg, "%c:%d", &args.component, &args.value);
            break;
        case 'D':
            args.divide_image = 1;
            sscanf(optarg, "%d,%d", &args.n, &args.m);
            break;
       	case 'h':
       		 printf("Usage: %s -i <input_file> -o <output_file> [-c <color_r>,<color_g>,<color_b>] [-l <line_color_r>,<line_color_g>,<line_color_b>] [-t <line_thickness>] [-f <fill_circle_color_r>,<fill_circle_color_g>,<fill_circle_color_b>] [-x <circle_x1>] [-y <circle_y1>] [-z <circle_x2>] [-r <circle_y2>] [-g <circle_radius>] [-F <component>:<value>] [-D <n>,<m>]\n", argv[0]);
       		return 1;
        default:
            printf("Usage: %s -i <input_file> -o <output_file> [-c <color_r>,<color_g>,<color_b>] [-l <line_color_r>,<line_color_g>,<line_color_b>] [-t <line_thickness>] [-f <fill_circle_color_r>,<fill_circle_color_g>,<fill_circle_color_b>] [-x <circle_x1>] [-y <circle_y1>] [-z <circle_x2>] [-r <circle_y2>] [-g <circle_radius>] [-F <component>:<value>] [-D <n>,<m>]\n", argv[0]);
            return 1;
    }
}	
    int radius = radius - args.line_thickness;
    PIXEL fillcolor = {args.fill_circle_color_b, args.fill_circle_color_g, args.fill_circle_color_r};
    PIXEL color = {args.color_b, args.color_g, args.color_r};
    PIXEL linecolor = {args.line_color_b,args.line_color_g,args.line_color_r};
    int linewidth = args.line_thickness;
    if(args.dorect == 1){
	get_rectangles(args.input_file, color, linecolor, args.line_thickness);
    }
    if(args.circle == 1){
    	draw_circle(args.input_file, args.circle_x1, args.circle_x1, args.circle_x2, args.circle_y2, args.line_thickness, linecolor, args);
    	draw_circle_r(args.input_file, args.circle_X,args.circle_Y,args.circle_radius,args.line_thickness,linecolor, args);
    }
    if(args.filter_rgb == 1){
            	if(args.value<0 || args.value > 255){
            		printf("value of component must be >0 and <255");
            		exit(0);
            	}
    	filter_rgb(args.input_file, args.component, args.value);
    }
    if(args.divide_image == 1){
    	split_image(args.input_file, args.n, args.m, linecolor, args.line_thickness, args.output_file);
    }
    if(args.dorect != 1 && args.circle != 1 && args.filter_rgb != 1 && args.divide_image != 1){
    	printf("Usage: %s -i <input_file> -o <output_file> [-c <color_r>,<color_g>,<color_b>] [-l <line_color_r>,<line_color_g>,<line_color_b>] [-t <line_thickness>] [-f <fill_circle_color_r>,<fill_circle_color_g>,<fill_circle_color_b>] [-x <circle_x1>] [-y <circle_y1>] [-z <circle_x2>] [-r <circle_y2>] [-g <circle_radius>] [-F <component>:<value>] [-D <n>,<m>]\n", argv[0]);
    }
    return 0;
}*/
int main(){
	BMP bitmap;
	readImage("sample.bmp", &bitmap);
	PIXEL color = {255,0,0};
	int width = 200;
	int w = bitmap.bm.width;
	int h = bitmap.bm.height;
	int new_h = h + width;
	int new_w = w+ width;
	void make_ramke(&bitmap, new_h, new_w, color);
}
