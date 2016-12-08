#include <iostream>
#include "commonDefs.h"
#include "Quantizer.h"
#include "Wavelet2D.h"
#include "EbcotCoder.h"
#include "MQcoder.h" 
#include "File_Format.h"
using namespace std;
//const char *inputtxt = "image pool/image0.txt";
image imcreate();
int main()
{
    //Wavelet2D *wavelet = new Wavelet2D();
   // Quantizer *quantizer = new Quantizer();
    //EbcotCoder *ebcotcoder = new EbcotCoder();
    //MQcoder *mqcoder = new MQcoder();
    //File_Format *format = new File_Format();
    //image newIm = imcreate();

    int pixel_l = sbsize*2;
    int pixel_w = sbsize*2;
    int levels = 1;
    setPkt pkt;

    pkt.pixel_l = pixel_l;
    pkt.pixel_w = pixel_w;
    pkt.levels = levels;
    queue<setPkt> queue1;
    queue1.push(pkt);
    queue<long> queue2;

  /*  for (int i=0; i<(sbsize*2); i++){
        for (int j=0; j<(sbsize*2); j++){
            queue2.push(newIm.imarray[i][j]);

        }
    } */

    ////////////////////////////////////////////////////////// open image //////////////////////////////////////////////////////////////
    FILE* f = fopen("image2.bmp","r");
    if(f!=NULL){
        cout << "succussfully open file" << endl;
    }
    else {
        cout << "error while opening the file" << endl;
    }

    unsigned char info[54];
    fread(info,sizeof(unsigned char),54,f);            // read the 54 byte header

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    uint16_t bit_per_px = *(int*)&info[28];


    printf("width = %d\n",width);
    printf("height = %d\n",height);
    printf("bit_per_px = %d\n",bit_per_px );

    int no_of_color_planes = bit_per_px/8;

    int size = no_of_color_planes * width * height;

    unsigned char *data = new unsigned char[size];   // allocate 3 byte per pixel


    int t =fread(data,sizeof(unsigned char),size,f);        //read the rest ot data at once
    fclose(f);

    printf("SIZE = %d\n",t);

    for (int i = 0; i < size; i += 3)
    {
        unsigned char tmp = data[i];
        data[i]           = data[i+2];
        data[i+2]         = tmp;
    }


    for (int j = 0; j < 10; j += 3)
    {
        for (int i = 0; i < width; i+= 3 )
        {
            printf("%d:%d:%d\t", data[j*width+i],data[j*width+i+1],data[j*width+i+2]);
        }
        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    queue<coeffPkt> queue3;
    queue<sbandPkt> LL;
    queue<sbandPkt> HL;
    queue<sbandPkt> LH;
    queue<sbandPkt> HH;
    coeffPkt Cpkt;
    queue<context> CONTEXT;
    queue<int> queue4;
    queue<uint8_t> queue5;
    queue<pktParamBPC> param2BPC;
    queue<pktParamfnl> param2fnl;


    //wavelet->run(&queue2, &queue3, &queue1);
    //quantizer->run(&queue3,&LL, &HL, &LH, &HH, &param2BPC, &param2fnl);
    //ebcotcoder->run(&LL, &LH, &HL, &HH, &CONTEXT, &param2BPC); // LL LH HL HH
    //mqcoder->run(&CONTEXT, &queue4, &queue5);
    //format->run(&queue4, &queue5, &param2fnl);//*/
}

image imcreate() {
		//ifstream file;

    FILE* f = fopen("bmwGray1024.bmp","r");
    if(f!=NULL){
        cout << "succussfully open file" << endl;
    }
    else {
        cout << "error while opening the file" << endl;
    }

    unsigned char info[54];
    fread(info,sizeof(unsigned char),54,f);            // read the 54 byte header

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];


    printf("width = \n",width);
    printf("height = \n",height);

    	//image newimage;

    	//int imsize = sbsize*2;

    	//printf("start\n");
    	//file.open ("bmwGray1024.txt");     //image file
    	//if (!file.is_open()) return 0;

    	//int word;
    	//int i = 0;
    	//int j = 0;
    /*	while (file >> word)
    	{
    	    newimage.imarray[i][j] =  (((int)(word))-128);
    	    //cout << newimage.imarray[i][j] << endl;
        	if (j == (imsize-1) && i == (imsize-1)) {
        	    return newimage;
        	}
        	if (j < (imsize-1)) {
        	    j++;
    	    }
    	    else if (j == (imsize-1)) {
    	        j = 0;
    	        i++;
    	    }
    	} */
}
