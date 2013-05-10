#ifndef _LCR_COMMAND_PACKETIZER_H_
#define _LCR_COMMAND_PACKETIZER_H_



#include <stdio.h>
#include <string.h>
#include "LCR_Common.h"

using namespace std;

class Command_Packetizer
{
private:
	void InitilizeCommandBuffer(uint8* command, uint8 packetType, uint16 commandId, uint8 flags, uint16 payLoadLength);
	void LoadPayLoadInBuffer (uint8 * command, uint8* payLoad, uint16 payLoadLength);
	void CalculateCheckSum(uint8* command, uint16 size);

public:
	Command_Packetizer(void);
	~Command_Packetizer(void);

	uint8* CreateCommand(uint8 packetType, uint16 commandId, uint8 flags, uint16 payLoadLength, uint8* payLoad);

};


#endif

