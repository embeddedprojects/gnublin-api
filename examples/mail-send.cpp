/*############################################
Note you have to change the server parameters
##############################################*/

#include "gnublin.h"
#include <iostream>

int main()
{
	bool bError = false;

	try
	{
		gnublin_smtp mail;

		mail.SetSMTPServer("mail.domain.tld",587);
		mail.SetLogin("smtptest@domain.tld");
		mail.SetPassword("secretpassword");
  		mail.SetSenderName("User");
  		mail.SetSenderMail("smtptest@domain.tld");
//  		mail.SetReplyTo("user@domain.com");
  		mail.SetSubject("The message 2");
  		mail.AddRecipient("recipient@domain.tld");
  		mail.SetXPriority(XPRIORITY_NORMAL);
  		mail.SetXMailer("The Bat! (v3.02) Professional");
  		mail.AddMsgLine("Hello,");
		mail.AddMsgLine("");
		mail.AddMsgLine("...");
		mail.AddMsgLine("How are you today?");
		mail.AddMsgLine("");
		mail.AddMsgLine("Regards");
		mail.ModMsgLine(5,"regards");
		mail.DelMsgLine(2);
		mail.AddMsgLine("User");
		
		mail.Send();
	}
	catch(gnublin_smtp e)
	{
		std::cout << "Error: " << e.GetErrorText().c_str() << ".\n";
		bError = true;
	}
	if(!bError)
		std::cout << "Mail was send successfully.\n";
	return 0;
}
