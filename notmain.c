#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ppm6.h"

/////////////////////////////////////////////////////////////

//TODO: -
/*
    Rainbow - 1 0 0
    Xmas r1 - 2 0 0
    Xmas r2 - 3 0 0
    */

int main(){
    const int stmX=300,stmY=200;
    /*
    struct PPM6* pav = kurkPaveiksleli("pvz4.ppm", stmX, stmY, 255);
    staciakampis(pav, 1, stmX, 1, stmY, 19,24,98);//background

    //int r=0,g=0,b=0;

    //x 0-5; y 0-7; cord*s

    */

    char pavadinimas[20]="";
    int sl=0,slc=2,cycle=1,chr=0,fall=3,st=10;
    for (int i=st;i<=78;i+=cycle){
        sprintf(pavadinimas, "kadras-%d.ppm", i);
        struct PPM6* pav = kurkPaveiksleli(pavadinimas, stmX, stmY, 255);
        staciakampis(pav, 0, stmX, 0, stmY, 19,24,98);//background

        char snow[]="* * * * * * * * * *";
        const int w=200;

        sl=slc*(1-2*(i%(cycle*2)));
        //? ((i%2)+1)*(-1+2*((i+1)%2))
        printf("TEST %d %d",i,sl);
        chr=((i%(cycle*2))==cycle)+2;


        for(int j=0;j<5;j++){
            if(i-9+j*36+14<stmY)
                strtoppm(pav,snow,sl+13,fall*(i-st)-9+j*36,2,stmX,stmY,w,w,w);
            if(i+9+j*36+14<stmY)
                strtoppm(pav,snow,sl+27,fall*(i-st)+9+j*36,2,stmX,stmY,w,w,w);
        }

        char msg[]="LINKSMU NAUJUJU METU!";
        strtoppm(pav,msg,15,15,5,stmX,stmY,chr,0,0);

        char yrs[]="2023";
        strtoppm(pav,yrs,160,148,4,stmX,stmY,chr,0,0);

        char tre[]="^^^^";
        strtoppm(pav,tre,5,165,5,150,stmY,chr,0,0);

        char tr3[]="^^^^^^^";
        strtoppm(pav,tr3,143,179,3,stmX,stmY,chr,0,0);

    rasykPaveiksleli(pav);
    trinkPaveiksleli(pav);
    }

//    system("ffmpeg -framerate 25 -i kadras-%d.ppm  filmas.mp4");
//    system("rm *.ppm");


    return 0;
}
