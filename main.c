#include <stdlib.h>
#include <stdio.h>
#include "motor/motor.h"
#include "gcode/gcode.h"
#include <string.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <highgui.h>
#include <unistd.h>



int conversao(double distance){
    return 0;    
}

int main(int argv, char *argc[]){
    if(argv < 2){
        perror("One argument is needed\n");
        exit(EXIT_FAILURE);
    }
    printf("Program name %s\n", argc[0]);
    printf("File name %s\n", argc[1]);
    gc_list * gl = create_list();
    if(gl == NULL){
        perror("List cannot be created");
        exit(EXIT_FAILURE);
    }
    read_gcodefile(argc[1], gl);
    for(gc_node *it = gl->head; it != NULL; it = it->next){
//        printf("Line %s", it->elem->line);
        get_gcode(it->elem);
        printf("Gcode %s\n", it->elem->gcode);
        if(strcmp(it->elem->gcode, G01) == 0){
            printf("G01 code\n");
            set_G01_coordenates(it->elem);
            printf("Coord x: %lf y: %lf z: %lf f: %lf\n", it->elem->x, it->elem->y, it->elem->z, it->elem->f);
        }
    }
    
    
    motor *m;
    alloc_motor(&m);
    read_conf(m, X_AXIS);
    if(m == NULL){
        printf("You have to allocated it\n");
    }
    read_conf(m, Y_AXIS);
    printf("Testando\n");
    free(m);
    cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
    cvMoveWindow("mainWin", 100, 100);
    int xini = 120; int yini = 400;
    CvPoint pi = cvPoint(xini, yini);
    IplImage* img=cvCreateImage(cvSize(1240,480),IPL_DEPTH_8U,3); 
    CvScalar color = cvScalar(0,255,0, 0);
    for(gc_node *it = gl->head; it != NULL; it = it->next){
printf("Coor point: %lf %lf\n", it->elem->x, it->elem->y);
        if(strcmp(it->elem->gcode, G01) == 0){
            double deltax = (it->elem->x + xini) - pi.x;
            double deltay = (yini - it->elem->y) - pi.y;
            double deltaerr = abs(deltay/deltax);
            double error = deltaerr - 0.5;
            int y = pi.y; 
            int i = 0;
            for(i = pi.x; i < (it->elem->x + xini); i++){
                CvPoint p1 = cvPoint(i,y);
printf("Coor point: %d %d\n", p1.x, p1.y);
                cvLine(img, pi, p1,color, 2, 8, 0);
                cvShowImage("mainWin", img );
                cvWaitKey(1000);
                error = error + deltaerr;
                if(error >= 0.5){
                     y = y - 1;
                     error = error - 1.0;
                }       
                
            }
            pi = cvPoint( i, y);
            //sleep(4);
        }
    }

    cvReleaseImage(&img);
    return 0;
}
