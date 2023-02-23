#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ppm6.h"


struct PPM6Antraste* naujaAntraste(){

     struct PPM6Antraste* pa =
          (struct PPM6Antraste*) calloc(sizeof(struct PPM6Antraste), 1);
     strcpy(pa->eilute1, "P6");
     pa->stulpeliu = 0;
     pa->eiluciu = 0;
     pa->didSpalva = 0;
     return pa;

}

void trinkAntraste(struct PPM6Antraste* pa){

     free(pa);

}

void infoAntraste(struct PPM6Antraste* pa){

     printf("%s\n%d %d\n%d\n",
            pa->eilute1, pa->stulpeliu, pa->eiluciu, pa->didSpalva);

}

struct PPM6Pikseliai* naujiPikseliai(int stulpeliu, int eiluciu,int didSpalva){

    struct PPM6Pikseliai* px = (struct PPM6Pikseliai*) calloc(sizeof(struct PPM6Pikseliai), 1);
    px->stulpeliu = stulpeliu;
    px->eiluciu = eiluciu;
    px->didSpalva = didSpalva;
    px->kiekBaitu = stulpeliu * eiluciu * 3;
    px->pikseliai = (uint8_t*) calloc(px->kiekBaitu, sizeof(uint8_t));
    return px;
}

void trinkPikselius(struct PPM6Pikseliai* px){

    free(px->pikseliai);
    free(px);

}

void infoPikseliai(struct PPM6Pikseliai* px){

     printf("%d x %d\n%d\n\%ld : ",
             px->stulpeliu, px->eiluciu,
             px->didSpalva, px->kiekBaitu);
     int sp = 20;
     if (sp > px->kiekBaitu) sp = px->kiekBaitu;
     for(int i=0;i<sp;i++)
        printf("%3x", px->pikseliai[i]);
     printf("\n");

}

/////////////////////////////////////
void skaitykAntraste(const char* failas, struct PPM6Antraste* pa){
   FILE* fin = fopen(failas, "r");

   char s[DID_EIL] = "";
   fscanf(fin,"%s", s);
   if(strcmp(s,"P6") != 0){
      perror("Blogas failo formatas\n");
      fclose(fin);
      exit(1);
   }
   fscanf(fin,"%d%d%d", &pa->stulpeliu,
                        &pa->eiluciu,
                        &pa->didSpalva);

   fclose(fin);

}


/////////////////////////////////////////////////////////////////


void skaitykPikselius(const char* failas, struct PPM6Pikseliai* px){
   FILE* fin = fopen(failas, "r");
   char ch;
   int kiekA = 0;
   while(kiekA<3){
     fread(&ch, 1, 1, fin);
     if(ch == '\n') kiekA++;
   }

   fread(px->pikseliai, sizeof(uint8_t), px->kiekBaitu, fin);

   fclose(fin);
}


/////////////////////////////////////////////////////////////////
struct PPM6* naujasPaveikslelis(){
   struct PPM6* p6 = (struct PPM6*) calloc(sizeof(struct PPM6), 1);

   p6->pavadinimas = NULL;
   p6->antraste = NULL;
   p6->pikseliai = NULL;

   return p6;
}

void trinkPaveiksleli(struct PPM6* pav){

   if(pav->pavadinimas != NULL)
       free(pav->pavadinimas);
   if(pav->antraste != NULL)
       trinkAntraste(pav->antraste);

   if(pav->pikseliai != NULL)
       trinkPikselius(pav->pikseliai);
   free(pav);

}

void skaitykPaveiksleli(struct PPM6* pav, const char* vardas){

   pav->pavadinimas = (char*) calloc(sizeof(char), strlen(vardas)+1);
   strcpy(pav->pavadinimas, vardas);

   pav->antraste = naujaAntraste();
   skaitykAntraste(pav->pavadinimas, pav->antraste);


   pav->pikseliai = naujiPikseliai(pav->antraste->stulpeliu, pav->antraste->eiluciu,
                  pav->antraste->didSpalva);

   skaitykPikselius(pav->pavadinimas, pav->pikseliai);
}


void rasykPaveiksleli(struct PPM6* pav){

   FILE* fout = fopen(pav->pavadinimas, "w");
   fprintf(fout,"P6\n");
   fprintf(fout,"%d %d\n%d\n", pav->antraste->stulpeliu, pav->antraste->eiluciu,
                  pav->antraste->didSpalva);

   fwrite(pav->pikseliai->pikseliai, pav->pikseliai->kiekBaitu, 1, fout);
   fclose(fout);

}

void rasykPaveiksleliKaip(struct PPM6* pav, const char* vardas){

   free(pav->pavadinimas);
   pav->pavadinimas = (char*) calloc(sizeof(char), strlen(vardas)+1);
   strcpy(pav->pavadinimas, vardas);
   rasykPaveiksleli(pav);

}

void infoPaveikslelis(struct PPM6* pav){

   printf("%s\n", pav->pavadinimas);
   infoAntraste(pav->antraste);
   infoPikseliai(pav->pikseliai);

}
/////////////////////////////////////////////////////////////////
// Pieðimas
/////////////////////////////////////////////////////////////////
struct PPM6* kurkPaveiksleli(const char* vardas, int stulpeliu, int eiluciu, int
                             didSpalva){


   struct PPM6* pav = naujasPaveikslelis();

   pav->pavadinimas = (char*) calloc(sizeof(char), strlen(vardas)+1);
   strcpy(pav->pavadinimas, vardas);

   pav->antraste = naujaAntraste();
   pav->antraste->stulpeliu = stulpeliu;
   pav->antraste->eiluciu = eiluciu;
   pav->antraste->didSpalva = didSpalva;

   pav->pikseliai = naujiPikseliai(pav->antraste->stulpeliu, pav->antraste->eiluciu,
                  pav->antraste->didSpalva);

   for(int i=0;i<pav->pikseliai->kiekBaitu;i++){
      pav->pikseliai->pikseliai[i] = (uint8_t) didSpalva;
   }

   return pav;
}
/////////////////////////////////////////////////////////////
// dekTaska(x,y,c);
/////////////////////////////////////////////////////////////
void dekTaska(struct PPM6* pav, int x, int y,
              uint8_t r, uint8_t g, uint8_t b){
   if( x >= 0 && x < pav->antraste->stulpeliu &&
       y >= 0 && y < pav->antraste->eiluciu){
       int adresas = (y * pav->antraste->stulpeliu + x)  * 3;
       //colour problem
       /*
       pav->pikseliai->pikseliai[adresas] = r;
       pav->pikseliai->pikseliai[adresas+1] = g;
       pav->pikseliai->pikseliai[adresas+2] = b;
       */
       pav->pikseliai->pikseliai[adresas-1] = r;
       pav->pikseliai->pikseliai[adresas] = g;
       pav->pikseliai->pikseliai[adresas+1] = b;
   } else{

      printf("Blogos koordinatës: (%d, %d)\n", x,y);

   }

}

/////////////////////////////////////////////////////////////
void atkarpa(struct PPM6* pav, int x1, int y1, int x2, int y2,
              uint8_t r, uint8_t g, uint8_t b){

   int tasku = abs(x2-x1) + abs(y2-y1);
   double dt =  0.5 / tasku;
   for(double t=0; t <= 1.0 + dt; t+=dt){
      int x = (int) (x1 * (1 - t) + x2 * t);
      int y = (int) (y1 * (1 - t) + y2 * t);
      dekTaska(pav, x, y, r, g, b);
   }

}
///////////////////////////////////////////////////////////////
void trikampis(struct PPM6* pav, int x1, int y1,int x2, int y2,
               int x3, int y3,uint8_t r, uint8_t g, uint8_t b){

   int tasku = abs(x2-x1) + abs(y2-y1);
   double dt =  0.5 / tasku;
   for(double t=0; t <= 1.0 + dt; t+=dt){
      int x = (int) (x1 * (1 - t) + x2 * t);
      int y = (int) (y1 * (1 - t) + y2 * t);
      atkarpa(pav, x3, y3, x, y, r, g, b);
   }

 }

void staciakampis(struct PPM6* pav, int x, int X, int y, int Y,
                   uint8_t r, uint8_t g, uint8_t b){

   for(int xx=x; xx<X; xx++)
      for(int yy=y; yy<Y; yy++) {

      dekTaska(pav, xx, yy, r, g, b);

  }

}


/*
void raideC(struct PPM6* pav,  int x, int y,                    // k.v. kampas
                 int p, int a, uint8_t r, uint8_t g, uint8_t b){



       int xs[] = {x, x+p, x+p/4, x, x+p/4, x+p};
       int ys[] = {y, y, y+a/4, y+a, y+3*a/4, y+a};

       trikampis(pav, xs[0],ys[0], xs[1],ys[1], xs[2],ys[2], r,g,b);
       trikampis(pav, xs[0],ys[0], xs[2],ys[2], xs[3],ys[3], r,g,b);
       trikampis(pav, xs[2],ys[2], xs[3],ys[3], xs[4],ys[4], r,g,b);
       trikampis(pav, xs[3],ys[3], xs[4],ys[4], xs[5],ys[5], r,g,b);


}
*/

void rA(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,4*s+y,5*s+y,r,g,b);
    staciakampis(pav,0*s+x,1*s+x,1*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,7*s+y,r,g,b);
}

void rB(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,3*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,4*s+y,6*s+y,r,g,b);
}

void rC(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,1*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,2*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,5*s+y,6*s+y,r,g,b);
}

void rD(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,6*s+y,r,g,b);
}

void rE(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,5*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,5*s+x,6*s+y,7*s+y,r,g,b);
}

void rF(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,5*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
}

void rG(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,1*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,2*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,4*s+y,6*s+y,r,g,b);
    staciakampis(pav,2*s+x,5*s+x,3*s+y,4*s+y,r,g,b);
}

void rH(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
}

void rI(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,2*s+x,3*s+x,1*s+y,6*s+y,r,g,b);
    staciakampis(pav,0*s+x,5*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,0*s+x,5*s+x,6*s+y,7*s+y,r,g,b);
}

void rJ(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,4*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,6*s+y,r,g,b);
}

void rK(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,1*s+y,2*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,4*s+y,5*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,5*s+y,6*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,6*s+y,7*s+y,r,g,b);
}

void rL(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,6*s+y,r,g,b);
    staciakampis(pav,0*s+x,5*s+x,6*s+y,7*s+y,r,g,b);
}

void rM(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,1*s+y,2*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,1*s+y,2*s+y,r,g,b);
}

void rN(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,4*s+y,5*s+y,r,g,b);
}

void rO(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,1*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,6*s+y,r,g,b);
}

void rP(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,3*s+y,r,g,b);
}

void rQ(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,1*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,3*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,5*s+y,r,g,b);
    staciakampis(pav,3*s+x,5*s+x,5*s+y,7*s+y,r,g,b);
}

void rR(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,3*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,4*s+y,7*s+y,r,g,b);
}

void rS(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,1*s+y,3*s+y,r,g,b);
    staciakampis(pav,0*s+x,1*s+x,5*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,2*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,4*s+y,6*s+y,r,g,b);
}

void rT(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,2*s+x,3*s+x,1*s+y,7*s+y,r,g,b);
    staciakampis(pav,0*s+x,5*s+x,0*s+y,1*s+y,r,g,b);
}

void rU(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,6*s+y,r,g,b);
}

void rV(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,4*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,4*s+y,6*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,4*s+y,6*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,6*s+y,7*s+y,r,g,b);
}

void rW(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,5*s+y,6*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,4*s+y,5*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,5*s+y,6*s+y,r,g,b);
}

void rX(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,2*s+y,r,g,b);
    staciakampis(pav,0*s+x,1*s+x,5*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,2*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,5*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,4*s+y,5*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,4*s+y,5*s+y,r,g,b);
}

void rY(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,2*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,2*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,3*s+y,7*s+y,r,g,b);
}

void rZ(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,5*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,0*s+x,5*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,2*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,4*s+y,5*s+y,r,g,b);
    staciakampis(pav,0*s+x,1*s+x,5*s+y,6*s+y,r,g,b);
}

void r1(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,2*s+x,3*s+x,1*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,1*s+y,2*s+y,r,g,b);
    staciakampis(pav,0*s+x,5*s+x,6*s+y,7*s+y,r,g,b);
}

void r2(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,0*s+x,5*s+x,6*s+y,7*s+y,r,g,b);

    staciakampis(pav,0*s+x,1*s+x,1*s+y,2*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,3*s+y,r,g,b);


    staciakampis(pav,3*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,4*s+y,5*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,5*s+y,6*s+y,r,g,b);
}

void r3(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,1*s+y,2*s+y,r,g,b);
    staciakampis(pav,0*s+x,1*s+x,5*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,2*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,3*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,4*s+y,6*s+y,r,g,b);
}

void r4(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,4*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,0*s+y,7*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
}

void r5(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,0*s+y,4*s+y,r,g,b);
    staciakampis(pav,0*s+x,1*s+x,5*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,5*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,4*s+y,6*s+y,r,g,b);
}

void r6(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,1*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,4*s+y,6*s+y,r,g,b);
}

void r7(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,5*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,3*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,4*s+y,7*s+y,r,g,b);
}

void r8(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,1*s+y,3*s+y,r,g,b);
    staciakampis(pav,0*s+x,1*s+x,4*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,3*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,4*s+y,6*s+y,r,g,b);
}

void r9(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,1*s+y,3*s+y,r,g,b);
    staciakampis(pav,0*s+x,1*s+x,5*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,6*s+y,r,g,b);
}

void r0(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,1*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,0*s+y,1*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,1*s+y,6*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,3*s+y,4*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,4*s+y,5*s+y,r,g,b);
}

void rDOT(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,2*s+x,3*s+x,5*s+y,7*s+y,r,g,b);
}

void rCOM(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,2*s+x,3*s+x,5*s+y,6*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,6*s+y,7*s+y,r,g,b);
}

void rEXC(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,2*s+x,3*s+x,6*s+y,7*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,0*s+y,4*s+y,r,g,b);
}

void rHYP(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,5*s+x,3*s+y,4*s+y,r,g,b);
}

void rSNW(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,0*s+x,1*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,0*s+x,1*s+x,4*s+y,5*s+y,r,g,b);
    staciakampis(pav,1*s+x,2*s+x,3*s+y,4*s+y,r,g,b);

    staciakampis(pav,2*s+x,3*s+x,1*s+y,6*s+y,r,g,b);

    staciakampis(pav,4*s+x,5*s+x,2*s+y,3*s+y,r,g,b);
    staciakampis(pav,4*s+x,5*s+x,4*s+y,5*s+y,r,g,b);
    staciakampis(pav,3*s+x,4*s+x,3*s+y,4*s+y,r,g,b);
}

void rTRE(struct PPM6* pav,  int x, int y, int s,
            uint8_t r, uint8_t g, uint8_t b){
    staciakampis(pav,2*s+x,3*s+x,0*s+y,2*s+y,r,g,b);
    staciakampis(pav,1*s+x,4*s+x,2*s+y,4*s+y,r,g,b);
    staciakampis(pav,0*s+x,5*s+x,4*s+y,6*s+y,r,g,b);
    staciakampis(pav,2*s+x,3*s+x,6*s+y,7*s+y,r,g,b);
}

void strtoppm(struct PPM6* pav, char* text, int x, int y, int s,
            int mX, int mY,
            uint8_t r, uint8_t g, uint8_t b){
    int X=0, Y=0, rainbow=0, christmas1=0, christmas2=0;
    if(r==1 && g==0 && b==0){
        rainbow++;
    }
    if(r==2 && g==0 && b==0){
        christmas1++;
    }
    if(r==3 && g==0 && b==0){
        christmas2++;
    }

    //bug fixing
    int len=strlen(text);
    printf("%s, %d\n",text,len);

    if(y+Y+s*9>=mY){
        printf(" ERROR: Tekstas netelpa\n");
    }

    for(int i=0;i<len;i++){

        if(rainbow){
            switch (i%7){
            case 0://red (from violet)
                r=255;b=0;break;
            case 1://orange
                g=127;break;
            case 2://yellow
                g=255;break;
            case 3://green
                r=0;break;
            case 4://blue
                g=0;b=255;break;
            case 5://indigo
                r=75;b=130;break;
            case 6://violet
                r=148;b=211;break;
            }
        }
        if(christmas1){
            switch(i%2){
            case 0:
                r=232;g=13;b=5;break;
            case 1:
                r=18;g=148;b=18;break;
            }
        }
        if(christmas2){
            switch(i%2){
            case 0:
                r=18;g=148;b=18;break;
            case 1:
                r=232;g=13;b=5;break;
            }
        }

        switch(text[i]){
        case ' ':
            break;
        case 'A':
            rA(pav,x+X,y+Y,s,r,g,b);break;
        case 'B':
            rB(pav,x+X,y+Y,s,r,g,b);break;
        case 'C':
            rC(pav,x+X,y+Y,s,r,g,b);break;
        case 'D':
            rD(pav,x+X,y+Y,s,r,g,b);break;
        case 'E':
            rE(pav,x+X,y+Y,s,r,g,b);break;
        case 'F':
            rF(pav,x+X,y+Y,s,r,g,b);break;
        case 'G':
            rG(pav,x+X,y+Y,s,r,g,b);break;
        case 'H':
            rH(pav,x+X,y+Y,s,r,g,b);break;
        case 'I':
            rI(pav,x+X,y+Y,s,r,g,b);break;
        case 'J':
            rJ(pav,x+X,y+Y,s,r,g,b);break;
        case 'K':
            rK(pav,x+X,y+Y,s,r,g,b);break;
        case 'L':
            rL(pav,x+X,y+Y,s,r,g,b);break;
        case 'M':
            rM(pav,x+X,y+Y,s,r,g,b);break;
        case 'N':
            rN(pav,x+X,y+Y,s,r,g,b);break;
        case 'O':
            rO(pav,x+X,y+Y,s,r,g,b);break;
        case 'P':
            rP(pav,x+X,y+Y,s,r,g,b);break;
        case 'Q':
            rQ(pav,x+X,y+Y,s,r,g,b);break;
        case 'R':
            rR(pav,x+X,y+Y,s,r,g,b);break;
        case 'S':
            rS(pav,x+X,y+Y,s,r,g,b);break;
        case 'T':
            rT(pav,x+X,y+Y,s,r,g,b);break;
        case 'U':
            rU(pav,x+X,y+Y,s,r,g,b);break;
        case 'V':
            rV(pav,x+X,y+Y,s,r,g,b);break;
        case 'W':
            rW(pav,x+X,y+Y,s,r,g,b);break;
        case 'X':
            rX(pav,x+X,y+Y,s,r,g,b);break;
        case 'Y':
            rY(pav,x+X,y+Y,s,r,g,b);break;
        case 'Z':
            rZ(pav,x+X,y+Y,s,r,g,b);break;
        case '1':
            r1(pav,x+X,y+Y,s,r,g,b);break;
        case '2':
            r2(pav,x+X,y+Y,s,r,g,b);break;
        case '3':
            r3(pav,x+X,y+Y,s,r,g,b);break;
        case '4':
            r4(pav,x+X,y+Y,s,r,g,b);break;
        case '5':
            r5(pav,x+X,y+Y,s,r,g,b);break;
        case '6':
            r6(pav,x+X,y+Y,s,r,g,b);break;
        case '7':
            r7(pav,x+X,y+Y,s,r,g,b);break;
        case '8':
            r8(pav,x+X,y+Y,s,r,g,b);break;
        case '9':
            r9(pav,x+X,y+Y,s,r,g,b);break;
        case '0':
            r0(pav,x+X,y+Y,s,r,g,b);break;
        case '.':
            rDOT(pav,x+X,y+Y,s,r,g,b);break;
        case ',':
            rCOM(pav,x+X,y+Y,s,r,g,b);break;
        case '!':
            rEXC(pav,x+X,y+Y,s,r,g,b);break;
        case '-':
            rHYP(pav,x+X,y+Y,s,r,g,b);break;
        case '*':
            rSNW(pav,x+X,y+Y,s,r,g,b);break;
        case '^':
            rTRE(pav,x+X,y+Y,s,r,g,b);break;

        default:
            printf(" ERROR: Netinkamas simbolis %d vietoje\n",i);
        }
        //FOR FIT 7->5
        X+=s*7;
        if(x+X+s*7>=mX){
            X=0;
            Y+=s*9;
        }
    }
}
























