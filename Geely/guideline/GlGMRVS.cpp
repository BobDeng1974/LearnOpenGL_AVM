/************************************************************************/
/*                                                                      */
/************************************************************************/
#include "GlGMRVS.h"
#include "RVSDefine.h"
GlGMRVS* GlGMRVS::instance = NULL;
GlGMRVS* GlGMRVS::getInstance()
{
	if(!instance)
	{
		instance = new GlGMRVS;
		return instance;
	}
	else
		return instance;
}


GlGMRVS::GlGMRVS()
{

	pEglDisplay = NULL;
}

GlGMRVS::~GlGMRVS()
{

}
int GlGMRVS::Init(int screenWidth, int screenHeight, dynamic_para_t GUIDELINE_PARA)
{
	//window size
	m_iScreenWidth = screenWidth;
	m_iScreenHeight = screenHeight;
	m_GuideLine.SetWindowParam(m_iScreenWidth,m_iScreenHeight);

	//camera parameters  
	cameraPara_t param;
	param.extrinsic = CAM_EXTRINSIC_GM;
	param.intrinsic = CAM_INTRINSIC_GM;
	m_LCamera.init(param);
	
	// calculate GuideLine length 
	float lineLength;
	if (GUIDELINE_PARA.vichelSpeed == 0.0)
	{
		lineLength = RVS_GUIDELINE_FAR_END_DISTANCE;
	}else{
		lineLength = GUIDELINE_PARA.vichelSpeed*GUIDELINE_PARA.forcastTime;
		if (lineLength<RVS_GUIDELINE_FAR_END_DISTANCE)
		{
			lineLength = RVS_GUIDELINE_FAR_END_DISTANCE;
		}else{
			lineLength = RVS_GUIDELINE_FAR_END_DISTANCE + 2.0;
		}
	}


	// lines' group 
	float groupSep[RVS_GUIDELINE_GROUP_NUM*2] = 
	{
    	0.0, RVS_GUIDELINE_FAR_END_DISTANCE,//Group 0 
		RVS_GUIDELINE_L_SEGMENT_DISTANCE,RVS_GUIDELINE_L_SEGMENT_DISTANCE,//group 1 no side line
		RVS_GUIDELINE_FAR_END_DISTANCE,RVS_GUIDELINE_FAR_END_DISTANCE,//group 2 no side line
	};
	// line color
	color3 color[RVS_GUIDELINE_GROUP_NUM] = {RVS_COLOR_RED,RVS_COLOR_GREEN,RVS_COLOR_BLUE};//line color

	float tickLength[RVS_GUIDELINE_GROUP_NUM] =  {0.0,0.0,0.0};//{RVS_GUIDELINE_L_SEGMENT_LENGTH,0.0,0.0};

	//trajectory parameter
	trajectory tmpTrajectory;
	rvs_steer_params vechicleInfo;
	vechicleInfo.wheelBase = GUIDELINE_PARA.wheelbase;
	vechicleInfo.rearAxleOffset = GUIDELINE_PARA.rearspace;
	vechicleInfo.swaDeltaAngle = GUIDELINE_PARA.angle;
	tmpTrajectory.Init(vechicleInfo);

	//static or dynamic line
	int Cfg_guideLine = GUIDELINE_PARA.dynamic;
	if( EGdLStatic == Cfg_guideLine || GUIDELINE_PARA.angle==0)//angle is 0 mean static line
	{
		m_GuideLine.InitLineParam(RVS_GUIDELINE_GROUP_NUM,groupSep,tickLength,color,
			RVS_GUIDELINE_MARKER_STARTDIS,RVS_OVLAY_DELTA_DIST_MARKS,RVS_OVLAY_THICK_DIST_MARKS,
			RVS_OVLAY_NUM_DIST_MARKS,RVS_OVLAY_TRACK_WIDTH,	RVS_GUIDELINE_WIDTH,&m_LCamera,tmpTrajectory);

	}
	else if( EGdLDynamic == Cfg_guideLine)
	{

		m_GuideLine.InitLineParam(RVS_GUIDELINE_GROUP_NUM,groupSep,tickLength,color,
			RVS_GUIDELINE_MARKER_STARTDIS,RVS_OVLAY_DELTA_DIST_MARKS,RVS_OVLAY_THICK_DIST_MARKS,
			RVS_OVLAY_NUM_DIST_MARKS,RVS_OVLAY_TRACK_WIDTH,RVS_GUIDELINE_WIDTH,&m_LCamera,tmpTrajectory,eDynamicGuideLine,false);
	}
	m_GuideLine.SetSWAUpdatePrecision(RVS_STEERING_WHEEL_DELTA);

	return 0;
}

int GlGMRVS::UpdatePositionData(int screenWidth, int screenHeight, dynamic_para_t GUIDELINE_PARA){

	// calculate GuideLine length 
	float lineLength;
	if (GUIDELINE_PARA.vichelSpeed == 0.0)
	{
		lineLength = RVS_GUIDELINE_FAR_END_DISTANCE;
	}else{
		lineLength = GUIDELINE_PARA.vichelSpeed*GUIDELINE_PARA.forcastTime;
		if (lineLength<RVS_GUIDELINE_FAR_END_DISTANCE)
		{
			lineLength = RVS_GUIDELINE_FAR_END_DISTANCE;
		}else{
			lineLength = RVS_GUIDELINE_FAR_END_DISTANCE + 2.0;
		}
	}


	// lines' group 
	float groupSep[RVS_GUIDELINE_GROUP_NUM*2] = 
	{
		0.0, RVS_GUIDELINE_FAR_END_DISTANCE,//Group 0 
		RVS_GUIDELINE_L_SEGMENT_DISTANCE,RVS_GUIDELINE_L_SEGMENT_DISTANCE,//group 1 no side line
		RVS_GUIDELINE_FAR_END_DISTANCE,RVS_GUIDELINE_FAR_END_DISTANCE,//group 2 no side line
	};
	// line color
	color3 color[RVS_GUIDELINE_GROUP_NUM] = {RVS_COLOR_RED,RVS_COLOR_GREEN,RVS_COLOR_BLUE};//line color

	float tickLength[RVS_GUIDELINE_GROUP_NUM] =  {0.0,0.0,0.0};//{RVS_GUIDELINE_L_SEGMENT_LENGTH,0.0,0.0};

	//trajectory parameter
	trajectory tmpTrajectory;
	rvs_steer_params vechicleInfo;
	vechicleInfo.wheelBase = GUIDELINE_PARA.wheelbase;
	vechicleInfo.rearAxleOffset = GUIDELINE_PARA.rearspace;
	vechicleInfo.swaDeltaAngle = GUIDELINE_PARA.angle;
	tmpTrajectory.Init(vechicleInfo);

	//static or dynamic line
	int Cfg_guideLine = GUIDELINE_PARA.dynamic;
	if( EGdLStatic == Cfg_guideLine || GUIDELINE_PARA.angle==0)//angle is 0 mean static line
	{
		m_GuideLine.UpdateLineData(RVS_GUIDELINE_GROUP_NUM,groupSep,tickLength,color,
			RVS_GUIDELINE_MARKER_STARTDIS,RVS_OVLAY_DELTA_DIST_MARKS,RVS_OVLAY_THICK_DIST_MARKS,
			RVS_OVLAY_NUM_DIST_MARKS,RVS_OVLAY_TRACK_WIDTH,	RVS_GUIDELINE_WIDTH,&m_LCamera,tmpTrajectory);

	}
	else if( EGdLDynamic == Cfg_guideLine)
	{

		m_GuideLine.UpdateLineData(RVS_GUIDELINE_GROUP_NUM,groupSep,tickLength,color,
			RVS_GUIDELINE_MARKER_STARTDIS,RVS_OVLAY_DELTA_DIST_MARKS,RVS_OVLAY_THICK_DIST_MARKS,
			RVS_OVLAY_NUM_DIST_MARKS,RVS_OVLAY_TRACK_WIDTH,RVS_GUIDELINE_WIDTH,&m_LCamera,tmpTrajectory,eDynamicGuideLine,false);
	}
	//m_GuideLine.SetSWAUpdatePrecision(RVS_STEERING_WHEEL_DELTA);
	return 0;

}
int GlGMRVS::Update()
{
	glClearColor(1.0, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glViewport(0,0,m_iScreenWidth,m_iScreenHeight);
	Update_EnableState();
	return 0;
}


void GlGMRVS::Update_EnableState()
{
	PVRTMat4 mat = PVRTMat4::Identity();
	m_GuideLine.Render(mat);

}
