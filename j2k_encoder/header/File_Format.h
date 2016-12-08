/*
 *  File_Format.h
 *
 *  created on  :   25/08/2016
 *      author  :   sajith
 *
 */
#include "commonDefs.h"

class File_Format
{
    public:
        /*constructor*/
        File_Format(void);

        FILE * fp;
        int remain_bits;
        uint8_t tmp_b[1];

        int tile_width,tile_height, DWT_r;

        /*functions*/
        int JP2_Signature_box(void);
        int Profile_box(void);
        int JP2_Header_box(int height,int width,int bcp);
        int Img_header_box(int height,int width,int bpc);
        int Colour_Spec_box(void);
        int Code_Stream_box(queue<int> * hdr_q, queue<uint8_t>  * code_stream_q,queue<pktParamfnl> *qnt_q);

        int SOC(void);
        int SIZ(int width,int height, int bcp,int s1,int s2);
        int COD(int op, int delta_t, int D_t, int E1_cb, int E2_cb);
        int QCD(queue<pktParamfnl> *qnt_q);
        int SOT(void);
        int SOD(void);

        int EOC(void);

        int packet(int pkt_index,queue<int> * hdr_q, queue<uint8_t> * code_stream_q);
        int code_block(int length, queue<uint8_t> * code_stream_q);
        int send_no_layers(int no_of_layers);
        int send_no_of_coding_passes(int no_of_coding_passes);
        int send_no_of_zero_bitplanes(int zero_bit_planes);
        int send_length( int length);
        int insert_value(int bit_cntr, int value);
        int insert_zeros(int number_of_zeros);
        int send_Lblock(int length , int no_of_coding_passes);
        int push_bytes(int byte_cnt, uint32_t value);
        int run(queue<int> *hdr_q,queue<uint8_t> *code_stream_q,queue<pktParamfnl> *qnt_q);
};
