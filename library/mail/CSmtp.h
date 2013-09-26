// CSmtp.h: interface for the Smtp class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __CSMTP_H__
#define __CSMTP_H__


#include <vector>
#include <string.h>
#include <assert.h>


#include <sys/types.h>
#include <sys/socket.h>
//#include <sys/ioctl.h>
#include <sys/unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
//#include <stdio.h>
#include <iostream>

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
	
typedef unsigned short WORD;
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct hostent* LPHOSTENT;
typedef struct servent* LPSERVENT;
typedef struct in_addr* LPIN_ADDR;
typedef struct sockaddr* LPSOCKADDR;

#define TIME_IN_SEC		10		// how long client will wait for server response in non-blocking mode
#define BUFFER_SIZE 10240	  // SendData and RecvData buffers sizes
#define MSG_SIZE_IN_MB 5		// the maximum size of the message with all attachments
#define COUNTER_VALUE	100		// how many times program will try to receive data

const char BOUNDARY_TEXT[] = "__MESSAGE__ID__54yg6f6h6y456345";

enum CSmptXPriority
{
	XPRIORITY_HIGH = 2,
	XPRIORITY_NORMAL = 3,
	XPRIORITY_LOW = 4
};


/**
* @class gnublin_smtp
* @~english
* @brief Class for sending mails.
*
* This class manages to send mails.
* @~german
* @brief Klasse um Mails zu versenden.
*
* Diese Klasse ermöglicht das Senden und Empfangen von E-Mails.
*/
class gnublin_smtp  
{
public:
	gnublin_smtp();
	virtual ~gnublin_smtp();
	void AddRecipient(const char *email, const char *name=NULL);
	void AddBCCRecipient(const char *email, const char *name=NULL);
	void AddCCRecipient(const char *email, const char *name=NULL);    
	void AddAttachment(const char *path);   
	void AddMsgLine(const char* text);
	void DelRecipients(void);
	void DelBCCRecipients(void);
	void DelCCRecipients(void);
	void DelAttachments(void);
	void DelMsgLines(void);
	void DelMsgLine(unsigned int line);
	void ModMsgLine(unsigned int line,const char* text);
	unsigned int GetBCCRecipientCount() const;    
	unsigned int GetCCRecipientCount() const;
	unsigned int GetRecipientCount() const;    
//	const char* GetLocalHostIP() const;
	const char* GetLocalHostName() const;
	const char* GetMsgLineText(unsigned int line) const;
	unsigned int GetMsgLines(void) const;
	const char* GetReplyTo() const;
	const char* GetMailFrom() const;
	const char* GetSenderName() const;
	const char* GetSubject() const;
	const char* GetXMailer() const;
	CSmptXPriority GetXPriority() const;
	void Send();
	void SetSubject(const char*);
	void SetSenderName(const char*);
	void SetSenderMail(const char*);
	void SetReplyTo(const char*);
	void SetXMailer(const char*);
	void SetLogin(const char*);
	void SetPassword(const char*);
	void SetXPriority(CSmptXPriority);
	void SetSMTPServer(const char* server,const unsigned short port=0);
	
	//Error handling
	enum CSmtpError
	{
		CSMTP_NO_ERROR = 0,
		WSA_STARTUP = 100, // WSAGetLastError()
		WSA_VER,
		WSA_SEND,
		WSA_RECV,
		WSA_CONNECT,
		WSA_GETHOSTBY_NAME_ADDR,
		WSA_INVALID_SOCKET,
		WSA_HOSTNAME,
		WSA_IOCTLSOCKET,
		WSA_SELECT,
		BAD_IPV4_ADDR,
		UNDEF_MSG_HEADER = 200,
		UNDEF_MAIL_FROM,
		UNDEF_SUBJECT,
		UNDEF_RECIPIENTS,
		UNDEF_LOGIN,
		UNDEF_PASSWORD,
		UNDEF_RECIPIENT_MAIL,
		COMMAND_MAIL_FROM = 300,
		COMMAND_EHLO,
		COMMAND_AUTH_LOGIN,
		COMMAND_DATA,
		COMMAND_QUIT,
		COMMAND_RCPT_TO,
		MSG_BODY_ERROR,
		CONNECTION_CLOSED = 400, // by server
		SERVER_NOT_READY, // remote server
		SERVER_NOT_RESPONDING,
		SELECT_TIMEOUT,
		FILE_NOT_EXIST,
		MSG_TOO_BIG,
		BAD_LOGIN_PASS,
		UNDEF_XYZ_RESPONSE,
		LACK_OF_MEMORY,
		TIME_ERROR,
		RECVBUF_IS_EMPTY,
		SENDBUF_IS_EMPTY,
		OUT_OF_MSG_RANGE,
	};
	gnublin_smtp (CSmtpError err_) : ErrorCode(err_) {}
	CSmtpError GetErrorNum(void) const {return ErrorCode;}
	std::string GetErrorText(void) const;





private:	

	CSmtpError ErrorCode;


	std::string m_sLocalHostName;
	std::string m_sMailFrom;
	std::string m_sNameFrom;
	std::string m_sSubject;
	std::string m_sXMailer;
	std::string m_sReplyTo;
	std::string m_sIPAddr;
	std::string m_sLogin;
	std::string m_sPassword;
	std::string m_sSMTPSrvName;
	unsigned short m_iSMTPSrvPort;
	CSmptXPriority m_iXPriority;
	char *SendBuf;
	char *RecvBuf;
	
	SOCKET hSocket;

	struct Recipient
	{
		std::string Name;
		std::string Mail;
	};

	std::vector<Recipient> Recipients;
	std::vector<Recipient> CCRecipients;
	std::vector<Recipient> BCCRecipients;
	std::vector<std::string> Attachments;
	std::vector<std::string> MsgBody;
 
	void ReceiveData();
	void SendData();
	void FormatHeader(char*);
	int SmtpXYZdigits();
	SOCKET ConnectRemoteServer(const char* server, const unsigned short port=0);
	
};


#endif // __CSMTP_H__
