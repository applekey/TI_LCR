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

int LightCrafter::GetHeight(void)
{
  //static image has to be a bmp at 608 x 684
  return 684;
}

int LightCrafter::GetWidth(void)
{
  //static image has to be a bmp at 608 x 684
  return 608;
}


bool LightCrafter::ProjectImage(cv::Mat image)
{

	bool connected = Commander ->Connect_LCR(LCR_Default_IP,LCR_Default_PORT);

	if(!connected)
	{
	  cout<<"Cannot Connect to LCR.";
	  return false;
	}
	else
	{
	  cout<<"Connected To LCR.";
	  IsConnected = true;
	}
	  
	


	DisplayMode staticImg = StaticImageMode;
	bool modeChanged = Commander -> SetDisplayMode(staticImg);

	if(!modeChanged)
	{
	  cout<<"Could not change display mode to Static.";
	  return false;
	}
	else
	  cout<<"Mode changed to Static Display.";


	bool imageLoaded  = Commander ->LCR_LOAD_STATIC_IMAGE( image.data, size);

	if(!imageLoaded)
	{
	  cout<<"Could not load static 608 x 684 24bit static bmp.";
	  return false;
	}
	else
	  cout<<"Static Image Loaded.";

	bool disconnected = Commander ->Disconnect_LCR();

	if(!disconnected)
	{
	  cout<< "Could not disconnect from the LCR.";
	  return false;
	}
	else
	{
	  cout<<"Disconnected from LCR.";
	  IsConnected = false;
	}
	  

	// everthing went smoothly

	return true;
 
	






 /* FILE *fp;
  fp = fopen(imageLocation.c_str(), "rb");
  
  if(fp == NULL)
  {
      return -1;
  }

  fseek (fp , 0 , SEEK_END);
  long  lSize = ftell (fp);
  rewind (fp);

  

  uint8* imageBuffer = (uint8*) malloc (sizeof(uint8)*lSize);
   int result = fread (imageBuffer,1,lSize,fp);
   fclose(fp);*/
 //  // Check the image and make sure it is the right size
 // if( image.cols != GetWidth() || image.rows != GetHeight() )
 // {
	//cout << "Incorrect image specified. Check size and channel count\n";
	//return false;
 // }

 // //auto binaryImage = _Convert2BinaryImage( image );
 // auto byteCount = GetWidth() * GetHeight()*24 / 8;

  
  //connect
   if(!IsConnected)
      Commander ->Connect_LCR(LCR_Default_IP,LCR_Default_PORT);

   


   int size = 608*684*24; 

 
  /* if(!operation1)
  {
	  Commander ->Disconnect_LCR();
	  return false;
  }*/
  
   bool operation  = Commander ->LCR_LOAD_STATIC_IMAGE( image.data, size);

   
 // DisplayMode staticImg = InternalTestPatter;
 // 
 // bool operation1;
 // operation1 = Commander -> SetDisplayMode(staticImg);
 //
 // /*if(!operation)
 // {
	//  Commander ->Disconnect_LCR();
	//  return false;
 // }*/

 // staticImg = StaticImageMode;
 // 
 // bool operation2;
 // operation2 = Commander -> SetDisplayMode(staticImg);
 ///*  if(!operation2)
 // {
	//  Commander ->Disconnect_LCR();
	//  return false;
 // }*/
 // 
 

  
   //disconnect
  bool disconnect = Commander ->Disconnect_LCR();
 
  if(!disconnect)
  {
	  return false;
  }
  IsConnected = false;
  return true;
}


unique_ptr<uint8[]> LightCrafter::_Convert2BinaryImage(cv::Mat byteImage)
{
  int binaryImageSize = GetWidth() * GetHeight() / 8;
  auto binaryImage = unique_ptr<uint8[]>( new uint8[binaryImageSize] );

  uint8 packedByte = 0;
  int currentByteIndex = 0;
  int currentBitIndex = 0;
  
  for(int row = 0; row < byteImage.rows; ++row)
  {
	for(int col = 0; col < byteImage.cols; ++col)
	{
	  packedByte = byteImage.at<uchar>(row, col) >= 128 ? packedByte | MSB_HIGH : packedByte & MSB_LOW;
	 
	  currentBitIndex = (currentBitIndex + 1) % 8;

	  if( !currentBitIndex )
	  {
	    binaryImage[currentByteIndex] = packedByte; 
		currentByteIndex++;
	  }
	  else
	  {
		packedByte = packedByte >> 1;
	  }
	}
  }

  return binaryImage;
}


