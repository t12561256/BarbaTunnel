#pragma once
#include "BarbaException.h"
#include "SimpleCriticalSection.h"

class BarbaSocketException : public BarbaException
{
public:
	virtual ~BarbaSocketException(){}
	explicit BarbaSocketException (int socketError)
	{
		this->SocketError = socketError;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK, NULL, socketError, 0,
			this->Description, _countof(this->Description), NULL);
	}

private:
	int SocketError;
};

class BarbaSocket
{
public:
	explicit BarbaSocket(SOCKET s, u_long remoteIp);
	virtual ~BarbaSocket();
	//@return 0 if connection closed
	int Receive(BYTE* buf, size_t bufCount, bool waitAll);
	int Send(BYTE* buf, size_t bufCount);
	void Close();
	bool IsWritable();
	//@return empty string if not found
	std::string ReadHttpRequest(int maxlen=5000);
	size_t GetSentBytesCount() {return this->SentBytesCount;}
	size_t GetReceiveBytesCount() {return this->ReceivedBytesCount;}
	void SetNoDelay(bool value);
	void SetKeepAlive(bool value);
	void SetReceiveTimeOut(DWORD milisecond);
	void SetSendTimeOut(DWORD milisecond);
	u_long GetLastReceivedTime() {return this->LastReceivedTime;}
	u_long GetLastSentTime() {return this->LastSentTime;}
	u_long GetRemoteIp() { return this->RemoteIp;}
	bool IsReceiving() {return this->_IsReceiving;}
	static bool InitializeLib(); 
	static void UninitializeLib(); 

protected:
	volatile u_long LastReceivedTime;
	volatile u_long LastSentTime;
	volatile u_long SentBytesCount;
	volatile u_long ReceivedBytesCount;
	u_long RemoteIp;
	BarbaSocket();
	SOCKET _Socket;
	void ThrowSocketError();
	void Init();

private:
	volatile bool _IsReceiving;
};  

//BarbaSocketClient
class BarbaSocketClient : public BarbaSocket 
{
public:
	explicit BarbaSocketClient(u_long serverIp, u_short port);
	virtual ~BarbaSocketClient(){}
};


//BarbaSocketServer 
class BarbaSocketServer : public BarbaSocket 
{
public:
	explicit BarbaSocketServer(u_short port);
	virtual ~BarbaSocketServer(){}
	BarbaSocket* Accept();
	u_short GetListenPort() {return this->ListenPort;}

private:
	u_short ListenPort;
};
