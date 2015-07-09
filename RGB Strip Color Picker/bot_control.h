/*
Copyright (c) 2015 Sigge Stjärnholm

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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