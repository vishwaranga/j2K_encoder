//*********************************************************************************************************************
//
// Project (EN 4202)
// Department of Electronic & Telecommuncation Engineering
// University of Moratuwa
// Sri Lanka
//
// ********************************************************************************************************************
//
// PROJECT      :   JPEG2000 IP Core Development
// FILE         :   Wavelet2D.h
// AUTHOR       :   malan
// DESCRIPTION  :   the header file for wavelet class(trial)
//
//
// ********************************************************************************************************************
//
// REVISIONS:
//
//  Date        Developer       Description
//  ----        ---------       -----------
//  24/06/2016  malan           started coding
//
//
//
// ********************************************************************************************************************

#ifndef _WAVELET2D_H
#define _WAVELET2D_H

#include "commonDefs.h"

class Wavelet2D {

    private:

        long a;
        long b;
        long c;
        long d;
        long k1;
        long k2;
        int pixel_w;
        int pixel_l;
        int levels;
        int x;
        int y;
        void waveletX(queue<long> *pixelBuf);
        void waveletXodd(queue<long> *pixelBuf);
        float pixlFetcher(queue<long> *pixelBuf);
        void waveletY(queue<coeffPkt> *coeffBuf);
        void waveletYodd(queue<coeffPkt> *coeffBuf);
        void coeffParser(queue<coeffPkt> *coeffBuf, int xc, int yc, int64_t coeff);
        int levelCompltd;
        long long image[sbsize*2][sbsize*2];
        long fixedMul(int a, int b, int shifter);

    public:

        Wavelet2D();
        ~Wavelet2D();
        void run(queue<long> *pixelBuf, queue<coeffPkt> *coeffBuf, queue<setPkt> *infoBuf);
};

#endif /* WAVELET2D_H__ */
