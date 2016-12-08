#ifndef _MQCODER_H
#define _MQCODER_H

#include "commonDefs.h"
#include <bitset>

class MQcoder
{
	public:
		MQcoder();
		~MQcoder();
		void run(queue <context> *localqueue, queue <int> *headerqueue, queue <uint8_t> *bit_stream);
	private:
		//----------global constants-------------------------------------------------------------------------------------

		unsigned int Q_e [47]; // probabilty estimation lookup table

		int MPS [19];

		unsigned int NMPS [47];

		unsigned int NLPS [47];

		bool SWITCH [47];

		// The initial index of context for BAC probability estimation lookup table
		int INDEX [19];
		//--------------global variables--------------------------------------------------
		int  A      ; // interval register
		int  C      ; // code register
		int  qe     ; // current probability value of index
		int  index  ; // current index of the CX
		int  BP     ; // compressed data buffer pointer
		int  BPST   ; // pointer which points to where the first byte is going to be placed
		int  CT     ; // counter for no: of shifts applied on A and C
		uint8_t B  	; // (output) byte pointed by the BP
		int  CX 	; // this is an input
		int D  		; // this is an input
		int L 		; // length(in bytes) of the bitstream for a given code block
		// vector<char> bit_stream; // vector to store the bit stream

		//--------------function prototypes--------------------------------------------------
		void Initialization();
		void push_B(queue <uint8_t> *bit_stream);
		void bit_Stuffing(queue <uint8_t> *bit_stream);
		void no_bit_Stuffing(queue <uint8_t> *bit_stream);
		void ByteOut(queue <uint8_t> *bit_stream);
		void RenormalizationENC(queue <uint8_t> *bit_stream);
		void CodeMPS(queue <uint8_t> *bit_stream);
		void CodeLPS(queue <uint8_t> *bit_stream);
		void FLUSHregister(queue <uint8_t> *bit_stream);

};

		// const unsigned int MQcoder::Q_e [47] = { 0x5601,0x3401,0x1801,0x0ACl,0x0521,0x0221,0x5601,0x5401,0x4801,0x3801
		// ,0x3001,0x2401,0x1C01,0x1601,0x5601,0x5401,0x5101,0x4801,0x3801,0x3401,0x3001,0x2801,0x2401,0x2201
		// ,0x1C01,0x1801,0x1601,0x1401,0x1201,0x1101,0x0AC1,0x09c1,0x08A1,0x0521,0x0441,0x02A1,0x0221,0x0141,0x0111,0x0085
		// ,0x0049,0x0025,0x0015,0x0009,0x0005,0x0001,0x5601};

		// int MQcoder::MPS [19]= {0};

		// const unsigned int MQcoder::NMPS [47] = {
		//  1,2,3,4,5,38,7,8,9,10,11
		// ,12,13,29,15
		// ,16,17,18,19
		// ,20,21,22,23
		// ,24,25,26,27
		// ,28,29,30,31
		// ,32,33,34,35
		// ,36,37,38,39
		// ,40,41,42,43
		// ,44,45,45,46 };

		// const unsigned int MQcoder::NLPS [47] = {
		//  1,6,9 ,12
		// ,29,33,6,14
		// ,14,14,17,18
		// ,20,21,14,14
		// ,15,16,17,18
		// ,19,19,20,21
		// ,22,23,24,25
		// ,26,27,28,29
		// ,30,31,32,33
		// ,34,35,36,37
		// ,38,39,40,41
		// ,42,43,46 };

		// const bool MQcoder::SWITCH [47] = {
		//  1,0,0,0
		// ,0,0,1,0
		// ,0,0,0,0
		// ,0,0,1,0
		// ,0,0,0,0
		// ,0,0,0,0
		// ,0,0,0,0
		// ,0,0,0,0
		// ,0,0,0,0
		// ,0,0,0,0
		// ,0,0,0,0
		// ,0,0,0 };

		// The initial index of context for BAC probability estimation lookup table
		/*int MQcoder::INDEX [19] = {
			 4,0
			,0,0,0,0
			,0,0,0,0
			,0,0,0,0
			,0,0,0,3
			,46 };*/


/*		char MQcoder::B   = 0;
		int  MQcoder::CX  = 0;
		bool MQcoder::D   = 0;
		bool MQcoder::EOD = 14;*/

#endif




