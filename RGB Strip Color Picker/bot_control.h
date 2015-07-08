#include "tserial.h"
extern bool connFail = true;

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
			if (com->connect(port, speed, spNONE)){
				printf("Failed to connect to Arduino.\n");
				connFail = true;
			}
			else{
				printf("Successfully connected to Arduino\n");
				connFail = false;
			}
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
		
	}
};