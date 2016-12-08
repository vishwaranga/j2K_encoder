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
// FILE         :   Quantizer.h
// AUTHOR       :   malan
// DESCRIPTION  :   the header file for quantizer class(trial)
//
//
// ********************************************************************************************************************
//
// REVISIONS:
//
//  Date        Developer       Description
//  ----        ---------       -----------
//  01/08/2016  malan           started coding
//
//
//
// ********************************************************************************************************************

#ifndef _QUANTIZER_H
#define _QUANTIZER_H

#include "commonDefs.h"

class Quantizer {

    private:

        float delta;
        int16_t quantizer(int xc, int yc, long long coeff);
        void coeffParser(queue<sbandPkt> *LLBuf, queue<sbandPkt> *HLBuf, queue<sbandPkt> *LHBuf, queue<sbandPkt> *HHBuf, int xc, int yc, long coeff);
        int ill, jll, ilh, jlh, ihl, jhl, ihh, jhh;
        void parameterParser(int precLL, int precLH, int precHL, int precHH, queue<pktParamBPC> *param2BPC, queue<pktParamfnl> *pkt2fnl);
        sbandPkt subbandLL;
        sbandPkt subbandHL;
        sbandPkt subbandLH;
        sbandPkt subbandHH;
        int maxLL;
        int maxHL;
        int maxLH;
        int maxHH;
        int counter;
    public:

        Quantizer();
        ~Quantizer();
        void run(queue<coeffPkt> *coeffBuf, queue<sbandPkt> *LLBuf, queue<sbandPkt> *HLBuf, queue<sbandPkt> *LHBuf, queue<sbandPkt> *HHBuf,queue<pktParamBPC> *param2BPC, queue<pktParamfnl> *param2fnl);
};

#endif /* QUANTIZER_H__ */
