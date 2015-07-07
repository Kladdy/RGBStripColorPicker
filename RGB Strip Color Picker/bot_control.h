#include "tserial.h"

class serial{

  private:
        // private attributes
		Tserial *com;
  public:


	serial() {
			
		 }
	
	bool startDevice(char *port,int speed)
	{
		com = new Tserial();
		if (com!=0)
		{
			if(com->connect(port, speed, spNONE))
				printf("Failed to connect to Arduino. Please check that it is plugged in to the computer and no other serial processes are interacting with it. \n");
			else
				printf("Successfully connected to the Arduino\n");
			return TRUE;
		}
		else
			return FALSE; 
	}

	void stopDevice()
	{
		com->disconnect();
        // ------------------
        delete com;
        com = 0;
	}

	void send_data(unsigned char data)
	{
	//	unsigned char data = 0;
	
		
		com->sendChar(data);
		printf("%c",data);
		
	}
};