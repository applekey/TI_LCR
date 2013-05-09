#include "LCR_Command_Packetizer.h"

Command_Packetizer::Command_Packetizer(void)
{
}

Command_Packetizer::~Command_Packetizer(void)
{
}




uint8* Command_Packetizer::CreateCommand(uint8 packetType, uint16 commandId, uint8 flags, uint16 payLoadLength, uint8* payLoad)
{
	uint8 * command = new uint8[HEADER_SIZE+MAX_PACKET_SIZE+CHECKSUM_SIZE]; 
	InitilizeCommandBuffer(command, packetType,commandId,flags,payLoadLength);
	LoadPayLoadInBuffer(command, payLoad, payLoadLength );

	return command;

}


void Command_Packetizer::InitilizeCommandBuffer(uint8* command, uint8 packetType, uint16 commandId, uint8 flags, uint16 payLoadLength)
{
	command[0] = packetType;
	command[1] = (commandId >> 8) & 0xFF;
	command[2] = commandId & 0xFF;
	command[3] = flags;
	command[4] = (payLoadLength >> 8) & 0xFF;
	command[5] = payLoadLength & 0xFF;
}

void Command_Packetizer::LoadPayLoadInBuffer(uint8 * command, uint8* payLoad, uint16 payLoadLength)
{
	if(command == NULL)
		return;

	for( int i =0; i< payLoadLength;i++)
	{
		memcpy ((void *) command[6+i],payLoad,sizeof(uint8));
	}
}	

void Command_Packetizer::CalculateCheckSum(uint8* command, uint16 payLoadLength)
{
    int sum = 0;

    for(int i = 0; i < payLoadLength + HEADER_SIZE; i++)
    {
        sum += command[i];
    }
	command[HEADER_SIZE +payLoadLength] = (uint8)(sum & 0xFF);
}
