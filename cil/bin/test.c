#include "test.h"

#define NUM 0x00000001

unint32 G_tmsDataBuf[2][256];
unint32 G_tmsTestBuf[256];
unint32 G_cntTmsSend;         
unint32 G_tmsPingPong;        

unint32 G_bTmsPack;
unint32 G_cntTmsInterrupt;


extern void B_IntTms(void);
extern void B_IemsTask(void);
extern void B_SendTmsData(void);
extern void B_TmPack(void);

void B_IntTms(void)
{
    if (G_bTmsPack == NUM)
    {
        G_cntTmsInterrupt = 0;
    }

    G_cntTmsSend = 36; 

    G_tmsPingPong = (G_tmsPingPong ^ 0x1) & 0x1; 

    ( *((volatile int *)(0x11000000+0x700+0x0C))) = (0x20);  

    
    ( *((volatile int *)(0x11000000+0x700+0x6C))) = (G_tmsDataBuf[G_tmsPingPong][0]);

    return;
}

void B_IemsTask(void)
{
    B_SendTmsData();

    if (G_cntTmsInterrupt == 0)
    {
        G_bTmsPack = 0x1ACFFC1D;
        G_cntTmsInterrupt++;
    }
    else if (G_cntTmsInterrupt < 7)
    {
        if (G_cntTmsInterrupt == 6)
        {
            G_bTmsPack = 0x00000000;

            B_TmPack();
        }
        G_cntTmsInterrupt++;
    }
    else
    {
        ;
    }

    return;
}


void B_SendTmsData(void)
{
    unint32 i;
    unint32 tmsLeftLen;   
    unint32 fifoEmptyLen; 
    unint32 sendLen;
    
    tmsLeftLen = 256 - G_cntTmsSend;
    fifoEmptyLen = 128 - (( *((volatile int *)(0x11000000+0x700+0x68)))&0xFF);

    if (tmsLeftLen > 0)
    {
	sendLen = (tmsLeftLen < fifoEmptyLen) ? tmsLeftLen : fifoEmptyLen;

        for (i=0; i<sendLen; i++)
        {
            ( *((volatile int *)(0x11000000+0x700+0x6C))) = (G_tmsDataBuf[G_tmsPingPong][G_cntTmsSend++]);
        }
    }
    else  
    {
        ;
    }

    return;
}


void B_TmPack(void)
{
    unint32 i;
    unint32 tmpPingPang;

    
    tmpPingPang = (G_tmsPingPong ^ 0x1) & 0x1; 
    
    for (i=4; i<256; i++)
    {
        G_tmsDataBuf[tmpPingPang][i] = G_tmsTestBuf[i];
    }

    return;
}

