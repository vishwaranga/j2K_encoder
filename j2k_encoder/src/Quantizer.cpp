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
// FILE         :   Quantizer.cpp
// AUTHOR       :   malan
// DESCRIPTION  :   the class for quantizing(trial)
//
//
// ********************************************************************************************************************
//
// REVISIONS:
//
//  Date        Developer       Description
//  ----        ---------       -----------
//  01/08/2016  malan           started coding
// ********************************************************************************************************************

#include "Quantizer.h"

static const float norms[4][10] = {
        {1.000, 1.965, 4.177, 8.403, 16.90, 33.84, 67.69, 135.3, 270.6, 540.9},
        {2.022, 3.989, 8.355, 17.04, 34.27, 68.63, 137.3, 274.6, 549.0},
        {2.022, 3.989, 8.355, 17.04, 34.27, 68.63, 137.3, 274.6, 549.0},
        {2.080, 3.865, 8.307, 17.18, 34.71, 69.59, 139.3, 278.6, 557.2}
        };

Quantizer::Quantizer(){
    cout << "quantizing begun.." << endl;
    //delta = 0.0078125;
    ill = 0;
    jll = 0;
    ilh = 0;
    jlh = 0;
    ihl = 0;
    jhl = 0;
    ihh = 0;
    jhh = 0;
    maxLL = 0;
    maxLH = 0;
    maxHL = 0;
    maxHH = 0;
    counter = 0;
//    minLL = 0;
};


Quantizer::~Quantizer(){
    cout << "quantizing ended.." << endl;
}

int16_t Quantizer::quantizer(int xc, int yc, long long coeff){
    long value;
    if(xc<(sbsize) && yc<(sbsize))
    {
        if (abs(coeff)>maxLL)
        maxLL = abs(coeff);
        //cout << maxLL << endl;
    }

    else if(xc>=(sbsize) && yc<(sbsize))
    {
        if (abs(coeff)>maxHL)
        maxHL = abs(coeff);
    }
    else if(xc<(sbsize) && yc>=(sbsize))
    {
        if (abs(coeff)>maxLH)
        maxLH = abs(coeff);
    }
    else
    {
        if (abs(coeff)>maxHH)
        maxHH = abs(coeff);
    }
    //cout << coeff << "\t" << value << endl;
    //counter ++;
    value = (long)coeff;
    return value;
}

void Quantizer::coeffParser(queue<sbandPkt> *LLBuf, queue<sbandPkt> *HLBuf, queue<sbandPkt> *LHBuf, queue<sbandPkt> *HHBuf, int xc, int yc, long coeff){
    if(xc<(sbsize) && yc<(sbsize))
    {
       subbandLL.sband[ill][jll] = coeff;
       if (ill == (sbsize-1) && jll == (sbsize-1)){
           LLBuf->push(subbandLL);
       }
       if (jll == (sbsize-1)){
           ill++;
           jll = 0;
       }
       else
           jll++;

    }
    else if(xc<(sbsize) && yc>=(sbsize))
    {
       subbandHL.sband[ihl][jhl] = coeff;
       if (ihl == (sbsize-1) && jhl == (sbsize-1)){
           HLBuf->push(subbandHL);
       }
       if (jhl == (sbsize-1)){
           ihl++;
           jhl=0;
       }
       else
           jhl++;
    }
    else if(xc>=(sbsize) && yc<(sbsize))
    {
       subbandLH.sband[ilh][jlh] = coeff;
       if (ilh == (sbsize-1) && jlh == (sbsize-1)){
           LHBuf->push(subbandLH);
       }
       if (jlh == (sbsize-1)){
           ilh++;
           jlh = 0;
       }
       else
           jlh++;
    }
    else
    {
       subbandHH.sband[ihh][jhh] = coeff;
       if (ihh == (sbsize-1) && jhh == (sbsize-1)){
           HHBuf->push(subbandHH);
       }
       if (jhh == (sbsize-1)){
           ihh++;
           jhh = 0;
       }
       else
           jhh++;
}
}
void Quantizer::parameterParser(int precLL, int precLH, int precHL, int precHH, queue<pktParamBPC> *param2BPC, queue<pktParamfnl> *pkt2fnl)
{
    int exponent, mantissa, p, n, stepsize;
    float norm;
    pktParamBPC pktBPC;
    pktParamfnl pktfnl;
    norm = norms[0][1];
    stepsize = floor((1/norm)*pow(2, coefShift));
    p = floor(log2(stepsize)) - coefShift;
    n = preShift - floor(log2(stepsize));
    mantissa = (n<0 ? stepsize >> -n : stepsize << n)& 0x7ff;
    exponent = precLL - p;
    cout << exponent << "\t" << mantissa << endl;
    pktBPC.LLe = exponent;
    pktfnl.LLe = exponent;
    pktfnl.LLm = mantissa;
    norm = norms[1][0];
    stepsize = floor((1/norm)*pow(2, coefShift));
    p = floor(log2(stepsize)) - coefShift;
    n = preShift - floor(log2(stepsize));
    mantissa = (n<0 ? stepsize >> -n : stepsize << n)& 0x7ff;
    exponent = precLH - p;
    cout << exponent << "\t" << mantissa << endl;
    pktBPC.LHe = exponent;
    pktfnl.LHe = exponent;
    pktfnl.LHm = mantissa;
    norm = norms[2][0];
    stepsize = floor((1/norm)*pow(2, coefShift));
    p = floor(log2(stepsize)) - coefShift;
    n = preShift - floor(log2(stepsize));
    mantissa = (n<0 ? stepsize >> -n : stepsize << n)& 0x7ff;
    exponent = precHL - p;
    cout << exponent << "\t" << mantissa << endl;
    pktBPC.HLe = exponent;
    pktfnl.HLe = exponent;
    pktfnl.HLm = mantissa;
    norm = norms[3][0];
    stepsize = floor((1/norm)*pow(2, coefShift));
    p = floor(log2(stepsize)) - coefShift;
    n = preShift - floor(log2(stepsize));
    mantissa = (n<0 ? stepsize >> -n : stepsize << n)& 0x7ff;
    exponent = precHH - p;
    cout << exponent << "\t" << mantissa << endl;
    pktBPC.HHe = exponent;
    pktfnl.HHe = exponent;
    pktfnl.HHm = mantissa;
    param2BPC->push(pktBPC);
    pkt2fnl->push(pktfnl);
}

void Quantizer::run(queue<coeffPkt> *coeffBuf, queue<sbandPkt> *LLBuf, queue<sbandPkt> *HLBuf, queue<sbandPkt> *LHBuf, queue<sbandPkt> *HHBuf, queue<pktParamBPC> *param2BPC, queue<pktParamfnl> *param2fnl){
    while(!coeffBuf->empty()){
        coeffPkt tmp;
        tmp = coeffBuf->front();
        coeffBuf->pop();
        coeffParser(LLBuf, HLBuf, LHBuf, HHBuf, tmp.x, tmp.y, quantizer(tmp.x, tmp.y, tmp.value));
    }
    if (maxLL == 0)
        maxLL = 1;
    else
        maxLL = ceil(log2(maxLL));
    if (maxLH == 0)
        maxLH = 1;
    else
        maxLH = ceil(log2(maxLH));
    if (maxHL == 0)
        maxHL = 1;
    else
        maxHL = ceil(log2(maxHL));
    if (maxHH == 0)
        maxHH = 1;
    else
        maxHH = ceil(log2(maxHH));
    parameterParser((maxLL-1), (maxLH-1), (maxHL-1), (maxHH-1), param2BPC, param2fnl);
    cout << counter << endl;

}

