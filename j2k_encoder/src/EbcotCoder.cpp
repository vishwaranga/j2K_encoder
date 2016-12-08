/*
*   JPEG2000 IP Core development
*   EbcotCoder.cpp
*
*   created : 2016/8/16
*   author  : Randika
*
*/


# include "EbcotCoder.h"

EbcotCoder::EbcotCoder()
{
    //ctor
}

context EbcotCoder::ZeroCoding (int cells[3][3],int signArray[3][3], int subband) {
    context newContext;
    int sigmaH = signArray[1][0] + signArray[1][2];
    int sigmaV = signArray[0][1] + signArray[2][1];
    int sigmaD = signArray[0][0] + signArray[0][2] + signArray[2][0] + signArray[2][2];
    int sigmaHV = sigmaH + sigmaV;

    /*  subband = 0 (LL)
    *   subband = 1 (LH)
    *   subband = 2 (HL)
    *   subband = 3 (HH)
    */

    if (subband == 0 || subband == 1) {                         // LL and LH
        if (sigmaH == 2) {
            newContext.cx = 8;
        }
        else if (sigmaH == 1 && sigmaV >= 1) {
            newContext.cx = 7;
        }
        else if (sigmaH == 1 && sigmaV == 0 && sigmaD >= 1) {
            newContext.cx = 6;
        }
        else if (sigmaH == 1 && sigmaV == 0 && sigmaD == 0) {
            newContext.cx = 5;
        }
        else if (sigmaH == 0 && sigmaV == 2) {
            newContext.cx = 4;
        }
        else if (sigmaH == 0 && sigmaV == 1) {
            newContext.cx = 3;
        }
        else if (sigmaH == 0 && sigmaV == 0 && sigmaD >= 2) {
            newContext.cx = 2;
        }
        else if (sigmaH == 0 && sigmaV == 0 && sigmaD == 1) {
            newContext.cx = 1;
        }
        else if (sigmaH == 0 && sigmaV == 0 && sigmaD == 0) {
            newContext.cx = 0;
        }
        else {
            newContext.cx = -1;
        }
    }

    else if (subband == 2) {                                // HL
        if(sigmaV == 2) {
            newContext.cx = 8;
        }
        else if (sigmaH >= 1 && sigmaV == 1) {
            newContext.cx = 7;
        }
        else if (sigmaH == 0 && sigmaV == 1 && sigmaD >= 1) {
            newContext.cx = 6;
        }
        else if (sigmaH == 0 && sigmaV == 1 && sigmaD == 0) {
            newContext.cx = 5;
        }
        else if (sigmaH == 2 && sigmaV == 0) {
            newContext.cx = 4;
        }
        else if (sigmaH == 1 && sigmaV == 0) {
            newContext.cx = 3;
        }
        else if (sigmaH == 0 && sigmaV == 0 && sigmaD >= 2) {
            newContext.cx = 2;
        }
        else if (sigmaH == 0 && sigmaV == 0 && sigmaD == 1) {
            newContext.cx = 1;
        }
        else if (sigmaH == 0 && sigmaV == 0 && sigmaD == 0) {
            newContext.cx = 0;
        }
        else {
            newContext.cx = -1;
        }
    }

    else if (subband == 3) {                                // HH
        if (sigmaD >= 3) {
            newContext.cx = 8;
        }
        else if (sigmaHV >= 1 && sigmaD == 2) {
            newContext.cx = 7;
        }
        else if (sigmaHV == 0 && sigmaD == 2) {
            newContext.cx = 6;
        }
        else if (sigmaHV >= 2 && sigmaD == 1) {
            newContext.cx = 5;
        }
        else if (sigmaHV == 1 && sigmaD == 1) {
            newContext.cx = 4;
        }
        else if (sigmaHV == 0 && sigmaD == 1) {
            newContext.cx = 3;
        }
        else if (sigmaHV >= 2 && sigmaD == 0) {
            newContext.cx = 2;
        }
        else if (sigmaHV == 1 && sigmaD == 0) {
            newContext.cx = 1;
        }
        else if (sigmaHV == 0 && sigmaD == 0) {
            newContext.cx = 0;
        }
        else {
            newContext.cx = -1;
        }
    }
    newContext.d = cells[1][1];
    //printf("ZC: %d ",newContext.cx);
    return newContext;
}

context EbcotCoder::SignCoding (int cells[3][3], int sigma[3][3], int signArray[3][3]) {
    context newContext;
    int binaryX;                        // takes only binary values. required to create d of context

    // m=1, n=1
    int horizontal = min(1, max(-1, sigma[1][0] * (1-2*signArray[1][0]) + sigma[1][2] * (1-2*signArray[1][2])));
    int vertical = min(1, max(-1, sigma[0][1] * (1-2*signArray[0][1]) + sigma[2][1] * (1-2*signArray[2][1])));

    if (horizontal == 1 && vertical == 1) {
        binaryX = 0;
        newContext.cx = 13;
    }
    else if (horizontal == 1 && vertical == 0) {
        binaryX = 0;
        newContext.cx = 12;
    }
    else if (horizontal == 1 && vertical == -1) {
        binaryX = 0;
        newContext.cx = 11;
    }
    else if (horizontal == 0 && vertical == 1) {
        binaryX = 0;
        newContext.cx = 10;
    }
    else if (horizontal == 0 && vertical == 0) {
        binaryX = 0;
        newContext.cx = 9;
    }
    else if (horizontal == 0 && vertical == -1) {
        binaryX = 1;
        newContext.cx = 10;
    }
    else if (horizontal == -1 && vertical == 1) {
        binaryX = 1;
        newContext.cx = 11;
    }
    else if (horizontal == -1 && vertical == 0) {
        binaryX = 1;
        newContext.cx = 12;
    }
    else if (horizontal == -1 && vertical == -1) {
        binaryX = 1;
        newContext.cx = 13;
    }
    else {
        newContext.cx = -1;
    }
    newContext.d = binaryX != signArray[1][1];
    //printf("SC: %d ",newContext.cx);
    return newContext;
}

context EbcotCoder::MgRefinementCoding (int cells [3][3], int sigma[3][3], int sigmaDash ) {
    context newContext;

    //int sigmaTotal = sigma [0][1] + sigma [2][1] + sigma [1][0] + sigma [1][2];
    int sigmaTotal = sigma [0][1] + sigma [2][1] + sigma [0][0] + sigma [0][2] + sigma [2][0] + sigma [2][2] + sigma [1][0] + sigma [1][2];
    //int sigmaTotal = sigma [0][1] + sigma [2][1] + sigma [0][0] + sigma [0][2] + sigma [2][0] + sigma [2][2];

    if (sigmaDash == 1) {
        newContext.cx = 16;
    }
    else if (sigmaDash == 0 && sigmaTotal >= 1) {
        newContext.cx = 15;
    }
    else if (sigmaDash == 0 && sigmaTotal == 0) {
        newContext.cx = 14;
    }
    else {
        newContext.cx = -1;
    }
    newContext.d = cells [1][1];
    //printf("MAG: %d ",newContext.cx);
    return newContext;
}

RLCcontext EbcotCoder::RunLengthCoding (int cellColumn[4]) {
    RLCcontext newContext;

    if (cellColumn [0] + cellColumn [1] + cellColumn [2] + cellColumn [3] == 0) {
        newContext.cx = 17;
        newContext.d = 0;
    }
    else {
        newContext.cx = 17;
        newContext.d = 1;
        newContext.uniformcx = 18;
        if (cellColumn [0] == 1) {
            newContext.uniformd1 = 0;
            newContext.uniformd2 = 0;
        }
        else if (cellColumn [1] == 1) {
            newContext.uniformd1 = 0;
            newContext.uniformd2 = 1;
        }
        else if (cellColumn [2] == 1) {
            newContext.uniformd1 = 1;
            newContext.uniformd2 = 0;
        }
        else if (cellColumn [3] == 1) {
            newContext.uniformd1 = 1;
            newContext.uniformd2 = 1;
        }
        else {
            newContext.uniformd1 = -1;
            newContext.uniformd2 = -1;
        }
    }
    return newContext;
}

int EbcotCoder::CleanUpPass (int (&bitplane)[bpsize][bpsize], int (&sigmaplane)[bpsize][bpsize], int (&eetaplane)[bpsize][bpsize], int (&signArray)[bpsize][bpsize], queue<context> *localqueue, int &subband) {
    int row = 0;
    int column = 0;
    int slice = 0;
    int sigmaFlag = 0;
    int m,n = 0;
    int rlcPassArray[4] = {};
    int cellArray[3][3] = {};
    int cellSign[3][3] = {};
    int cellSigma[3][3] = {};
    int cellSignVertical[3][3] = {};
    int cellSigmaVertical[3][3] = {};
    int cellSignNormal[3][3] = {};
    int cellSigmaNormal[3][3] = {};
    int counting = 0;
    int no_sc = 1;
    RLCcontext rlcpairs;
    context rlccontext;

    //printf("CUP for plane :\n");

    for (column = 0; column < bpsize ; column ++) {
        for (row = slice*4 ; row < (slice+1)*4 ; row++) {
            //cout << row << "\t" << column << endl;
            no_sc = 1;
            //cout << "CUP CHECK "<< sigmaplane[row][column] << " : " << eetaplane[row][column] << endl;
            if(sigmaplane[row][column] == 0 && eetaplane[row][column] == 0) {
                // preparing the cell array

                cellArray[0][0] = (row - 1 < 0 || column-1 < 0) ? 0 : bitplane[row-1][column-1];
                cellArray[1][0] = (column-1 < 0) ? 0 : bitplane[row][column-1];
                cellArray[2][0] = (row + 1 > bpsize-1 || column-1 < 0) ? 0 : bitplane[row+1][column-1];
                cellArray[0][1] = (row - 1 < 0 ) ? 0 : bitplane[row-1][column];
                cellArray[1][1] = bitplane[row][column];
                cellArray[2][1] = (row + 1 > bpsize-1) ? 0 : bitplane[row+1][column];
                cellArray[0][2] = (row - 1 < 0 || column + 1 > bpsize-1) ? 0 : bitplane[row-1][column+1];
                cellArray[1][2] = (column + 1 > bpsize-1) ? 0 : bitplane[row][column+1];
                cellArray[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1) ? 0 : bitplane[row+1][column+1];

                // preparing the cellSigma - normal mode
                cellSigmaNormal[0][0] = (row - 1 < 0 || column-1 < 0) ? 0 : sigmaplane[row-1][column-1];
                cellSigmaNormal[1][0] = (column-1 < 0) ? 0 : sigmaplane[row][column-1];
                cellSigmaNormal[2][0] = (row + 1 > bpsize-1 || column-1 < 0) ? 0 : sigmaplane[row+1][column-1];
                cellSigmaNormal[0][1] = (row - 1 < 0 ) ? 0 : sigmaplane[row-1][column];
                cellSigmaNormal[1][1] = sigmaplane[row][column];
                cellSigmaNormal[2][1] = (row + 1 > bpsize-1) ? 0 : sigmaplane[row+1][column];
                cellSigmaNormal[0][2] = (row - 1 < 0 || column + 1 > bpsize-1) ? 0 : sigmaplane[row-1][column+1];
                cellSigmaNormal[1][2] = (column + 1 > bpsize-1) ? 0 : sigmaplane[row][column+1];
                cellSigmaNormal[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1) ? 0 : sigmaplane[row+1][column+1];

                // preparing the cellSigma - vertical causal mode
                cellSigmaVertical[0][0] = (row - 1 < 0 || column-1 < 0 || row % 4 == 0) ? 0 : sigmaplane[row-1][column-1];
                cellSigmaVertical[1][0] = (column-1 < 0) ? 0 : sigmaplane[row][column-1];
                cellSigmaVertical[2][0] = (row + 1 > bpsize-1 || column-1 < 0 || (row + 1) % 4 == 0) ? 0 : sigmaplane[row+1][column-1];
                cellSigmaVertical[0][1] = (row - 1 < 0 || row % 4 == 0 ) ? 0 : sigmaplane[row-1][column];
                cellSigmaVertical[1][1] = sigmaplane[row][column];
                cellSigmaVertical[2][1] = (row + 1 > bpsize-1 || (row + 1) % 4 == 0) ? 0 : sigmaplane[row+1][column];
                cellSigmaVertical[0][2] = (row - 1 < 0 || column + 1 > bpsize-1 || row % 4 == 0) ? 0 : sigmaplane[row-1][column+1];
                cellSigmaVertical[1][2] = (column + 1 > bpsize-1) ? 0 : sigmaplane[row][column+1];
                cellSigmaVertical[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1 || (row + 1) % 4 == 0) ? 0 : sigmaplane[row+1][column+1];

                if (verticalModeOn == 1) {
                    for (int i = 0;i<3;i++) {
                        for (int j = 0;j<3;j++) {
                            cellSigma[i][j] = cellSigmaVertical[i][j];
                        }
                    }
                }
                else {
                    for (int i = 0;i<3;i++) {
                        for (int j = 0;j<3;j++) {
                            cellSigma[i][j] = cellSigmaNormal[i][j];
                        }
                    }
                }
                /* sigmaFlag calculation */
                sigmaFlag = 0;
                for(m = row-1; m <= row + 4; m++) {
                    for (n = column-1; n<= column+1; n++) {
                        if(m >= 0 && n >= 0 && m < bpsize && n < bpsize) {
                            sigmaFlag = sigmaFlag + sigmaplane[m][n];
                            //cout << "sigmaPlane :" << sigmaplane[m][n] << "\t" << m << n << endl;
                        }
                    }
                }
                // check for run kength coding
                //cout << "RLC CHECK "<< row % 4 << " : " << sigmaFlag << endl;
                if (row % 4 == 0 && sigmaFlag == 0) {
                    //cout << "RLC.." << endl;
                    // apply run length coding : should pass 4 consecutive bits in the column
                    rlcPassArray[0] = bitplane[row][column];
                    rlcPassArray[1] = bitplane[row + 1][column];
                    rlcPassArray[2] = bitplane[row + 2][column];
                    rlcPassArray[3] = bitplane[row + 3][column];
                    // call run length coding function
                    rlcpairs = RunLengthCoding (rlcPassArray);
                    // push the first pair
                    rlccontext.cx = rlcpairs.cx;
                    rlccontext.d = rlcpairs.d;
                    localqueue -> push(rlccontext);

                    // calculate next bit position
                    if (rlcpairs.d == 0) {
                        row = row + 3;
                        no_sc = 0;
                        //printf("skipping\n");
                    }
                    else {
                        row = row + (rlcpairs.uniformd1*2 + rlcpairs.uniformd2 );
                        rlccontext.cx = rlcpairs.uniformcx;
                        rlccontext.d = rlcpairs.uniformd1;
                        localqueue -> push(rlccontext);

                        rlccontext.d = rlcpairs.uniformd2;
                        localqueue -> push(rlccontext);
                    }
                }
                else {
                    localqueue -> push(ZeroCoding(cellArray,cellSigma, subband));
                    //printf("zero coding at : (%d,%d)\n",row,column);
                }
                if (bitplane[row][column] == 1 && no_sc) {
                    // preparing the cellSign - normal mode
                        cellSignNormal[0][0] = (row - 1 < 0 || column-1 < 0) ? 0 : signArray[row-1][column-1];
                        cellSignNormal[1][0] = (column-1 < 0) ? 0 : signArray[row][column-1];
                        cellSignNormal[2][0] = (row + 1 > bpsize-1 || column-1 < 0) ? 0 : signArray[row+1][column-1];
                        cellSignNormal[0][1] = (row - 1 < 0 ) ? 0 : signArray[row-1][column];
                        cellSignNormal[1][1] = signArray[row][column];
                        cellSignNormal[2][1] = (row + 1 > bpsize-1) ? 0 : signArray[row+1][column];
                        cellSignNormal[0][2] = (row - 1 < 0 || column + 1 > bpsize-1) ? 0 : signArray[row-1][column+1];
                        cellSignNormal[1][2] = (column + 1 > bpsize-1) ? 0 : signArray[row][column+1];
                        cellSignNormal[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1) ? 0 : signArray[row+1][column+1];

                        // preparing the cellSign - vertical causal mode
                        cellSignVertical[0][0] = (row - 1 < 0 || column-1 < 0 || row % 4 == 0) ? 0 : signArray[row-1][column-1];
                        cellSignVertical[1][0] = (column-1 < 0) ? 0 : signArray[row][column-1];
                        cellSignVertical[2][0] = (row + 1 > bpsize-1 || column-1 < 0 || (row +1) % 4 == 0) ? 0 : signArray[row+1][column-1];
                        cellSignVertical[0][1] = (row - 1 < 0 || row % 4 == 0) ? 0 : signArray[row-1][column];
                        cellSignVertical[1][1] = signArray[row][column];
                        cellSignVertical[2][1] = (row + 1 > bpsize-1 || (row +1) % 4 == 0) ? 0 : signArray[row+1][column];
                        cellSignVertical[0][2] = (row - 1 < 0 || column + 1 > bpsize-1 || row % 4 == 0) ? 0 : signArray[row-1][column+1];
                        cellSignVertical[1][2] = (column + 1 > bpsize-1) ? 0 : signArray[row][column+1];
                        cellSignVertical[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1 || (row +1) % 4 == 0) ? 0 : signArray[row+1][column+1];

                        if (verticalModeOn == 1) {
                            for (int i = 0;i<3;i++) {
                                for (int j = 0;j<3;j++) {
                                    cellSign[i][j] = cellSignVertical[i][j];
                                }
                            }
                        }
                        else {
                            for (int i = 0;i<3;i++) {
                                for (int j = 0;j<3;j++) {
                                    cellSign[i][j] = cellSignNormal[i][j];
                                }
                            }
                        }

                    // apply sign coding
                    localqueue -> push(SignCoding(cellArray,cellSigma,cellSign));
                    sigmaplane[row][column] = 1;
                    //printf("Sign coding at : (%d,%d)\n",row,column);
                }
                else {
                    if (row >= bpsize - 1 && column >= bpsize -1) {
                        return 0;
                    }
                }
            }
            if (row == bpsize - 1 && column == bpsize -1) {
                return 0;
            }
        }
        if (column == bpsize - 1) {
            slice++;
            column = -1;
        }
    }
}

int EbcotCoder::SPropagationPass (int (&bitplane)[bpsize][bpsize], int (&sigmaplane)[bpsize][bpsize], int (&eetaplane)[bpsize][bpsize], int (&signArray)[bpsize][bpsize], queue<context> *localqueue, int &subband) {
    int cellSign[3][3] = {};
    int cellSigma[3][3] = {};
    int cellArray[3][3] = {};
    int cellSignVertical[3][3] = {};
    int cellSigmaVertical[3][3] = {};
    int cellSignNormal[3][3] = {};
    int cellSigmaNormal[3][3] = {};

    int column = 0;
    int row = 0;
    int slice = 0;
    int prefNeighbourhood = 0;

    // int counter=0;
    for (column = 0; column < bpsize ; column ++) {
        for (row = slice*4 ; row < (slice+1)*4 ; row++) {
            if(sigmaplane[row][column] == 0) {
                // preparing the cell array
                cellArray[0][0] = (row - 1 < 0 || column-1 < 0) ? 0 : bitplane[row-1][column-1];
                cellArray[1][0] = (column-1 < 0) ? 0 : bitplane[row][column-1];
                cellArray[2][0] = (row + 1 > bpsize-1 || column-1 < 0) ? 0 : bitplane[row+1][column-1];
                cellArray[0][1] = (row - 1 < 0 ) ? 0 : bitplane[row-1][column];
                cellArray[1][1] = bitplane[row][column];
                cellArray[2][1] = (row + 1 > bpsize-1) ? 0 : bitplane[row+1][column];
                cellArray[0][2] = (row - 1 < 0 || column + 1 > bpsize-1) ? 0 : bitplane[row-1][column+1];
                cellArray[1][2] = (column + 1 > bpsize-1) ? 0 : bitplane[row][column+1];
                cellArray[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1) ? 0 : bitplane[row+1][column+1];

                // preparing the cellSigma - normal mode
                cellSigmaNormal[0][0] = (row - 1 < 0 || column-1 < 0) ? 0 : sigmaplane[row-1][column-1];
                cellSigmaNormal[1][0] = (column-1 < 0) ? 0 : sigmaplane[row][column-1];
                cellSigmaNormal[2][0] = (row + 1 > bpsize-1 || column-1 < 0) ? 0 : sigmaplane[row+1][column-1];
                cellSigmaNormal[0][1] = (row - 1 < 0 ) ? 0 : sigmaplane[row-1][column];
                cellSigmaNormal[1][1] = sigmaplane[row][column];
                cellSigmaNormal[2][1] = (row + 1 > bpsize-1) ? 0 : sigmaplane[row+1][column];
                cellSigmaNormal[0][2] = (row - 1 < 0 || column + 1 > bpsize-1) ? 0 : sigmaplane[row-1][column+1];
                cellSigmaNormal[1][2] = (column + 1 > bpsize-1) ? 0 : sigmaplane[row][column+1];
                cellSigmaNormal[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1) ? 0 : sigmaplane[row+1][column+1];

                // preparing the cellSigma - vertical causal mode
                cellSigmaVertical[0][0] = (row - 1 < 0 || column-1 < 0 || row % 4 == 0) ? 0 : sigmaplane[row-1][column-1];
                cellSigmaVertical[1][0] = (column-1 < 0) ? 0 : sigmaplane[row][column-1];
                cellSigmaVertical[2][0] = (row + 1 > bpsize-1 || column-1 < 0 || (row + 1) % 4 == 0) ? 0 : sigmaplane[row+1][column-1];
                cellSigmaVertical[0][1] = (row - 1 < 0 || row % 4 == 0 ) ? 0 : sigmaplane[row-1][column];
                cellSigmaVertical[1][1] = sigmaplane[row][column];
                cellSigmaVertical[2][1] = (row + 1 > bpsize-1 || (row + 1) % 4 == 0) ? 0 : sigmaplane[row+1][column];
                cellSigmaVertical[0][2] = (row - 1 < 0 || column + 1 > bpsize-1 || row % 4 == 0) ? 0 : sigmaplane[row-1][column+1];
                cellSigmaVertical[1][2] = (column + 1 > bpsize-1) ? 0 : sigmaplane[row][column+1];
                cellSigmaVertical[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1 || (row + 1) % 4 == 0) ? 0 : sigmaplane[row+1][column+1];

                if (verticalModeOn == 1) {
                    for (int i = 0;i<3;i++) {
                        for (int j = 0;j<3;j++) {
                            cellSigma[i][j] = cellSigmaVertical[i][j];
                        }
                    }
                }
                else {
                    for (int i = 0;i<3;i++) {
                        for (int j = 0;j<3;j++) {
                            cellSigma[i][j] = cellSigmaNormal[i][j];
                        }
                    }
                }

                // preferred neighbourhood calculation
                prefNeighbourhood = 0;
                prefNeighbourhood = ((row - 1 < 0 || column-1 < 0) ? 0 : sigmaplane[row-1][column-1]) + ((column-1 < 0) ? 0 : sigmaplane[row][column-1]) + ((row + 1 > bpsize-1 || column-1 < 0) ? 0 : sigmaplane[row+1][column-1]) + ((row - 1 < 0 ) ? 0 : sigmaplane[row-1][column]) + ((row + 1 > bpsize-1) ? 0 : sigmaplane[row+1][column]) + ((row - 1 < 0 || column + 1 > bpsize-1) ? 0 : sigmaplane[row-1][column+1]) + ((column + 1 > bpsize-1) ? 0 : sigmaplane[row][column+1]) + ((row + 1 > bpsize-1 || column + 1 > bpsize-1) ? 0 : sigmaplane[row+1][column+1]);
                // cout << "pref Neighbourhood :" << prefNeighbourhood << "-" << row << column << endl;
                if(prefNeighbourhood >= 1) {
                    // apply zero coding
                    //printf("%d ",cellArray[1][1]);
                    localqueue -> push(ZeroCoding(cellArray,cellSigma, subband));
                    eetaplane[row][column] = 1;
                    /************************************************************************************************/
                    // if (localqueue -> size() != 0) {
                    //     context newcontext;
                    //     newcontext = localqueue -> front();
                    //     localqueue -> pop();
                    //    cout << "ZC : (" << row << "," << column << ") :"<< newcontext.cx << "\t" << newcontext.d << endl;
                    // }
                    /************************************************************************************************/

                    if (bitplane[row][column] == 1) {

                        // preparing the cellSign - normal mode
                        cellSignNormal[0][0] = (row - 1 < 0 || column-1 < 0) ? 0 : signArray[row-1][column-1];
                        cellSignNormal[1][0] = (column-1 < 0) ? 0 : signArray[row][column-1];
                        cellSignNormal[2][0] = (row + 1 > bpsize-1 || column-1 < 0) ? 0 : signArray[row+1][column-1];
                        cellSignNormal[0][1] = (row - 1 < 0 ) ? 0 : signArray[row-1][column];
                        cellSignNormal[1][1] = signArray[row][column];
                        cellSignNormal[2][1] = (row + 1 > bpsize-1) ? 0 : signArray[row+1][column];
                        cellSignNormal[0][2] = (row - 1 < 0 || column + 1 > bpsize-1) ? 0 : signArray[row-1][column+1];
                        cellSignNormal[1][2] = (column + 1 > bpsize-1) ? 0 : signArray[row][column+1];
                        cellSignNormal[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1) ? 0 : signArray[row+1][column+1];

                        // preparing the cellSign - vertical causal mode
                        cellSignVertical[0][0] = (row - 1 < 0 || column-1 < 0 || row % 4 == 0) ? 0 : signArray[row-1][column-1];
                        cellSignVertical[1][0] = (column-1 < 0) ? 0 : signArray[row][column-1];
                        cellSignVertical[2][0] = (row + 1 > bpsize-1 || column-1 < 0 || (row +1) % 4 == 0) ? 0 : signArray[row+1][column-1];
                        cellSignVertical[0][1] = (row - 1 < 0 || row % 4 == 0) ? 0 : signArray[row-1][column];
                        cellSignVertical[1][1] = signArray[row][column];
                        cellSignVertical[2][1] = (row + 1 > bpsize-1 || (row +1) % 4 == 0) ? 0 : signArray[row+1][column];
                        cellSignVertical[0][2] = (row - 1 < 0 || column + 1 > bpsize-1 || row % 4 == 0) ? 0 : signArray[row-1][column+1];
                        cellSignVertical[1][2] = (column + 1 > bpsize-1) ? 0 : signArray[row][column+1];
                        cellSignVertical[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1 || (row +1) % 4 == 0) ? 0 : signArray[row+1][column+1];

                        if (verticalModeOn == 1) {
                            for (int i = 0;i<3;i++) {
                                for (int j = 0;j<3;j++) {
                                    cellSign[i][j] = cellSignVertical[i][j];
                                }
                            }
                        }
                        else {
                            for (int i = 0;i<3;i++) {
                                for (int j = 0;j<3;j++) {
                                    cellSign[i][j] = cellSignNormal[i][j];
                                }
                            }
                        }

                        // apply sign coding
                        localqueue -> push(SignCoding(cellArray,cellSigma,cellSign));
                        sigmaplane[row][column] = 1;
                        //printf("Sign coding at : (%d,%d)\n",row,column);
                        /************************************************************************************************/
                        // if (localqueue -> size() != 0) {
                        //     context newcontext;
                        //     newcontext = localqueue -> front();
                        //     localqueue -> pop();
                        //    cout << "SC : (" << row << "," << column << ") :"<< newcontext.cx << "\t" << newcontext.d << endl;
                        // }
                        /************************************************************************************************/
                    }
                }
            }
            if (row >= bpsize - 1 && column >= bpsize - 1) {
                return 0;
            }
        }
        if (column == bpsize - 1) {
            slice++;
            column = -1;
        }
    }
}

int EbcotCoder::MagnitudeRefPass(int (&bitplane)[bpsize][bpsize], int (&sigmaplane)[bpsize][bpsize], int (&eetaplane)[bpsize][bpsize], int (&sigmaDash)[bpsize][bpsize], queue<context> *localqueue) {

    context newcontext;
    int cellArray[3][3] = {};
    int cellSigma[3][3] = {};
    int cellSigmaVertical[3][3] = {};
    int cellSigmaNormal[3][3] = {};

    int row = 0;
    int column = 0;
    int slice = 0;

    for (column = 0; column < bpsize ; column ++) {
        for (row = slice*4 ; row < (slice+1)*4 ; row++) {
            if (sigmaplane[row][column] == 1 && eetaplane[row][column] == 0) {
                // preparing the cell array
                cellArray[0][0] = (row - 1 < 0 || column-1 < 0) ? 0 : bitplane[row-1][column-1];
                cellArray[1][0] = (column-1 < 0) ? 0 : bitplane[row][column-1];
                cellArray[2][0] = (row + 1 > bpsize-1 || column-1 < 0) ? 0 : bitplane[row+1][column-1];
                cellArray[0][1] = (row - 1 < 0 ) ? 0 : bitplane[row-1][column];
                cellArray[1][1] = bitplane[row][column];
                cellArray[2][1] = (row + 1 > bpsize-1) ? 0 : bitplane[row+1][column];
                cellArray[0][2] = (row - 1 < 0 || column + 1 > bpsize-1) ? 0 : bitplane[row-1][column+1];
                cellArray[1][2] = (column + 1 > bpsize-1) ? 0 : bitplane[row][column+1];
                cellArray[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1) ? 0 : bitplane[row+1][column+1];

                // preparing the cellSigma - normal mode
                cellSigmaNormal[0][0] = (row - 1 < 0 || column-1 < 0) ? 0 : sigmaplane[row-1][column-1];
                cellSigmaNormal[1][0] = (column-1 < 0) ? 0 : sigmaplane[row][column-1];
                cellSigmaNormal[2][0] = (row + 1 > bpsize-1 || column-1 < 0) ? 0 : sigmaplane[row+1][column-1];
                cellSigmaNormal[0][1] = (row - 1 < 0 ) ? 0 : sigmaplane[row-1][column];
                cellSigmaNormal[1][1] = sigmaplane[row][column];
                cellSigmaNormal[2][1] = (row + 1 > bpsize-1) ? 0 : sigmaplane[row+1][column];
                cellSigmaNormal[0][2] = (row - 1 < 0 || column + 1 > bpsize-1) ? 0 : sigmaplane[row-1][column+1];
                cellSigmaNormal[1][2] = (column + 1 > bpsize-1) ? 0 : sigmaplane[row][column+1];
                cellSigmaNormal[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1) ? 0 : sigmaplane[row+1][column+1];

                // preparing the cellSigma - vertical causal mode
                cellSigmaVertical[0][0] = (row - 1 < 0 || column-1 < 0 || row % 4 == 0) ? 0 : sigmaplane[row-1][column-1];
                cellSigmaVertical[1][0] = (column-1 < 0) ? 0 : sigmaplane[row][column-1];
                cellSigmaVertical[2][0] = (row + 1 > bpsize-1 || column-1 < 0 || (row + 1) % 4 == 0) ? 0 : sigmaplane[row+1][column-1];
                cellSigmaVertical[0][1] = (row - 1 < 0 || row % 4 == 0 ) ? 0 : sigmaplane[row-1][column];
                cellSigmaVertical[1][1] = sigmaplane[row][column];
                cellSigmaVertical[2][1] = (row + 1 > bpsize-1 || (row + 1) % 4 == 0) ? 0 : sigmaplane[row+1][column];
                cellSigmaVertical[0][2] = (row - 1 < 0 || column + 1 > bpsize-1 || row % 4 == 0) ? 0 : sigmaplane[row-1][column+1];
                cellSigmaVertical[1][2] = (column + 1 > bpsize-1) ? 0 : sigmaplane[row][column+1];
                cellSigmaVertical[2][2] = (row + 1 > bpsize-1 || column + 1 > bpsize-1 || (row + 1) % 4 == 0) ? 0 : sigmaplane[row+1][column+1];

                if (verticalModeOn == 1) {
                    for (int i = 0;i<3;i++) {
                        for (int j = 0;j<3;j++) {
                            cellSigma[i][j] = cellSigmaVertical[i][j];
                        }
                    }
                }
                else {
                    for (int i = 0;i<3;i++) {
                        for (int j = 0;j<3;j++) {
                            cellSigma[i][j] = cellSigmaNormal[i][j];
                        }
                    }
                }

                // apply magnitude refinement pass
                localqueue -> push(MgRefinementCoding(cellArray, cellSigma, sigmaDash[row][column]));
                sigmaDash[row][column] = 1;
                /************************************************************************************************/
                // if (localqueue -> size() != 0) {
                //     context newcontext;
                //     newcontext = localqueue -> front();
                //     localqueue -> pop();
                //    cout << "MRC : (" << row << "," << column << ") :"<< newcontext.cx << "\t" << newcontext.d << endl;
                // }
                /************************************************************************************************/
            }
            if (row >= bpsize - 1 && column >= bpsize - 1) {
                return 0;
            }
        }
        if (column >= bpsize - 1) {
            slice++;
            column = -1;
        }
    }
}

int EbcotCoder::BitPlaneSeparator(int (&codeblock)[bpsize][bpsize], int (&bitplanes)[maxbitplanes][bpsize][bpsize], int (&signArray)[bpsize][bpsize]) {
    int row = 0;
    int column = 0;
    int cellValue = 0;
    int plane = 0;
    int maxplane = 0;
    int cellvalmax = 0;
    queue<int> binaryQ;

    for (row = 0; row < bpsize; row ++) {
        for (column = 0; column < bpsize; column ++) {
            cellValue = abs(codeblock[row][column]);
            //printf("%d ",codeblock[row][column]);
            if (cellvalmax<cellValue) cellvalmax = cellValue;
            plane = 0;
            while (abs(cellValue) > 1) {
                binaryQ.push(abs(cellValue) % 2);
                cellValue = abs(cellValue) / 2;
            }
            binaryQ.push(abs(cellValue));
            if (maxplane < binaryQ.size()) {
                maxplane = binaryQ.size();
            }
            while (binaryQ.size() != 0) {
                bitplanes[plane][row][column] = binaryQ.front();
                binaryQ.pop();
                plane ++;
            }
            if (codeblock[row][column] < 0) {
                signArray[row][column] = 1;
            }
            else {
                signArray[row][column] = 0;
            }
        }
        //printf("\n");
    }
    // cout << "Number of non zero bit planes :" << maxplane << endl;
    printf("maxCellVal: %d\n",cellvalmax);
    return maxplane;
}

int EbcotCoder::CodeBlockCoder(int (&codeblock)[bpsize][bpsize], int subband, queue<context> *localqueue, int &codingPassCount, int maxplanes) {
    int sigmaplane[bpsize][bpsize] = {};
    int sigmadashplane[bpsize][bpsize] = {};
    int eetaplane[bpsize][bpsize] = {};
    int signArray[bpsize][bpsize] = {};
    int bitplanearray[maxbitplanes][bpsize][bpsize] = {};
    int bitplane[bpsize][bpsize] = {};

    int bitplaneCount = 0;
    int zerobp = 0;
    int localQsize = 0;
    int row = 0;
    int col = 0;
    int v;
    int flag = 0;
    int storecount = 0;

    int initrow = 0;
    int initcol = 0;

    codingPassCount =  0;
    bitplaneCount = BitPlaneSeparator(codeblock, bitplanearray, signArray) - 1;

    for (initrow = 0; initrow < bpsize; initrow++) {
        for (initcol = 0; initcol < bpsize; initcol++) {
            sigmaplane[initrow][initcol] = 0;
            sigmadashplane[initrow][initcol] = 0;
            eetaplane[initrow][initcol] = 0;
        }
    }


    zerobp = (maxplanes + 2 - 1) - (bitplaneCount + 1);
    storecount = bitplaneCount ;
    cout << "bpcount :" << bitplaneCount + 1<< " : "<< maxplanes << endl;
    while (1) {
        // creating 2D array bit plane
        for (row = 0; row < bpsize; row ++) {
            for (col = 0; col < bpsize; col ++) {
                bitplane[row][col] = (abs(codeblock[row][col]) & (1 << (bitplaneCount))? 1:0);
                //printf("%d ",bitplane[row][col]);
            }
            //printf("\n");
        }
        //printf("\n");
        localQsize = localqueue -> size();
       // if (bitplaneCount >= 0) {
        //printf("\nCLNPASS START : %d\n",bitplaneCount);
            CleanUpPass(bitplane, sigmaplane, eetaplane , signArray, localqueue, subband);
            if (localQsize != localqueue -> size()) {
                localQsize = localqueue -> size();
                //codingPassCount ++;
                //cout << "CUP :" << "passCount" << ":" << codingPassCount << endl;
            }
            else {
                flag = 1;
            }
        //printf("\nCLNPASS END : %d\n",bitplaneCount);
       // }
        if (bitplaneCount != 0) {
            bitplaneCount --;
            // creating 2D array bit plane
            for (row = 0; row < bpsize; row ++) {
                for (col = 0; col < bpsize; col ++) {
                    bitplane[row][col] = (abs(codeblock[row][col]) & (1 << (bitplaneCount))? 1:0);
                    eetaplane[row][col] = 0;
                }
            }
            //printf("\nSIGPASS START : %d\n",bitplaneCount);
            SPropagationPass(bitplane, sigmaplane, eetaplane, signArray, localqueue, subband);
            if (localQsize != localqueue -> size()) {
                localQsize = localqueue -> size();
                //codingPassCount ++;

            }
            //printf("\nSIGPASS END : %d\n",bitplaneCount);
            //printf("\nREFPASS START : %d\n",bitplaneCount);
            MagnitudeRefPass(bitplane, sigmaplane, eetaplane, sigmadashplane, localqueue);
            if (localQsize != localqueue -> size()) {
                localQsize = localqueue -> size();
                //codingPassCount ++;
            }
            //printf("\nREFPASS END : %d\n",bitplaneCount);
        }
        else {
            codingPassCount = storecount*3+1 - flag;
            cout << "codingPassCount:" << codingPassCount << endl;
            break;
        }
    }
    return zerobp;
}

int EbcotCoder::cbloader (queue <sbandPkt> *llblock, queue <sbandPkt> *lhblock, queue <sbandPkt> *hlblock, queue <sbandPkt> *hhblock,queue <cblock> *llblockcb, queue <cblock> *lhblockcb, queue <cblock> *hlblockcb, queue <cblock> *hhblockcb) {

    /* ADD THIS TO PACKET DEFS FILE */
    /*  struct cblock{
        signed int      array[64][64];
        };                          */

    int codeblock[bpsize][bpsize] = {};
    int row = 0;
    int col = 0;
    int rowinit = 0;
    int colinit = 0;
    int cbcount = 0;
    sbandPkt subbandll;
    sbandPkt subbandlh;
    sbandPkt subbandhl;
    sbandPkt subbandhh;
    cblock cbll;
    cblock cblh;
    cblock cbhl;
    cblock cbhh;

    int sbwidth = 64;
    int sbheight = 64;
    int istart, iend, jstart, jend = 0;
    int lldone = 0;

    subbandll = llblock -> front();
    subbandlh = lhblock -> front();
    subbandhl = hlblock -> front();
    subbandhh = hhblock -> front();

    for (int counth = 0; counth < (sbheight/bpsize); counth ++) {
        for (int countw = 0; countw < (sbwidth/bpsize); countw ++) {

            row = rowinit;
            for (int i = 0; i < bpsize; i++) {
                col = colinit;
                for (int j = 0; j < bpsize; j++) {
                    printf("(%d,%d) - (%d,%d)\n",i,j,row,col);
                    cbll.array[i][j] = subbandll.sband[row][col];
                    cblh.array[i][j] = subbandlh.sband[row][col];
                    cbhl.array[i][j] = subbandhl.sband[row][col];
                    cbhh.array[i][j] = subbandhh.sband[row][col];
                    col++;
                }
                row++;
            }
            llblockcb -> push(cbll);
            lhblockcb -> push(cblh);
            hlblockcb -> push(cbhl);
            hhblockcb -> push(cbhh);

            rowinit = (colinit + bpsize == sbwidth ) ? row:row-bpsize;
            colinit = (colinit + bpsize == sbwidth ) ? 0 : col;

        }
    }
    return 0;
}

int EbcotCoder::run (queue <sbandPkt> *llblock, queue <sbandPkt> *lhblock, queue <sbandPkt> *hlblock, queue <sbandPkt> *hhblock, queue <context> *contextQ, queue<pktParamBPC> *param2BPC) {
    int codeblock[bpsize][bpsize] = {};
    int allBlocksDone = 0;
    int zerobitplanes = 0;
    int llblockdone = 0;
    int lhblockdone = 0;
    int hlblockdone = 0;
    int hhblockdone = 0;
    int codingPassCount = 0;

    int epsilonll = 0;
    int epsilonhl = 0;
    int epsilonlh = 0;
    int epsilonhh = 0;

    sbandPkt subband;
    cblock block;
    context cbseperator;
    pktParamBPC epsilon;

    queue <cblock> llblockcb;
    queue <cblock> lhblockcb;
    queue <cblock> hlblockcb;
    queue <cblock> hhblockcb;
    cout << "inside run" << endl;

    cbloader (llblock, lhblock, hlblock, hhblock, &llblockcb, &lhblockcb, &hlblockcb, &hhblockcb);

    if (param2BPC -> size () !=0) {
        epsilon = param2BPC -> front();
        param2BPC -> pop();
        epsilonll = epsilon.LLe;
        epsilonhl = epsilon.HLe;
        epsilonlh = epsilon.LHe;
        epsilonhh = epsilon.HHe;
    }

    //while (!allBlocksDone) {
        while (llblockcb.size () != 0) {
            block = llblockcb.front();
            zerobitplanes = CodeBlockCoder( block.array ,0 , contextQ, codingPassCount, epsilonll);
            cbseperator.cx = zerobitplanes;
            cbseperator.d = - (codingPassCount + 2);
            contextQ -> push(cbseperator);                                      // signaling end of a codeblock
            llblockcb.pop();
            llblockdone = 1;
        }
        while (hlblockcb.size () != 0) {
            block = hlblockcb.front();
            zerobitplanes = CodeBlockCoder( block.array ,2 , contextQ, codingPassCount, epsilonhl);
            cbseperator.cx = zerobitplanes;
            cbseperator.d = - (codingPassCount + 1);
            contextQ -> push(cbseperator);                                      // signaling end of a codeblock
            hlblockcb.pop();
            hlblockdone = 1;
        }
        while (lhblockcb.size () != 0) {
            block = lhblockcb.front();
            zerobitplanes = CodeBlockCoder( block.array ,1 , contextQ, codingPassCount, epsilonlh);
            cbseperator.cx = zerobitplanes;
            cbseperator.d = - (codingPassCount + 1);
            contextQ -> push(cbseperator);                                      // signaling end of a codeblock
            lhblockcb.pop();
            lhblockdone = 1;
        }
        while (hhblockcb.size () != 0) {
            block = hhblockcb.front();
            zerobitplanes = CodeBlockCoder( block.array ,3 , contextQ, codingPassCount, epsilonhh);
            cbseperator.cx = zerobitplanes;
            cbseperator.d = - (codingPassCount + 1);
            contextQ -> push(cbseperator);                                      // signaling end of a codeblock
            hhblockcb.pop();
            hhblockdone = 1;
        }
        //allBlocksDone = llblockdone && lhblockdone && hlblockdone && hhblockdone;
    //}
    return 0;
}
