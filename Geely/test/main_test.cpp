/*
 * HelloWorld Service driver.
 * Copyright (C) 2009 Michael Richardson <mcr@credil.org>
 *
 * Released under the terms of the file ../NOTICE
 */
#define LOG_TAG "adastest"

#include <sys/types.h>
#include <unistd.h>
#include <grp.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <log/log.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <utils/RefBase.h>

#include "IAdasService.h"
#include "IAdasClient.h"
#include "IAdasStream.h"
#include "test_AdasVideoClient.h"

using namespace android;

#define LOG_TAG "adastest"

static const int DEFAULT_LAYER = 0xA7DCA8;
int main(int argc, char *argv[])
{
        
        FILE* fp = fopen("/system/usr/pdcusr.cfg", "r");
        if(fp == NULL) {
            ALOGI("strerror: %s\n", strerror(errno));
            ALOGI("fopen failed");
            return 0;
        }
        fclose(fp);
        ALOGI("adas test is now starting");
        sp<android::IServiceManager> sm = android::defaultServiceManager();
        sp<android::IBinder> binder;
        sp<IAdasService> svvse;
        sp<IAdasClient> svvct;
        sp<IAdasStream> svvsm;
        int bEnd = 0;
        int cmd = -1;
        int tmp = -1;
        int nret;

        do {
                binder = sm->getService(String16("AdasService"));
                if (binder != 0)
                        break;
                ALOGI("adas service not published, waiting...");
                usleep(500000); // 0.5 s
        } while(true);

	ALOGI("Adas test is now trying");


        svvse = interface_cast<IAdasService>(binder);
        if (svvse != NULL){
           ALOGI("svvse OK");     
        }
        
        svvct = new test_AdasVideoClient();
        if (svvse != NULL){
           ALOGI("svvct OK");     
        }

        int res = svvse->connect(svvct, 10, String16("adastest"), 10, svvsm);
        ALOGI("svvse->connect  res = %d", res);
       
       ALOGI("adas client is now exiting");


while (!bEnd) {
        printf("\n\n Select test item \n");
        printf("0 --- Exit\n");
        printf("1 --- Rear Gear Event   = 1\n");
        printf("2 --- speed < 15\n");
        printf("3 --- speed = 15.5\n");
        printf("4 --- speed >15\n");
        printf("5 --- Rear Gear Event   = 0\n");
        printf("6 --- camera error\n");
        printf("7 --- camera OK\n");
        printf("8 --- radar error\n");
        printf("9 --- radar  OK\n");

        nret = scanf("%4d", &cmd);
        if(nret!=1)
        {
            printf("Wrong cmd! Please input again!\n");
            cmd = -1;
        }
        
        printf("\n your select cmd is :%d\n", cmd);
        switch (cmd) {  
            case 0:
            {
                printf("exit.....\n");
                bEnd = 1;
                break;
            }
              
            case 1:
            {
                  svvsm->enlargeSingleView(1);           
                 break;
            }

            case 2:
            {               
                 svvsm->enlargeSingleView(2);
                 break;
            }

             case 3:
            {
                 svvsm->enlargeSingleView(3);
                 break;
            }
             case 4:
            {
                 svvsm->enlargeSingleView(4);
                 break;
            }
              case 5:
            {
                 svvsm->enlargeSingleView(5);
                 break;
            }
            case 6:
            {
                 svvsm->enlargeSingleView(6);
                 break;
            }
            case 7:
            {
                 svvsm->enlargeSingleView(7);
                 break;
            }
            case 8:
            {
                 svvsm->enlargeSingleView(8);
                 break;
            }
            case 9:
            {
                 svvsm->enlargeSingleView(9);
                 break;
            }
            default:
                break;
       }              
    } 
    return 0;
}

