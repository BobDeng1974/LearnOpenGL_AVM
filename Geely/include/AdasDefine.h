#ifndef	 ADAS_DEFINE_
#define ADAS_DEFINE_

#include <log/log.h>
#include <string>
#include <gui/Surface.h>
#include <utils/RefBase.h>
#include <ui/PixelFormat.h>
#include <utils/KeyedVector.h>
#include <json/json.h>
#include <json/config.h>
#include <json/writer.h>
#include <utils/Mutex.h>


#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>

#include <utils/Errors.h>
#include <utils/SystemClock.h>
#include <utils/String16.h>

namespace android {

static const int DEFAULT_LAYER = 0x50000000;

#define GUIDELINE_VIEW_WIDTH	1120
#define GUIDELINE_VIEW_HEIGHT	720
#define GUIDELINE_VIEW_POS_X           800
#define GUIDELINE_VIEW_POS_Y           0

#define PDC_VIEW_WIDTH	800
#define PDC_VIEW_HEIGHT	720
#define PDC_VIEW_POS_X          0
#define PDC_VIEW_POS_Y          0

#define RADAR_NUM_MAX        8
#define RVC_RADAR_NUM         3
#define CAMERA_NUM_MAX     4


enum eViewType
{
    eViewType_RVC = 0,
    eViewType_APA = 1,
    eViewType_ALL = 2	
};

enum eType
{
    eType_RVC = 0

};

typedef struct
{
    int x;
    int y;
}posXY;

typedef struct 
{
    int                        width;
    int                        height;
    char*                   name;
    posXY                   viewPos;
    PixelFormat         format;
    int 		       layer;
}viewInfo;

typedef struct
{
    int  radarID;
    int  obstacleFlag;
    int  distanceLevel; 
}radarType;

enum radarInfo
{
    radarInfo_RearRight = 0x01,//
    radarInfo_RearMiddleLeft = 0x04,
    radarInfo_RearLeft = 0x08,
    radarInfo_RearRightAndRearMiddleLeft = 0x05,
    radarInfo_RearLeftAndRearMiddleLeft = 0x0c,
    radarInfo_RearLeftAndRearRight = 0x09,
    radarInfo_ALL = 0x0d
};

enum radarError
{
    radarError_RearRight = 0x01,//
    radarError_RearMiddleLeft = 0x04,
    radarError_RearLeft = 0x08,
    radarError_RearRightAndRearMiddleLeft = 0x05,
    radarError_RearLeftAndRearMiddleLeft = 0x0c,
    radarError_RearLeftAndRearRight = 0x09,
    radarError_ALL = 0x0d
};

enum MSG
{
    MSG_A15_INIT_OK = 0x01,
    MSG_IPU_CONFIRM_OK = 0x02,
    MSG_IPU_CONFIRM_ERROR = 0x03,
    MSG_A15_READY_FOR_REQUEST = 0x04,

    MSG_A15_REQUEST_REAR = 0x05,

    MSG_A15_REQUEST_DISCONNECT_UART = 0x06,
    MSG_IPC_DISCONNECT = 0x07,

    MSG_REAR_DATA_REVERS = 0xFF,
    MSG_REAR_DATA_NO_REVERS = 0xF0
};


}

#endif