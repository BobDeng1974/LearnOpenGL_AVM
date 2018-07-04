#include "GraphicPDC.h"
#include "lineBorderOp.h"
#include "lodepng.h"
#include "Bmp.h"

GraphicPDC::GraphicPDC()
    :m_initStatus(false)
{
    PDCL_LOG_INFO("GraphicPDC | GraphicPDC");
}


GraphicPDC::~GraphicPDC()
{

}

void GraphicPDC::saveAsBmp(char* path, void* databuffer)
{
    BITMAPFILEHEADER1  bfh;
    BITMAPINFOHEADER1 bih;

    bih.wbiBitCount = 32;
    bih.dwbiClrImportant = 0;
    bih.dwbiClrUsed = 0;
    bih.dwbiCompression = 0;
    bih.lbiHeight = 228;
    bih.wbiPlanes = 1;
    bih.dwbiSize = sizeof(BITMAPINFOHEADER1);
    bih.dwbiSizeImage = 152*228*4;
    bih.lbiWidth = 152;
    bih.lbiXPelsPerMeter = 0;
    bih.lbiYpelsPerMeter = 0;

    bfh.wbfReserved1 = bfh.wbfReserved2 = 0;
    bfh.dwbfSize = sizeof(BITMAPFILEHEADER1) + sizeof(BITMAPINFOHEADER1) + bih.dwbiSizeImage;
    bfh.wbfType = 0x4D42;
    bfh.dwbfOffBits = sizeof(BITMAPFILEHEADER1) + sizeof(BITMAPINFOHEADER1);

    FILE * stream = fopen(path, "wb");
    if(stream == NULL) return;

    fwrite(&bfh, sizeof(bfh), 1, stream);
    fwrite(&bih, sizeof(bih), 1, stream);

    fwrite(databuffer,  bih.dwbiSizeImage, 1, stream);

    fclose(stream);
}

static char bigbuff[800*720*4];
void GraphicPDC::InitPDC(int screenWidth, int screenHeight)
{
    PDCL_LOG_INFO("GraphicPDC | InitPDC");
    ReadConfigFile(CFG_PATH);
    PDCL_LOG_INFO("1");
    setMode();
    PDCL_LOG_INFO("2");
    SetSumNum();
    PDCL_LOG_INFO("3");
    MallocGroupPlaneData();
    PDCL_LOG_INFO("4");
    CalPlaneData();
    PDCL_LOG_INFO("5");
    InitGroupMap();
    PDCL_LOG_INFO("6");
    LoadTextures();
     PDCL_LOG_INFO("7");
    tex = bigbuff;//(char*)malloc(texWidth*texHeight*4*sizeof(char));
    CompoundTex();
     PDCL_LOG_INFO("8");
    InitPdcFinish();
    PDCL_LOG_INFO("GraphicPDC | InitPDC end!!!");
}

void GraphicPDC::InitPdcFinish()
{
    m_initStatus = true;
}

bool GraphicPDC:: ReadConfigFile(const char* cfgfilepath)
{
    PDCL_LOG_INFO("GraphicPDC | ReadConfigFile  path = %s", cfgfilepath);

    fstream cfgFile;
    cfgFile.open(cfgfilepath, ios::in);//OpenFiles
    if( ! cfgFile.is_open())
    {
        PDCL_LOG_INFO("can not open cfg file!");
        return false;
    }

    char tmp[1000];
    int lineNum = 0;
    size_t notePos,assignmentPos;

    while(!cfgFile.eof())//read line
    {
        cfgFile.getline(tmp,200);

        string line(tmp);
        //cout<< lineNum<<endl;
        lineNum++;
        // fine note line and blank line
        assignmentPos = line.find('=');//find '='
         if ( assignmentPos==string::npos)
            {
                continue;
            } else{
                 //bug on this text parser....
               // assignmentPos+=1;
                //cout<< "assignment line"<<endl;
                string tmpKey = line.substr(0,assignmentPos-1);
                PDCL_LOG_INFO("tmpKey : %s", tmpKey.c_str());

                string value;
                if (tmpKey == "mode" )
                {
                    value = line.substr(assignmentPos+2);
                    modeStr = value.substr(0,value.size());
                    PDCL_LOG_INFO("modeStr is  ----------- %s", modeStr.c_str());
                }

                else if (tmpKey == "texfilesPathDayCam" )
                {
                    value = line.substr(assignmentPos+2);//
                    texfilesPathDayCam = value.substr(0,value.size());
                    PDCL_LOG_INFO("tmpKey is  ----------- %s", texfilesPathDayCam.c_str());

                }
                else if (tmpKey == "texfilesPathDayNoCam" )
                {
                    //cout<< "texfilesPathDayNoCam config "<<endl;
                    value = line.substr(assignmentPos+2);//
                    texfilesPathDayNoCam = value.substr(0,value.size());
                   PDCL_LOG_INFO("tmpKey is  ----------- %s", texfilesPathDayNoCam.c_str());
                }
                else if (tmpKey == "texfilesPathNightCam" )
                {
                    //cout<< "texfilesPathNightCam config "<<endl;
                    value = line.substr(assignmentPos+2);//
                    texfilesPathNightCam = value.substr(0,value.size());
                }
                else if (tmpKey == "texfilesPathNightNoCam" )
                {
                    //cout<< "texfilesPathNightNoCam config "<<endl;
                    value = line.substr(assignmentPos+2);//
                    texfilesPathNightNoCam = value.substr(0,value.size());                

                }
                else if(tmpKey == "groupNum")
                {
                    value = line.substr(assignmentPos+2);//
                    m_iGroupNum = atoi(value.c_str());
                   PDCL_LOG_INFO("groupNum is  ----------- %d", m_iGroupNum);
                }
                else if(tmpKey == "familyNum")
                {
                    //cout<< "familyNum config"<<endl;
                    size_t frontPos = line.find('[');//
                    size_t rearPos = line.find(']');//
                    int  delta = (rearPos - frontPos);
                    value = line.substr(frontPos+1,delta-1);
                    familyNumStr = value;

                }
                else if(tmpKey == "defaultShow")
                {
                    //cout<< "defaultShow config"<<endl;
                    size_t frontPos = line.find('[');//
                    size_t rearPos = line.find(']');//
                    int  delta = (rearPos - frontPos);
                    value = line.substr(frontPos+1,delta-1);
                    defaultShowStr = value;

                }
                else if(tmpKey == "sectorTexString")
                {
                    //cout<< "sectorTexString config"<<endl;

                    size_t frontPos,endPos;

                    char texStr[1000];
                    int i = 0;
                    while (!cfgFile.eof())
                    {
                        cfgFile.getline(texStr,1000);//1000 char
                        string texName(texStr);
                        endPos = texName.find('}');//find '}' end symbol
                        if (endPos==string::npos)
                        {
                            frontPos = texName.find(',');//find '='
                            if (frontPos==string::npos)
                            {
                                //cout<<"Cannot find texture name!";
                                ;
                            }
                            else
                            {
                                value = texName.substr(0,frontPos);
                                size_t n = value.find_last_not_of( " \r\n\t" );
                                if( n != string::npos )
                                {
                                    value.erase( n + 1 , value.size() - n );
                                }

                                n = value.find_first_not_of( " \r\n\t" );
                                if( n != string::npos )
                                {
                                    value.erase( 0 , n );
                                }

                                texNames[i] = value;
                               // cout<<value<<endl;
                                i++;
                            }

                        }
                        else
                        {
                            break;
                        }
                    }

                }
                else if(tmpKey == "dayCamPosx")
                {
                    value = line.substr(assignmentPos+2);//
                    dayCamPosx = atoi(value.c_str());
                   // cout<< "dayCamPosx config"<<endl;
                }
                else if(tmpKey == "dayCamPosy")
                {
                    value = line.substr(assignmentPos+2);//
                    dayCamPosy = atoi(value.c_str());
                   // cout<< "dayCamPosy config"<<endl;
                }
                else if(tmpKey == "dayCamTexWidth")
                {
                    value = line.substr(assignmentPos+2);//
                    dayCamTexWidth = atoi(value.c_str());
                    //cout<< "dayCamTexWidth config"<<endl;
                }
                else if(tmpKey == "dayCamTexHeight")
                {
                    value = line.substr(assignmentPos+2);//
                    dayCamTexHeight = atoi(value.c_str());
                    //cout<< "dayCamTexHeight config"<<endl;
                }
                else if(tmpKey == "dayNoCamPosx")
                {
                    value = line.substr(assignmentPos+2);//
                    dayNoCamPosx = atoi(value.c_str());
                    //cout<< "dayNoCamPosx config"<<endl;
                }
                else if(tmpKey == "dayNoCamPosy")
                {
                    value = line.substr(assignmentPos+2);//
                    dayNoCamPosy = atoi(value.c_str());
                    //cout<< "dayNoCamPosy config"<<endl;
                }
                else if(tmpKey == "dayNoCamTexWidth")
                {
                    value = line.substr(assignmentPos+2);//
                    dayNoCamTexWidth = atoi(value.c_str());
                    //cout<< "dayNoCamTexWidth config"<<endl;
                }
                else if(tmpKey == "dayNoCamTexHeight")
                {
                    value = line.substr(assignmentPos+2);//
                    dayNoCamTexHeight = atoi(value.c_str());
                    //cout<< "dayNoCamTexHeight config"<<endl;
                }
                else if(tmpKey == "nightCamPosx")
                {
                    value = line.substr(assignmentPos+2);//
                    nightCamPosx = atoi(value.c_str());
                    //cout<< "nightCamPosx config"<<endl;
                }
                else if(tmpKey == "nightCamPosy")
                {
                    value = line.substr(assignmentPos+2);//
                    nightCamPosy = atoi(value.c_str());
                    //cout<< "nightCamPosy config"<<endl;
                }
                else if(tmpKey == "nightCamTexWidth")
                {
                    value = line.substr(assignmentPos+2);//
                    nightCamTexWidth = atoi(value.c_str());
                    //cout<< "nightCamTexWidth config"<<endl;
                }
                else if(tmpKey == "nightCamTexHeight")
                {
                    value = line.substr(assignmentPos+2);//
                    nightCamTexHeight = atoi(value.c_str());
                    //cout<< "nightCamTexHeight config"<<endl;
                }
                else if(tmpKey == "nightNoCamPosx")
                {
                    value = line.substr(assignmentPos+2);//
                    nightNoCamPosx = atoi(value.c_str());
                    //cout<< "nightNoCamPosx config"<<endl;
                }
                else if(tmpKey == "nightNoCamPosy")
                {
                    value = line.substr(assignmentPos+2);//
                    nightNoCamPosy = atoi(value.c_str());
                    //cout<< "nightNoCamPosy config"<<endl;
                }
                else if(tmpKey == "nightNoCamTexWidth")
                {
                    value = line.substr(assignmentPos+2);//
                    nightNoCamTexWidth = atoi(value.c_str());
                    //cout<< "nightNoCamTexWidth config"<<endl;
                }
                else if(tmpKey == "nightNoCamTexHeight")
                {
                    value = line.substr(assignmentPos+2);//
                    nightNoCamTexHeight = atoi(value.c_str());
                    //cout<< "nightNoCamTexHeight config"<<endl;
                }

            }

    }

    if(cfgFile.is_open())
    {
        cfgFile.close();
    }
    return true;

}

void GraphicPDC::setMode()
{
    PDCL_LOG_INFO("Set Mode");

 
    PDCL_LOG_INFO("texfilesPathNightCam :%s",texfilesPathNightCam.c_str());
    texFilePath = texfilesPathNightCam;
    posX = nightCamPosx;
    posY = nightCamPosy;
    planeHeight = nightCamTexHeight;
    planeWidth = nightCamTexWidth;
 
    PDCL_LOG_INFO("modeStr is ------%s", modeStr.c_str());

    PDCL_LOG_INFO("texFilePath is ------%s", texFilePath.c_str());
}


void GraphicPDC::SetSumNum()
{
    //cout<<"Set Sum Number"<<endl;
    stringstream numStr(familyNumStr);
    string tempStr;
    familyNum = (int *)malloc(m_iGroupNum*sizeof(int));
    int i = 0;
    sumNum =0;
    while(getline(numStr,tempStr,','))
    {
        //cout<<tempStr<<endl;
        familyNum[i] = atoi(tempStr.c_str());
        sumNum += familyNum[i];
        i++;
    }
}


void GraphicPDC::MallocGroupPlaneData()
{
   // cout<<"Malloc Group Plane Data"<<endl;
    m_Planes = (planesData*)malloc(sumNum*sizeof(planesData));
    for(int i=0;i<sumNum;i++)
        MallocGroupBorderData(i);
}

void GraphicPDC::MallocGroupBorderData(int index)
{
    planesData* curLineData = &m_Planes[index];

    unsigned short tmpPtElemNum;
    unsigned short tmpIndexElemNum;
    unsigned short totalPtElemNum=0;
    unsigned short totalIndexElemNum = 0;

    lineBorderOp borderOp;
    borderOp.CalLine2DBorderPtNum(4,tmpPtElemNum,tmpIndexElemNum);
    totalPtElemNum += tmpPtElemNum;
    totalIndexElemNum += tmpIndexElemNum;

    curLineData->pBorderPtElemNum = totalPtElemNum;
    curLineData->pBorderIndexNum = totalIndexElemNum;
    curLineData->pBorderData = (float*)malloc(totalPtElemNum*sizeof(float));
    curLineData->pBorderIndex = (unsigned short*)malloc(totalIndexElemNum*sizeof(unsigned short));

}


void GraphicPDC::CalPlaneData()
{
    //cout<<"Calculate Plane Data"<<endl;
    for(int i=0;i<sumNum;i++)
        CalPlaneBorder(i);

}

void GraphicPDC::CalPlaneBorder(int index)
{
    lineBorderOp borderOp;
    planesData* group = &m_Planes[index];
    float* borderData = group->pBorderData;
    unsigned short* borderIndex = group->pBorderIndex;
    borderOp.CalLine2D(index,borderData,borderIndex,posX,posY,planeWidth,planeHeight);
    // for(int i=0; i<24; i++){
    //       cout<<"group->pBorderData["<<i<<"]is ----------"<<group->pBorderData[i]<<endl;
    // }
 
}

void GraphicPDC::InitGroupMap()
{
    stringstream numStr(defaultShowStr);
    string tempStr;
    PDCL_LOG_INFO("m_iGroupNum is ------->%d", m_iGroupNum);
    defaultShow = (int *)malloc(m_iGroupNum*sizeof(int));
    int k = 0;
    while(getline(numStr,tempStr,','))
    {
        defaultShow[k] = atoi(tempStr.c_str());
        k++;
    }
    m_iGroupMaps = (groupMap *)malloc(m_iGroupNum*sizeof(groupMap));

    m_iGroupMaps[0].beginHead = 0;
    m_iGroupMaps[0].planeNum = familyNum[0];
    m_iGroupMaps[0].showPlaneNum = 1;
    m_iGroupMaps[0].planeSeq[0] = defaultShow[0]-1;

    for (int i=1;i<m_iGroupNum;i++)
    {
        m_iGroupMaps[i].beginHead = m_iGroupMaps[i-1].beginHead + familyNum[i-1];
        m_iGroupMaps[i].planeNum = familyNum[i];
        m_iGroupMaps[i].showPlaneNum = 1;
        m_iGroupMaps[i].planeSeq [0] = defaultShow[i]-1;
    }

}


void GraphicPDC::LoadTextureData(int index)
{
    planesData* curPlaneData = &m_Planes[index];
    string filepath = "/system/usr/" + texNames[index] +".png";
    //cout<<"filepath name is -----------------"<<filepath<<endl;
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filepath, LCT_RGBA, 8);
    texHeight = (int)height;
    texWidth = (int)width;
    if(error)
    {
         PDCL_LOG_INFO("decode Image Error");
    }

    curPlaneData->tex = (char*)malloc(texWidth*texHeight*4*sizeof(char));
    char* pTexData = curPlaneData->tex;
    for (int i=0;i<texHeight;i++)
    {
        for (int j=0;j<texWidth;j++)
        {
            pTexData[4*i*texWidth+4*j] = (char)image[4*i*texWidth+4*j];
            pTexData[4*i*texWidth+4*j+1] = (char)image[4*i*texWidth+4*j+1];
            pTexData[4*i*texWidth+4*j+2] = (char)image[4*i*texWidth+4*j+2];
            pTexData[4*i*texWidth+4*j+3] = (char)image[4*i*texWidth+4*j+3];
        }
    }


}


void GraphicPDC::LoadTextures()
{

    for (int i=0; i<sumNum;i++)
    {
        LoadTextureData(i);
    }
    //cout<<"Load Texture Data finished!"<<endl;
}
//todo read default pdc state
void GraphicPDC::CompoundTex()
{
    int indexTex = 0;
    planesData plansGroup = m_Planes[sumNum-1];
    while(indexTex<texWidth*texHeight*4)
    {
        tex[indexTex] = plansGroup.tex[indexTex];
        indexTex++;
    }
}



bool GraphicPDC::bSetPDCLeftTop(int x,int y)
{
    PDCL_LOG_INFO( " GraphicPDC SetLeftTop %d,%d\n",x,y);
    posX = x;
    posY = y;
    CalPlaneData();
    return true;
}

bool GraphicPDC::bSetPDCSize(int w,int h)
{
    while(!getPdcInitStatus()) {
        PDCL_LOG_INFO("waiting for pdc init finished.....");
        usleep(100000);
    }
    PDCL_LOG_INFO( "GraphicPDC SetSize %d,%d\n",w,h);
    planeWidth = w;
    planeHeight = h;
    CalPlaneData();
    return true;
}

bool GraphicPDC::bSetGraphicPDCState(int *pdcStatus)
{
    
    while(!getPdcInitStatus()) {
        PDCL_LOG_INFO("waiting for pdc init finished.....");
        usleep(100000);
    }
    int indexTex = 0;

    PDCL_LOG_INFO("bSetGraphicPDCState run !! %d-%d-%d-%d=%d-%d-%d-%d=%d-%d-%d-%d=%d-%d-%d-%d=%d", \
    pdcStatus[0],pdcStatus[1],pdcStatus[2],pdcStatus[3], \
    pdcStatus[4],pdcStatus[5],pdcStatus[6],pdcStatus[7], \
    pdcStatus[8],pdcStatus[9],pdcStatus[10],pdcStatus[11], \
    pdcStatus[12],pdcStatus[13],pdcStatus[14],pdcStatus[15], \
    pdcStatus[16]);
#if 1
    PDCL_LOG_INFO("m_iGroupNum : %d\n", m_iGroupNum);
    // status check
    for (int i=0;i<m_iGroupNum;i++)
    {
        groupMap* currentMap = &m_iGroupMaps[i];
		//take -1 as no need to render the pixel value of current res file
        if ((pdcStatus[i]<-1)||(pdcStatus[i]>=currentMap->planeNum))
        {
            PDCL_LOG_INFO( "GraphicPDC Status is Exception %d %d, \n",pdcStatus[i],currentMap->planeNum);
            return false;
        }
    }
#endif
PDCL_LOG_INFO( "GraphicPDC Status is  %d %d, \n",pdcStatus[16],m_iGroupMaps[16].planeNum);
#if 0
     printf("currentMap run !! \n%d-%d-%d-%d-\n%d-%d-%d-%d-\n%d-%d-%d-%d-\n%d-%d-%d-%d-\n%d\n", \
            m_iGroupMaps[0].beginHead,m_iGroupMaps[1].beginHead,m_iGroupMaps[2].beginHead,m_iGroupMaps[3].beginHead, \
            m_iGroupMaps[4].beginHead,m_iGroupMaps[5].beginHead,m_iGroupMaps[6].beginHead,m_iGroupMaps[7].beginHead, \
            m_iGroupMaps[8].beginHead,m_iGroupMaps[9].beginHead,m_iGroupMaps[10].beginHead,m_iGroupMaps[11].beginHead, \
            m_iGroupMaps[12].beginHead,m_iGroupMaps[13].beginHead,m_iGroupMaps[14].beginHead,m_iGroupMaps[15].beginHead, \
            m_iGroupMaps[16].beginHead);     
 #endif
    //flash last valid state first
    while(indexTex<(texWidth*texHeight*4))
    {
        tex[indexTex] = 0;
        indexTex++;
    }
    int k;

    for (int i=0;i<m_iGroupNum;i++)
    {
        //printf( "pdcStatus[%d] value is --------|  %d\n",i,pdcStatus[i]);
        if (pdcStatus[i]==-1)
        {
            PDCL_LOG_INFO( "pdcStatus[%d] value is   %d \n",i,pdcStatus[i]);
            continue;
        }

        groupMap* currentMap = &m_iGroupMaps[i];
        planesData plansGroup = m_Planes[currentMap->beginHead+pdcStatus[i]];
        //planesData plansGroup = m_Planes[3];
        k=0;
        while(k<texWidth*texHeight*4)
        {
            if(plansGroup.tex[k+3]!=0)
            {
                tex[k] =  plansGroup.tex[k];
                tex[k+1] =  plansGroup.tex[k+1];
                tex[k+2] =  plansGroup.tex[k+2];
                tex[k+3] =  plansGroup.tex[k+3];
            }
            k = k+4;
        }
    }
    return true;
}


bool GraphicPDC::bSetPDCDisable()
{
    PDCL_LOG_INFO( "GraphicPDC bSetPDCDisable \n");
    CompoundTex();
    return true;
}


bool GraphicPDC::bSetGraphicPDCPause()
{
    PDCL_LOG_INFO( "bSetGraphicPDC Pause run...........\n");
    m_bPause = true;
    return true;
}

bool GraphicPDC::bSetGraphicPDCResume()
{
    PDCL_LOG_INFO( "bSetGraphicPDC Resume run...........\n");
    m_bPause = false;
    return true;
}
