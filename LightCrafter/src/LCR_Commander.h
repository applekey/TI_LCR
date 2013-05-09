#ifndef _LCR_COMMANDER_H_
#define _LCR_COMMANDER_H_

using namespace std;
#include <string>

#include "Tcp.h"
#include "LCR_Common.h"
#include "LCR_Command_Packetizer.h"


#define ConnectionAttempts 5

class LCR_Commander
{
private:
	Tcp * tcpClient;
	int connectedSocket;
	Command_Packetizer* packetizer;
public:
	LCR_Commander(void);
	~LCR_Commander(void);

	int Connect_LCR(string ipAddress, string port);
	void Disconnect_LCR(void);

	LCR_Byte_Zero_Packet LCR_Component_Revision(LCR_Revision device, string* version);

};


#endif