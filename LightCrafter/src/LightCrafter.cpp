#include "LightCrafter.h"


LightCrafter::LightCrafter(void)
{
	LCR_Commander lcr_commander;
	Commander = &lcr_commander;
	IsConnected = false;
}


LightCrafter::~LightCrafter(void)
{

}

int LightCrafter::start()
{
	return Commander->Connect_LCR(LCR_Default_IP,LCR_Default_PORT);
}

void LightCrafter::stop(void)
{
	Commander ->Disconnect_LCR();
}

LCR_Byte_Zero_Packet LightCrafter::LCR_Component_Revision(LCR_Revision device, string* version)
{
	if(IsConnected)
		Commander -> LCR_Component_Revision(device, version);
	// initlize the command

	//send the command

	//recieve the response
	LCR_Byte_Zero_Packet response = Success;
	return response;
}