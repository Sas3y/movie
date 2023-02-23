#ifndef PPM6_H_INCLUDED
#define PPM6_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DID_EIL 20
#define DID_EIL1 (DID_EIL-1)

struct PPM6Antraste{
     char eilute1[DID_EIL];
     int stulpeliu;
     int eiluciu;
     int didSpalva;
};

struct PPM6Pikseliai{
     int stulpeliu;
     int eiluciu;
     int didSpalva;
     size_t kiekBaitu;
     uint8_t* pikseliai;
};

struct PPM6{
     char* pavadinimas;
     struct PPM6Antraste* antraste;
     struct PPM6Pikseliai* pikseliai;
};


struct PPM6Antraste* naujaAntraste();

void trinkAntraste(struct PPM6Antraste* pa);

void infoAntraste(struct PPM6Antraste* pa);

struct PPM6Pikseliai* naujiPikseliai(int stulpeliu, int eiluciu,int didSpalva);

void trinkPikselius(struct PPM6Pikseliai* px);

void infoPikseliai(struct PPM6Pikseliai* px);

void skaitykAntraste(const char* failas, struct PPM6Antraste* pa);

void skaitykPikselius(const char* failas, struct PPM6Pikseliai* px);

struct PPM6* naujasPaveikslelis();

void trinkPaveiksleli(struct PPM6* pav);

void skaitykPaveiksleli(struct PPM6* pav, const char* vardas);

void rasykPaveiksleli(struct PPM6* pav);

void rasykPaveiksleliKaip(struct PPM6* pav, const char* vardas);

void infoPaveikslelis(struct PPM6* pav);

struct PPM6* kurkPaveiksleli(const char* vardas,
                             int stulpeliu,
                             int eiluciu,
                             int didSpalva);

void dekTaska(struct PPM6* pav,
              int x, int y,
              uint8_t r, uint8_t g, uint8_t b);

void atkarpa(struct PPM6* pav,
             int x1, int y1,
             int x2, int y2,
             uint8_t r, uint8_t g, uint8_t b);

void trikampis(struct PPM6* pav,
               int x1, int y1,
               int x2, int y2,
               int x3, int y3,
               uint8_t r, uint8_t g, uint8_t b);

void staciakampis(struct PPM6* pav,
                 int x, int y,
                 int p, int a,
                 uint8_t r, uint8_t g, uint8_t b);

void raideC(struct PPM6* pav,
                 int x, int y,                    // k.v. kampas
                 int p, int a,
                 uint8_t r, uint8_t g, uint8_t b);



#endif // PPM6_H_INCLUDED
