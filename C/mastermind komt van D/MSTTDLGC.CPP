/*  Project mastermi
	EDRON
	Copyright © 1995. All Rights Reserved.

	SUBSYSTEM:    mastermi.exe Application
	FILE:         msttdlgc.cpp
	AUTHOR:       Ed N & Ron L


	OVERVIEW
	========
	Source file for implementation of mastermiTDLGClient (TDialog).
*/

#define MCI_PARM2(p) ((long)(void far*)&p)

#include <owl\owlpch.h>
#pragma hdrstop
#include <stdlib.h>
#include <mmsystem.h>

#include "mstrmapp.h"
#include "msttdlgc.h"

 UINT  DeviceId = 0;
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(mastermiTDLGClient, TDialog)
//{{mastermiTDLGClientRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_NEWGAME, BNClickedNewGame),
	EV_BN_CLICKED(IDC_KNOPCLEAR, BNClickedClear),
	EV_BN_CLICKED(IDC_KNOPOK, BNClickedOK),
	EV_BN_CLICKED(IDC_BMP1, BNClickedBMP1),
	EV_BN_CLICKED(IDC_BMP2, BNClickedBMP2),
	EV_BN_CLICKED(IDC_BMP3, BNClickedBMP3),
	EV_BN_CLICKED(IDC_BMP4, BNClickedBMP4),
	EV_BN_CLICKED(IDC_BMP5, BNClickedBMP5),
	EV_BN_CLICKED(IDC_BMP6, BNClickedBMP6),
	EV_BN_CLICKED(IDC_BMP7, BNClickedBMP7),
	EV_BN_CLICKED(IDC_BMP8, BNClickedBMP8),
	EV_BN_CLICKED(IDC_BMP9, BNClickedBMP9),
	EV_BN_CLICKED(IDC_SOLUTION, BNClickedSolution),
	EV_WM_PAINT,
	EV_BN_CLICKED(IDC_POS1, BNClickedPos1),
	EV_BN_CLICKED(IDC_POS2, BNClickedPos2),
	EV_BN_CLICKED(IDC_POS3, BNClickedPos3),
	EV_BN_CLICKED(IDC_POS4, BNClickedPos4),
	EV_BN_CLICKED(IDC_POS5, BNClickedPos5),
	//{{mastermiTDLGClientRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{mastermiTDLGClient Implementation}}


//////////////////////////////////////////////////////////
// mastermiTDLGClient
// ==========
// Construction/Destruction handling.
static mastermiTDLGClientXfer mastermiTDLGClientData;

mastermiTDLGClient::mastermiTDLGClient (TWindow *parent, TResId resId, TModule *module)
	: TDialog(parent, resId, module)
{
//{{mastermiTDLGClientXFER_USE}}
	start=0;
//{{mastermiTDLGClientXFER_USE_END}}

	SetBkgndColor(RGB(64,128,64));
}


mastermiTDLGClient::~mastermiTDLGClient (){	Destroy();}


void mastermiTDLGClient::Controle_Invoer(char cijfer)
{
	if (pos_in_regel < MAXCOL && regel<MAXROW)
	{
		MMarray[regel][pos_in_regel++]=cijfer;
		SetLampjes(regel);
	}
	if (regel==MAXROW)
		{
		MessageBox( "Game over","Game over", MB_OK);
		BNClickedNewGame ();
        }

}

void mastermiTDLGClient::BNClickedBMP1 (){	Controle_Invoer('1');}
void mastermiTDLGClient::BNClickedBMP2 (){	Controle_Invoer('2');}
void mastermiTDLGClient::BNClickedBMP3 (){	Controle_Invoer('3');}
void mastermiTDLGClient::BNClickedBMP4 (){	Controle_Invoer('4');}
void mastermiTDLGClient::BNClickedBMP5 (){	Controle_Invoer('5');}
void mastermiTDLGClient::BNClickedBMP6 (){	Controle_Invoer('6');}
void mastermiTDLGClient::BNClickedBMP7 (){	Controle_Invoer('7');}
void mastermiTDLGClient::BNClickedBMP8 (){	Controle_Invoer('8');}
void mastermiTDLGClient::BNClickedBMP9 (){	Controle_Invoer('9');}

void mastermiTDLGClient::BNClickedPos1 () {pos_in_regel=0;}
void mastermiTDLGClient::BNClickedPos2 () {pos_in_regel=1;}
void mastermiTDLGClient::BNClickedPos3 () {pos_in_regel=2;}
void mastermiTDLGClient::BNClickedPos4 () {pos_in_regel=3;}
void mastermiTDLGClient::BNClickedPos5 () {pos_in_regel=4;}

void mastermiTDLGClient::BNClickedNewGame ()
{
	int m,n;
	GameOver=FALSE;
	Solution=FALSE;
	randomize();
	regel=0;
	pos_in_regel=0;
	for (n=0;n<MAXROW;n++)
	  {
		KleurGoed[n]=0;
		PosGoed[n]=0;
		for(m=0;m<MAXCOL;m++) 	MMarray[n][m]='0';
		SetLampjes(n);
		SetPaaltjes(n);
	  }
	GekozenGetal[0]=random(9)+'1';
	for(m=1;m<MAXCOL;m++)
		{ char flag;
		 do	{
			flag=false;
			GekozenGetal[m]=random(9)+'1';
			for(n=m-1;n>=0;n--)	if(GekozenGetal[m]==GekozenGetal[n]) flag=true;
			}	 while(flag==true);
		 }
	GekozenGetal[m]=0;
}


void mastermiTDLGClient::BNClickedClear ()
{
 int m;
 pos_in_regel=0;
 for(m=0;m<MAXCOL;m++)	MMarray[regel][m]='0';  MMarray[regel][m]=0;
 SetLampjes(regel);
}


void mastermiTDLGClient::DrawBitmap(HDC hdc, HBITMAP hbitmap, short xStart, short yStart)
{
BITMAP bm;
HDC hdcMem;
POINT ptSize,ptOrg;

hdcMem = CreateCompatibleDC(hdc);
SelectObject(hdcMem,hbitmap);
SetMapMode(hdcMem,GetMapMode(hdc));

GetObject(hbitmap,sizeof(BITMAP), (LPSTR) &bm);
BMPsizex=ptSize.x = bm.bmWidth;
BMPsizey=ptSize.y = bm.bmHeight;
DPtoLP (hdc,&ptSize,1);
ptOrg.x=0;
ptOrg.y=0;
DPtoLP(hdcMem,&ptOrg,1);
BitBlt(hdc, xStart,yStart,ptSize.x,ptSize.y,hdcMem,ptOrg.x,ptOrg.y,SRCCOPY);
DeleteDC(hdcMem);
}

void mastermiTDLGClient::SetPaaltjes(int regel)
{
short m,n;
HDC hdc;
HBITMAP hbm;
char BMPName[10];

BMPsizex=0;
hdc=GetDC(HWindow);//GetDlgItem(IDD_CLIENT));
 strcpy(BMPName,"PAAL0");
 hbm= LoadBitmap(GetModule()->GetInstance(),BMPName);
 for(m=0;m<PosGoed[regel];m++)
			  DrawBitmap(hdc, hbm, 280+m*BMPsizex, 120+regel*BMPsizey);
 DeleteObject(hbm);
 strcpy(BMPName,"PAAL1");
 hbm= LoadBitmap(GetModule()->GetInstance(),BMPName);
 for(n=0;n<KleurGoed[regel];n++)
			  DrawBitmap(hdc, hbm,280+(m+n)*BMPsizex,120+regel*BMPsizey);
 DeleteObject(hbm);
 strcpy(BMPName,"PAALLEEG");//Paalnaam);
// strcpy(BMPName,Paalnaam);
 hbm= LoadBitmap(GetModule()->GetInstance(),BMPName);
 for(m=PosGoed[regel]+KleurGoed[regel];m<MAXCOL;m++)
			 DrawBitmap(hdc, hbm, 280+m*BMPsizex, 120+regel*BMPsizey);
 DeleteObject(hbm);

DeleteDC(hdc);

}

void mastermiTDLGClient::SetLampjes(int regel)
{
short m;
HDC hdc;
HBITMAP hbm;
char BMPName[10];

BMPsizex=0;
hdc=GetDC(HWindow);
for(m=0;m<MAXCOL;m++)
 {
 strcpy(BMPName,"BMP");
 BMPName[3]=MMarray[regel][m];
 BMPName[4]=0;
 hbm= LoadBitmap(GetModule()->GetInstance(),BMPName);
 DrawBitmap(hdc, hbm, 110+m*BMPsizex, 120+regel*BMPsizey);
 DeleteObject(hbm);
 }
DeleteDC(hdc);
}


void mastermiTDLGClient::BNClickedOK ()
{
	// Bereken de invoer en plaats positie OK tekens

 int m,n;
 PosGoed[regel]=KleurGoed[regel]=0;
 for(m=0;m<MAXCOL;m++)
   {
	if(MMarray[regel][m]==GekozenGetal[m]) PosGoed[regel]++;
	for(n=0;n<MAXCOL;n++)
		if(MMarray[regel][m]==GekozenGetal[n] && m!=n) KleurGoed[regel]++;
	}
 if(PosGoed[regel]==MAXCOL)
  {
	GameOver=TRUE;
	CmPlayWave();
  }
 SetPaaltjes(regel);
 regel++;
 pos_in_regel=0;

}


void mastermiTDLGClient::BNClickedRedraw ()
{
 int n;

 if(start==0)
  {
   strcpy(Paalnaam,"PAALLEEG"); // Zet leeg veld zonder punt
   BNClickedNewGame();
   start++;
   return;
  } 							 // Alleen bij opstarten programma

   strcpy(Paalnaam,"PAALNUL");   // Zet leeg veld met punt Wordt nog niet gebruikt
 for (n=0;n<MAXROW;n++)
   {
	SetLampjes(n);
	SetPaaltjes(n);
   }
}


void mastermiTDLGClient::BNClickedSolution ()
{
 strcpy(MMarray[MAXROW],GekozenGetal);
 SetLampjes(MAXROW);
 Solution=TRUE;
}


void mastermiTDLGClient::EvPaint ()
{
	TDialog::EvPaint();
	BNClickedRedraw ();
	if(Solution) SetLampjes(MAXROW);
}

void
mastermiTDLGClient::CmPlayWave()
{
  MCI_PLAY_PARMS    MciPlayParm;
  MCI_OPEN_PARMS    MciOpenParm;
  MCI_GENERIC_PARMS MciGenParm;
  DeviceId = 0;
  memset(&MciOpenParm, 0, sizeof MciOpenParm);
  strcpy(ElementName, "Found.wav");

  MciOpenParm.dwCallback = 0;
  MciOpenParm.wDeviceID = 0;
  #if !defined(__WIN32__)
	MciOpenParm.wReserved0 = 0;
  #endif
  MciOpenParm.lpstrDeviceType = 0;
  MciOpenParm.lpstrElementName = ElementName;
  MciOpenParm.lpstrAlias = 0;

  if (mciSendCommand(0, MCI_OPEN,  MCI_OPEN_ELEMENT, MCI_PARM2(MciOpenParm))) {
		MessageBox( "File Found.wav not found","Open Error", MB_OK);
		return;
	 }
  DeviceId = MciOpenParm.wDeviceID;
  MciPlayParm.dwCallback = (long)HWindow;
  MciPlayParm.dwFrom = 0;
  MciPlayParm.dwTo = 0;
  mciSendCommand(DeviceId, MCI_PLAY, MCI_WAIT, MCI_PARM2(MciPlayParm));
  mciSendCommand(DeviceId, MCI_CLOSE, MCI_WAIT,MCI_PARM2(MciGenParm));
}
