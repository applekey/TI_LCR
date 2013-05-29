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
	Command_Flags flag = Beginning;

	
	//create the header packet
	uint8* commandHeader = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, MAX_PAYLOAD_SIZE, image);

	LCR_Byte_Zero_Packet operationStatus;

	int totalLength = MAX_PACKET_SIZE;
	
	// header packet
	int headerSend = tcpClient->TCP_Send(connectedSocket,commandHeader,totalLength);

	

	if(headerSend ==SOCKET_ERROR)
	{

		cout <<"TCP SOCKET ERROR";
		return false;
	}

	// recieve the response 

	int headerRecieve = tcpClient->TCP_Receive(connectedSocket, commandHeader,MAX_PAYLOAD_SIZE);

	if(headerRecieve == SOCKET_ERROR)
	{
	  cout<<"TCP SOCKET ERROR";
	  return false;
	}

	/*if(!RecieveSuccessful(commandHeader))
	{
	  return false;
	}*/

	delete[] commandHeader;

	int NumberOfIntermediatePackets = byteCount/MAX_PAYLOAD_SIZE+1 -2;
	// intermediate packet
	 
	flag = Intermediate; // change the flag to intermediate

	for(int i = 0;i<NumberOfIntermediatePackets;i++)
	{
	  uint8* commandIntermediate = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, MAX_PAYLOAD_SIZE, image+(MAX_PAYLOAD_SIZE)*(i+1));

	  int intermediateSend = tcpClient->TCP_Send(connectedSocket,commandIntermediate,totalLength);

	  if(intermediateSend == SOCKET_ERROR)
	  {
		cout <<"TCP SOCKET ERROR";
		return false;
	  }

	  // recieve the response 

	  int headerRecieve = tcpClient->TCP_Receive(connectedSocket, commandHeader,MAX_PACKET_SIZE);

	  if(headerRecieve == SOCKET_ERROR)
	  {
		cout<<"TCP SOCKET ERROR";
		return false;
	  }

	 /* if(!RecieveSuccessful(commandHeader))
	  {
		return false;
	  }*/

	  delete[] commandIntermediate;

	}

	// final packets
	flag = End;
	int remainingBytes = -(byteCount - ((MAX_PAYLOAD_SIZE)*(NumberOfIntermediatePackets+2)));

	uint8* commandFinal = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, remainingBytes, image+MAX_PAYLOAD_SIZE*NumberOfIntermediatePackets);
	
	int finalSend = tcpClient->TCP_Send(connectedSocket,commandFinal,remainingBytes); 

	

	if(finalSend == SOCKET_ERROR)
	{
	  cout <<"TCP SOCKET ERROR";
	  return false;
	}

	/* int finalRecieve = tcpClient->TCP_Receive(connectedSocket, commandFinal,MAX_PACKET_SIZE);

	  if(finalRecieve == SOCKET_ERROR)
	  {
		cout<<"TCP SOCKET ERROR";
		return false;
	  }*/

	  delete[] commandFinal;

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

	uint16 payLoadLength = 0x1;


	//create the command
	uint8* command = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, payLoadLength, payLoad);

	

	LCR_Byte_Zero_Packet operationStatus;

	int totalLength = HEADER_SIZE + payLoadLength + CHECKSUM_SIZE;
	int sendResult = tcpClient->TCP_Send(connectedSocket,command,totalLength);

	if(sendResult ==SOCKET_ERROR)
	{
		cout <<"TCP SOCKET ERROR";
		delete[] command;
		return false;
	}

	int headerRecieve = tcpClient->TCP_Receive(connectedSocket, command,MAX_PACKET_SIZE);

	  if(headerRecieve == SOCKET_ERROR)
	  {
		delete[] command;
		cout<<"TCP SOCKET ERROR";
		return false;
	  }

	 

	/*  if(!RecieveSuccessful(command))
	  {
		return false;
	  }*/


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

bool LCR_Commander::RecieveSuccessful(uint8* recieveBuffer)
{
  return true;

  if(recieveBuffer == NULL)
	return false;

  //read the first bit

  int errorInPayLoad = 0;

  uint8 firstBit = recieveBuffer[0];
  switch(firstBit)
  {
	case LCR_System_Busy:
	{
	  cout<< "Lcr system busy";
	  return false;
	}
	case LCR_Error:
	{
	 errorInPayLoad = 1;
	  
	}

	case Host_Write:
	{
	  cout<< "Lcr system busy";
	  return false;
	}

	case LCR_Write_Response:
	{
	  cout<< "Lcr write response";
	  return true;
	}

	case Host_Read_Command:
	{
	  cout<< "Lcr read response";
	  return true;
	}

	case LCR_ReponsePacket:
	{
	  cout<< "Lcr system busy";
	  return false;
	}

	if(errorInPayLoad == 0)
	  return true;
	else // process the error in the payload
	{
	  return false;
	}

  }


}



 // switch( firstBit)
 // {
 // case UnknownError:
	//{
	//  cout<< "Recieve Unkown Error";
	//  return false;
	//}
	// case InvalidCommand:
	//{
	//  cout<< "Recieve Unkown Error";
	//  return false;
	//}
	// case InvalidParameter:
	//{
	//  cout<< "Recieve Unkown Error";
	//  return false;
	//}
	// case OutofMemory:
	//{
	//  cout<< "Recieve Unkown Error";
	//  return false;
	//}
	// case HardwareFailure:
	//{
	//  cout<< "Recieve Unkown Error";
	//  return false;
	//}
	// case NotInitlized:
	//{
	//  cout<< "Recieve Unkown Error";
	//  return false;
	//}

	// case ReferenceError:
	//{
	//  cout<< "Recieve Unkown Error";
	//  return false;
	//}

	// case CheckSumError:
	//{
	//  cout<< "Recieve Unkown Error";
	//  return false;
	//}

	// case PacketFormatError:
	//{
	//  cout<< "Recieve Unkown Error";
	//  return false;
	//}

	// case CommandContinuation:
	//{
	//  cout<< "Recieve Unkown Error";
	//  return false;
	//}

