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
// FILE         :   Wavelet2D.cpp
// AUTHOR       :   malan
// DESCRIPTION  :   the class for wavelet calculation(trial)
//
//
// ********************************************************************************************************************
//
// REVISIONS:
//
//  Date        Developer       Description
//  ----        ---------       -----------
//  28/06/2016  malan           started coding
//  09/07/2016  malan           complteted for 1-level
//  10/07/2016  malan           started testing for bugs
//  13/07/2016  malan           testing successfully completed for 1-level wavelet
//  13/07/2016  malan           started extending the code more levels
// ********************************************************************************************************************

#include "Wavelet2D.h"

Wavelet2D::Wavelet2D(){
    cout << "Wavelet calculation begun.." << endl;
    a = floor(1.586134342*pow(2, coefShift));
    b = floor(0.052980118*pow(2, coefShift));
    c = floor(0.882911075*pow(2, coefShift));
    d = floor(0.443506852*pow(2, coefShift));
    /*//
    k2 = 1.14960;
    k1 = -0.86987;//*/
    //k1 =1, k2 =1;
    //
    k1 = floor(1.230174105*pow(2, coefShift-1));
    k2 = floor(1.625732422*pow(2, coefShift-1));//*/
    /*//
    k1 = 8192;
    k2 = 8192;//*/
    levelCompltd = 0;
}

Wavelet2D::~Wavelet2D(){
    cout << "Wavelet calculation ended.." << endl;
}

void Wavelet2D::waveletX(queue<long> *pixelBuf){
    if (levelCompltd==0){
        image[0][y] = pixlFetcher(pixelBuf);
        image[1][y] = pixlFetcher(pixelBuf);
        image[2][y] = pixlFetcher(pixelBuf);
    }
    image[1][y] = image[1][y] - fixedMul(a,image[0][y]+image[2][y], 10);
    image[0][y] = image[0][y] - fixedMul(b,2*image[1][y], 10);
    for (int i=3; i<(pixel_w-1); i = i+2){
        if (levelCompltd==0){
            image[i][y] = pixlFetcher(pixelBuf);
            image[i+1][y] = pixlFetcher(pixelBuf);
        }
        image[i][y] = image[i][y] - fixedMul(a,(image[i-1][y]+image[i+1][y]), 10);
        image[i-1][y] = image[i-1][y] - fixedMul(b,(image[i-2][y]+image[i][y]), 10);
        image[i-2][y] = image[i-2][y] + fixedMul(c,(image[i-3][y]+image[i-1][y]), 10);
        if (i==3){
            image[i-3][y] = image[i-3][y] + fixedMul(d,2*image[i-2][y], 10);
        }
        else{
            image[i-3][y] = image[i-3][y] + fixedMul(d,(image[i-4][y]+image[i-2][y]), 10);
        }
        image[i-2][y] = fixedMul(k1,image[i-2][y], 10);
        image[i-3][y] = fixedMul(k2,image[i-3][y], 10);
    }
    if (levelCompltd==0){
        image[pixel_w-1][y] = pixlFetcher(pixelBuf) - fixedMul(a,2*image[pixel_w-2][y], 10);
    }
    else {
        image[pixel_w-1][y] = image[pixel_w-1][y] - fixedMul(a,2*image[pixel_w-2][y], 10);
    }
    image[pixel_w-2][y] = image[pixel_w-2][y] - fixedMul(b,(image[pixel_w-3][y]+image[pixel_w-1][y]), 10);
    image[pixel_w-3][y] = image[pixel_w-3][y] + fixedMul(c,(image[pixel_w-4][y]+image[pixel_w-2][y]), 10);
    image[pixel_w-4][y] = image[pixel_w-4][y] + fixedMul(d,(image[pixel_w-5][y]+image[pixel_w-3][y]), 10);
    image[pixel_w-1][y] = image[pixel_w-1][y] + fixedMul(c,2*image[pixel_w-2][y], 10);
    image[pixel_w-2][y] = image[pixel_w-2][y] + fixedMul(d,(image[pixel_w-3][y]+image[pixel_w-1][y]), 10);
    image[pixel_w-3][y] = fixedMul(k1,image[pixel_w-3][y], 10);
    image[pixel_w-4][y] = fixedMul(k2,image[pixel_w-4][y], 10);
    image[pixel_w-1][y] = fixedMul((k1),image[pixel_w-1][y], 10);
    image[pixel_w-2][y] = fixedMul(k2,image[pixel_w-2][y], 10);

}


void Wavelet2D::waveletXodd(queue<long> *pixelBuf){
    if (levelCompltd==0){
        image[0][y] = pixlFetcher(pixelBuf);
        image[1][y] = pixlFetcher(pixelBuf);
        image[2][y] = pixlFetcher(pixelBuf);
    }
    image[1][y] = image[1][y] - fixedMul(a,(image[0][y]+image[2][y]), 10);
    image[0][y] = image[0][y] - fixedMul(b,2*image[1][y], 10);
    for (int i=3; i<(pixel_w-1); i = i+2){
        if (levelCompltd==0){
            image[i][y] = pixlFetcher(pixelBuf);
            image[i+1][y] = pixlFetcher(pixelBuf);
        }
        image[i][y] = image[i][y] - fixedMul(a,(image[i-1][y]+image[i+1][y]), 10);
        image[i-1][y] = image[i-1][y] - fixedMul(b,(image[i-2][y]+image[i][y]), 10);
        image[i-2][y] = image[i-2][y] + fixedMul(c,(image[i-3][y]+image[i-1][y]), 10);
        if (i==3){
            image[i-3][y] = image[i-3][y] - fixedMul(d,2*image[1][y], 10);
        }
        else{
            image[i-3][y] = image[i-3][y] + fixedMul(d,(image[i-4][y]+image[i-2][y]), 10);
        }
        image[i-2][y] = fixedMul(k1,image[i-2][y], 10);
        image[i-3][y] = fixedMul(k2,image[i-3][y], 10);
    }
    image[pixel_w-1][y] = image[pixel_w-1][y] - fixedMul(b,2*image[pixel_w-2][y], 10);
    image[pixel_w-2][y] = image[pixel_w-2][y] + fixedMul(c,(image[pixel_w-3][y]+image[pixel_w-1][y]), 10);
    image[pixel_w-3][y] = image[pixel_w-3][y] + fixedMul(d,(image[pixel_w-4][y]+image[pixel_w-2][y]), 10);
    image[pixel_w-1][y] = image[pixel_w-1][y] + fixedMul(d,2*image[pixel_w-2][y], 10);
    image[pixel_w-2][y] = fixedMul(k1,image[pixel_w-2][y], 10);
    image[pixel_w-3][y] = fixedMul(k2,image[pixel_w-3][y], 10);
    image[pixel_w-1][y] = fixedMul(k2,image[pixel_w-1][y], 10);

}

void Wavelet2D::waveletY(queue<coeffPkt> *coeffBuf){
    int tmpx;
    int tmpy;
    float factor;
    if (x%2 == 0){
        tmpx = x/2;
    }
    else{
        if (pixel_w%2==0){
            tmpx = (pixel_w+x-1)/2;
        }
        else {
            tmpx = (pixel_w+x)/2;
        }
    }
    image[x][1] = image[x][1] - fixedMul(a,(image[x][0]+image[x][2]), 10);
    image[x][0] = image[x][0] - fixedMul(b,2*image[x][1], 10);
    for (int i=3; i<pixel_l-1; i = i+2){
        image[x][i] = image[x][i] - fixedMul(a,(image[x][i-1]+image[x][i+1]), 10);
        image[x][i-1] = image[x][i-1] - fixedMul(b,(image[x][i-2]+image[x][i]), 10);
        image[x][i-2] = image[x][i-2] + fixedMul(c,(image[x][i-3]+image[x][i-1]), 10);
        if (i==3){
            image[x][i-3] = image[x][i-3] + fixedMul(d,2*image[x][i-2], 10);
        }
        else{
            image[x][i-3] = image[x][i-3] + fixedMul(d,(image[x][i-4]+image[x][i-2]), 10);
        }
        image[x][i-2] = fixedMul(k1,image[x][i-2], 10);
        image[x][i-3] = fixedMul(k2,image[x][i-3], 10);
        tmpy = (pixel_l + i - 3)/2;
        coeffParser(coeffBuf, tmpx, tmpy, image[x][i-2]);
        tmpy = (i - 3)/2;
        if (x%2==0){
            if (levelCompltd == -1){
                coeffParser(coeffBuf, tmpx, tmpy, image[x][i-3]);
            }
            else{
                image[tmpx][tmpy] = image[x][i-3];
            }
        }
        else{
            coeffParser(coeffBuf, tmpx, tmpy, image[x][i-3]);
        }
        //cout << "column no: " << i << "is done..." << endl;
    }
    image[x][pixel_l-1] = image[x][pixel_l-1] - fixedMul(a,2*image[x][pixel_l-2], 10);
    image[x][pixel_l-2] = image[x][pixel_l-2] - fixedMul(b,(image[x][pixel_l-3]+image[x][pixel_l-1]), 10);
    image[x][pixel_l-3] = image[x][pixel_l-3] + fixedMul(c,(image[x][pixel_l-4]+image[x][pixel_l-2]), 10);
    image[x][pixel_l-4] = image[x][pixel_l-4] + fixedMul(d,(image[x][pixel_l-5]+image[x][pixel_l-3]), 10);
    image[x][pixel_l-1] = image[x][pixel_l-1] + fixedMul(c,2*image[x][pixel_l-2], 10);
    image[x][pixel_l-2] = image[x][pixel_l-2] + fixedMul(d,(image[x][pixel_l-3]+image[x][pixel_l-1]), 10);
    image[x][pixel_l-3] = fixedMul(k1,image[x][pixel_l-3], 10);
    image[x][pixel_l-4] = fixedMul(k2,image[x][pixel_l-4], 10);
    image[x][pixel_l-1] = fixedMul(k1,image[x][pixel_l-1], 10);
    image[x][pixel_l-2] = fixedMul(k2,image[x][pixel_l-2], 10);
    coeffParser(coeffBuf, tmpx, pixel_l-2, image[x][pixel_l-3]);
    coeffParser(coeffBuf, tmpx, pixel_l-1, image[x][pixel_l-1]);
    if (x%2==0){
        if (levelCompltd == -1){
            coeffParser(coeffBuf, tmpx, (pixel_l/2-1), image[x][pixel_l-2]);
            coeffParser(coeffBuf, tmpx, (pixel_l/2-2), image[x][pixel_l-4]);
        }
        else{
            image[tmpx][pixel_l/2-1] = image[x][pixel_l-2];
            image[tmpx][pixel_l/2-2] = image[x][pixel_l-4];
        }
    }
    else{
        coeffParser(coeffBuf, tmpx, pixel_l/2-1, image[x][pixel_l-2]);
        coeffParser(coeffBuf, tmpx, (pixel_l/2-2), image[x][pixel_l-4]);
    }
}

void Wavelet2D::waveletYodd(queue<coeffPkt> *coeffBuf){
    int tmpx;
    int tmpy;
    float factor;
    if (x%2 == 0){
        tmpx = x/2;
    }
    else{
        if (pixel_w%2==0){
            tmpx = (pixel_w+x-1)/2;
        }
        else {
            tmpx = (pixel_w+x)/2;
        }
    }
    image[x][1] = image[x][1] - a*(image[x][0]+image[x][2]);
    image[x][0] = image[x][0] - 2*b*(image[x][1]);
    for (int i=3; i<pixel_l-1; i = i+2){
        image[x][i] = image[x][i] - a*(image[x][i-1]+image[x][i+1]);
        image[x][i-1] = image[x][i-1] - b*(image[x][i-2]+image[x][i]);
        image[x][i-2] = image[x][i-2] + c*(image[x][i-3]+image[x][i-1]);
        if (i==3){
            image[x][i-3] = image[x][i-3] + 2*d*image[x][i-2];
        }
        else{
            image[x][i-3] = image[x][i-3] + d*(image[x][i-4]+image[x][i-2]);
        }
        image[x][i-2] = image[x][i-2]*(k1);
        image[x][i-3] = image[x][i-3]*k2;
        tmpy = (pixel_l + i - 2)/2;
        coeffParser(coeffBuf, tmpx, tmpy, image[x][i-2]);
        tmpy = (i - 3)/2;
        if (x%2==0){
            if (levelCompltd == -1){
                coeffParser(coeffBuf, tmpx, tmpy, image[x][i-3]);
            }
            else{
                image[tmpx][tmpy] = image[x][i-3];
            }
        }
        else{
            coeffParser(coeffBuf, tmpx, tmpy, image[x][i-3]);
        }
        //cout << "column no: " << i << "is done..." << endl;
    }
    image[x][pixel_l-1] = image[x][pixel_l-1] - 2*b*image[x][pixel_l-2];
    image[x][pixel_l-2] = image[x][pixel_l-2] + c*(image[x][pixel_l-3]+image[x][pixel_l-1]);
    image[x][pixel_l-3] = image[x][pixel_l-3] + d*(image[x][pixel_l-4]+image[x][pixel_l-2]);
    image[x][pixel_l-1] = image[x][pixel_l-1] + 2*d*image[x][pixel_l-2];
    image[x][pixel_l-2] = image[x][pixel_l-2]*(k1);
    image[x][pixel_l-3] = image[x][pixel_l-3]*k2;
    image[x][pixel_l-1] = image[x][pixel_l-1]*k2;
    coeffParser(coeffBuf, tmpx, pixel_l-1, image[x][pixel_l-2]);
    if (x%2==0){
        if (levelCompltd == -1){
            coeffParser(coeffBuf, tmpx, (pixel_l/2-1), image[x][pixel_l-2]);
            coeffParser(coeffBuf, tmpx, (pixel_l/2-2), image[x][pixel_l-4]);
        }
        else{
            image[tmpx][pixel_l/2-1] = image[x][pixel_l-2];
            image[tmpx][pixel_l/2-2] = image[x][pixel_l-4];
        }
    }
    else{
        coeffParser(coeffBuf, tmpx, (pixel_l+1)/2-1, image[x][pixel_l-1]);
        coeffParser(coeffBuf, tmpx, (pixel_l+1)/2-2, image[x][pixel_l-3]);
    }
}

void Wavelet2D::coeffParser(queue<coeffPkt> *coeffBuf, int xc, int yc, int64_t  coeff){
    coeffPkt pktOut;
    pktOut.x = xc;
    pktOut.y = yc;
    pktOut.value = coeff;
    coeffBuf->push(pktOut);
}

float Wavelet2D::pixlFetcher(queue<long> *pixelBuf){
    float temp = pixelBuf->front();
    pixelBuf->pop();
    return temp;
}

long Wavelet2D::fixedMul(int a, int b, int shifter)
{
    long long temp;
    temp = (long long) a * (long long ) b;
    //temp = temp>>shifter;
    //temp += 4096;
    temp = temp >> coefShift;
    return (long)temp;
}

void Wavelet2D::run(queue<long> *pixelBuf, queue<coeffPkt> *coeffBuf, queue<setPkt> *infoBuf){
        if (!infoBuf->empty()) {
            x = 0;
            y = 0;
            setPkt tmpPkt;
            tmpPkt = infoBuf->front();
            infoBuf->pop();
            pixel_l = tmpPkt.pixel_l;
            pixel_w = tmpPkt.pixel_w;
            levels  = tmpPkt.levels;
            if (!pixelBuf->empty()){
                while(levelCompltd!=-1){
                    if (pixel_w%2==0){
                        for (int i=0; i<pixel_l; i++){
                            waveletX(pixelBuf);
                            y++;
                        }
                    }
                    else {
                        for (int i=0; i<pixel_l; i++){
                            waveletXodd(pixelBuf);
                            y++;
                        }
                    }
                    levelCompltd ++;
                    if (levelCompltd == levels){
                        levelCompltd = -1;
                    }
                    if (pixel_l%2==0){
                        for (int j=0; j<pixel_w; j++){
                            waveletY(coeffBuf);
                            x++;
                        }
                    }
                    else {
                        for (int j=0; j<pixel_w; j++){
                            waveletYodd(coeffBuf);
                            x++;
                        }
                    }
                    x = 0;
                    y = 0;
                    if (pixel_l%2==0){
                        pixel_l = pixel_l/2;
                    }
                    else{
                        pixel_l = (pixel_l+1)/2;
                    }
                    if (pixel_w%2==0){
                        pixel_w = pixel_w/2;
                    }
                    else{
                        pixel_w = (pixel_w+1)/2;
                    }
                }

            }
        }

}
