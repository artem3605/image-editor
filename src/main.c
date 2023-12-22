#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include "bmp.h"

typedef struct IMG{
    HEADER hdr;
    INFO info;
    PIXEL *pixels;
} image;

image read_image(const char* filename){
    FILE* file = fopen(filename, "rb");
    if (file == NULL){
        printf("Error opening file to read image\n");
        exit(EXIT_FAILURE);
    }
    image answer;

    fread(&answer.hdr, sizeof(HEADER), 1, file);

    fread(&answer.info, sizeof(INFO), 1, file); 

    answer.pixels = calloc(answer.info.height * answer.info.width, sizeof(PIXEL));
    fread(answer.pixels, sizeof(PIXEL), answer.info.height * answer.info.width, file);

    fclose(file);
    return answer;
}       

void write_image(image* img, const char* filename){
    FILE *file = fopen(filename, "wb");

    if (file == NULL){
        printf("Error opening file to write image\n");
        exit(EXIT_FAILURE);
    }

    fwrite(&img->hdr, sizeof(HEADER), 1, file);
    
    fwrite(&img->info, sizeof(INFO), 1, file);
    
    fwrite(img->pixels, sizeof(PIXEL), img->info.width * img->info.height, file);
    
    fclose(file); 
}

void change_pixel(image *img, int x, int y, int red, int green, int blue){
    img->pixels[y * img->info.width + x].red = red;
    img->pixels[y * img->info.width + x].green = green;
    img->pixels[y * img->info.width + x].blue = blue;
}

void printHelp() {
    printf("Usage: ./image_editor [options] [input_file.bmp] [output_file.bmp]\n");
    printf("\nOptions:\n");
    printf("  -i, --invert      Invert color in a specified area\n");
    printf("  -c, --convert     Convert to black and white in a specified area\n");
    printf("  -r, --resize      Resize the image\n");
    printf("  -d, --draw        Draw a line segment on the image\n");
    printf("  -I, --info        Print detailed information about a BMP file\n");
    printf("  -h, --help        Display this help information\n");
    printf("  -S, --start       Specify the starting coordinates\n");
    printf("  -E, --end         Specify the ending coordinates\n");
    printf("  -C, --color       Specify the color (red, green, blue, black, white, yellow, cyan, magenta)\n");
    printf("  -T, --thickness   Specify the thickness\n");
    printf("  -A, --algorithm   Specify the image transformation algorithm (standart)\n");
    printf("  -a, --action      Specify the action (enlarge, reduce)\n");
    printf("\nExamples:\n");
    printf("  ./image_editor --invert --start 0 0 --end 100 100 --color red input.bmp output.bmp\n");
    printf("  ./image_editor --convert --start 0 0 --end 100 100 --algorithm custom input.bmp output.bmp\n");
    printf("  ./image_editor --resize --start x1 y1 --end x2 y2 --action enlarge --color white --point center input.bmp output.bmp\n");
    printf("  ./image_editor --draw --start 0 0 --end 100 100 --color blue --thickness 2 input.bmp output.bmp\n");
    printf("  ./image_editor --info image.bmp\n");
}

image draw_line(image* img, int x1, int y1, int x2, int y2, char* color, int thickness){
    image answer = *img;
    int red, green, blue;
    if (strcmp(color, "red") == 0){
        red = 255;
        green = 0;
        blue = 0;
    }
    else if (strcmp(color, "green") == 0){
        red = 0;
        green = 255;
        blue = 0;
    }
    else if (strcmp(color, "blue") == 0){
        red = 0;
        green = 0;
        blue = 255;
    }
    else if (strcmp(color, "black") == 0){
        red = 0;
        green = 0;
        blue = 0;
    }
    else if (strcmp(color, "white") == 0){
        red = 255;
        green = 255;
        blue = 255;
    }
    else if (strcmp(color, "yellow") == 0){
        red = 255;
        green = 255;
        blue = 0;
    }
    else if (strcmp(color, "cyan") == 0){
        red = 0;
        green = 255;
        blue = 255;
    }
    else if (strcmp(color, "magenta") == 0){
        red = 255;
        green = 0;
        blue = 255;
    }
    else{
        printf("Invalid color\n");
        printHelp();
        exit(EXIT_FAILURE);
    }
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps, k;
    float xIncrement, yIncrement, x = x1, y = y1;

    if (abs(dx) > abs(dy)) {
        steps = abs(dx);
    } else {
        steps = abs(dy);
    }

    xIncrement = (float)dx / (float)steps;
    yIncrement = (float)dy / (float)steps;

    for (k = 0; k < steps; k++) {
        x += xIncrement;
        y += yIncrement;

        for (int i = -thickness / 2; i <= thickness / 2; i++) {
            for (int j = -thickness / 2; j <= thickness / 2; j++) {
                if (round(x) + i >= 0 && round(x) + i < img->info.width && round(y) + j >= 0 && round(y) + j < img->info.height){
                    change_pixel(&answer, round(x) + i, round(y) + j, red, green, blue);
                }
            }
        }
    }
    return answer;
}

image invert(image* img, int x1, int y1, int x2, int y2){
    image answer = *img;
    for (int i = y1; i <= y2; i++){
        for (int j = x1; j <= x2; j++){
            change_pixel(&answer, j, i, 255 - img->pixels[i * img->info.width + j].red, 255 - img->pixels[i * img->info.width + j].green, 255 - img->pixels[i * img->info.width + j].blue);
        }
    }
    return answer;
}

image convert(image* img, int x1, int y1, int x2, int y2){
    image answer = *img;
    for (int i = y1; i <= y2; i++){
        for (int j = x1; j <= x2; j++){
            int average = (img->pixels[i * img->info.width + j].red + img->pixels[i * img->info.width + j].green + img->pixels[i * img->info.width + j].blue) / 3;
            change_pixel(&answer, j, i, average, average, average);
        }
    }
    return answer;
}


int main(int argc, char *argv[]) {
    struct option long_options[] = {
        {"invert", no_argument, NULL, 'i'},
        {"convert", no_argument, NULL, 'c'},
        {"resize", no_argument, NULL, 'r'},
        {"draw", no_argument, NULL, 'd'},
        {"info", no_argument, NULL, 'I'},
        {"help", no_argument, NULL, 'h'},
        {"start", required_argument, NULL, 'S'},
        {"end", required_argument, NULL, 'E'},
        {"color", required_argument, NULL, 'C'},
        {"thickness", required_argument, NULL, 'T'},
        {"algorithm", required_argument, NULL, 'A'},
        {"action", required_argument, NULL, 'a'},
        {0, 0, 0, 0}
    };

    int opt;
    char* outputFileName = "output.bmp";
    char* input_file = "input.bmp";
    int x1 = 0, y1 = 0, x2 = -1, y2 = -1;
    char* color = "black";
    int thickness = 1;
    int type = -1;
    char* action = NULL;
    char* algorithm = "standart";


    while ((opt = getopt_long(argc, argv, "icrdIhS:E:C:T:A:a:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'i':
                if (type != -1){
                    printf("Several functions are selected. Choose one\n");
                    exit(EXIT_FAILURE);
                }
                type = 2;
                break;

            case 'c':
                if (type != -1){
                    printf("Several functions are selected. Choose one\n");
                    exit(EXIT_FAILURE);
                }
                type = 3;
                break;

            case 'r':
                if (type != -1){
                    printf("Several functions are selected. Choose one\n");
                    exit(EXIT_FAILURE);
                }
                type = 4;
                break;

            case 'd':
                if (type != -1){
                    printf("Several functions are selected. Choose one\n");
                    exit(EXIT_FAILURE);
                }
                type = 5;
                break;

            case 'I':
                if (type != -1){
                    printf("Several functions are selected. Choose one\n");
                    exit(EXIT_FAILURE);
                }
                type = 1;
                break;

            case 'S':
                sscanf(argv[optind - 1], "%d", &x1);
                sscanf(argv[optind], "%d", &y1);
                optind++;
                break;

            case 'E':
                sscanf(argv[optind - 1], "%d", &x2);
                sscanf(argv[optind], "%d", &y2);
                optind++;
                break;

            case 'C':
                color = optarg;
                break;

            case 'T':
                sscanf(optarg, "%d", &thickness);
                break;

            case 'A':
                algorithm = optarg;
                break;

            case 'a':
                action = optarg;
                break;

            case 'h':
                if (type != -1){
                    printf("Several functions are selected. Choose one\n");
                    exit(EXIT_FAILURE);
                }
                type = 0;
                break;

            case '?':              
                printHelp();
                exit(EXIT_FAILURE);
        }
    }
    if (optind < argc) {
        input_file = argv[optind];
        optind++;
    }
    if (optind < argc) {
        outputFileName = argv[optind];
        optind++;
    }
    image img, img_res;
    if (type == -1){
        type = 0;
    }
    switch (type) {
    case 0:
        printHelp();
        exit(EXIT_SUCCESS);
        break;
    case 1:
        image img_info = read_image(input_file);
        printf("File size: %d\n", img_info.hdr.size);
        printf("Width: %d\n", img_info.info.width);
        printf("Height: %d\n", img_info.info.height);
        printf("Bits per pixel: %d\n", img_info.info.bitsPerPixel);
        printf("Compression: %d\n", img_info.info.compression);
        printf("Image size: %d\n", img_info.info.imageSize);
        printf("X resolution: %d\n", img_info.info.xResolution);
        printf("Y resolution: %d\n", img_info.info.yResolution);
        printf("Number of colors: %d\n", img_info.info.nColors);
        printf("Important colors: %d\n", img_info.info.importantColors);
        exit(EXIT_SUCCESS);
        break;
    case 2:
        img = read_image(input_file);
        if (x2 == -1){
            x2 = img.info.width - 1;
            y2 = img.info.height - 1;
        }
        if (x1 < 0 || x1 >= img.info.width || x2 < 0 || x2 >= img.info.width || y1 < 0 || y1 >= img.info.height || y2 < 0 || y2 >= img.info.height){
            printf("Invalid coordinates\n");
            printHelp();
            exit(EXIT_FAILURE);
        }
        img_res = invert(&img, x1, y1, x2, y2);
        write_image(&img_res, outputFileName);
        exit(EXIT_SUCCESS);
        break;
    case 3:
        img = read_image(input_file);
        if (x2 == -1){
            x2 = img.info.width - 1;
            y2 = img.info.height - 1;
        }
        if (x1 < 0 || x1 >= img.info.width || x2 < 0 || x2 >= img.info.width || y1 < 0 || y1 >= img.info.height || y2 < 0 || y2 >= img.info.height){
            printf("Invalid coordinates\n");
            printHelp();
            exit(EXIT_FAILURE);
        }
        img_res = convert(&img, x1, y1, x2, y2);
        write_image(&img_res, outputFileName);
        exit(EXIT_SUCCESS);

    case 5:
        img = read_image(input_file);
        if (x2 == -1){
            x2 = img.info.width - 1;
            y2 = img.info.height - 1;
        }
        if (x1 < 0 || x1 >= img.info.width || x2 < 0 || x2 >= img.info.width || y1 < 0 || y1 >= img.info.height || y2 < 0 || y2 >= img.info.height){
            printf("Invalid coordinates\n");
            printHelp();
            exit(EXIT_FAILURE);
        }
        img_res = draw_line(&img, x1, y1, x2, y2, color, thickness);
        write_image(&img, outputFileName);
        exit(EXIT_SUCCESS);
    
    }


    return 0;
}
