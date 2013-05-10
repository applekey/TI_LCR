#ifndef _LIGHTCRAFTER_H_
#define _LIGHTCRAFTER_H_

using namespace std;
#include <cv.h> 
#include <string>
#include "LCR_Commander.h"
#include "LCR_Common.h"
#include <iostream>

#define LCR_Default_IP			"192.168.1.100"
#define LCR_Default_PORT		"21845"

class LightCrafter
{
private:
	bool IsConnected;
	LCR_Commander*  Commander;
public:
	LightCrafter(void);
	~LightCrafter(void);

	int start();
	void stop();

	//upload a static image, static image has to be a bmp at 608 x 684


	//void LCR_Load_Static_Image(cv::Mat image);
	bool ProjectImage(cv::Mat image);


	int LightCrafter::GetHeight(void);

	int LightCrafter::GetWidth(void);


	//Lcr Commands

	//versioning
	LCR_Byte_Zero_Packet LCR_Component_Revision(LCR_Revision device, string* version);

	//// display mode
 //   LCT_Error_Packets LCR_CMD_SetDisplayMode(LCR_DisplayMode_t Mode); 
	//LCR_DisplayMode_t LCR_CMD_GetDisplayMode(void);

	////power mode
 //   LCT_Error_Packets LCR_CMD_SetPowerMode(LCR_PowerMode_t Mode);
 //   LCR_PowerMode_t LCR_CMD_GetPowerMode(void);

	//// test pattern
 //   LCT_Error_Packets LCR_CMD_SetTestPattern(LCR_TestPattern_t TestPtn);
 //   LCR_TestPattern_t LCR_CMD_GetTestPattern(void);

	////led settings
 //   LCT_Error_Packets LCR_CMD_SetLEDCurrent(LCR_LEDCurrent_t *LEDSetting);
 //   LCT_Error_Packets LCR_CMD_GetLEDCurrent(LCR_LEDCurrent_t *LEDSetting);

	////display static
 //   LCT_Error_Packets LCR_CMD_DisplayStaticImage(char const *fileNameWithPath);
 //   LCT_Error_Packets LCR_CMD_DisplayStaticColor(uint32 Color);

	////display settings
 //   LCT_Error_Packets LCR_CMD_SetDisplaySetting(LCR_DisplaySetting_t const *Setting);
 //   LCT_Error_Packets LCR_CMD_GetDisplaySetting(LCR_DisplaySetting_t *Setting);

	////video settings
 //   LCT_Error_Packets LCR_CMD_SetVideoSetting(LCR_VideoSetting_t const *Setting);
 //   LCT_Error_Packets LCR_CMD_GetVideoSetting(LCR_VideoSetting_t *Setting);

	////video mode
 //   LCT_Error_Packets LCR_CMD_SetVideoMode(LCR_VideoModeSetting_t *Setting);
 //   LCT_Error_Packets LCR_CMD_GetVideoMode(LCR_VideoModeSetting_t *Setting);

	////InterLeavePatterOrder
 //   LCT_Error_Packets LCR_CMD_SetInterleavePatternOrder(uint8 NumPatterns, uint8 const *PatternOrder);
 //   LCT_Error_Packets LCR_CMD_GetInterleavePatternOrder(uint8 *NumPatterns, uint8 *PatternOrder);

	////Patern sequence settings
 //   LCT_Error_Packets LCR_CMD_SetPatternSeqSetting(LCR_PatternSeqSetting_t const *Setting);
 //   LCT_Error_Packets LCR_CMD_GetPatternSeqSetting(LCR_PatternSeqSetting_t *Setting);

	//// pattern
 //   LCT_Error_Packets LCR_CMD_DefinePatternBMP(LCR_PatternCount_t PatternNum, char const *fileNameWithPath);
 //   //ErrorCode_t LCR_CMD_DefinePatternPix(LCR_PatternCount_t PatternNum, uint32 X, uint32 Y,
 //   //														uint8 *Data, uint32 Count);
 //   LCT_Error_Packets LCR_CMD_ReadPattern(LCR_PatternCount_t PatternNum, char *fileName);

 //   LCT_Error_Packets LCR_CMD_StartPatternSeq(uint8 Start);

 //   LCT_Error_Packets LCR_CMD_AdvancePatternSeq(void);

	////get camera trigger
 //   LCT_Error_Packets LCR_CMD_GetCamTriggerSetting(LCR_CamTriggerSetting_t *Setting);
 //   LCT_Error_Packets LCR_CMD_SetCamTriggerSetting(LCR_CamTriggerSetting_t *Setting);

 //   //ErrorCode_t LCR_CMD_DisplayPattern(BOOL Enable, LCR_PatternCount_t PtnNumber);
 //   //
 //   //ErrorCode_t LCR_CMD_CaptureImage(Image_t *Image, LCR_CaptureMode_t Mode);
 //   //
 //   LCT_Error_Packets LCR_CMD_SaveSolution(char *SolutionName);
 //   LCT_Error_Packets LCR_CMD_GetSolutionNames(uint8 *Count, uint8 *DefaultSolution, char *SolutionName);

 //   LCT_Error_Packets LCR_CMD_ManageSolution(LCR_SolutionCommand_t Cmd, char *SolutionName);
};

#endif