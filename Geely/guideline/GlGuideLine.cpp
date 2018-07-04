
#include "GlGuideLine.h"
#include <math.h>
FILE * g_pFile=NULL;
GlGuideLine::GlGuideLine()
{
	pLCamera = NULL;
	m_pSideLineInfo = NULL;
	m_pDistLineInfo = NULL;
	m_bDistortCorrection = false;
	m_GuideLineType = eStaticGuideLine;
	m_fPrevSWADeg = 0.0f;

}

GlGuideLine::~GlGuideLine()
{
	FreeGMLineInfo();
}

void GlGuideLine::SetWindowParam(int winWidth,int winHeight)
{
	m_iScreenWidth = winWidth;
	m_iScreenHeight = winHeight;
}


void GlGuideLine::InitLineParam(int groupNum,float* sepDis,float* LSegExtendLength,color3* groupColor,float markStartDis,
	float markDeltaDis,float markThickDis,int markNum,float trackWidth,int lineWidth,LCamera* pCam,trajectory traj,
	GuideLineType type,bool bUnDistort)
{
  
	m_iGroupNum = groupNum;
	m_fMarkDeltaDis = markDeltaDis;
	m_iMarkDeltaNum = markNum;
	m_fTrackWidth_dist= trackWidth;
    m_fTrackWidth_side = RVS_OVLAY_SIDE_WIDTH;
    m_iLinePixelWidth = lineWidth*2;

	m_bDistortCorrection = bUnDistort;
	m_GuideLineType = type;

	pLCamera = pCam;
	m_Trajectory = traj;
	SetSideLineParam(sepDis,LSegExtendLength);
	SetDistLineParam(markStartDis,markDeltaDis,markThickDis,markNum);
	MallocGroupLineData();
	SetGroupColor(groupColor);
	CalGuideLineData();
	InitView();
}

void GlGuideLine::UpdateLineData(int groupNum,float* sepDis,float* LSegExtendLength,color3* groupColor,float markStartDis,
								float markDeltaDis,float markThickDis,int markNum,float trackWidth,int lineWidth,LCamera* pCam,trajectory traj,
								GuideLineType type,bool bUnDistort)
{

	m_iGroupNum = groupNum;
	m_fMarkDeltaDis = markDeltaDis;
	m_iMarkDeltaNum = markNum;
	m_fTrackWidth_dist= trackWidth;
	m_fTrackWidth_side = RVS_OVLAY_SIDE_WIDTH;
	m_iLinePixelWidth = lineWidth*2;

	m_bDistortCorrection = bUnDistort;
	m_GuideLineType = type;

	pLCamera = pCam;
	m_Trajectory = traj;
	//SetSideLineParam(sepDis,LSegExtendLength);
	//SetDistLineParam(markStartDis,markDeltaDis,markThickDis,markNum);
	//MallocGroupLineData();
	//SetGroupColor(groupColor);
	CalGuideLineData();
	//InitView();
}



void GlGuideLine::SetSWAUpdatePrecision(f32_t delta)
{
	m_fUpdateDeltaAngleDeg = delta;
}

void GlGuideLine::Update(f32_t swaDeg,bool bForceUpdate)
{
	if(bForceUpdate)
	{
		CalGuideLineData();
	}
	else if(fabs(swaDeg - m_fPrevSWADeg)>m_fUpdateDeltaAngleDeg)
	{
		m_Trajectory.SetVehicleSteeringAngle(swaDeg);
		CalGuideLineData();
		m_fPrevSWADeg = swaDeg;
	}
}

void GlGuideLine::ForceUpdate()
{
	CalGuideLineData();
}

void GlGuideLine::UpdateTrackWidth(float width)
{
	//tbd :m_fTrackWidth = width;
	m_Trajectory.UpdateTrackWidth(width);
}


void GlGuideLine::SetGroupColor(color3* pColor)
{
	for(int i=0;i<m_iGroupNum;i++)
	{
		for(int j=0;j<3;j++)
			m_pGroups[i].color[j] = pColor[i][j];
	}
}

void GlGuideLine::FreeGMLineInfo()
{
	if(m_pSideLineInfo)
		free(m_pSideLineInfo);

	if(m_pDistLineInfo)
	{
		for(int i=0;i<m_iGroupNum;i++)
		{
			free(m_pDistLineInfo[i].dist);
		}
		free(m_pDistLineInfo);
	}
}

void GlGuideLine::MallocGroupLineData()
{
	m_pGroups = (linesData*)malloc(m_iGroupNum*sizeof(linesData));
	for(int i=0;i<m_iGroupNum;i++)
		MallocGroupLineData(i);
}

int GlGuideLine::CalSidePtNum(float startDis,float endDis)
{
	int outNum;
	float deltaDis = DIST_PER_POINT_CLOSE;
	float curDis = startDis;
	if(startDis>START_FAR_DIST)
		deltaDis = DIST_PER_POINT_FAR;
	if(startDis<START_FAR_DIST&&endDis>START_FAR_DIST)
	{
		outNum = 0;
		while(curDis<=endDis)
		{
			outNum++;
			curDis += deltaDis;
			if(fabs(curDis -START_FAR_DIST)<0.01)
				deltaDis = DIST_PER_POINT_FAR;
		}
		if(fabs(endDis - curDis)<deltaDis-0.01)
			outNum ++;
	}
	else
	{
		outNum = (endDis-startDis)/deltaDis+1;
		//curDis = startDis + outNum * deltaDis;
	}

	return outNum*2;
}

void GlGuideLine::MallocGroupLineData(int index)
{
	linesData* curLineData = &m_pGroups[index];
	sideLineInfo sideLineInfo = m_pSideLineInfo[index];
	distLineInfo distLineInfo = m_pDistLineInfo[index];
	int totalLineNum = sideLineInfo.lineNum + distLineInfo.lineNum;
	curLineData->lineNums = totalLineNum;
	curLineData->pLineData = (float**)malloc(totalLineNum*sizeof(float*));
	curLineData->pLinePtNums = (int*)malloc(totalLineNum*sizeof(int));

	MallocGroupSideLineData(index);
	MallocGroupDistLineData(index);
	MallocGroupBorderData(index);
}

void GlGuideLine::MallocGroupSideLineData(int index)
{
	sideLineInfo lineInfo = m_pSideLineInfo[index];
	linesData* curLineData = &m_pGroups[index];
	int sideLineNum = lineInfo.lineNum;
	int linePtNum = lineInfo.linePtNum;
	for(int i=0;i<sideLineNum;i++)
	{
		curLineData->pLinePtNums[i] = linePtNum;
		curLineData->pLineData[i] = (float*)malloc(linePtNum*2*sizeof(float));
	}
}

void GlGuideLine::MallocGroupDistLineData(int index)
{
	distLineInfo lineInfo = m_pDistLineInfo[index];
	linesData* curLineData = &m_pGroups[index];
	int distLineNum = lineInfo.lineNum;
	int linePtNum = lineInfo.linePtNum;
//	unsigned short tmpPtElemNum;
//	unsigned short tmpIndexElemNum;
//	lineBorderOp borderOp;
//	borderOp.CalLine2DBorderPtNum(linePtNum,tmpPtElemNum,tmpIndexElemNum);
	int lineoffset=0;//default has NO side line in this group
	//if(1 == curLineData->lineNums) lineoffset=0;
    if(RVS_OVLAY_SUM_LINE_PER_GROUP == curLineData->lineNums) lineoffset=2;
    //if(2 == curLineData->lineNums)&&(index>0) lineoffset=0;
	for(int i=0;i<distLineNum;i++)
	{
		curLineData->pLinePtNums[lineoffset] = linePtNum;
		curLineData->pLineData[lineoffset] = (float*)malloc(linePtNum*2*sizeof(float));
//		curLineData->pLineBorderIndexNum[curLineIndex]= tmpIndexElemNum;
//		curLineData->pLineBorderData[curLineIndex] = (float*)malloc(tmpPtElemNum*sizeof(float));
//		curLineData->pLineBorderIndex[curLineIndex] = (unsigned short*)malloc(tmpIndexElemNum*sizeof(unsigned short));
		lineoffset++;
	}
}

void GlGuideLine::MallocGroupBorderData(int index)
{
	linesData* curLineData = &m_pGroups[index];
	int lineNum = curLineData->lineNums;
	curLineData->pLinePtElemStartNum = (unsigned short*)malloc(lineNum*sizeof(unsigned short));
	curLineData->pLineIndexElemStartNum = (unsigned short*)malloc(lineNum*sizeof(unsigned short));
	curLineData->pLineBorderStartIndex = (unsigned short*)malloc(lineNum*sizeof(unsigned short));

	unsigned short tmpPtElemNum;
	unsigned short tmpIndexElemNum;
	unsigned short totalPtElemNum=0;
	unsigned short totalIndexElemNum = 0;
	unsigned short curStartIndex = 0;
	lineBorderOp borderOp;

	for(int i=0;i<lineNum;i++)
	{
		curLineData->pLinePtElemStartNum[i] = totalPtElemNum;
		curLineData->pLineIndexElemStartNum[i] = totalIndexElemNum;
		curLineData->pLineBorderStartIndex[i] = curStartIndex;
		borderOp.CalLine2DBorderPtNum(curLineData->pLinePtNums[i]/2,tmpPtElemNum,tmpIndexElemNum);
		totalPtElemNum += tmpPtElemNum;
		totalIndexElemNum += tmpIndexElemNum;
		curStartIndex += (tmpPtElemNum/PTELEMNUM);
	}
	curLineData->pBorderPtElemNum = totalPtElemNum;
	curLineData->pBorderIndexNum = totalIndexElemNum;
	curLineData->pBorderData = (float*)malloc(totalPtElemNum*sizeof(float));
	curLineData->pBorderIndex = (unsigned short*)malloc(totalIndexElemNum*sizeof(unsigned short));
}

void GlGuideLine::CalGuideLineData()
{

	for(int i=0;i<m_iGroupNum;i++)
		CalGroupValue(i);
}

void GlGuideLine::CalGroupValue(int index)
{	

	Cal2SideLinesPts(index);
	CalDistLinePts(index);
	CalLineBorder(index);
    
}


void GlGuideLine::Cal2SideLinesPts(int index)
{
	linesData* curLineData = &m_pGroups[index];
	sideLineInfo curInfo = m_pSideLineInfo[index];
	float startDis = curInfo.startDis;
	float endDis = curInfo.endDis;
	float deltaDis;
	deltaDis = DIST_PER_POINT_CLOSE;
	if(startDis>=START_FAR_DIST )
		deltaDis = DIST_PER_POINT_FAR;
    
    
	if(curInfo.lineNum == 2)
	{
		float* pLeftLine = curLineData->pLineData[0];
		float* pRightLine = curLineData->pLineData[1];
		int sideLinePtNum = curInfo.linePtNum;
       
		if(curInfo.bIsLSeg)
			sideLinePtNum = sideLinePtNum-1;      
		int curElemIndex = 0;
		float curDis = startDis;
		if(startDis<START_FAR_DIST&&endDis>START_FAR_DIST)
		{
			for(int i=0;i<sideLinePtNum;i++)
			{
				Cal2SideLineScreenPos(curDis,&pLeftLine[curElemIndex],&pRightLine[curElemIndex]);
				curElemIndex += 4;
				curDis += deltaDis;
				if(curDis>endDis)
					curDis = endDis;
				if(curDis>=START_FAR_DIST)
					deltaDis = DIST_PER_POINT_FAR;
			}
		}
		else
		{
			for(int i=0;i<sideLinePtNum/2;i++)
			{
				Cal2SideLineScreenPos(curDis,&pLeftLine[curElemIndex],&pRightLine[curElemIndex]);
				curElemIndex += 4;
				curDis += deltaDis;
				if(curDis>endDis)
					curDis = endDis;
			}
		}

		if(curInfo.bIsLSeg)
		{
			float tickLength = curInfo.LSegExtendLength/**2.0/m_iScreenWidth*/;
			float leftx = pLeftLine[(sideLinePtNum-1)*2];
			float lefty = pLeftLine[(sideLinePtNum-1)*2+1];
			float rightx = pRightLine[(sideLinePtNum-1)*2];
			float righty = pRightLine[(sideLinePtNum-1)*2+1];
			float length = sqrt((leftx-rightx)*(leftx-rightx)+(lefty-righty)*(lefty-righty));
			float dirx = (rightx-leftx)/length;
			float diry = (righty-lefty)/length;
			pLeftLine[sideLinePtNum*2] = leftx+dirx*tickLength;
			pLeftLine[sideLinePtNum*2+1] = lefty + diry*tickLength;
			pRightLine[sideLinePtNum*2] = rightx - dirx*tickLength;
			pRightLine[sideLinePtNum*2+1] = righty - diry*tickLength;
		}
	}

}

void GlGuideLine::CalDistLinePts(int index)
{
	distLineInfo lineInfo = m_pDistLineInfo[index];
	int lineNum = lineInfo.lineNum;//lineNum = 0 or 5
	if(lineNum == 0)
		return;
//tbd:  dependency--->coordinate of linedata  vs m_pDistLineInfo. lineNum
	linesData* curLineData = &m_pGroups[index];
	int offset=0;
	if(RVS_OVLAY_SUM_LINE_PER_GROUP==curLineData->lineNums) offset=2;
	for(int i=0;i<lineNum;i++)//5 distance mark lines ,20 vertex on each mark line
		CalDistLineScreenPos(lineInfo.dist[i],lineInfo.linePtNum,curLineData->pLineData[i+offset]);//output to pLineData
}


void GlGuideLine::CalScreenPos(float x,float y,float& u,float& v)
{
	float tmpU,tmpV;
	if(m_bDistortCorrection)
		pLCamera->world2Pixel_noDistort(x,y,0.0,tmpU,tmpV);
	else
		pLCamera->world2Pixel(x,y,0.0,tmpU,tmpV);


	u = tmpU * m_iScreenWidth;
	v = (1-tmpV)*m_iScreenHeight;
}

void GlGuideLine::Get2SideLineWorldPos(float dist,float& leftx,float& lefty,float& rightx,float& righty)
{
	float halfWidth = m_fTrackWidth_side/2;
	float curSwa = m_Trajectory.GetVehicleAngle();
	if(eStaticGuideLine == m_GuideLineType||0 == curSwa)
	{
		leftx = -halfWidth;
		lefty = dist;
		rightx = halfWidth;
		righty = dist;

	}
	else// dynamic line
	{

		float l = m_Trajectory.GetVehicleWheelBase();
		float angle = m_Trajectory.GetVehicleAngle();
		lefty = (l*cos(angle)/sin(angle)+halfWidth)*sin(sin(angle)*dist/l);
		leftx = -(l*cos(angle)/sin(angle)+halfWidth)*cos(sin(angle)*dist/l)+l*cos(angle)/sin(angle);

		righty = (l*cos(angle)/sin(angle)-halfWidth)*sin(sin(angle)*dist/l);
		rightx = -(l*cos(angle)/sin(angle)-halfWidth)*cos(sin(angle)*dist/l)+l*cos(angle)/sin(angle);
	}
}

void GlGuideLine::Cal2SideLineScreenPos(float dist,float* left,float* right)
{
	float leftx,lefty,rightx,righty;

	Get2SideLineWorldPos(dist,leftx,lefty,rightx,righty);
  	CalScreenPos(leftx,lefty,left[0],left[1]);
	CalScreenPos(rightx,righty,right[0],right[1]);
	//second times
	//Get2SideLineWorldPos(dist,leftx,lefty,rightx,righty);
	leftx = leftx-DIST_LINE_WIDTH;
	rightx = rightx+DIST_LINE_WIDTH;
	CalScreenPos(leftx,lefty,left[2],left[3]);
	CalScreenPos(rightx,righty,right[2],right[3]);
	
}

void GlGuideLine::CalDistLineScreenPos(float dist,int ptNum,float* pData)
{
	float halfWidth = m_fTrackWidth_dist/2;
	int curSwa = m_Trajectory.GetVehicleSteerWheelAngle();
	float leftx,lefty,rightx,righty;
	leftx = -halfWidth;
	lefty = dist;
	rightx = halfWidth;
	righty = dist;

	float dlefty,dleftx,drightx,drighty;
	float curX=leftx,curY=lefty,curYS = lefty+SIDE_LINE_WIDTH;
	ptNum = ptNum/2;
	float deltaX = (rightx - leftx)/(ptNum-1);
	float deltaY = (righty - lefty)/(ptNum-1);
	int elemIndex = 0;
	float l = m_Trajectory.GetVehicleWheelBase();
	float angle = m_Trajectory.GetVehicleAngle();

	if(eStaticGuideLine == m_GuideLineType||0 == angle)
	{
		for(int i=0;i<ptNum;i++)
		{

			CalScreenPos(curX,curY,pData[elemIndex],pData[elemIndex+1]);
			CalScreenPos(curX,curYS,pData[elemIndex+2],pData[elemIndex+3]);       
			elemIndex += 4;
			curX += deltaX;
		}

	}
	else//dynamic GuideLine
	{
		for(int i=0;i<ptNum;i++)
		{

			dlefty = (l*cos(angle)/sin(angle)-curX)*sin(sin(angle)*curY/l);
			dleftx = -(l*cos(angle)/sin(angle)-curX)*cos(sin(angle)*curY/l)+l*cos(angle)/sin(angle);
			CalScreenPos(dleftx,dlefty,pData[elemIndex],pData[elemIndex+1]);
// 
			drighty = (l*cos(angle)/sin(angle)-curX)*sin(sin(angle)*curYS/l);
			drightx = -(l*cos(angle)/sin(angle)-curX)*cos(sin(angle)*curYS/l)+l*cos(angle)/sin(angle);
// 
// 			CalScreenPos(leftx,lefty,pData[elemIndex],pData[elemIndex+1]);
 			CalScreenPos(drightx,drighty,pData[elemIndex+2],pData[elemIndex+3]); 


			
			//CalScreenPos(curX,curY,pData[elemIndex],pData[elemIndex+1]);

			/*CalScreenPos(curX,curY+SIDE_LINE_WIDTH,pData[elemIndex+2],pData[elemIndex+3]);       */
			elemIndex += 4;
			curX += deltaX;
			curY += deltaY;
		}
		

	}
	
}

void GlGuideLine::CalLineBorder(int index)
{
	int lineNum = m_pGroups[index].lineNums;
	lineBorderOp borderOp;
	linesData* group = &m_pGroups[index];
	float** lineData = group->pLineData;
	int *linePtNum = group->pLinePtNums;
	unsigned short* lineStartIndex = group->pLineBorderStartIndex;
	unsigned short* linePtElemNum = group->pLinePtElemStartNum;
	unsigned short* lineIndexNum = group->pLineIndexElemStartNum;
	float* borderData = group->pBorderData;
	unsigned short* borderIndex = group->pBorderIndex;

	if (index==3)
	{
		for(int i=0;i<lineNum;i++)
		{
			borderOp.CalLine2DBackground(lineData[i],linePtNum[i],m_iLinePixelWidth/*m_fRenderLineWidth*/,lineStartIndex[i],
				&borderData[linePtElemNum[i]],&borderIndex[lineIndexNum[i]]);

		}

	} 
	else
	{
		for(int i=0;i<lineNum;i++)
		{
			//		borderOp.CalLine2DBorder(group->pLineData[i],group->pLinePtNums[i],m_fRenderLineWidth,0,group->pLineBorderData[i],group->pLineBorderIndex[i]);
			borderOp.CalLine2DBorder(lineData[i],linePtNum[i],m_iLinePixelWidth/*m_fRenderLineWidth*/,lineStartIndex[i],
				&borderData[linePtElemNum[i]],&borderIndex[lineIndexNum[i]]);
		}
	}
	

}

void GlGuideLine::SetDistLineParam(float startDis,float deltaDis, float markThickDis/*no use*/,int MarklineNum)
{
	int j=0;
	m_pDistLineInfo = (distLineInfo*)malloc(m_iGroupNum*MarklineNum*sizeof(distLineInfo));

	distLineInfo* pLineInfo;// = &m_pDistLineInfo[0];
    
    for(int i=0;i<(m_iGroupNum);i++)
    {   
        pLineInfo = &m_pDistLineInfo[i];
        pLineInfo->lineNum = MarklineNum;
        pLineInfo->linePtNum = DIST_LINE_PTNUM;

        pLineInfo->dist = (float*)malloc(MarklineNum*sizeof(float)); 
        pLineInfo->dist[0] = startDis;
        startDis=pLineInfo->dist[0]+ deltaDis;

    }
    
}


void GlGuideLine::SetSideLineParam(float* sepDis,float* LSegExtendLength)
{
	m_pSideLineInfo = (sideLineInfo*)malloc(m_iGroupNum*sizeof(sideLineInfo));
	sideLineInfo* pLineInfo;
	for(int i=0;i<m_iGroupNum;i++)
	{
		pLineInfo = &m_pSideLineInfo[i];
		//set start and end distance;
		pLineInfo->startDis = sepDis[2*i];
		pLineInfo->endDis =  sepDis[2*i+1];
		if(pLineInfo->endDis<=pLineInfo->startDis)
		{
			pLineInfo->lineNum = 0;
		}
		else
		{
			pLineInfo->lineNum = 2;
		}
		pLineInfo->LSegExtendLength = LSegExtendLength[i]*m_iScreenWidth;
        pLineInfo->linePtNum = CalSidePtNum(pLineInfo->startDis,pLineInfo->endDis);
		if(fabs(LSegExtendLength[i])<0.0001)
		{
			pLineInfo->bIsLSeg = false;
		}
		else
		{
			pLineInfo->bIsLSeg = true;
			pLineInfo->linePtNum += 1;
		}
	}
}
