#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "imagelib.h"
#define PER_LINE 16

/*-------------------------------------------------------------------------
 * Image allocation and free routines
 *   nr = number of rows
 *   nc = number of columns
 *   ml = max gray level
 *   tp = type of image
 *-------------------------------------------------------------------------*/
image img_create(int nr, int nc, int ml, int tp)
{
    image img = malloc(sizeof(image *));
    img->px = malloc(nr * nc * sizeof(int));
    img->nr = nr;
    img->nc = nc;
    img->ml = ml;
    img->tp = tp;
    return img;
}

image img_clone(image In)
{
    return img_create(In->nr, In->nc, In->ml, In->tp);
}

int img_free(image Im)
{
    free(Im->px);
    free(Im);
}

/*-------------------------------------------------------------------------
 * Define input and output image name
 * Params:
 *   name = image name file
 *   in = input image name
 *   out = output image name
 *   tpIn, tpOut = image type (BW, GRAY, COLOR)
 *-------------------------------------------------------------------------*/
void img_name(char *name, char *in, char *out, int tpIn, int tpOut)
{
    char *ext[3] = {".pbm", ".pgm", ".ppm"};
    char *p = strstr(name, ext[tpIn - 1]);
    if (p)
        *p = 0;
    sprintf(in, "%s%s", name, ext[tpIn - 1]);
    sprintf(out, "%s-result%s", name, ext[tpOut - 1]);
}

/*-------------------------------------------------------------------------
 * Display image information
 *   name = file name image
 *   nr = number of rows
 *   nc = number os columns
 *   ml = max grayscale level
 *   tp = image type (1, 2 ou 3)
 *-------------------------------------------------------------------------*/
void img_info(char *name, image img)
{
    printf("\nImage Informations:");
    printf("\n--------------------------\n");
    printf("Image file name.............: %s \n", name);
    printf("Image type..................: P%d\n", img->tp);
    printf("Number of rows..............: %d \n", img->nr);
    printf("Number of columns...........: %d \n", img->nc);
    printf("Max intensity level.........: %d \n\n", img->ml);
}

/*-------------------------------------------------------------------------
 * Error message
 *   str - string message control
 *   ... - parameters
 *-------------------------------------------------------------------------*/
void errormsg(char *str, ...)
{
    char format[255];
    va_list arg;
    va_start(arg, str);
    sprintf(format, "ERROR: %s\n\n", str);
    vprintf(format, arg);
    va_end(arg);
    exit(1);
}

/*-------------------------------------------------------------------------
 * Read pnm ascii image
 * Params (in):
 *   name = image file name
 *   tp = image type (BW, GRAY or COLOR)
 * Returns:
 *   image structure
 *-------------------------------------------------------------------------+*/
image img_get(char *name, int tp)
{
    char lines[100];
    int nr, nc, ml;
    image img;
    FILE *fimg;
    ERROR((fimg = fopen(name, "r")) == NULL, errormsg("File open error: <%s>", name));
    /*--- PNM = "P1" or "P2" or "P3" ---*/
    fgets(lines, 80, fimg);
    ERROR((lines[0] != 'P' || (lines[1] != tp + '0')), errormsg("File type image error: <%s>", name));
    /*--- Comment lines ---*/
    fgets(lines, 80, fimg);
    while (strchr(lines, '#'))
        fgets(lines, 80, fimg);
    sscanf(lines, "%d %d", &nc, &nr);
    if (tp != BW)
        fscanf(fimg, "%d", &ml);
    else
        ml = 1;
    ERROR(nc == 0 || nr == 0 || ml == 0, errormsg("Image dimensions error: <%s>", name));
    img = img_create(nr, nc, ml, tp);
    ERROR(!img, errormsg("Image allocation error: %s\n\n img_get routine", name));
    for (int i = 0; i < nr * nc; i++)
        if (tp != COLOR)
        {
            int k;
            fscanf(fimg, "%d", &k);
            ERROR(k > ml, errormsg("Max pixel intensity in the image error: <%s>", name));
            img->px[i] = k;
        }
        else
        {
            int r, g, b;
            fscanf(fimg, "%d %d %d", &r, &g, &b);
            ERROR(r > ml || r < 0 || g > ml || g < 0 || b > ml || b < 0,
                  errormsg("Max intensity of color in the image error: <%s>", name));
            img->px[i] = (r << 16) + (g << 8) + b;
        }
    fclose(fimg);
    img_info(name, img);
    return img;
}

/*-------------------------------------------------------------------------
 * Write pnm image
 * Params:
 *   img = image structure
 *   name = image file name
 *   tp = image type (BW, GRAY or COLOR)
 *-------------------------------------------------------------------------*/
void img_put(image img, char *name, int tp)
{
    int count;
    FILE *fimg;
    ERROR((fimg = fopen(name, "wt")) == NULL, errormsg("Image creation error: <%s>", name));
    fprintf(fimg, "P%c\n", tp + '0');
    fputs(CREATOR, fimg);
    fprintf(fimg, "%d  %d\n", img->nc, img->nr);
    if (tp != BW)
        fprintf(fimg, "%d\n", img->ml);
    count = 0;
    for (int i = 0; i < img->nr * img->nc; i++)
    {
        if (tp != COLOR)
        {
            int x = img->px[i];
            fprintf(fimg, "%3d ", x);
        }
        else
        {
            int r = (img->px[i] >> 16) & 0xFF;
            int g = (img->px[i] >> 8) & 0xFF;
            int b = img->px[i] & 0xFF;
            fprintf(fimg, "%3d %3d %3d ", r, g, b);
        }
        count++;
        if (count > PER_LINE)
        {
            fprintf(fimg, "\n");
            count = 0;
        }
    }
    fclose(fimg);
}
