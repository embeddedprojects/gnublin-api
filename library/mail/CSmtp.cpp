////////////////////////////////////////////////////////////////////////////////
// Original class CFastSmtp written by 
// christopher w. backen <immortal@cox.net>
// More details at: http://www.codeproject.com/KB/IP/zsmtp.aspx
// 
// Modifications introduced by Jakub Piwowarczyk:
// 1. name of the class and functions
// 2. new functions added: SendData,ReceiveData and more
// 3. authentication added
// 4. attachments added
// 5 .comments added
// 6. DELAY_IN_MS removed (no delay during sending the message)
// 7. non-blocking mode
// More details at: http://www.codeproject.com/KB/mcpp/CSmtp.aspx
////////////////////////////////////////////////////////////////////////////////

#include "CSmtp.h"
#include "base64.h"

////////////////////////////////////////////////////////////////////////////////
//        NAME: CSmtp
// DESCRIPTION: Constructor of CSmtp class.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: m_iXPriority, m_iSMTPSrvPort, RecvBuf, SendBuf
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Prepares the Mail-Class
*
* @~german
* @brief Richtet die Mail-Klasse ein
*/

gnublin_smtp::gnublin_smtp()
{
	m_iXPriority = XPRIORITY_NORMAL;
	m_iSMTPSrvPort = 0;
	
	if((RecvBuf = new char[BUFFER_SIZE]) == NULL)
		throw gnublin_smtp(gnublin_smtp::LACK_OF_MEMORY);
	
	if((SendBuf = new char[BUFFER_SIZE]) == NULL)
		throw gnublin_smtp(gnublin_smtp::LACK_OF_MEMORY);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: CSmtp
// DESCRIPTION: Destructor of CSmtp class.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf, SendBuf
// MODIFIES GL: RecvBuf, SendBuf
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief destroys buffer in the Mail-Class
*
* @~german
* @brief Löscht buffer in der Mail-Klasse
*/
gnublin_smtp::~gnublin_smtp()
{
	if(SendBuf)
	{
		delete[] SendBuf;
		SendBuf = NULL;
	}
	if(RecvBuf)
	{
		delete[] RecvBuf;
		RecvBuf = NULL;
	}

}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddAttachment
// DESCRIPTION: New attachment is added.
//   ARGUMENTS: const char *Path - name of attachment added
// USES GLOBAL: Attachments
// MODIFIES GL: Attachments
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Add new attachment
*
* @param Path name of the attachment
*
* @~german
* @brief Fügt einen neuen Dateianhang hinzu
*
* @param Path Dateiname
*/
void gnublin_smtp::AddAttachment(const char *Path)
{
	assert(Path);
	Attachments.insert(Attachments.end(),Path);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddRecipient
// DESCRIPTION: New recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the recipient
//              const char *name - name of the recipient
// USES GLOBAL: Recipients
// MODIFIES GL: Recipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief New recipient data is added i.e.: email and name. .
*
* @param email mail of the recipent
* @param name name of the recipent
*
* @~german
* @brief Fügt einen neuen Empfänger hinzu
*
* @param email E-Mail Adresse des Empfängers
* @param name Name des Empfängers
*/
void gnublin_smtp::AddRecipient(const char *email, const char *name)
{	
	if(!email)
		throw gnublin_smtp(gnublin_smtp::UNDEF_RECIPIENT_MAIL);

	Recipient recipient;
	recipient.Mail.insert(0,email);
	name!=NULL ? recipient.Name.insert(0,name) : recipient.Name.insert(0,"");

	Recipients.insert(Recipients.end(), recipient);   
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddCCRecipient
// DESCRIPTION: New cc-recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the cc-recipient
//              const char *name - name of the ccc-recipient
// USES GLOBAL: CCRecipients
// MODIFIES GL: CCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief New cc-recipient data is added i.e.: email and name. .
*
* @param email mail of the cc-recipent
* @param name name of the cc-recipent
*
* @~german
* @brief Fügt einen neuen CC-Empfänger hinzu
*
* @param email E-Mail Adresse des CC-Empfängers
* @param name Name des CC-Empfängers
*/
void gnublin_smtp::AddCCRecipient(const char *email, const char *name)
{	
	if(!email)
		throw gnublin_smtp(gnublin_smtp::UNDEF_RECIPIENT_MAIL);

	Recipient recipient;
	recipient.Mail.insert(0,email);
	name!=NULL ? recipient.Name.insert(0,name) : recipient.Name.insert(0,"");

	CCRecipients.insert(CCRecipients.end(), recipient);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddBCCRecipient
// DESCRIPTION: New bcc-recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the bcc-recipient
//              const char *name - name of the bccc-recipient
// USES GLOBAL: BCCRecipients
// MODIFIES GL: BCCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief New bcc-recipient data is added i.e.: email and name. .
*
* @param email mail of the bcc-recipent
* @param name name of the bcc-recipent
*
* @~german
* @brief Fügt einen neuen BCC-Empfänger hinzu
*
* @param email E-Mail Adresse des BCC-Empfängers
* @param name Name des BCC-Empfängers
*/
void gnublin_smtp::AddBCCRecipient(const char *email, const char *name)
{	
	if(!email)
		throw gnublin_smtp(gnublin_smtp::UNDEF_RECIPIENT_MAIL);

	Recipient recipient;
	recipient.Mail.insert(0,email);
	name!=NULL ? recipient.Name.insert(0,name) : recipient.Name.insert(0,"");

	BCCRecipients.insert(BCCRecipients.end(), recipient);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddMsgLine
// DESCRIPTION: Adds new line in a message.
//   ARGUMENTS: const char *Text - text of the new line
// USES GLOBAL: MsgBody
// MODIFIES GL: MsgBody
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Adds new line in a message.
*
* @param Text text of the new line
*
* @~german
* @brief Fügt eine neue Zeile in die Nachricht ein
*
* @param Text Text der neuen Zeile
*/
void gnublin_smtp::AddMsgLine(const char* Text)
{
	MsgBody.insert(MsgBody.end(),Text);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelMsgLine
// DESCRIPTION: Deletes specified line in text message.. .
//   ARGUMENTS: unsigned int Line - line to be delete
// USES GLOBAL: MsgBody
// MODIFIES GL: MsgBody
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes specified line in text message..
*
* @param Line line to be delete
*
* @~german
* @brief Löscht eine bestimmte Zeile in der Nachricht
*
* @param Line Zeilennummer die gelöscht werden soll
*/
void gnublin_smtp::DelMsgLine(unsigned int Line)
{
	if(Line > MsgBody.size())
		throw gnublin_smtp(gnublin_smtp::OUT_OF_MSG_RANGE);
	MsgBody.erase(MsgBody.begin()+Line);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelRecipients
// DESCRIPTION: Deletes all recipients. .
//   ARGUMENTS: void
// USES GLOBAL: Recipients
// MODIFIES GL: Recipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes all recipients.
*
*
* @~german
* @brief Löscht alle Empfänger
*
*/
void gnublin_smtp::DelRecipients()
{
	Recipients.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelBCCRecipients
// DESCRIPTION: Deletes all BCC recipients. .
//   ARGUMENTS: void
// USES GLOBAL: BCCRecipients
// MODIFIES GL: BCCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes all BCC-recipients.
*
*
* @~german
* @brief Löscht alle BCC-Empfänger
*
*/
void gnublin_smtp::DelBCCRecipients()
{
	BCCRecipients.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelCCRecipients
// DESCRIPTION: Deletes all CC recipients. .
//   ARGUMENTS: void
// USES GLOBAL: CCRecipients
// MODIFIES GL: CCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes all CC-recipients.
*
*
* @~german
* @brief Löscht alle CC-Empfänger
*
*/
void gnublin_smtp::DelCCRecipients()
{
	CCRecipients.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelMsgLines
// DESCRIPTION: Deletes message text.
//   ARGUMENTS: void
// USES GLOBAL: MsgBody
// MODIFIES GL: MsgBody
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes message text.
*
*
* @~german
* @brief Löscht den Nachrichtentext
*
*/
void gnublin_smtp::DelMsgLines()
{
	MsgBody.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelAttachments
// DESCRIPTION: Deletes all attachments. .
//   ARGUMENTS: void
// USES GLOBAL: Attchments
// MODIFIES GL: Attachments
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Deletes all attachments.
*
*
* @~german
* @brief Löscht alle Dateianhänge.
*
*/
void gnublin_smtp::DelAttachments()
{
	Attachments.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddBCCRecipient
// DESCRIPTION: New bcc-recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the bcc-recipient
//              const char *name - name of the bccc-recipient
// USES GLOBAL: BCCRecipients
// MODIFIES GL: BCCRecipients, m_oError
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Edit a specific message line
*
* @param Line Line to edite
* @param Text New text of the specific line
*
* @~german
* @brief Bearbeiten einer bestimmten Nachrichtenzeile
*
* @param Line Zu editierende Zeile
* @param Text Neuer Text der Zeile
*/
void gnublin_smtp::ModMsgLine(unsigned int Line,const char* Text)
{
	if(Text)
	{
		if(Line > MsgBody.size())
			throw gnublin_smtp(gnublin_smtp::OUT_OF_MSG_RANGE);
		MsgBody.at(Line) = std::string(Text);
	}
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: Send
// DESCRIPTION: Sending the mail. .
//   ARGUMENTS: none
// USES GLOBAL: m_sSMTPSrvName, m_iSMTPSrvPort, SendBuf, RecvBuf, m_sLogin,
//              m_sPassword, m_sMailFrom, Recipients, CCRecipients,
//              BCCRecipients, m_sMsgBody, Attachments, 
// MODIFIES GL: SendBuf 
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Sending the mail.
*
*
* @~german
* @brief E-Mail verschicken.
*
*/
void gnublin_smtp::Send()
{
	unsigned int i,rcpt_count,res,FileId;
	char *FileBuf = NULL, *FileName = NULL;
	FILE* hFile = NULL;
	unsigned long int FileSize,TotalSize,MsgPart;
	bool bAccepted;

	// ***** CONNECTING TO SMTP SERVER *****

	// connecting to remote host:
	if( (hSocket = ConnectRemoteServer(m_sSMTPSrvName.c_str(), m_iSMTPSrvPort)) == INVALID_SOCKET ) 
		throw gnublin_smtp(gnublin_smtp::WSA_INVALID_SOCKET);

	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 220:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp::SERVER_NOT_READY);
		}
	}while(!bAccepted);

	// EHLO <SP> <domain> <CRLF>
	sprintf(SendBuf,"EHLO [127.0.0.1]\r\n");//,GetLocalHostName()!=NULL ? m_sLocalHostName.c_str() : "domain");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 250:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::COMMAND_EHLO);
		}
	}while(!bAccepted);

	// AUTH <SP> LOGIN <CRLF>
	strcpy(SendBuf,"AUTH LOGIN\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 250:
				break;
			case 334:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::COMMAND_AUTH_LOGIN);
		}
	}while(!bAccepted);

	// send login:
	if(!m_sLogin.size())
		throw gnublin_smtp(gnublin_smtp ::UNDEF_LOGIN);
	std::string encoded_login = base64_encode(reinterpret_cast<const unsigned char*>(m_sLogin.c_str()),m_sLogin.size());
	sprintf(SendBuf,"%s\r\n",encoded_login.c_str());
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 334:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::UNDEF_XYZ_RESPONSE);
		}
	}while(!bAccepted);
	
	// send password:
	if(!m_sPassword.size())
		throw gnublin_smtp(gnublin_smtp ::UNDEF_PASSWORD);
	std::string encoded_password = base64_encode(reinterpret_cast<const unsigned char*>(m_sPassword.c_str()),m_sPassword.size());
	sprintf(SendBuf,"%s\r\n",encoded_password.c_str());
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 235:
				bAccepted = true;
				break;
			case 334:
				break;
			case 535:
				throw gnublin_smtp(gnublin_smtp ::BAD_LOGIN_PASS);
			default:
				throw gnublin_smtp(gnublin_smtp ::UNDEF_XYZ_RESPONSE);
		}
	}while(!bAccepted);

	// ***** SENDING E-MAIL *****
	
	// MAIL <SP> FROM:<reverse-path> <CRLF>
	if(!m_sMailFrom.size())
		throw gnublin_smtp(gnublin_smtp ::UNDEF_MAIL_FROM);
	sprintf(SendBuf,"MAIL FROM:<%s>\r\n",m_sMailFrom.c_str());
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 250:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::COMMAND_MAIL_FROM);
		}
	}while(!bAccepted);

	// RCPT <SP> TO:<forward-path> <CRLF>
	if(!(rcpt_count = Recipients.size()))
		throw gnublin_smtp(gnublin_smtp ::UNDEF_RECIPIENTS);
	for(i=0;i<Recipients.size();i++)
	{
		sprintf(SendBuf,"RCPT TO:<%s>\r\n",(Recipients.at(i).Mail).c_str());
		SendData();
		bAccepted = false;
		do
		{
			ReceiveData();
			switch(SmtpXYZdigits())
			{
				case 250:
					bAccepted = true;
					break;
				default:
					rcpt_count--;
			}
		}while(!bAccepted);
	}
	if(rcpt_count <= 0)
		throw gnublin_smtp(gnublin_smtp ::COMMAND_RCPT_TO);

	for(i=0;i<CCRecipients.size();i++)
	{
		sprintf(SendBuf,"RCPT TO:<%s>\r\n",(CCRecipients.at(i).Mail).c_str());
		SendData();
		bAccepted = false;
		do
		{
			ReceiveData();
			switch(SmtpXYZdigits())
			{
				case 250:
					bAccepted = true;
					break;
				default:
					; // not necessary to throw
			}
		}while(!bAccepted);
	}

	for(i=0;i<BCCRecipients.size();i++)
	{
		sprintf(SendBuf,"RCPT TO:<%s>\r\n",(BCCRecipients.at(i).Mail).c_str());
		SendData();
		bAccepted = false;
		do
		{
			ReceiveData();
			switch(SmtpXYZdigits())
			{
				case 250:
					bAccepted = true;
					break;
				default:
					; // not necessary to throw
			}
		}while(!bAccepted);
	}
	
	// DATA <CRLF>
	strcpy(SendBuf,"DATA\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 354:
				bAccepted = true;
				break;
			case 250:
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::COMMAND_DATA);
		}
	}while(!bAccepted);
	
	// send header(s)
	FormatHeader(SendBuf);
	SendData();

	// send text message
	if(GetMsgLines())
	{
		for(i=0;i<GetMsgLines();i++)
		{
			sprintf(SendBuf,"%s\r\n",GetMsgLineText(i));
			SendData();
		}
	}
	else
	{
		sprintf(SendBuf,"%s\r\n"," ");
		SendData();
	}

	// next goes attachments (if they are)
	if((FileBuf = new char[55]) == NULL)
		throw gnublin_smtp(gnublin_smtp ::LACK_OF_MEMORY);

	if((FileName = new char[255]) == NULL)
		throw gnublin_smtp(gnublin_smtp ::LACK_OF_MEMORY);

	TotalSize = 0;
	for(FileId=0;FileId<Attachments.size();FileId++)
	{
		strcpy(FileName,Attachments[FileId].c_str());

		sprintf(SendBuf,"--%s\r\n",BOUNDARY_TEXT);
		strcat(SendBuf,"Content-Type: application/x-msdownload; name=\"");
		strcat(SendBuf,&FileName[Attachments[FileId].find_last_of("\\") + 1]);
		strcat(SendBuf,"\"\r\n");
		strcat(SendBuf,"Content-Transfer-Encoding: base64\r\n");
		strcat(SendBuf,"Content-Disposition: attachment; filename=\"");
		strcat(SendBuf,&FileName[Attachments[FileId].find_last_of("\\") + 1]);
		strcat(SendBuf,"\"\r\n");
		strcat(SendBuf,"\r\n");

		SendData();

		// opening the file:
		hFile = fopen(FileName,"rb");
		if(hFile == NULL)
			throw gnublin_smtp(gnublin_smtp ::FILE_NOT_EXIST);
		
		// checking file size:
		FileSize = 0;
		while(!feof(hFile))
			FileSize += fread(FileBuf,sizeof(char),54,hFile);
		TotalSize += FileSize;

		// sending the file:
		if(TotalSize/1024 > MSG_SIZE_IN_MB*1024)
			throw gnublin_smtp(gnublin_smtp ::MSG_TOO_BIG);
		else
		{
			fseek (hFile,0,SEEK_SET);

			MsgPart = 0;
			for(i=0;i<FileSize/54+1;i++)
			{

				res = fread(FileBuf,sizeof(char),54,hFile);
				MsgPart ? strcat(SendBuf,base64_encode(reinterpret_cast<const unsigned char*>(FileBuf),res).c_str())
					      : strcpy(SendBuf,base64_encode(reinterpret_cast<const unsigned char*>(FileBuf),res).c_str());
				strcat(SendBuf,"\r\n");
				MsgPart += res + 2;
				if(MsgPart >= BUFFER_SIZE/2)
				{ // sending part of the message
					MsgPart = 0;
					SendData(); // FileBuf, FileName, fclose(hFile);
				}
			}
			if(MsgPart)
			{
				SendData(); // FileBuf, FileName, fclose(hFile);
			}
		}
		fclose(hFile);
	}
	delete[] FileBuf;
	delete[] FileName;
	
	// sending last message block (if there is one or more attachments)
	if(Attachments.size())
	{
		sprintf(SendBuf,"\r\n--%s--\r\n",BOUNDARY_TEXT);
		SendData();
	}
	
	// <CRLF> . <CRLF>
	strcpy(SendBuf,"\r\n.\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 250:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::MSG_BODY_ERROR);
		}
	}while(!bAccepted);

	// ***** CLOSING CONNECTION *****
	
	// QUIT <CRLF>
	strcpy(SendBuf,"QUIT\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
			case 221:
				bAccepted = true;
				break;
			default:
				throw gnublin_smtp(gnublin_smtp ::COMMAND_QUIT);
		}
	}while(!bAccepted);


	close(hSocket);
	//hSocket = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: ConnectRemoteServer
// DESCRIPTION: Connecting to the service running on the remote server. 
//   ARGUMENTS: const char *server - service name
//              const unsigned short port - service port
// USES GLOBAL: m_pcSMTPSrvName, m_iSMTPSrvPort, SendBuf, RecvBuf, m_pcLogin,
//              m_pcPassword, m_pcMailFrom, Recipients, CCRecipients,
//              BCCRecipients, m_pcMsgBody, Attachments, 
// MODIFIES GL: m_oError 
//     RETURNS: socket of the remote service
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Connecting to the service running on the remote server. 
*
* @param server server name
* @param port service port
* @return socket of the remote service
*
* @~german
* @brief Verbindung mit dem Service auf dem Remote Server aufnehmen
*
* @param server Servername
* @param Port Port des Service
* @return Socket des Remote Server
*/
SOCKET gnublin_smtp::ConnectRemoteServer(const char *szServer,const unsigned short nPort_)
{
	unsigned short nPort = 0;
	LPSERVENT lpServEnt;
	SOCKADDR_IN sockAddr;
	unsigned long ul = 1;
	fd_set fdwrite,fdexcept;
	timeval timeout;
	int res = 0;

	timeout.tv_sec = TIME_IN_SEC;
	timeout.tv_usec = 0;

	SOCKET hSocket = INVALID_SOCKET;

	if((hSocket = socket(PF_INET, SOCK_STREAM,0)) == INVALID_SOCKET)
		throw gnublin_smtp(gnublin_smtp ::WSA_INVALID_SOCKET);

	if(nPort_ != 0)
		nPort = htons(nPort_);
	else
	{
		lpServEnt = getservbyname("mail", 0);
		if (lpServEnt == NULL)
			nPort = htons(25);
		else 
			nPort = lpServEnt->s_port;
	}
			
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = nPort;
	if((sockAddr.sin_addr.s_addr = inet_addr(szServer)) == INADDR_NONE)
	{
		LPHOSTENT host;
			
		host = gethostbyname(szServer);
		if (host)
			memcpy(&sockAddr.sin_addr,host->h_addr_list[0],host->h_length);
		else
		{
			close(hSocket);
			throw gnublin_smtp(gnublin_smtp ::WSA_GETHOSTBY_NAME_ADDR);
		}				
	}

	// start non-blocking mode for socket:
	if(ioctl(hSocket,FIONBIO, (unsigned long*)&ul) == SOCKET_ERROR)
	{
		close(hSocket);
		throw gnublin_smtp(gnublin_smtp ::WSA_IOCTLSOCKET);
	}

	if(connect(hSocket,(LPSOCKADDR)&sockAddr,sizeof(sockAddr)) == SOCKET_ERROR)
	{
		if(errno != EINPROGRESS)

		{
			close(hSocket);
			throw gnublin_smtp(gnublin_smtp ::WSA_CONNECT);
		}
	}
	else
		return hSocket;

	while(true)
	{
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexcept);

		FD_SET(hSocket,&fdwrite);
		FD_SET(hSocket,&fdexcept);

		if((res = select(hSocket+1,NULL,&fdwrite,&fdexcept,&timeout)) == SOCKET_ERROR)
		{
			close(hSocket);
			throw gnublin_smtp(gnublin_smtp ::WSA_SELECT);
		}

		if(!res)
		{
			close(hSocket);
			throw gnublin_smtp(gnublin_smtp ::SELECT_TIMEOUT);
		}
		if(res && FD_ISSET(hSocket,&fdwrite))
			break;
		if(res && FD_ISSET(hSocket,&fdexcept))
		{
			close(hSocket);
			throw gnublin_smtp(gnublin_smtp ::WSA_SELECT);
		}
	} // while

	FD_CLR(hSocket,&fdwrite);
	FD_CLR(hSocket,&fdexcept);

	return hSocket;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SmtpXYZdigits
// DESCRIPTION: Converts three letters from RecvBuf to the number.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf
// MODIFIES GL: none
//     RETURNS: integer number
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Converts three letters from RecvBuf to the number.
* @return integer number
*
* @~german
* @brief Konvertiert drei Buchstaben des RecvBuf in Zahlen.
* @return Zahl im integer-Format
*/
int gnublin_smtp::SmtpXYZdigits()
{
	assert(RecvBuf);
	if(RecvBuf == NULL)
		return 0;
	return (RecvBuf[0]-'0')*100 + (RecvBuf[1]-'0')*10 + RecvBuf[2]-'0';
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: FormatHeader
// DESCRIPTION: Prepares a header of the message.
//   ARGUMENTS: char* header - formated header string
// USES GLOBAL: Recipients, CCRecipients, BCCRecipients
// MODIFIES GL: none
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Prepares a header of the message.
*
* @param header formated header string
*
* @~german
* @brief Bereitet den Header der Nachricht vor.
*
* @param header Formatierter Header-String
*/
void gnublin_smtp::FormatHeader(char* header)
{
	char month[][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	size_t i;
	std::string to;
	std::string cc;
	std::string bcc;
	time_t rawtime;
	struct tm* timeinfo;

	// date/time check
	if(time(&rawtime) > 0)
		timeinfo = localtime(&rawtime);
	else
		throw gnublin_smtp(gnublin_smtp ::TIME_ERROR);

	// check for at least one recipient
	if(Recipients.size())
	{
		for (i=0;i<Recipients.size();i++)
		{
			if(i > 0)
				to.append(",");
			to += Recipients[i].Name;
			to.append("<");
			to += Recipients[i].Mail;
			to.append(">");
		}
	}
	else
		throw gnublin_smtp(gnublin_smtp ::UNDEF_RECIPIENTS);

	if(CCRecipients.size())
	{
		for (i=0;i<CCRecipients.size();i++)
		{
			if(i > 0)
				cc. append(",");
			cc += CCRecipients[i].Name;
			cc.append("<");
			cc += CCRecipients[i].Mail;
			cc.append(">");
		}
	}

	if(BCCRecipients.size())
	{
		for (i=0;i<BCCRecipients.size();i++)
		{
			if(i > 0)
				bcc.append(",");
			bcc += BCCRecipients[i].Name;
			bcc.append("<");
			bcc += BCCRecipients[i].Mail;
			bcc.append(">");
		}
	}
	
	// Date: <SP> <dd> <SP> <mon> <SP> <yy> <SP> <hh> ":" <mm> ":" <ss> <SP> <zone> <CRLF>
	sprintf(header,"Date: %d %s %d %d:%d:%d\r\n",	timeinfo->tm_mday,
																								month[timeinfo->tm_mon],
																								timeinfo->tm_year+1900,
																								timeinfo->tm_hour,
																								timeinfo->tm_min,
																								timeinfo->tm_sec); 
	
	// From: <SP> <sender>  <SP> "<" <sender-email> ">" <CRLF>
	if(!m_sMailFrom.size())
		throw gnublin_smtp(gnublin_smtp ::UNDEF_MAIL_FROM);
	strcat(header,"From: ");
	if(m_sNameFrom.size())
		strcat(header, m_sNameFrom.c_str());
	strcat(header," <");
	if(m_sNameFrom.size())
		strcat(header,m_sMailFrom.c_str());
	else
		strcat(header,"mail@domain.com");
	strcat(header, ">\r\n");

	// X-Mailer: <SP> <xmailer-app> <CRLF>
	if(m_sXMailer.size())
	{
		strcat(header,"X-Mailer: ");
		strcat(header, m_sXMailer.c_str());
		strcat(header, "\r\n");
	}

	// Reply-To: <SP> <reverse-path> <CRLF>
	if(m_sReplyTo.size())
	{
		strcat(header, "Reply-To: ");
		strcat(header, m_sReplyTo.c_str());
		strcat(header, "\r\n");
	}

	// X-Priority: <SP> <number> <CRLF>
	switch(m_iXPriority)
	{
		case XPRIORITY_HIGH:
			strcat(header,"X-Priority: 2 (High)\r\n");
			break;
		case XPRIORITY_NORMAL:
			strcat(header,"X-Priority: 3 (Normal)\r\n");
			break;
		case XPRIORITY_LOW:
			strcat(header,"X-Priority: 4 (Low)\r\n");
			break;
		default:
			strcat(header,"X-Priority: 3 (Normal)\r\n");
	}

	// To: <SP> <remote-user-mail> <CRLF>
	strcat(header,"To: ");
	strcat(header, to.c_str());
	strcat(header, "\r\n");

	// Cc: <SP> <remote-user-mail> <CRLF>
	if(CCRecipients.size())
	{
		strcat(header,"Cc: ");
		strcat(header, cc.c_str());
		strcat(header, "\r\n");
	}

	if(BCCRecipients.size())
	{
		strcat(header,"Bcc: ");
		strcat(header, bcc.c_str());
		strcat(header, "\r\n");
	}

	// Subject: <SP> <subject-text> <CRLF>
	if(!m_sSubject.size()) 
		strcat(header, "Subject:  ");
	else
	{
	  strcat(header, "Subject: ");
	  strcat(header, m_sSubject.c_str());
	}
	strcat(header, "\r\n");
	
	// MIME-Version: <SP> 1.0 <CRLF>
	strcat(header,"MIME-Version: 1.0\r\n");
	if(!Attachments.size())
	{ // no attachments
		strcat(header,"Content-type: text/plain; charset=US-ASCII\r\n");
		strcat(header,"Content-Transfer-Encoding: 7bit\r\n");
		strcat(SendBuf,"\r\n");
	}
	else
	{ // there is one or more attachments
		strcat(header,"Content-Type: multipart/mixed; boundary=\"");
		strcat(header,BOUNDARY_TEXT);
		strcat(header,"\"\r\n");
		strcat(header,"\r\n");
		// first goes text message
		strcat(SendBuf,"--");
		strcat(SendBuf,BOUNDARY_TEXT);
		strcat(SendBuf,"\r\n");
		strcat(SendBuf,"Content-type: text/plain; charset=US-ASCII\r\n");
		strcat(SendBuf,"Content-Transfer-Encoding: 7bit\r\n");
		strcat(SendBuf,"\r\n");
	}

	// done
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: ReceiveData
// DESCRIPTION: Receives a row terminated '\n'.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf
// MODIFIES GL: RecvBuf
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Receives a row terminated '\n'.
*
*
* @~german
* @brief Empfängt eine mi '\n' terminierte Zeile.
*
*/
void gnublin_smtp::ReceiveData()
{
	int res,i = 0;
	fd_set fdread;
	timeval time;

	time.tv_sec = TIME_IN_SEC;
	time.tv_usec = 0;

	assert(RecvBuf);

	if(RecvBuf == NULL)
		throw gnublin_smtp(gnublin_smtp ::RECVBUF_IS_EMPTY);

	while(1)
	{
		FD_ZERO(&fdread);

		FD_SET(hSocket,&fdread);

		if((res = select(hSocket+1, &fdread, NULL, NULL, &time)) == SOCKET_ERROR)
		{
			FD_CLR(hSocket,&fdread);
			throw gnublin_smtp(gnublin_smtp ::WSA_SELECT);
		}

		if(!res)
		{
			//timeout
			FD_CLR(hSocket,&fdread);
			throw gnublin_smtp(gnublin_smtp ::SERVER_NOT_RESPONDING);
		}

		if(res && FD_ISSET(hSocket,&fdread))
		{
			if(i >= BUFFER_SIZE)
			{
				FD_CLR(hSocket,&fdread);
				throw gnublin_smtp(gnublin_smtp ::LACK_OF_MEMORY);
			}
			if(recv(hSocket,&RecvBuf[i++],1,0) == SOCKET_ERROR)
			{
				FD_CLR(hSocket,&fdread);
				throw gnublin_smtp(gnublin_smtp ::WSA_RECV);
			}
			if(RecvBuf[i-1]=='\n')
			{
				RecvBuf[i] = '\0';
				break;
			}
		}
	}

	FD_CLR(hSocket,&fdread);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SendData
// DESCRIPTION: Sends data from SendBuf buffer.
//   ARGUMENTS: none
// USES GLOBAL: SendBuf
// MODIFIES GL: none
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Sends data from SendBuf buffer.
*
*
* @~german
* @brief Sendet die daten des SendBuf.
*
*/
void gnublin_smtp::SendData()
{
	int idx = 0,res,nLeft = strlen(SendBuf);
	fd_set fdwrite;
	timeval time;

	time.tv_sec = TIME_IN_SEC;
	time.tv_usec = 0;

	assert(SendBuf);

	if(SendBuf == NULL)
		throw gnublin_smtp(gnublin_smtp ::SENDBUF_IS_EMPTY);

	while(1)
	{
		FD_ZERO(&fdwrite);

		FD_SET(hSocket,&fdwrite);

		if((res = select(hSocket+1,NULL,&fdwrite,NULL,&time)) == SOCKET_ERROR)
		{
			FD_CLR(hSocket,&fdwrite);
			throw gnublin_smtp(gnublin_smtp ::WSA_SELECT);
		}

		if(!res)
		{
			//timeout
			FD_CLR(hSocket,&fdwrite);
			throw gnublin_smtp(gnublin_smtp ::SERVER_NOT_RESPONDING);
		}

		if(res && FD_ISSET(hSocket,&fdwrite))
		{
			if(nLeft > 0)
			{
				if((res = send(hSocket,&SendBuf[idx],nLeft,0)) == SOCKET_ERROR)
				{
					FD_CLR(hSocket,&fdwrite);
					throw gnublin_smtp(gnublin_smtp ::WSA_SEND);
				}
				if(!res)
					break;
				nLeft -= res;
				idx += res;
			}
			else
				break;
		}
	}

	FD_CLR(hSocket,&fdwrite);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetLocalHostName
// DESCRIPTION: Returns local host name. 
//   ARGUMENTS: none
// USES GLOBAL: m_pcLocalHostName
// MODIFIES GL: m_oError, m_pcLocalHostName 
//     RETURNS: socket of the remote service
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns local host name. 
* @return socket of the remote service
*
* @~german
* @brief Gibt den lokalen Host Namen zurück.
* @return Socket des Remote Service
*/
const char* gnublin_smtp::GetLocalHostName() const
{
	char* str = NULL;

	if((str = new char[255]) == NULL)
		throw gnublin_smtp(gnublin_smtp ::LACK_OF_MEMORY);
	if(gethostname(str,255) == SOCKET_ERROR)
	{
		delete[] str;
		throw gnublin_smtp(gnublin_smtp ::WSA_HOSTNAME);
	}
	delete[] str;
	return m_sLocalHostName.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetRecipientCount
// DESCRIPTION: Returns the number of recipents.
//   ARGUMENTS: none
// USES GLOBAL: Recipients
// MODIFIES GL: none 
//     RETURNS: number of recipents
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns the number of recipents.
* @return number of recipents.
*
* @~german
* @brief Gibt die anzahl der Empfänger zurück.
* @return Anzahl der Empfänger
*/
unsigned int gnublin_smtp::GetRecipientCount() const
{
	return Recipients.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetBCCRecipientCount
// DESCRIPTION: Returns the number of bcc-recipents. 
//   ARGUMENTS: none
// USES GLOBAL: BCCRecipients
// MODIFIES GL: none 
//     RETURNS: number of bcc-recipents
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns the number of BCC-recipents.
* @return number of BCC-recipents.
*
* @~german
* @brief Gibt die anzahl der BCC-Empfänger zurück.
* @return Anzahl der BCC-Empfänger
*/
unsigned int gnublin_smtp::GetBCCRecipientCount() const
{
	return BCCRecipients.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetCCRecipientCount
// DESCRIPTION: Returns the number of cc-recipents.
//   ARGUMENTS: none
// USES GLOBAL: CCRecipients
// MODIFIES GL: none 
//     RETURNS: number of cc-recipents
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns the number of CC-recipents.
* @return number of CC-recipents.
*
* @~german
* @brief Gibt die anzahl der CC-Empfänger zurück.
* @return Anzahl der CC-Empfänger
*/
unsigned int gnublin_smtp::GetCCRecipientCount() const
{
	return CCRecipients.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetReplyTo
// DESCRIPTION: Returns m_pcReplyTo string.
//   ARGUMENTS: none
// USES GLOBAL: m_sReplyTo
// MODIFIES GL: none 
//     RETURNS: m_sReplyTo string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcReplyTo string.
* @return m_pcReplyTo string
*
* @~german
* @brief Gibt den m_pcReplyTo string zurück.
* @return m_pcReplyTo string
*/
const char* gnublin_smtp::GetReplyTo() const
{
	return m_sReplyTo.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetMailFrom
// DESCRIPTION: Returns m_pcMailFrom string.
//   ARGUMENTS: none
// USES GLOBAL: m_sMailFrom
// MODIFIES GL: none 
//     RETURNS: m_sMailFrom string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcMailFrom string.
* @return m_pcMailFrom string
*
* @~german
* @brief Gibt den m_pcMailFrom string zurück.
* @return m_pcMailFrom string
*/
const char* gnublin_smtp::GetMailFrom() const
{
	return m_sMailFrom.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetSenderName
// DESCRIPTION: Returns m_pcNameFrom string.
//   ARGUMENTS: none
// USES GLOBAL: m_sNameFrom
// MODIFIES GL: none 
//     RETURNS: m_sNameFrom string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcNameFrom string.
* @return m_pcNameFrom string
*
* @~german
* @brief Gibt den m_pcNameFrom string zurück.
* @return m_pcNameFrom string
*/
const char* gnublin_smtp::GetSenderName() const
{
	return m_sNameFrom.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetSubject
// DESCRIPTION: Returns m_pcSubject string.
//   ARGUMENTS: none
// USES GLOBAL: m_sSubject
// MODIFIES GL: none 
//     RETURNS: m_sSubject string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcSubject string.
* @return m_pcSubject string
*
* @~german
* @brief Gibt den m_pcSubject string zurück.
* @return m_pcSubject string
*/
const char* gnublin_smtp::GetSubject() const
{
	return m_sSubject.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetXMailer
// DESCRIPTION: Returns m_pcXMailer string.
//   ARGUMENTS: none
// USES GLOBAL: m_pcXMailer
// MODIFIES GL: none 
//     RETURNS: m_pcXMailer string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcXMailer string.
* @return m_pcXMailer string
*
* @~german
* @brief Gibt den m_pcXMailer string zurück.
* @return m_pcXMailer string
*/
const char* gnublin_smtp::GetXMailer() const
{
	return m_sXMailer.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetXPriority
// DESCRIPTION: Returns m_iXPriority string.
//   ARGUMENTS: none
// USES GLOBAL: m_iXPriority
// MODIFIES GL: none 
//     RETURNS: CSmptXPriority m_pcXMailer
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns m_pcXPriority string.
* @return m_pcXPriority string
*
* @~german
* @brief Gibt den m_pcXPriority string zurück.
* @return m_pcXPriority string
*/
CSmptXPriority gnublin_smtp::GetXPriority() const
{
	return m_iXPriority;
}

/**
* @~english
* @brief Returns text of a specific message Line
* @param Line Linenumber
* @return Text
*
* @~german
* @brief Gibt Text einer bestimmten Nachrichtenzeile zurück
* @param Line Zeilennummer
* @return Text
*/
const char* gnublin_smtp::GetMsgLineText(unsigned int Line) const
{
	if(Line > MsgBody.size())
		throw gnublin_smtp(gnublin_smtp ::OUT_OF_MSG_RANGE);
	return MsgBody.at(Line).c_str();
}

unsigned int gnublin_smtp::GetMsgLines() const
{
	return MsgBody.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetXPriority
// DESCRIPTION: Setting priority of the message.
//   ARGUMENTS: CSmptXPriority priority - priority of the message (	XPRIORITY_HIGH,
//              XPRIORITY_NORMAL, XPRIORITY_LOW)
// USES GLOBAL: none
// MODIFIES GL: m_iXPriority 
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting priority of the message.
* @param priority Priority of the message (XPRIORITY_HIGH, XPRIORITY_NORMAL, XPRIORITY_LOW)
*
* @~german
* @brief Priorität der Nachricht festlegen
* @param priority Priorität der Nachricht (XPRIORITY_HIGH, XPRIORITY_NORMAL, XPRIORITY_LOW)
*/
void gnublin_smtp::SetXPriority(CSmptXPriority priority)
{
	m_iXPriority = priority;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetReplyTo
// DESCRIPTION: Setting the return address.
//   ARGUMENTS: const char *ReplyTo - return address
// USES GLOBAL: m_sReplyTo
// MODIFIES GL: m_sReplyTo
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting the return address.
* @param ReplyTo return address
*
* @~german
* @brief Rücksendeadresse festlegen
* @param ReplyTo Rücksendeadresse
*/
void gnublin_smtp::SetReplyTo(const char *ReplyTo)
{
	//m_sReplyTo.erase();
	m_sReplyTo.insert(0,ReplyTo);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSenderMail
// DESCRIPTION: Setting sender's mail.
//   ARGUMENTS: const char *EMail - sender's e-mail
// USES GLOBAL: m_sMailFrom
// MODIFIES GL: m_sMailFrom
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting sender's mail.
* @param E-Mail sender's e-mail
*
* @~german
* @brief Absenderadresse setzen
* @param E-Mail Absenderadresse
*/
void gnublin_smtp::SetSenderMail(const char *EMail)
{
	m_sMailFrom.erase();
	m_sMailFrom.insert(0,EMail);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSenderName
// DESCRIPTION: Setting sender's name.
//   ARGUMENTS: const char *Name - sender's name
// USES GLOBAL: m_sNameFrom
// MODIFIES GL: m_sNameFrom
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting sender's name.
* @param Name sender's name
*
* @~german
* @brief Absendername setzen
* @param Name Absendername
*/
void gnublin_smtp::SetSenderName(const char *Name)
{
	m_sNameFrom.erase();
	m_sNameFrom.insert(0,Name);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSubject
// DESCRIPTION: Setting subject of the message.
//   ARGUMENTS: const char *Subject - subject of the message
// USES GLOBAL: m_sSubject
// MODIFIES GL: m_sSubject
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting subject of message.
* @param Subject subject of the message
*
* @~german
* @brief Betraff der Nachricht festlegen
* @param Subject Betreff
*/
void gnublin_smtp::SetSubject(const char *Subject)
{
	m_sSubject.erase();
	m_sSubject.insert(0,Subject);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetXMailer
// DESCRIPTION: Setting the name of program which is sending the mail.
//   ARGUMENTS: const char *XMailer - programe name
// USES GLOBAL: m_sXMailer
// MODIFIES GL: m_sXMailer
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting the name of program which is sending the mail.
* @param XMailer programe name
*
* @~german
* @brief Name des Absendeprogramms bestimmen
* @param XMailer Programmname
*/
void gnublin_smtp::SetXMailer(const char *XMailer)
{
	m_sXMailer.erase();
	m_sXMailer.insert(0,XMailer);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetLogin
// DESCRIPTION: Setting the login of SMTP account's owner.
//   ARGUMENTS: const char *Login - login of SMTP account's owner
// USES GLOBAL: m_sLogin
// MODIFIES GL: m_sLogin
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting the login of SMTP account's owner.
* @param Login login of SMTP account's owner
*
* @~german
* @brief SMTP Benutzername angeben
* @param Login Benutzername
*/
void gnublin_smtp::SetLogin(const char *Login)
{
	m_sLogin.erase();
	m_sLogin.insert(0,Login);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetPassword
// DESCRIPTION: Setting the password of SMTP account's owner.
//   ARGUMENTS: const char *Password - password of SMTP account's owner
// USES GLOBAL: m_sPassword
// MODIFIES GL: m_sPassword
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting the password of SMTP account's owner.
* @param Password password of SMTP account's owner
*
* @~german
* @brief Passwort des SMTP accounts angeben
* @param Password Passwort des Accounts
*/
void gnublin_smtp::SetPassword(const char *Password)
{
	m_sPassword.erase();
	m_sPassword.insert(0,Password);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSMTPServer
// DESCRIPTION: Setting the SMTP service name and port.
//   ARGUMENTS: const char* SrvName - SMTP service name
//              const unsigned short SrvPort - SMTO service port
// USES GLOBAL: m_sSMTPSrvName
// MODIFIES GL: m_sSMTPSrvName 
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JO 2010-0708
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Setting the SMTP service name and port.
* @param SrvName SMTP service name
* @param SrvPort SMTP service port
*
* @~german
* @brief SMTP Server setzen
* @param SrvName Servername
* @param SrvPort Serverport
*/
void gnublin_smtp::SetSMTPServer(const char* SrvName,const unsigned short SrvPort)
{
	m_iSMTPSrvPort = SrvPort;
	m_sSMTPSrvName.erase();
	m_sSMTPSrvName.insert(0,SrvName);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetErrorText (friend function)
// DESCRIPTION: Returns the string for specified error code.
//   ARGUMENTS: CSmtpError ErrorId - error code
// USES GLOBAL: none
// MODIFIES GL: none 
//     RETURNS: error string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
/**
* @~english
* @brief Returns the string for specified error code.
* @return error string
*
* @~german
* @brief Gibt einen String mit einem spezifischem Fehlercode zurück.
* @return Fehlerstring
*/
std::string gnublin_smtp::GetErrorText() const
{
	switch(ErrorCode)
	{
		case gnublin_smtp::CSMTP_NO_ERROR:
			return "";
		case gnublin_smtp::WSA_STARTUP:
			return "Unable to initialise winsock2";
		case gnublin_smtp::WSA_VER:
			return "Wrong version of the winsock2";
		case gnublin_smtp::WSA_SEND:
			return "Function send() failed";
		case gnublin_smtp::WSA_RECV:
			return "Function recv() failed";
		case gnublin_smtp::WSA_CONNECT:
			return "Function connect failed";
		case gnublin_smtp::WSA_GETHOSTBY_NAME_ADDR:
			return "Unable to determine remote server";
		case gnublin_smtp::WSA_INVALID_SOCKET:
			return "Invalid winsock2 socket";
		case gnublin_smtp::WSA_HOSTNAME:
			return "Function hostname() failed";
		case gnublin_smtp::WSA_IOCTLSOCKET:
			return "Function ioctlsocket() failed";
		case gnublin_smtp::BAD_IPV4_ADDR:
			return "Improper IPv4 address";
		case gnublin_smtp::UNDEF_MSG_HEADER:
			return "Undefined message header";
		case gnublin_smtp::UNDEF_MAIL_FROM:
			return "Undefined mail sender";
		case gnublin_smtp::UNDEF_SUBJECT:
			return "Undefined message subject";
		case gnublin_smtp::UNDEF_RECIPIENTS:
			return "Undefined at least one reciepent";
		case gnublin_smtp::UNDEF_RECIPIENT_MAIL:
			return "Undefined recipent mail";
		case gnublin_smtp::UNDEF_LOGIN:
			return "Undefined user login";
		case gnublin_smtp::UNDEF_PASSWORD:
			return "Undefined user password";
		case gnublin_smtp::COMMAND_MAIL_FROM:
			return "Server returned error after sending MAIL FROM";
		case gnublin_smtp::COMMAND_EHLO:
			return "Server returned error after sending EHLO";
		case gnublin_smtp::COMMAND_AUTH_LOGIN:
			return "Server returned error after sending AUTH LOGIN";
		case gnublin_smtp::COMMAND_DATA:
			return "Server returned error after sending DATA";
		case gnublin_smtp::COMMAND_QUIT:
			return "Server returned error after sending QUIT";
		case gnublin_smtp::COMMAND_RCPT_TO:
			return "Server returned error after sending RCPT TO";
		case gnublin_smtp::MSG_BODY_ERROR:
			return "Error in message body";
		case gnublin_smtp::CONNECTION_CLOSED:
			return "Server has closed the connection";
		case gnublin_smtp::SERVER_NOT_READY:
			return "Server is not ready";
		case gnublin_smtp::SERVER_NOT_RESPONDING:
			return "Server not responding";
		case gnublin_smtp::FILE_NOT_EXIST:
			return "File not exist";
		case gnublin_smtp::MSG_TOO_BIG:
			return "Message is too big";
		case gnublin_smtp::BAD_LOGIN_PASS:
			return "Bad login or password";
		case gnublin_smtp::UNDEF_XYZ_RESPONSE:
			return "Undefined xyz SMTP response";
		case gnublin_smtp::LACK_OF_MEMORY:
			return "Lack of memory";
		case gnublin_smtp::TIME_ERROR:
			return "time() error";
		case gnublin_smtp::RECVBUF_IS_EMPTY:
			return "RecvBuf is empty";
		case gnublin_smtp::SENDBUF_IS_EMPTY:
			return "SendBuf is empty";
		case gnublin_smtp::OUT_OF_MSG_RANGE:
			return "Specified line number is out of message size";
		default:
			return "Undefined error id";
	}
}

