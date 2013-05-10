#include "LCR_Commander.h"

LCR_Commander::LCR_Commander(void)
{
	tcpClient = new Tcp();
	connectedSocket = -1;
	
	packetizer = new Command_Packetizer();
}

LCR_Commander::~LCR_Commander(void)
{
	if(tcpClient != NULL)
		delete tcpClient;

	if(packetizer != NULL)
		delete packetizer;


}

int LCR_Commander:: Connect_LCR(string ipAddress, string port)
{
	if(connectedSocket>0)
		return -2;

	for(int i = 0; i<ConnectionAttempts; ++i)
	{ 
		int socket = tcpClient->TCP_Connect(ipAddress, port);
		if(socket >0)
		{
			connectedSocket = socket;
			return 0;
		}
			
	}

	return -1;
	
}

LCR_Byte_Zero_Packet LCR_Commander::LCR_LOAD_STATIC_IMAGE(uint8 * image,int byteCount)
{
	if(connectedSocket < 0)
	{
		// not connected
	}

	// read command
	Packet_Type pType = Host_Write;

	//commandId
	CommandIds cmdId = StaticImage;

	//flags
	Command_Flags flag = DataComplete;

	//create the command
	uint8* command = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, byteCount, image);

	LCR_Byte_Zero_Packet operationStatus;

	int totalLength = HEADER_SIZE + byteCount + CHECKSUM_SIZE;
	int sendResult = tcpClient->TCP_Send(connectedSocket,command,totalLength);


	LCR_Byte_Zero_Packet returnStatus = Success;
	return returnStatus;

	//disconnect

}

LCR_Byte_Zero_Packet LCR_Commander::SetDisplayMode(DisplayMode displayMode)
{
	if(connectedSocket < 0)
	{
		// not connected
	}

	// read command
	Packet_Type pType = Host_Write;

	//commandId
	CommandIds cmdId = CurrentDisplayMode;

	//flags
	Command_Flags flag = DataComplete;


	uint8 dplMode = (uint8)displayMode;
	
	uint8 * payLoad;
	payLoad = &dplMode;

	uint16 byteCount = 0x1;


	//create the command
	uint8* command = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, byteCount, payLoad);

	LCR_Byte_Zero_Packet operationStatus;

	int totalLength = HEADER_SIZE + byteCount + CHECKSUM_SIZE;
	int sendResult = tcpClient->TCP_Send(connectedSocket,command,totalLength);


	LCR_Byte_Zero_Packet returnStatus = Success;
	return returnStatus;
}


LCR_Byte_Zero_Packet LCR_Commander::LCR_Component_Revision(LCR_Revision device, string* version)
{
	if(connectedSocket < 0)
	{
		// not connected
	}

	// read command
	Packet_Type pType = Host_Read_Command;

	//commandId
	CommandIds cmdId = VersionString;

	//flags
	Command_Flags flag = DataComplete;

	//payLoadLength
	uint16 payLoadLength = 0x0001;

	//create payLoad
	uint8 * payLoad = new uint8[1];

	//load the payLoad
	payLoad[0] = (uint8) device; 

	//create the command
	uint8* command = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, payLoadLength, payLoad);

	//send the command
	LCR_Byte_Zero_Packet operationStatus;

	int totalLength = HEADER_SIZE + payLoadLength + CHECKSUM_SIZE;
	int sendResult = tcpClient->TCP_Send(connectedSocket,command,totalLength);

	// read the result code
	delete[] command; //clean up command

	// if send is successful, get a response

	//allocate buffer
	

	int returnPayLoadBytes = 32;
	uint16 * recieveBuffer  = new uint16[returnPayLoadBytes];
	int recieveResult = tcpClient ->TCP_Receive(connectedSocket,(unsigned char*)recieveBuffer, returnPayLoadBytes);


	//check first bit for error

	switch(recieveBuffer[0])
	{
		case 0x00: //LCR_System_Busy
			break;
		
		case 0x01: //LCR_Error
			{
				//get error from payload
				switch(recieveBuffer[HEADER_SIZE])
				{

				}


			}
			break;
		
		case 0x02:  //Host_Write
			break;
		
		case 0x03: //LCR_Write_Response
			break;
		
		case 0x04: //Host_Read_Command
			break;

		case 0x05: //LCR_ReponsePacket
			break;
		
		default:
			break;

	}

	//Packet_Type 
	



	//delete[] recieveBuffer;


	//string version((char *) buffer);

    operationStatus = Success;
	return operationStatus;
}

void LCR_Commander::Disconnect_LCR(void)
{
	if(connectedSocket >0)
		tcpClient->TCP_Disconnect(connectedSocket);
}