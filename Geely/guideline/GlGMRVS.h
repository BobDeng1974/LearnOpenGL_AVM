#ifndef GLGMRVS_H_
#define GLGMRVS_H_


#include "LCommon.h"
#include "trajectory.h"

#include "RVSDefine.h"
#include "GlGuideLine.h"



class GlGMRVS
{
public:
	static GlGMRVS* getInstance();
	~GlGMRVS();
	int Init(int screenWidth, int screenHeight,dynamic_para_t GUIDELINE_PARA);
	int UpdatePositionData(int screenWidth, int screenHeight,dynamic_para_t GUIDELINE_PARA);
	int Update();
	void Update_EnableState();
	

private:
	static GlGMRVS* instance;
	GlGMRVS();

protected:

	LCamera m_LCamera;
	GlGuideLine m_GuideLine;
	int m_iScreenWidth;
	int m_iScreenHeight;
	void*  pEglDisplay;
};

#endif /* GLGMRVS_H_ */
