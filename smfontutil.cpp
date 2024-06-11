
//------------------------------------------------------------------
//+CRi+H  Start of CRi header block
//------------------------------------------------------------------
//               Copyright 1996 Coherent Research Inc.
//      Author:Randy More
//     Created:11/6/96
//   $Revision: 8 $
//Last Changed:  $Author: Rmore $ $Date: 6/10/99 2:32p $
//                Added this header block and comment blocks
//------------------------------------------------------------------
//-CRi-H  End of CRi header block
//------------------------------------------------------------------
#include "stdafx.h"
#include <afxtempl.h>
#include "resource.h"
#include <math.h>
#include "SMCharacter.h"
#include "SMFontUtil.h"

CString smffile;
CString xtbfile;

#define BASE_CHARACTER_WIDTH 16.0
#define BASE_CHARACTER_HEIGHT 22.0
#define TEXT_DROP 2
//#define WADJUST 2.25
//#define HADJUST 2.0
#define WADJUST 1.5
#define HADJUST 1.0
#define NUM_FONTS 12


//------------------------------------------------------------------
//+CRi+M Start of CRi method block
//------------------------------------------------------------------
//      Class: SMFontUtil
//
//     Method: SMFontUtil
//
//    Returns: 
//
//  Arguments:
//
//Description:
//         Constructor method for SMFontUtil class
//
//------------------------------------------------------------------
//-CRi-M  End of CRi method block
//------------------------------------------------------------------

SMFontUtil::SMFontUtil(BOOL pForceReload)
{
	char s[MAX_PATH];
	GetModuleFileName(NULL, s, MAX_PATH); 
	smffile = s;
	int i = smffile.ReverseFind('\\');
	if( i > 0 )
		smffile = smffile.Left(i);
	else
		ASSERT(0);
	smffile += "\\Hershey.smf";
	xtbfile = s;
	xtbfile = xtbfile.Left(i);
	xtbfile += "\\Hershey.xtb";
	//
	cCharCount = 0;
	int outer, inner;
	for(outer = 0; outer < NUM_FONTS; outer++)
	{
		for(inner = 0; inner < 256; inner++)
		{
			cXlationTable[outer][inner]=-1;
		}
	}
	if(!pForceReload)
	{
		LoadFontData();
		LoadXlationData();
	}
	else
	{
		LoadOldStyleFile();
		LoadOldStyleXlation();
	}
}


//------------------------------------------------------------------
//+CRi+M Start of CRi method block
//------------------------------------------------------------------
//      Class: SMFontUtil
//
//     Method: ~SMFontUtil
//
//    Returns: 
//
//  Arguments:
//
//Description:
//         Destructor method for SMFontUtil class
//
//------------------------------------------------------------------
//-CRi-M  End of CRi method block
//------------------------------------------------------------------
SMFontUtil::~SMFontUtil()
{
	unsigned int itter;
	SMCharacter * chr;

	for(itter=0; itter<cCharCount; itter++)
	{
		chr = SMCharList[itter];
		delete chr;
	}
	SMCharList.RemoveAll();
}








//------------------------------------------------------------------
//+CRi+M Start of CRi method block
//------------------------------------------------------------------
//      Class: SMFontUtil
//
//     Method: LoadFontData
//
//    Returns: void
//
//  Arguments:
//         void
//
//Description:
//         Perform the LoadFontData operation    *AG*
//
//------------------------------------------------------------------
//-CRi-M  End of CRi method block
//------------------------------------------------------------------
void SMFontUtil::LoadFontData(void)
{
	FILE * infile;
	SMCharacter * chr;
	unsigned int loop;
	if(!(infile = fopen(smffile,"rb")))
	{
		MessageBox(NULL,
			"Font stroke file was not found\nSpecify font stroke .CSV data file.",
			"Load Font Data",MB_OK);
		LoadOldStyleFile();
		SaveFontData();
	}
	else
	{
		fread(&loop, sizeof(unsigned int), 1, infile);
		for(cCharCount = 0; cCharCount < loop; cCharCount++)
		{
			chr = new SMCharacter;
			chr->Read(infile);	
			SMCharList.Add(chr);
		}
		for(; cCharCount < NUM_FONTS*256; cCharCount++)
		{
			chr = new SMCharacter;
			SMCharList.Add(chr);
		}
		cCharCount = NUM_FONTS*256;
		fclose(infile);
	}
}









//------------------------------------------------------------------
//+CRi+M Start of CRi method block
//------------------------------------------------------------------
//      Class: SMFontUtil
//
//     Method: LoadXlationData
//
//    Returns: void
//
//  Arguments:
//         void
//
//Description:
//         Perform the LoadXlationData operation    *AG*
//
//------------------------------------------------------------------
//-CRi-M  End of CRi method block
//------------------------------------------------------------------
void SMFontUtil::LoadXlationData(void)
{
	FILE * infile;
	int i,j,k;
	if(!(infile = fopen(xtbfile,"rb")))
	{
		MessageBox(NULL,
			"Font translation file was not found in the system directory\nSpecify translation data file (TranslationChart.CSV).",
			"Load Xlation Data",MB_OK);
		LoadOldStyleXlation();
		SaveXlationData();
	}
	else
	{
		for(i=0; i<NUM_FONTS; i++)
		{
			for(j=0; j<128; j++)
			{
				if(!feof(infile))
				{
					fread(&k, sizeof(int), 1, infile);
					cXlationTable[i][j] = k;
				}
			}
		}
		for(i=0; i<NUM_FONTS; i++)
		{
			for(j=128; j<256; j++)
			{
				if(!feof(infile))
				{
					fread(&k, sizeof(int), 1, infile);
					cXlationTable[i][j] = k;
				}
			}
		}
		fclose(infile);
	}
}









//------------------------------------------------------------------
//+CRi+M Start of CRi method block
//------------------------------------------------------------------
//      Class: SMFontUtil
//
//     Method: SaveXlationData
//
//    Returns: void
//
//  Arguments:
//         void
//
//Description:
//         Perform the SaveXlationData operation    *AG*
//
//------------------------------------------------------------------
//-CRi-M  End of CRi method block
//------------------------------------------------------------------
void SMFontUtil::SaveXlationData(void)
{
	FILE * outfile;
	int i,j,k;
	outfile = fopen(xtbfile,"wb");
	for(i=0; i<NUM_FONTS; i++)
	{
		for(j=0; j<128; j++)
		{
			k = cXlationTable[i][j];
			fwrite(&k, sizeof(int), 1, outfile);
		}
	}
	for(i=0; i<NUM_FONTS; i++)
	{
		for(j=128; j<256; j++)
		{
			k = cXlationTable[i][j];
			fwrite(&k, sizeof(int), 1, outfile);
		}
	}
	fclose(outfile);
}







//------------------------------------------------------------------
//+CRi+M Start of CRi method block
//------------------------------------------------------------------
//      Class: SMFontUtil
//
//     Method: SaveFontData
//
//    Returns: void
//
//  Arguments:
//         void
//
//Description:
//         Perform the SaveFontData operation    *AG*
//
//------------------------------------------------------------------
//-CRi-M  End of CRi method block
//------------------------------------------------------------------
void SMFontUtil::SaveFontData(void)
{
	FILE * outfile;
	unsigned int loop;
	outfile = fopen(smffile,"wb");
	loop = cCharCount;
	fwrite(&loop, sizeof(unsigned int), 1, outfile);
	for(loop = 0; loop < cCharCount; loop++)
	{
		SMCharList[loop]->Write(outfile);
		
	}
	fclose(outfile);
}







//------------------------------------------------------------------
//+CRi+M Start of CRi method block
//------------------------------------------------------------------
//      Class: SMFontUtil
//
//     Method: LoadOldStyleXlation
//
//    Returns: void
//
//  Arguments:
//         void
//
//Description:
//         Perform the LoadOldStyleXlation operation    *AG*
//
//------------------------------------------------------------------
//-CRi-M  End of CRi method block
//------------------------------------------------------------------
void SMFontUtil::LoadOldStyleXlation(void)
{

	FILE * infile;
	char inbuffer[4096];
	char * scanner;
	int i,j,k;

	cCharCount = 0;
	CFileDialog open(TRUE,NULL,"*.csv",
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		"Font Translation File (*.csv)|*.csv||",NULL);
	open.m_ofn.lpstrInitialDir = "\\";
	open.m_ofn.lpstrTitle = "Select Font Translation File";
	open.DoModal();
	if(!(infile = fopen(open.GetPathName(),"rb")))
	{
		MessageBox(NULL,"Translation file did not open\nLoadOldStyleXlationFile","Error",MB_OK);
		_exit(0);
	}

	fgets((char *)inbuffer,4096,infile);
	for(i=0; i<256; i++)
	{
		fgets((char *)inbuffer,4096,infile);
		scanner = inbuffer;
		while(*scanner != ',') scanner++;
		scanner++;
		for(k=0; k<NUM_FONTS; k++)
		{
			while(*scanner != ',') scanner++;
			scanner++;
			sscanf(scanner,"%d",&j);
			cXlationTable[k][i] = j;
		}
	}

	fclose(infile);

}








//------------------------------------------------------------------
//+CRi+M Start of CRi method block
//------------------------------------------------------------------
//      Class: SMFontUtil
//
//     Method: LoadOldStyleFile
//
//    Returns: void
//
//  Arguments:
//         void
//
//Description:
//         Perform the LoadOldStyleFile operation    *AG*
//
//------------------------------------------------------------------
//-CRi-M  End of CRi method block
//------------------------------------------------------------------
void SMFontUtil::LoadOldStyleFile(void)
{

	FILE * infile;
	SMCharacter * chr;

	cCharCount = 0;
	CFileDialog open(TRUE,NULL,"*.csv",
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		"Font Stroke File (*.csv)|*.csv||",NULL);
	open.m_ofn.lpstrInitialDir = "\\";
	open.m_ofn.lpstrTitle = "Select Font Stroke File";
	open.DoModal();
	if(!(infile = fopen(open.GetPathName(),"rb")))
	{
		MessageBox(NULL,"Font file did not open\nLoadOldStyleFile","Error",MB_OK);
		_exit(0);
	}

	for(;;)
	{
		chr = new SMCharacter;
		if(chr->ReadOldStyle(infile))
		{
			SMCharList.Add(chr);
			cCharCount++;
		}
		else
		{
			delete chr;
			break;
		}
	}

	fclose(infile);

}








//------------------------------------------------------------------
//+CRi+M Start of CRi method block
//------------------------------------------------------------------
//      Class: SMFontUtil
//
//     Method: GetCharID
//
//    Returns: int
//
//  Arguments:
//         unsigned char pCharValue
//         FONT_TYPE pFont
//
//Description:
//         Perform the GetCharID operation    *AG*
//
//------------------------------------------------------------------
//-CRi-M  End of CRi method block
//------------------------------------------------------------------
int SMFontUtil::GetCharID(unsigned char pCharValue,
	FONT_TYPE pFont)
{
	int index;
	int charindex;
	index = (int)pFont;
	charindex = pCharValue;
	if(index < 0)
		return(-1);
	if(index > (int)GOTHIC)
		return(-1);
	return(cXlationTable[index][charindex]);
}


void SMFontUtil::DrawString(
	CDC * pDC,				//device context to draw to
	CPoint pStart,			//starting point
	double pRotation,		//rotation angle clockwise in radians (0 = 12:00)
	double pCharWidth,		//width of each character
	double pCharHeight,		//height of each character
	FONT_TYPE pFontType,	//the font to use
	CString pString)		//the string
{
	int curx = pStart.x;
	int cury = pStart.y;
	int length = pString.GetLength();
	double x_scale = (pCharWidth / WADJUST) / BASE_CHARACTER_WIDTH;
	double y_scale = (pCharHeight / HADJUST)  / BASE_CHARACTER_HEIGHT;

	double sin_val = sin(pRotation);
	double cos_val = cos(pRotation);

	double cZoomValue = 1.0;

	double deltax = sin_val * ((pCharWidth / WADJUST) + 2);
	double deltay = cos_val * ((pCharWidth / WADJUST) + 2);

	for(int loop = 0; loop<length; loop++)
	{
		int charid = GetCharID(pString.GetAt(loop),pFontType);
		if(charid>=0)
		{
			CharVertex chrvertex;
			int iter;
			SMCharacter * chr = GetCharacter(charid);
			SMCharacter::CHARVERTEX_TYPE result = 
				chr->GetFirstVertex(chrvertex,iter);
			while(result != SMCharacter::TERMINATE)
			{
				chrvertex.X = chrvertex.X * x_scale;
				chrvertex.Y = (chrvertex.Y+TEXT_DROP) * y_scale;
				if(result == SMCharacter::MOVE_TO)
				{
					pDC->MoveTo(
						(int)(((curx + (sin_val * chrvertex.X - 
							cos_val * chrvertex.Y))) / cZoomValue),
						(int)(((cury + (cos_val * chrvertex.X + 
							sin_val * chrvertex.Y))) / cZoomValue));
				}
				else
				{
					pDC->LineTo(
						(int)(((curx + (sin_val * chrvertex.X - 
							cos_val * chrvertex.Y))) / cZoomValue),
						(int)(((cury + (cos_val * chrvertex.X + 
							sin_val * chrvertex.Y))) / cZoomValue));
				}
				result = chr->GetNextVertex(chrvertex,iter);
			}
		}
		curx += (int)deltax;
		cury += (int)deltay;
	}
}
