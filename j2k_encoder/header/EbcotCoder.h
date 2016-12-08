/*
*	JPEG2000 IP Core development
*	EbcotCoder.h
*
*	created : 2016/8/16
*	author	: Randika
*
*/

#ifndef EBCOTCODER_H__
#define EBCOTCODER_H__

# include "commonDefs.h"

using namespace std;

const int maxbitplanes = 11;
const int verticalModeOn = 0;

class EbcotCoder {
	private:
	queue<context> localqueue;

	context ZeroCoding (int cells[3][3],int signArray[3][3], int subband);
	context SignCoding (int cells[3][3], int sigma[3][3], int signArray[3][3]);
	context MgRefinementCoding (int cells [3][3], int sigma[3][3], int sigmaDash);
	RLCcontext RunLengthCoding (int cellColumn[4]);

	int CleanUpPass (int (&bitplane)[bpsize][bpsize], int (&sigmaplane)[bpsize][bpsize], int (&eetaplane)[bpsize][bpsize], int (&signArray)[bpsize][bpsize], queue<context> *localqueue, int &subband);
	int SPropagationPass (int (&bitplane)[bpsize][bpsize], int (&sigmaplane)[bpsize][bpsize], int (&eetaplane)[bpsize][bpsize], int (&signArray)[bpsize][bpsize], queue<context> *localqueue, int &subband);
	int MagnitudeRefPass (int (&bitplane)[bpsize][bpsize], int (&sigmaplane)[bpsize][bpsize], int (&eetaplane)[bpsize][bpsize], int (&sigmaDash)[bpsize][bpsize], queue<context> *localqueue);
	int BitPlaneSeparator (int (&codeblock)[bpsize][bpsize], int (&bitplanes)[maxbitplanes][bpsize][bpsize], int (&signArray)[bpsize][bpsize]);
	int CodeBlockCoder (int (&codeblock)[bpsize][bpsize], int subband, queue<context> *localqueue, int &codingPassCount, int maxplanes);
	int cbloader (queue <sbandPkt> *llblock, queue <sbandPkt> *lhblock, queue <sbandPkt> *hlblock, queue <sbandPkt> *hhblock,queue <cblock> *llblockcb, queue <cblock> *lhblockcb, queue <cblock> *hlblockcb, queue <cblock> *hhblockcb);

	public:
	EbcotCoder();

	//int run (queue <sbandPkt> *llblock, queue <sbandPkt> *lhblock, queue <sbandPkt> *hlblock, queue <sbandPkt> *hhblock, queue <context> *contextQ, int epsilonll, int epsilonlh, int epsilonhl, int epsilonhh);
	int run (queue <sbandPkt> *llblock, queue <sbandPkt> *lhblock, queue <sbandPkt> *hlblock, queue <sbandPkt> *hhblock, queue <context> *contextQ, queue<pktParamBPC> *param2BPC);
};

#endif
