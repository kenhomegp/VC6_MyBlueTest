// Mifare.cpp: implementation of the CMifare class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dio.h"
#include "Mifare.h"

#include <MfErrNo.h>
#include <MfRc500.h>

#include <PICCCmdConst.h>
#include <CommonLib.h>

// Modes for 'Mf500InterfaceOpen':   
#define USB   0x30
#define RS232 0x40

// Options for 'Mf500InterfaceOpen':
#define COMPORT_1 1
#define COMPORT_2 2

#define MIF_LIGHT 0x01
#define MIF_STANDARD 0x08
#define MIF_PLUS 0x10

unsigned char keyA[]  =  {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5};
unsigned char keyB[]  =  {0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5};
unsigned char keyF[]  =  {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

static signed short previousStatus = MI_OK;

static char ReadblockData = 1;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMifare::CMifare()
{

}

CMifare::~CMifare()
{
	
}

int CMifare::HWInitialize()
{
   signed short status = MI_OK;
   unsigned char Interface = USB;
   unsigned char Port = 0x00;

   if ((status = Mf500InterfaceOpen(Interface,Port)) == MI_POLLING)
   {
		return 1;
   }
   else
   {
		return 0;
   }

   //return status;
}

CString CMifare::RdNFCTagImage()
{
    static unsigned short errorCnt = 0;
    short status = MI_OK;
    int nextauth = 0;
    char i;
    unsigned char ats = 0;
    unsigned char adr = 0;
    unsigned nblocks = 256;
    unsigned char tt[2];
    unsigned char snr[4];
    unsigned char data[16];
    unsigned char keyCoded[12];
	CString tmp;

	m_TagImage = "";

	if(ReadblockData == 1)
	{
		while ((status = Mf500PcdConfig()) != MI_OK)
		{
			//AfxMessageBox("Mf500PcdConfig error");
			//return "";

			if (status == previousStatus)
			{
			   //if ( ! (errorCnt++ % 100 ) )
			   //   printf(".");
			}
			else
			{
			   previousStatus = status;
			   //printf("\ninitialization error %d %s ",status,GetErrorMessage(status));
			}
		}
	}

	while(1)
	{
		if ((status = Mf500PiccRequest(PICC_REQALL,tt)) == MI_OK)
		{
			if ((status = Mf500PiccAnticoll(0,snr)) == MI_OK)
			{
				if ((status = Mf500PiccSelect(snr,&ats)) != MI_OK)
				{
					m_TagImage = "";
					/*
					if(status == MI_OK)
					{
						if(ReadblockData == 1)
						{
							for(nblocks = 0 ; nblocks < 6 ; nblocks++)
							{
								status = Mf500PiccRead(nblocks , data);

								if (status == MI_OK)
								{
									printf("Read block %d\r\n",nblocks);
									for(nextauth = 0 ; nextauth < 16 ; nextauth++)
									{
										if(data[nextauth] < 16)
										{
						 					//printf(" 0%X",data[nextauth]);
											tmp.Format(" 0%X",data[nextauth]);
										}
										else
										{
											//printf(" %X",data[nextauth]);
											tmp.Format(" %X",data[nextauth]);
										}
										m_TagImage += tmp;
									}
									//printf("\r\n");
									m_TagImage += "\r\n";
								}
							}
							ReadblockData = 0;
							Mf500PiccHalt();
							return m_TagImage;
						}
					}
					*/
				}
			}
		}	
	
		if(status == MI_OK)
		{
			if(ReadblockData == 1)
			{
				for(nblocks = 0 ; nblocks < 21 ; nblocks+=4)//Read Block 0/4/8/12/20
				{
					status = Mf500PiccRead(nblocks , data);

					if (status == MI_OK)
					{
						printf("Read block %d\r\n",nblocks);
						for(nextauth = 0 ; nextauth < 16 ; nextauth++)
						{
							if(data[nextauth] < 16)
							{
						 		//printf(" 0%X",data[nextauth]);
								tmp.Format(" 0%X",data[nextauth]);
							}
							else
							{
								//printf(" %X",data[nextauth]);
								tmp.Format(" %X",data[nextauth]);
							}
							m_TagImage += tmp;
						}
						//printf("\r\n");
						m_TagImage += "\r\n";
					}
				}
				ReadblockData = 0;
				Mf500PiccHalt();
				break;
			}
		}	
	}

	return m_TagImage;
}

void CMifare::MifareClose()
{
	Mf500InterfaceClose();
}

void CMifare::WriteNFCTagImage()
{
	//*************************************************************************BHC302
	//NFC Tag B3.1 data

	//05 34 00 B9 
	//00 20 9E 3C 
	//82 00 00 00 
	//E1 10 0E 0F
	//03 4C D2 20 
	//27 61 70 70 
	//6C 69 63 61 
	//74 69 6F 6E
	//2F 76 6E 64 
	//2E 62 6C 75 
	//65 74 6F 6F 
	//74 68 2E 65
	//70 2E 6F 6F 
	//62 21 00 33 
	//30 32 33 30 
	//08 15 09 4E
	//6F 6B 69 61 
	//20 4D 44 2D 
	//35 30 57 20 
	//53 70 65 61
	//6B 65 72 04 
	//0D 04 04 20 
	//05 03 18 11 
	//23 00 00 00
	unsigned char TagImage[96] = {0x05,0x34,0x00,0xB9,
								0x00,0x20,0x9E,0x3C,
								0x82,0x00,0x00,0x00,
								0xE1,0x10,0x0E,0x0F,
								0x03,0x4C,0xD2,0x20,
								0x27,0x61,0x70,0x70,
								0x6C,0x69,0x63,0x61,
								0x74,0x69,0x6F,0x6E,
								0x2F,0x76,0x6E,0x64,
								0x2E,0x62,0x6C,0x75, 
								0x65,0x74,0x6F,0x6F,
								0x74,0x68,0x2E,0x65,
								0x70,0x2E,0x6F,0x6F,
								0x62,0x21,0x00,0x33,
								0x30,0x32,0x33,0x30,
								0x08,0x15,0x09,0x4E,
								0x6F,0x6B,0x69,0x61,
								0x20,0x4D,0x44,0x2D, 
								0x35,0x30,0x57,0x20,
								0x53,0x70,0x65,0x61,
								0x6B,0x65,0x72,0x04,
								0x0D,0x04,0x04,0x20,
								0x05,0x03,0x18,0x11,
								0x23,0x00,0x00,0x00
								}; 

	unsigned char data[16] = {0};

	unsigned char nblocks = 0;
    unsigned char tt[2];
    unsigned char snr[4];
	unsigned char ats = 0;
	short status = MI_OK;

	if(ReadblockData == 1)
	{
		while ((status = Mf500PcdConfig()) != MI_OK)
		{
			//AfxMessageBox("Mf500PcdConfig error");
			//return "";

			if (status == previousStatus)
			{
			   //if ( ! (errorCnt++ % 100 ) )
			   //   printf(".");
			}
			else
			{
			   previousStatus = status;
			   //printf("\ninitialization error %d %s ",status,GetErrorMessage(status));
			}
		}
	}

	while(1)
	{
		if ((status = Mf500PiccRequest(PICC_REQALL,tt)) == MI_OK)
		{
			if ((status = Mf500PiccAnticoll(0,snr)) == MI_OK)
			{
				if ((status = Mf500PiccSelect(snr,&ats)) != MI_OK)
				{

				}
			}
		}

		if(status == MI_OK)
		{
			if(ReadblockData == 1)
			{
				//ReadblockData = 0;

				//nblocks = 3;
				//memset(data , 0 ,16);
				//data[12] = TagImage[nblocks*4]; 
				//data[13] = TagImage[nblocks*4+1]; 
				//data[14] = TagImage[nblocks*4+2]; 
				//data[15] = TagImage[nblocks*4+3]; 
				//status = Mf500PiccWrite(nblocks , data);

				nblocks = 4;
				memset(data , 0 ,16);
				data[0] = TagImage[nblocks*4]; 
				data[1] = TagImage[nblocks*4+1]; 
				data[2] = TagImage[nblocks*4+2]; 
				data[3] = TagImage[nblocks*4+3]; 
				//status = Mf500PiccWrite(nblocks , data);
				status = Mf500PiccWrite4(nblocks , data);
				/*
				nblocks = 5;
				memset(data , 0 ,16);
				data[4] = TagImage[nblocks*4]; 
				data[5] = TagImage[nblocks*4+1]; 
				data[6] = TagImage[nblocks*4+2]; 
				data[7] = TagImage[nblocks*4+3]; 
				status = Mf500PiccWrite(nblocks , data);

				nblocks = 6;
				memset(data , 0 ,16);
				data[8] = TagImage[nblocks*4]; 
				data[9] = TagImage[nblocks*4+1]; 
				data[10] = TagImage[nblocks*4+2]; 
				data[11] = TagImage[nblocks*4+3]; 
				status = Mf500PiccWrite(nblocks , data);

				nblocks = 7;
				memset(data , 0 ,16);
				data[12] = TagImage[nblocks*4]; 
				data[13] = TagImage[nblocks*4+1]; 
				data[14] = TagImage[nblocks*4+2]; 
				data[15] = TagImage[nblocks*4+3]; 
				status = Mf500PiccWrite(nblocks , data);

				nblocks = 8;
				memset(data , 0 ,16);
				data[0] = TagImage[nblocks*4]; 
				data[1] = TagImage[nblocks*4+1]; 
				data[2] = TagImage[nblocks*4+2]; 
				data[3] = TagImage[nblocks*4+3]; 
				status = Mf500PiccWrite(nblocks , data);
				*/
				ReadblockData = 0;
				Mf500PiccHalt();
				break;
			}
		}
	}

	if(status != 0)
		AfxMessageBox("Write Error!!");
}
