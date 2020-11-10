#pragma once
class CCore;
class CServer
{
	CCore* m_pCore;
	int m_nDeamon;
public:
	CServer();
	~CServer();
	void InitConnServer(char* const pLogFile,USHORT nSerNo,USHORT nPort,USHORT nWebSockPort);
	void InitGameServer(char* const pLogFile,USHORT nSerNo,UINT nGameId);
	void InitDataServer(char* const pLogFile,USHORT nPort);
	
	void Run();
private:
	void InitLogFile(char* const pLogFile);
	void ConnectDataServer();
	void ConnectConnServer();
};
