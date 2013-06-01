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
	for(int i = 0; i<ConnectionAttempts; ++i)
	{ 
		int socket = tcpClient->TCP_Connect(ipAddress, port);
		if(socket >0)
		{
			connectedSocket = socket;
			return true;
		}
	}

	return false;

}

bool LCR_Commander::SendLCRWriteCommand(uint8* command, long packetSize, int packetNumber)
{
	if(connectedSocket<0)
	{
		cout << "No connected Socket.\n";
		return false;
	}

	//send the  Packet
	int send = tcpClient->TCP_Send(connectedSocket,command,packetSize);
	
	if(send ==SOCKET_ERROR)
	{
		if(packetNumber != -1)
			cout <<"PACKET SEND, NUMBER:"<<packetNumber<<" with length:"<<packetSize<<" has recieved a socket error.\n";
		else
			cout <<"PACKET SEND, with length:"<<packetSize<<" has recieved a socket error.\n";
		return false;
	}


	//-----------Recieve  Packet----------------------------
	uint8 recieve[HEADER_SIZE];
	int rec = tcpClient->TCP_Receive(connectedSocket,recieve,HEADER_SIZE);
	if(rec ==SOCKET_ERROR)
	{
		if(packetNumber != -1)
			cout <<"PACKET Recieve Header, NUMBER:"<<packetNumber<<" with length:"<<packetSize<<" has recieved a socket error.\n";
		else
			cout <<"PACKET Recieve Header, with length:"<<packetSize<<" has recieved a socket error.\n";
		return false;
	}

	int payLoadLength = recieve[5]<<16+recieve[4];
	int sizeToRecieve = HEADER_SIZE+payLoadLength+CHECKSUM_SIZE;
	
	uint8* recievePayLoad = new uint8[sizeToRecieve];
	int recPayload = tcpClient->TCP_Receive(connectedSocket,recievePayLoad,sizeToRecieve);
	if(recPayload ==SOCKET_ERROR)
	{
		if(packetNumber != -1)
			cout <<"PACKET Recieve PayLoad, NUMBER:"<<packetNumber<<" with length:"<<packetSize<<" has recieved a socket error.\n";
		else
			cout <<"PACKET Recieve PayLoad, with length:"<<packetSize<<" has recieved a socket error.\n";
		return false;
	}
	delete[] recievePayLoad;

}


bool LCR_Commander::LCR_LOAD_STATIC_IMAGE(uint8 * image,int byteCount)
{
	if(connectedSocket < 0)
	{
		cout << "No connected Socket.\n";
		return false;
	}

	Packet_Type pType = Host_Write; // read command 
	CommandIds cmdId = StaticImage; //commandId
	Command_Flags flag = Beginning; //flags


			//-------------First Packet----------------------------------------------
	uint8* commandHeader = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, MAX_PAYLOAD_SIZE, image);

	//send the first Packet
	int headerSend = tcpClient->TCP_Send(connectedSocket,commandHeader,MAX_PACKET_SIZE);
	//delete[] commandHeader;
	delete[] commandHeader;
	if(headerSend ==SOCKET_ERROR)
	{
		cout <<"Static Image Load, First Packet send error.\n";
		
		return false;
	}


	//-----------Recieve First Packet----------------------------
	uint8 recieve[HEADER_SIZE];
	int recF = tcpClient->TCP_Receive(connectedSocket,recieve,HEADER_SIZE);
	if(recF ==SOCKET_ERROR)
	{
		cout <<"Static Image Load, First Packet Recieve error.\n";
		return false;
	}

	int payLoadLength = recieve[5]<<16+recieve[4];
	int sizeToRecieve = HEADER_SIZE+payLoadLength+CHECKSUM_SIZE;
	
	uint8* recievePayLoad = new uint8[sizeToRecieve];
	int recFP = tcpClient->TCP_Receive(connectedSocket,recievePayLoad,sizeToRecieve);
	if(recFP ==SOCKET_ERROR)
	{
		delete[] recievePayLoad;
		cout <<"Static Image Load, First Packet Recieve error.\n";
		return false;
	}
	delete[] recievePayLoad;
    

	//analyize the payload

	//blahblah not really need to do this
	

	//-------------Intermediate Packets---------------------------------------------
	flag = Intermediate; // change the flag to intermediate

	int NumberOfIntermediatePackets = 37;

	for(int i = 0;i<NumberOfIntermediatePackets;i++)
	{
	  uint8* commandIntermediate = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, MAX_PAYLOAD_SIZE, image+(MAX_PAYLOAD_SIZE)*(i+1));

	  int intermediateSend = tcpClient->TCP_Send(connectedSocket,commandIntermediate,MAX_PACKET_SIZE);
	  delete[] commandIntermediate;

	  if(intermediateSend == SOCKET_ERROR)
	  {
		cout <<"Static Image Load, the intermediate packet:"<<i<<"has a send error.\n";
		return false;
	  }
	 

			//-----------Recieve Intermediate Packet Packet----------------------------
	uint8 recieve[HEADER_SIZE];
	int recF = tcpClient->TCP_Receive(connectedSocket,recieve,HEADER_SIZE);
	if(recF ==SOCKET_ERROR)
	{
		cout <<"Static Image Load, Intermediate Packet:"<<i<< "Recieve error.\n";
		return false;
	}

	int payLoadLength = recieve[5]<<16+recieve[4];
	int sizeToRecieve = HEADER_SIZE+payLoadLength+CHECKSUM_SIZE;
	
	uint8* recievePayLoad = new uint8[sizeToRecieve];
	int recIP = tcpClient->TCP_Receive(connectedSocket,recievePayLoad,sizeToRecieve);
	if(recIP ==SOCKET_ERROR)
	{
		delete[] recievePayLoad;
		cout <<"Static Image Load, Intermediate Packet:"<<i<<"Recieve error.\n";
		return false;
	}
	delete[] recievePayLoad;
    

	//analyize the payload

	//blahblah not really need to do this
	  
	}


	//-------------Final Packet---------------------------------------------

	flag = End; // change the flag to indicate last packet

	int remainingBytes = -(byteCount - ((MAX_PAYLOAD_SIZE)*(NumberOfIntermediatePackets+2)));

	uint8* commandFinal = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, remainingBytes, image+MAX_PAYLOAD_SIZE*NumberOfIntermediatePackets);

	int finalSend = tcpClient->TCP_Send(connectedSocket,commandFinal,remainingBytes+HEADER_SIZE+CHECKSUM_SIZE); 

	delete[] commandFinal;

	if(finalSend == SOCKET_ERROR)
	{
	  cout <<"Static Image Load, Final Packet send error.\n";
	  return false;
	}

			//-----------Recieve Final Packet Packet----------------------------
	/*uint8 recieveF[HEADER_SIZE];
	int recF = tcpClient->TCP_Receive(connectedSocket,recieveF,HEADER_SIZE);
	if(recF ==SOCKET_ERROR)
	{
		cout <<"Static Image Load, Final Packet Recieve error.\n";
		return false;
	}

	int payLoadLength = recieve[5]<<16+recieve[4];
	int sizeToRecieve = HEADER_SIZE+payLoadLength+CHECKSUM_SIZE;
	
	uint8* recievePayLoad = new uint8[sizeToRecieve];
	int recIP = tcpClient->TCP_Receive(connectedSocket,recievePayLoad,sizeToRecieve);
	if(recIP ==SOCKET_ERROR)
	{
		delete[] recievePayLoad;
		cout <<"Static Image Load, Final Packet:Recieve error.\n";
		return false;
	}*/
	

	return true;
}

bool LCR_Commander::SetDisplayMode(DisplayMode displayMode)
{
	if(connectedSocket < 0)
	{
      cout << "No connected Socket.\n";
	  return false;
	}

	Packet_Type pType = Host_Write; // read command
	CommandIds cmdId = CurrentDisplayMode; //commandId
	Command_Flags flag = DataComplete; //flags


	uint8 dplMode = (uint8)displayMode; // set the display mode
	uint8 * payLoad;
	payLoad = &dplMode;

	uint16 payLoadLength = 0x1;

	//create the command
	uint8* command = packetizer->CreateCommand((uint8) pType, (uint16) cmdId, (uint8) flag, payLoadLength, payLoad);


	int totalLength = HEADER_SIZE + payLoadLength + CHECKSUM_SIZE;
	int sendResult = tcpClient->TCP_Send(connectedSocket,command,totalLength);

	delete[] command;

	if(sendResult ==SOCKET_ERROR)
	{
		cout <<"LCR Set Display mode send has send error.\n";

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
		  connectedSocket = -1;
		  return true;
		}

		else
		  return false;
	}
	else
	  return false;


}
