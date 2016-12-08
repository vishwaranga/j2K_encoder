/*
 *  File_Format.cpp
 *
 *  created on  :   25/08/2016
 *      author  :   sajith
 *
 */
#include "File_Format.h"

//--------------- constructer--------------------//
File_Format::File_Format(void){

}

//---------------- creating main boxes ----------//
int File_Format::JP2_Signature_box(void){


    push_bytes(4,uint32_t(0x0000000C));     // Lbox
    push_bytes(4,uint32_t(0x6A502020));      // Tbox
    push_bytes(4,uint32_t(0x0D0A870A));      // Dbox

    return 1;
}

int File_Format::Profile_box(void){



    push_bytes(4,uint32_t(0x14));             // Lbox
    push_bytes(4,uint32_t(0x66747970));      // Tbox
    push_bytes(4,uint32_t(0x6A703220));      // Dbox   // Br
    push_bytes(4,uint32_t(0x0));                // MV
    push_bytes(4,uint32_t(0x6A703220));         // CL

    return 1;
}

int File_Format::JP2_Header_box(int height,int width,int bpc){


    push_bytes(4,uint32_t(0x002d));             // Lbox
    push_bytes(4,uint32_t(0x6A703268));      // Tbox

    Img_header_box(height,width,bpc);
    Colour_Spec_box();
    return 1;
}

int File_Format::Img_header_box(int height,int width,int bpc){


    push_bytes(4,uint32_t(0x16));       //Lbox
    push_bytes(4,uint32_t(0x69686472)); // Tbox
    push_bytes(4,uint32_t(height));     //height
    push_bytes(4,uint32_t(width));      //width

    push_bytes(2,uint32_t(0x1));       // C


    uint8_t tmp_b[4];

    tmp_b[0] = uint8_t(bpc-1);        // BCP
    tmp_b[1] = uint8_t(0x07);       // CT
    tmp_b[2] = uint8_t(0x00);       //UC
    tmp_b[3] = uint8_t(0x00);       //IPR

    fwrite(tmp_b,1,4,fp);
    return 1;
}

int File_Format::Colour_Spec_box(void){

    push_bytes(4,uint32_t(0x0F));        // Lbox
    push_bytes(4,uint32_t(0x636F6C72));  // Tbox


    uint8_t tmp_b[3];

    tmp_b[0] = uint8_t(0x01);       // METH
    tmp_b[1] = uint8_t(0x00);       //PREC
    tmp_b[2] = uint8_t(0x00);       //Aprox

    fwrite(tmp_b,1,3,fp);

    push_bytes(4,uint32_t(0x11));        // //Enumes(colour space)


    return 1;
}

int File_Format::Code_Stream_box(queue<int> * hdr_q, queue<uint8_t>  * code_stream_q,queue<pktParamfnl> *qnt_q) ////////////////////////////////////////////(have to write)
{

    push_bytes(4,uint32_t(0x0));                         //Lbox
    push_bytes(4,uint32_t(0x6A703263));                  //Tbox


    SOC();
    SIZ(128,128,8,1,1);              //  width =128 // height =128 // bcp =8// s1 = 1 // s2 = 1
    COD(2,1,1,4,4);                   // Op =2 // delta_t =0 // D_t =1 // E1_cb =4 // E2_cb
    QCD(qnt_q);                     /////////////////////////////// E_b 10  // U_b =0 fill
    SOT();
    SOD();
    for (int i = 0; i < 2; ++i)
    {
       packet(i,hdr_q,code_stream_q);
    }
    EOC();
    return 1;
}
//---------------- marking segment funtions----------//

int File_Format::SOC(void){

    push_bytes(2,uint16_t(0xFF4F));

    return 1;
}

int File_Format::SIZ(int width,int height, int bcp,int s1,int s2){



    push_bytes(2,uint16_t(0xFF51));  // SIZ
    push_bytes(2,uint16_t(0x0029));  // Lsiz
    push_bytes(2,uint16_t(0x0000));  //CA



    push_bytes(4,uint32_t(width));       // F2
    push_bytes(4,uint32_t(height));      // F1
    push_bytes(4,uint32_t(0x0));         // E2
    push_bytes(4,uint32_t(0x0));         // E1
    push_bytes(4,uint32_t(width));       // T2
    push_bytes(4,uint32_t(height));      // T1
    push_bytes(4,uint32_t(0x0));         // omg2
    push_bytes(4,uint32_t(0x0));         // omg1


    push_bytes(2,uint16_t(0x0001));   // C

    uint8_t tmp_b[3];

    tmp_b[0] = uint8_t((bcp-1));        // B     ///////////////////////////////////////////////////////// filll
    tmp_b[1] = uint8_t(s2);         // s2
    tmp_b[2] = uint8_t(s1);         // s1




    fwrite(tmp_b,1,3,fp);

    return 1;
}

int File_Format::COD(int op, int delta_t, int D_t, int E1_cb, int E2_cb){


    push_bytes(2,uint16_t(0xFF52));   // COD
    push_bytes(2,uint16_t(0x000c));   // L cod //////////////////////////// fill


    uint8_t tmp_b[8];

    tmp_b[0] = uint8_t(6);          // CS ////////////////////////////// fill
    tmp_b[1] = uint8_t(op);         // Op

    fwrite(tmp_b,1,2,fp);

    push_bytes(2,uint16_t(delta_t));     // Delta t

    tmp_b[2] = uint8_t(0);          // MC  // no colour transformation
    tmp_b[3] = uint8_t(D_t);        // D_t
    tmp_b[4] = uint8_t(E2_cb);      // E1_cb
    tmp_b[5] = uint8_t(E1_cb);      // E2_cb
    tmp_b[6] = uint8_t(0);          // MS
    tmp_b[7] = uint8_t(0);          // WT /////////////////////////// fill

    fwrite(&tmp_b[2],1,6,fp);
    return 1;
}

int File_Format::QCD(queue<pktParamfnl> *qnt_q){

    int Q = 2;
    int N_b =4;

    uint8_t tmp_b[1];
    uint8_t st_b1[1];
    uint16_t st_b2[4];
    pktParamfnl qnt;
    qnt = qnt_q -> front();
    qnt_q->pop();

    st_b2[0] = (uint16_t(qnt.LLe)<<11) + uint16_t(qnt.LLm);                  //  E_b = 10    U_b = 0
    st_b2[1] = (uint16_t(qnt.HLe)<<11) + uint16_t(qnt.HLm);                   //
    st_b2[2] = (uint16_t(qnt.LHe)<<11) + uint16_t(qnt.LHm);                   //
    st_b2[3] = (uint16_t(qnt.HHe)<<11) + uint16_t(qnt.HHm);                   //

    printf("e = %d         m = %d\n",qnt.LLe,qnt.LLm);
    printf("e = %d         m = %d\n",qnt.HLe,qnt.HLm);
    printf("e = %d         m = %d\n",qnt.LHe,qnt.LHm);
    printf("e = %d         m = %d\n",qnt.HHe,qnt.HHm);

    push_bytes(2,uint16_t(0xFF5C));                      // QCD

    if (Q == 0)                                        // L_qcd
    {
        push_bytes(2,uint16_t(3 + N_b));
    }
    else if (Q == 1)                                 ///
    {
        push_bytes(2,uint16_t(5));
    }
    else if (Q == 2)
    {
        push_bytes(2,uint16_t(3+2*N_b));
    }
    else
    {
        printf("Q is incorrect\n");
        push_bytes(2,uint16_t(3 + N_b));
        return 0;
    }


    tmp_b[0] = uint8_t(0x42);              // G =2  + Q = 2
    fwrite(tmp_b,1,1,fp);

    if ( Q == 0)                       // ST
    {
        fwrite(st_b1,1,N_b,fp);
    }
    else if (Q == 1)
    {
        push_bytes(2,uint16_t(st_b2[0]));
        printf("ST %x\n",st_b2[0] );
    }
    else if (Q == 2)
    {

        for (int i = 0; i < N_b; ++i)
        {
            push_bytes(2,uint16_t(st_b2[i]));
            printf("ST %x\n",st_b2[i] );
        }
    }
    else
    {
        printf("Q is incorrect\n");
        return 0;
    }

    return 1;
}

int File_Format::SOT(void){


    push_bytes(2,uint16_t(0xFF90)); // SOT
    push_bytes(2,uint16_t(0x000A)); //L_sot
    push_bytes(2,uint16_t(0x0000)); // i_t


    push_bytes(4,uint16_t(0x0000)); // L_tp

    uint8_t tmp_b[2];

    tmp_b[0] = uint8_t(0);      // i_tp
    tmp_b[1] = uint8_t(1);      // N_tp

    fwrite(tmp_b,1,2,fp);

    return 1;
}
int File_Format::SOD(void){

    push_bytes(2,uint16_t(0xFF93));

    return 1;
}

int File_Format::EOC(void){

    push_bytes(2,uint16_t(0xFFD9));

    return 1;
}

int File_Format::packet(int pkt_index,queue<int> * hdr_q, queue<uint8_t> * code_stream_q)
{
    int suband_width  = tile_width/(2*DWT_r * 64 );
    int suband_height = tile_height/(2*DWT_r * 64 );

    if (pkt_index == 0)
    {
        int suband_width  = tile_width/(2*DWT_r * 64 );
        int suband_height = tile_height/(2*DWT_r * 64 );

        if (suband_width == 1 && suband_height == 1)
        {
            int zero_bit_planes = hdr_q->front();
            printf("zero_bit_planes = %d\n",zero_bit_planes);
            hdr_q->pop();
            int no_of_coding_passes = hdr_q->front();
            printf("no_of_coding_passes = %d\n",no_of_coding_passes);
            hdr_q->pop();
            int length = hdr_q->front();
            printf("length = %d\n", length);
            hdr_q->pop();

            push_bytes(2,uint16_t(0xFF91));       //SOP
            push_bytes(2,uint16_t(0x0004));       //Lsop
            push_bytes(2,uint16_t(pkt_index));    //packet index


            if (no_of_coding_passes == 0)
            {
                push_bytes(1,0);                      // zero packet
                push_bytes(2,uint16_t(0xFF92));     // EPH
            }
            else
            {
                tmp_b[0] = 0;
                tmp_b[0] = uint8_t(1<<7);            //  none zero packet
                remain_bits = 7;

                send_no_layers(0);

                send_no_of_zero_bitplanes(zero_bit_planes);
                send_no_of_coding_passes(no_of_coding_passes);
                send_Lblock(length,no_of_coding_passes);
                send_length(length);

                if (remain_bits < 8)                // make remain bits zero
                {
                    fwrite(tmp_b,1,1,fp);
                    remain_bits = 8;
                }

                push_bytes(2,uint16_t(0xFF92));     // EPH

                code_block(length,code_stream_q);
            }
        }
        else if (suband_width == 4 && suband_height == 4)
        {
            int no_of_coding_passes[16];
            int empty_pkt_flag = 0;
            int length[16];

            int q[3][4][4];
            int q_u[3][4][4];

            for (int i = 0; i < suband_height * suband_width; ++i)                    //   read hdr q
            {
                q[0][i/suband_width][i%suband_width] = hdr_q->front();
                printf("zero_bit_planes = %d\n",q[0][i/suband_width][i%suband_width]);
                hdr_q->pop();
                no_of_coding_passes[i] = hdr_q->front();
                printf("no_of_coding_passes = %d\n",no_of_coding_passes[i]);
                if (no_of_coding_passes[i] == 0)
                {
                    empty_pkt_flag = 1;
                }
                hdr_q->pop();
                length[i] = hdr_q->front();
                printf("length = %d\n", length[i]);
                hdr_q->pop();
            }

            for (int n = 1; n < 3; ++n)                                              // fill zero bit pale chart
            {
                printf("n = %d\n", n);
                int x_limit = suband_width /(2*(n));
                int y_limit = suband_height /(2*(n));

                for (int y = 0; y < y_limit; ++y)
                {
                    for (int x = 0; x < x_limit; ++x)
                    {
                        q[n][y][x] = min(min(q[n-1][2*y][2*x],q[n-1][2*y][2*x+1]),min(q[n-1][2*y+1][2*x],q[n-1][2*y+1][2*x+1]));

                        printf("%d   ", q[n][y][x] );

                        q_u[n][y][x] = 0;
                    }
                    printf("\n");
                }

               printf("\n\n");
            }

            push_bytes(2,uint16_t(0xFF91));       //SOP
            push_bytes(2,uint16_t(0x0004));       //Lsop
            push_bytes(2,uint16_t(pkt_index));    //packet index

            if (empty_pkt_flag == 1)
            {
                push_bytes(1,0);                      // zero packet
                push_bytes(2,uint16_t(0xFF92));     // EPH
            }
            else
            {
                insert_value(1,1);                 // none zero packet

                for (int i = 0; i < suband_width * suband_height; ++i)                                // sub sample pkt hdrs
                {
                     int y = i / 4;
                     int x = i % 4;

                     if (i == 0)                              // send no of layers and zero bit planes
                         {
                             insert_value(1,1);
                             insert_value(1,1);
                             insert_value(1,1);

                             insert_zeros(q[2][0][0]);                       // zero bit planes
                             insert_zeros(q[1][0][0]);
                             insert_zeros(q[0][0][0]);
                         }
                     else
                     {
                         if ((y%2 == 0)&&(x%2 == 0))
                         {
                             insert_value(1,1);
                             insert_value(1,1);

                             insert_zeros(q[1][y/2][x/2]);                 // zero bit planes
                             insert_zeros(q[0][y][x]);
                         }
                         else
                         {
                             insert_value(1,1);
                             insert_zeros(q[0][y][x]);                    // zero bit planes
                         }
                     }

                     send_no_of_coding_passes(no_of_coding_passes[i]);
                     send_Lblock(length[i],no_of_coding_passes[i]);
                     send_length(length[i]);
                }
                if (remain_bits < 8)                // make remain bits zero
                {
                    fwrite(tmp_b,1,1,fp);
                    remain_bits = 8;
                }

                push_bytes(2,uint16_t(0xFF92));     // EPH

                for (int i = 0; i < 3; ++i)
                {
                    code_block(length[i],code_stream_q);
                }
            }
        }
        else
        {
            printf("error_in subband width and height\n");
        }
    }
    else
    {
        if(pkt_index == 1)
        {
            if (suband_width == 1 && suband_height == 1)
            {
                push_bytes(2,uint16_t(0xFF91));       //SOP
                push_bytes(2,uint16_t(0x0004));       //Lsop
                push_bytes(2,uint16_t(pkt_index));    //packet index

                int zero_bit_planes [3];
                int no_of_coding_passes[3];
                int length [3];

                for (int i = 0; i < 3; ++i)
                {
                    zero_bit_planes[i] = hdr_q->front();
                    printf("zero_bit_planes = %d\n",zero_bit_planes[i]);
                    hdr_q->pop();
                    no_of_coding_passes[i] = hdr_q->front();
                    printf("no_of_coding_passes = %d\n",no_of_coding_passes[i]);
                    hdr_q->pop();
                    length[i] = hdr_q->front();
                    printf("length = %d\n", length[i]);
                    hdr_q->pop();
                }

                if (no_of_coding_passes[0] == 0 )
                {
                    push_bytes(1,0);                      // zero packet
                    push_bytes(2,uint16_t(0xFF92));      // EPH
                }
                else
                {
                    insert_value(1,1);                   // non zero packet

                    for (int i = 0; i < 3; ++i)
                    {
                        send_no_layers(0);
                        send_no_of_zero_bitplanes(zero_bit_planes[i]);
                        send_no_of_coding_passes(no_of_coding_passes[i]);
                        send_Lblock(length[i],no_of_coding_passes[i]);
                        send_length(length[i]);
                    }

                    if (remain_bits < 8)                // make remain bits zero
                    {
                        fwrite(tmp_b,1,1,fp);
                        remain_bits = 8;
                    }

                    push_bytes(2,uint16_t(0xFF92));     // EPH

                    for (int i = 0; i < 3; ++i)
                    {
                        code_block(length[i],code_stream_q);
                    }
                }
            }
            else if (suband_width == 4 && suband_height == 4)
            {
                int length[3][16];

                push_bytes(2,uint16_t(0xFF91));       //SOP
                push_bytes(2,uint16_t(0x0004));       //Lsop
                push_bytes(2,uint16_t(pkt_index));    //packet index

                for (int p = 0; p < 3; ++p)
                {
                    int no_of_coding_passes[16];
                    int empty_pkt_flag = 0;
                    int q[3][4][4];
                    int q_u[3][4][4];

                    for (int i = 0; i < suband_height * suband_width; ++i)                    //   read hdr q
                    {
                        q[0][i/suband_width][i%suband_width] = hdr_q->front();
                        printf("zero_bit_planes = %d\n",q[0][i/suband_width][i%suband_width]);
                        hdr_q->pop();
                        no_of_coding_passes[i] = hdr_q->front();
                        printf("no_of_coding_passes = %d\n",no_of_coding_passes[i]);
                        hdr_q->pop();
                        length[p][i] = hdr_q->front();
                        printf("length = %d\n", length[p][i]);
                        hdr_q->pop();
                    }
                    for (int n = 1; n < 3; ++n)                                              // fill zero bit pale chart
                    {
                        printf("n = %d\n", n);
                        int x_limit = suband_width /(2*(n));
                        int y_limit = suband_height /(2*(n));

                        for (int y = 0; y < y_limit; ++y)
                        {
                            for (int x = 0; x < x_limit; ++x)
                            {
                                q[n][y][x] = min(min(q[n-1][2*y][2*x],q[n-1][2*y][2*x+1]),min(q[n-1][2*y+1][2*x],q[n-1][2*y+1][2*x+1]));

                                printf("%d   ", q[n][y][x] );

                                q_u[n][y][x] = 0;
                            }
                            printf("\n");
                        }
                       printf("\n\n");
                    }
                    if (empty_pkt_flag == 1)
                    {
                        if (p == 0)
                        {
                            insert_value(1,1);                                      // zero packet
                        }
                        break;
                    }
                    else
                    {
                        if (p == 0)
                        {
                            insert_value(1,1);                 // none zero packet
                        }

                        for (int i = 0; i < suband_width * suband_height; ++i)                                // sub sample pkt hdrs
                        {
                             int y = i / 4;
                             int x = i % 4;

                             if (i == 0)                              // send no of layers and zero bit planes
                                 {
                                     insert_value(1,1);
                                     insert_value(1,1);
                                     insert_value(1,1);

                                     insert_zeros(q[2][0][0]);                       // zero bit planes
                                     insert_zeros(q[1][0][0]);
                                     insert_zeros(q[0][0][0]);
                                 }
                             else
                             {
                                 if ((y%2 == 0)&&(x%2 == 0))
                                 {
                                     insert_value(1,1);
                                     insert_value(1,1);

                                     insert_zeros(q[1][y/2][x/2]);                 // zero bit planes
                                     insert_zeros(q[0][y][x]);
                                 }
                                 else
                                 {
                                     insert_value(1,1);
                                     insert_zeros(q[0][y][x]);                    // zero bit planes
                                 }
                             }

                             send_no_of_coding_passes(no_of_coding_passes[i]);
                             send_Lblock(length[p][i],no_of_coding_passes[i]);
                             send_length(length[p][i]);
                        }
                    }

                }

                if (remain_bits < 8)                // make remain bits zero
                {
                    fwrite(tmp_b,1,1,fp);
                    remain_bits = 8;
                }
                push_bytes(2,uint16_t(0xFF92));     // EPH

                for (int i = 0; i < 48; ++i)
                {
                    code_block(length[i/16][i%16],code_stream_q);
                }
            }
            else
            {
                printf( "error in sub band width and height\n");
            }
        }
    }

    return 1;
}

/*int File_Format::subband_pkt_hdr(int i)
{
    int y = i / 4;
    int x = i % 4;

    if (code_block_index == 0)                              // send no of layers and zero bit planes
        {
            insert_value(1,1);
            insert_value(1,1);
            insert_value(1,1);

            insert_zeros(q[2][0][0]);                       // zero bit planes
            insert_zeros(q[1][0][0]);
            insert_zeros(q[0][0][0]);
        }
    else
    {
        if ((y%2 == 0)&&(x%2 == 0))
        {
            insert_value(1,1);
            insert_value(1,1);

            insert_zeros(q[1][y/2][x/2]);                 // zero bit planes
            insert_zeros(q[0][y][x]);
        }
        else
        {
            insert_value(1,1);
            insert_zeros(q[0][y][x]);                    // zero bit planes
        }
    }

    send_no_of_coding_passes(no_of_coding_passes[i]);
    send_Lblock(length[i],no_of_coding_passes[i]);
    send_length(length[i]);
} */
int File_Format::code_block(int length, queue<uint8_t> * code_stream_q){

    for (int i = 0; i < length; ++i)
    {
        if (code_stream_q->empty())
        {
            printf("size_of_the code_stream_q is not match with the length i== %d\n",i);
            return 0;
        }
        else
        {
            tmp_b[0] = code_stream_q->front();
            code_stream_q->pop();
            fwrite(tmp_b,1,1,fp);
            tmp_b[0] = 0;
        }
    }
        return 1;
}
int File_Format::send_no_layers(int no_of_layers)
{
    insert_value(1,1);
    return 1;
}

int File_Format::send_no_of_coding_passes(int no_of_coding_passes)
{
    if (no_of_coding_passes < 6)
    {
        if (no_of_coding_passes == 1)                // # coding passes 1
        {
            insert_value(1,0);
        }
        else if (no_of_coding_passes == 2)           // # coding passes 2
        {
            insert_value(2,2);
        }
        else if (no_of_coding_passes == 3)              // # coding passes 3
        {
            insert_value(4,12);
        }
        else if (no_of_coding_passes == 4)               // # coding passes 4
        {
            insert_value(4,13);
        }
        else if (no_of_coding_passes == 5)                // # coding passes 5
        {
            insert_value(4,14);
        }
    }
    else if ((6<= no_of_coding_passes)&&(no_of_coding_passes <= 36))
    {
        uint8_t offset = uint8_t(no_of_coding_passes-6);

        insert_value(4,15);
        insert_value(5,offset);
    }
    else if ((37<= no_of_coding_passes)&&(no_of_coding_passes <= 164))
    {
            insert_value(9, 0x1ff);
            insert_value(7,(no_of_coding_passes-36));
    }
    else
    {
        printf("error # of coding passes eecees the limite \n");
    }
    return 1;
}

int File_Format::send_no_of_zero_bitplanes(int zero_bit_planes)
{
    printf("zero = %d\n", zero_bit_planes);

    for (int i = 0; i < zero_bit_planes; ++i)
    {
        insert_value(1,0);
    }

    insert_value(1,1);

    /*if ((zero_bit_planes+1) < remain_bits )
    {
        remain_bits -= (zero_bit_planes + 1);
        tmp_b[0] += (1 << remain_bits);
    }
    else if ((zero_bit_planes + 1) == remain_bits)
    {
        remain_bits = 8;
        tmp_b[0] += 1;
        fwrite(tmp_b,1,1,fp);
        tmp_b[0] = 0;
    }
    else
    {
        printf("remain_bits = %d\n",remain_bits );

        fwrite(tmp_b,1,1,fp);
        tmp_b[0] = 0;

        int n = int((zero_bit_planes - remain_bits)/8);

        printf("n = %d \n", n);

        for (int i = 0; i < n; ++i)
        {
            fwrite(tmp_b,1,1,fp);
        }

        n = int ((zero_bit_planes - remain_bits)%8);

        printf("n = %d \n", n);

        remain_bits = 8 - (n+1);
        tmp_b[0] += uint8_t(1<<remain_bits);

        if (remain_bits == 0)
        {
            fwrite(tmp_b,1,1,fp);
            tmp_b[0] = 0;
            remain_bits = 8;
        }
        printf("remain_bits = %d\n",remain_bits );
    } */

    return 1;
}
int File_Format::send_length(int length)
{

    insert_value(int(log2(length)) +1,length);
    return 1;
}
int File_Format::send_Lblock(int length , int no_of_coding_passes)
{
    int bits;
    bits = int(log2 (length)) + 1;

    int Lblock = bits - int(log2(no_of_coding_passes));

    printf("Lblock = %d\n",Lblock );

    int k = Lblock - 3;

    for (int i = 0; i < k; ++i)
    {
        insert_value(1,1);
    }
    insert_value(1,0);

    return 1;
}

int File_Format::insert_value(int bit_cntr, int value)
{


    if (bit_cntr<=8)
    {
        if (bit_cntr < remain_bits)
        {
            remain_bits = remain_bits -bit_cntr;
            tmp_b[0] = tmp_b[0] + uint8_t(value << remain_bits);
        }
        else if (bit_cntr == remain_bits)
        {
           tmp_b[0] = tmp_b[0] + uint8_t(value);
           fwrite(tmp_b,1,1,fp);

           if (tmp_b[0] == 0xff)
           {
            printf("in 0xff \n");
                tmp_b[0] = 0;
                remain_bits = 7;
           }
           else
           {
                tmp_b[0] = 0;
                remain_bits = 8;
           }
        }
        else
        {
            tmp_b[0] = tmp_b[0] + uint8_t(value >> (bit_cntr - remain_bits));
            fwrite(tmp_b,1,1,fp);

            if (tmp_b[0] == 0xff)
           {
            printf("in 0xff \n");
            remain_bits = 7 - (bit_cntr - remain_bits);
            tmp_b[0] = uint8_t(value << remain_bits);
           }
           else
           {
            remain_bits = 8 - (bit_cntr - remain_bits);
            tmp_b[0] = uint8_t(value << remain_bits);
           }
        }

    }
    else if ((8 < bit_cntr)&&( bit_cntr < 16))
    {

        tmp_b[0] = tmp_b[0] + uint8_t(value >> (bit_cntr - remain_bits));
        fwrite(tmp_b,1,1,fp);

        if (tmp_b[0] == 0xff)
        {

            printf("in 0xff \n");
            if ((bit_cntr - remain_bits) < 7)
            {
                remain_bits = 8 - (bit_cntr - remain_bits);
                tmp_b[0] = uint8_t(value << remain_bits);
            }
            else if ((bit_cntr - remain_bits) == 7)
            {
                tmp_b[0] = uint8_t(value);
                fwrite(tmp_b,1,1,fp);

                if (tmp_b[0] == 0xff)
                {
                    printf("in 0xff \n");
                   tmp_b[0] = 0;
                  remain_bits = 7;
                }
                else
                {
                    tmp_b[0] = 0;
                    remain_bits = 8;
                }
            }
            else
            {
                tmp_b[0] = uint8_t(value >> (bit_cntr - remain_bits - 8));
                fwrite(tmp_b,1,1,fp);

                if (tmp_b[0] == 0xff)
                {
                    printf("in 0xff \n");
                    remain_bits = 7 - (bit_cntr - remain_bits - 8);
                    tmp_b[0] = uint8_t(value << remain_bits);
                }
                else
                {
                    remain_bits = 8 - (bit_cntr - remain_bits - 8);
                    tmp_b[0] = uint8_t(value << remain_bits);
                }
            }
        }
        else
        {
            if ((bit_cntr - remain_bits) < 8)
            {
                remain_bits = 8 - (bit_cntr - remain_bits);
                tmp_b[0] = uint8_t(value << remain_bits);
            }
            else if ((bit_cntr - remain_bits) == 8)
            {
                tmp_b[0] = uint8_t(value);
                fwrite(tmp_b,1,1,fp);

                if (tmp_b[0] == 0xff)
                {
                    printf("in 0xff \n");
                   tmp_b[0] = 0;
                  remain_bits = 7;
                }
                else
                {
                    tmp_b[0] = 0;
                    remain_bits = 8;
                }
            }
            else
            {
                tmp_b[0] = uint8_t(value >> (bit_cntr - remain_bits - 8));
                fwrite(tmp_b,1,1,fp);

                if (tmp_b[0] == 0xff)
                {
                    printf("in 0xff \n");
                    remain_bits = 7 - (bit_cntr - remain_bits - 8);
                    tmp_b[0] = uint8_t(value << remain_bits);
                }
                else
                {
                    remain_bits = 8 - (bit_cntr - remain_bits - 8);
                    tmp_b[0] = uint8_t(value << (remain_bits));
                }
            }
        }
    }
    else
    {
        printf("error bit_cntr exceed the limit\n");
        return 0;
    }


    return 1;
}

int File_Format::push_bytes(int byte_cnt, uint32_t value)
{
    for (int i = 1; i <= byte_cnt; ++i)
    {
         tmp_b[0] = uint8_t(value >> 8*(byte_cnt-i));
         fwrite(tmp_b,1,1,fp);
         tmp_b[0] = 0;
    }

    return 1;
}
int File_Format::insert_zeros(int number_of_zeros)
{
    for (int i = 0; i < number_of_zeros; ++i)
    {
        insert_value(1,0);
    }
    insert_value(1,1);

    return 1;
}
//----------------------- make file_____________________//

int File_Format::run(queue<int> *hdr_q,queue<uint8_t> *code_stream_q, queue<pktParamfnl> *qnt_q)
{
    fp = fopen("bmw_128.jp2","wb");
    tmp_b[0] = 0;
    remain_bits = 8;

    tile_width =sbsize*2 ,tile_height = sbsize*2;  DWT_r = 1;     // get_image_width height and DWT r

    JP2_Signature_box();
    Profile_box();
    JP2_Header_box(tile_width,tile_height,8);                   // width =128 // height = 128 //
    Code_Stream_box(hdr_q,code_stream_q,qnt_q);
    fclose(fp);
    return 1;
}
