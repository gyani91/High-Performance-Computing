#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "pngwriter.h"
#include "consts.h"
#include <omp.h>

int main (int argc, char** argv)
{
    png_data* pPng = png_create (IMAGE_WIDTH, IMAGE_HEIGHT);
    double x, y, x2, y2, cx, cy;
    cy = MIN_Y;
    double fDeltaX = (MAX_X - MIN_X) / (double) IMAGE_WIDTH;
    double fDeltaY = (MAX_Y - MIN_Y) / (double) IMAGE_HEIGHT;
    long nTotalIterationsCount = 0;
    unsigned long nTimeStart = get_time ();
    long i, j, n;
    n = 0;

    #pragma omp parallel for private(cx,cy,x,y,x2,y2,n,i,j)
    for (j = 0; j < IMAGE_HEIGHT; j++) {
    	cy = MIN_Y + fDeltaY * j;
        cx = MIN_X;
        for (i = 0; i < IMAGE_WIDTH; i++){
            x = cx;
            y = cy;
            x2 = x * x;
            y2 = y * y;
            // compute the orbit z, f(z), f²(z), f³(z), ...
            // count the iterations until the orbit leaves the circle |z|=2.
            // stop if the number of iterations exceeds the bound MAX_ITERS.
            for (n = 0; (n < MAX_ITERS) && (x2 + y2 < 4); n++) {
                y = 2 * x * y + cy;
                x = x2 - y2 + cx;
                x2 = x * x;
                y2 = y * y;
            }
            // n indicates if the point belongs to the mandelbrot set
            // plot the number of iterations at point (i, j)
            int c = ((long) n * 255) / MAX_ITERS;
            png_plot (pPng, i, j, c, c, c);
            nTotalIterationsCount++;
            cx += fDeltaX;
        }
    }
    unsigned long nTimeEnd = get_time ();
    // print benchmark data
    printf ("Total time:                 %g ms\n", (nTimeEnd - nTimeStart) / 1000.0);
    printf ("Image size:                 %ld x %ld = %ld Pixels\n",
    (long) IMAGE_WIDTH, (long) IMAGE_HEIGHT, (long) (IMAGE_WIDTH * IMAGE_HEIGHT));
    printf ("Total number of iterations: %ld\n", nTotalIterationsCount);
    printf ("Avg. time per pixel:        %g µs\n", (nTimeEnd - nTimeStart) / (double) (IMAGE_WIDTH * IMAGE_HEIGHT));
    printf ("Avg. time per iteration:    %g µs\n", (nTimeEnd - nTimeStart) / (double) nTotalIterationsCount);
    printf ("Iterations/second:          %g\n", nTotalIterationsCount / (double) (nTimeEnd - nTimeStart) * 1e6);
    // assume there are 8 floating point operations per iteration
    printf ("MFlop/s:                    %g\n", nTotalIterationsCount * 8.0 / (double) (nTimeEnd - nTimeStart));
    png_write (pPng, "mandel.png");
    return 0;
}