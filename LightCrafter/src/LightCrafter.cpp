#include "LightCrafter.h"


LightCrafter::LightCrafter(void)
{
	Commander = new LCR_Commander();
	IsConnected = false;
}


LightCrafter::~LightCrafter(void)
{
	if(Commander != NULL)
		delete Commander;
}

int LightCrafter::start()
{
	return Commander->Connect_LCR(LCR_Default_IP,LCR_Default_PORT);
}

void LightCrafter::stop(void)
{
	Commander ->Disconnect_LCR();
}

int LightCrafter::GetHeight(void)
{
  return 608;
}

int LightCrafter::GetWidth(void)
{
  return 684;
}


bool LightCrafter::ProjectImage(uint8* image)
{

 //  // Check the image and make sure it is the right size
 // if( image.cols != GetWidth() || image.rows != GetHeight() )
 // {
	//cout << "Incorrect image specified. Check size and channel count\n";
	//return false;
 // }

 // auto binaryImage = _Convert2BinaryImage( image );
 // auto byteCount = GetWidth() * GetHeight() / 8;

  //connect

  Commander ->Connect_LCR(LCR_Default_IP,LCR_Default_PORT);

  uint8* binaryImage = new uint8[4];
  binaryImage[0] = 0x12;
  binaryImage[1] = 0x34;
  binaryImage[2] = 0x56;
  binaryImage[3] = 0x78;


  Commander ->LCR_LOAD_STATIC_IMAGE( binaryImage, 4);
  
  DisplayMode staticImg = StaticImageMode;
  Commander ->SetDisplayMode(staticImg);

  //disconnect
  Commander ->Disconnect_LCR();


 /* if ( !_CheckLogError( DLP_Img_DownloadBitplanePatternToExtMem( binaryImage.get(), byteCount, 0 ) ) )
  {
	cout << "Unable to transfer images to the projector\n";
	return false;
  }

  if( !_CheckLogError( DLP_Display_DisplayPatternManualForceFirstPattern( ) ) )
  {
	cout << "Unable to display the first pattern\n";
	return false;
  }

  if( !_CheckLogError( DLP_Display_DisplayPatternAutoStepRepeatForMultiplePasses( ) ) )
  {
	cout << "Unable to project pattern\n";
	return false;
  }*/

  return true;
}

//LCR_Byte_Zero_Packet LightCrafter::LCR_Component_Revision(LCR_Revision device, string* version)
//{
//	if(IsConnected)
//		Commander -> LCR_Component_Revision(device, version);
//	// initlize the command
//
//	//send the command
//
//	//recieve the response
//	LCR_Byte_Zero_Packet response = Success;
//	return response;
//}