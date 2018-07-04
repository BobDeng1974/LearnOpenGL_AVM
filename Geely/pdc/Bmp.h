/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */
/**
 * @file bmp.h
 * @brief bmp
 *
 * including definition and interface for bmp
 */
#ifndef OPENINGBMP_H
#define OPENINGBMP_H
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef signed char        BOOL;
typedef unsigned char    BYTE;
typedef int                      INT;
typedef void                  *LPVOID;
typedef unsigned short   WORD;
typedef uint32_t              DWORD;
typedef long                    LONG;
typedef uint32_t              UINT32;


typedef struct tagBITMAPFILEHEADER  {
        WORD wByteAligned; // two bytes for byte aligning
        WORD wbfType;
        DWORD dwbfSize;
        WORD wbfReserved1;
        WORD wbfReserved2;
        DWORD dwbfOffBits;
    } BITMAPFILEHEADER, *PBmpHeader;

typedef struct tagBITMAPINFOHEADER {
        DWORD dwbiSize;
        LONG lbiWidth;
        LONG lbiHeight;
        WORD wbiPlanes;
        WORD wbiBitCount;
        DWORD dwbiCompression;
        DWORD dwbiSizeImage;
        LONG lbiXPelsPerMeter;
        LONG lbiYpelsPerMeter;
        DWORD dwbiClrUsed;
        DWORD dwbiClrImportant;
    } BITMAPINFOHEADER, *PBmpInfo;

typedef struct tagRGBQuad {
        BYTE byRgbBlue;
        BYTE byRgbGreen;
        BYTE byRgbRed;
        BYTE byRgbReserved;
    } BITMAPRGBQuad, *PRGBQuad;

#pragma pack(push,2)
 typedef struct bitmapfileinfo
 {
        WORD wbfType;
        DWORD dwbfSize;
        WORD wbfReserved1;
        WORD wbfReserved2;
        DWORD dwbfOffBits;
 } BITMAPFILEHEADER1;

typedef struct bitmapinfo
{
        DWORD dwbiSize;
        LONG lbiWidth;
        LONG lbiHeight;
        WORD wbiPlanes;
        WORD wbiBitCount;
        DWORD dwbiCompression;
        DWORD dwbiSizeImage;
        LONG lbiXPelsPerMeter;
        LONG lbiYpelsPerMeter;
        DWORD dwbiClrUsed;
        DWORD dwbiClrImportant;
} BITMAPINFOHEADER1;

#pragma pack(pop)

typedef enum {
        BMP_PIXEL_FORMAT_BGRA8888  = 0,
        BMP_PIXEL_FORMAT_RGBA8888,
        BMP_PIXEL_FORMAT_YUV420,
        BMP_PIXEL_FORMAT_YUYV,
        BMP_PIXEL_FORMAT_UYVY,
        BMP_PIXEL_FORMAT_INVALID
} BMPPIXELFORMAT;



#endif // OPENINGBMP_H
/* EOF */
