#ifndef _HEAD_DEFINE_H__
#define _HEAD_DEFINE_H__

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <android/log.h>
using namespace std;
#define LINE_NUM 2
#define LINE_PTN_NUM 30
#define PLANE_SEG 2
#define R_PI      3.1415926f
#define CFG_PATH  "/system/usr/pdcusr.cfg"
#define TAG  "pdc"
#define PDCL_LOG_INFO(...)  __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)

typedef struct _planesData{
	char* tex; 
	float* pBorderData;//two for vertex and two for TEXCOORD,so stride is 4
	unsigned short* pBorderIndex;
	unsigned short pBorderIndexNum;
	unsigned short pBorderPtElemNum;
}planesData;

typedef struct _linesData{
    //char* tex;
    float* pLineData;
    unsigned short startIndex;
    unsigned short* pLineIndex;
    unsigned short pLineIndexNum;
    unsigned short pLinePtElemNum;
}linesData;

typedef struct _groupMap{
	int planeNum;//total plane number
	int beginHead;//begin line
	int showPlaneNum;// how many plans need to show
	int planeSeq[10];// which one or multiple
} groupMap;




#endif