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
// FILE         :   packetDefs.h
// AUTHOR       :   malan
// DESCRIPTION  :   the packet definition file(trial)
//
//
// ********************************************************************************************************************
//
// REVISIONS:
//
//  Date        Developer       Description
//  ----        ---------       -----------
//  30/06/2016  malan           started coding
//
//
//
// ********************************************************************************************************************

#ifndef _PACKETDEFS_H
#define	_PACKETDEFS_H

#include "commonDefs.h"

struct setPkt {

	unsigned int 	pixel_w;
	unsigned int  	pixel_l;
	unsigned int 	levels;

};

struct coeffPkt{

	unsigned int 	x;
	unsigned int 	y;
	int64_t	value;

};

struct cblock{
        signed int      array[64][64];
        };

struct sbandPkt{
    int    sband[64][64];
};

struct context {
        int cx;
        int d;
};

struct RLCcontext {
    int cx;
    int d;
    int uniformcx;
    int uniformd1;
    int uniformd2;
};

struct image{
    long imarray[128][128];
};

struct pktParamBPC
{
    int LLe;
    int LHe;
    int HLe;
    int HHe;
};
struct pktParamfnl
{
    int LLe;
    int LLm;
    int LHe;
    int LHm;
    int HLe;
    int HLm;
    int HHe;
    int HHm;
};

#endif /* PACKETDEFS_H__ */
