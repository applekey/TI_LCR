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

bool LCR_Commander:: Connect_LCR(string ipAddress, string port)
{
	if(connectedSocket>0)
	{
		cout << "LCR is already conncted.";
		return false;
	}

	for(int i = 0; i<ConnectionAttempts; ++i)
	{ 
		int socket = tcpClient->TCP_Connect(ipAddress, port);
		if(socket >0)
		{
			connectedSocket = socket;
			cout << "LCR connected.";
			return true;
		}
			
	}

	cout << "LCR connect attemps exceede maxium.";
	return false;
	
}

bool LCR_Commander::LCR_LOAD_STATIC_IMAGE(uint8 * image,int byteCount)
{
	if(connectedSocket < 0)
	{
		cout << "LCR has not being connected";
		return false;
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

	if(sendResult ==SOCKET_ERROR)
	{
		cout <<"TCP SOCKET ERROR";
		return false;
	}

	
	return true;
}

bool LCR_Commander::SetDisplayMode(DisplayMode displayMode)
{
	if(connectedSocket < 0)
	{
      cout << "LCR has not being connected";
	  return false;
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


	if(sendResult ==SOCKET_ERROR)
	{
		cout <<"TCP SOCKET ERROR";
		return false;
	}

	return true;
}

bool LCR_Commander::Disconnect_LCR(void)
{
	if(connectedSocket >0)
	{
		int status = tcpClient->TCP_Disconnect(connectedSocket);
		if(status == 0)
		{
			return true;
		}
		else
		{
			cout <<"Cannot Disconnect LCR";
			return false;
		}

	}
	else
	{
		cout <<"Cannot Disconnect LCR because no LCR Connected";
		return false;
	}
		
}

