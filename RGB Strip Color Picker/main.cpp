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

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <sstream> 
#include <string>
#include <stdio.h>
#include <conio.h>
#include "tserial.h"
#include "bot_control.h"
#include <stdlib.h>    
#include <time.h>  
#include <assert.h> 

using namespace std;

serial comm;

sf::Vertex vertex(sf::Vector2f(100, 0), sf::Color::Black, sf::Vector2f(100, 100));

sf::VertexArray line(sf::Lines, 10);
sf::VertexArray lineBox(sf::Lines, 8);
sf::VertexArray menuFade(sf::Quads, 20);
sf::VertexArray hueBar(sf::Quads, 24);
sf::VertexArray lineBox2(sf::Lines, 8);
sf::VertexArray lineBox3(sf::Lines, 8);
sf::RectangleShape currentColor(sf::Vector2f(70, 70));
sf::RectangleShape chooseColor(sf::Vector2f(58, 100));
sf::RectangleShape chooseWhite(sf::Vector2f(58, 100));
sf::RectangleShape chooseBlack(sf::Vector2f(58, 100));

sf::Text globalText[28];

int numChars;

void menuOverview();
void menuLightning();
void menuInstructions();
void menuOptions();
void menuCredits();
void getValue();
void setValue(int com, int baud, int dark);
template <typename T>
T StringToNumber(const string &Text, T defValue);
void sendData(int lightMode);
void previewLightning(int previewMode);
void applyLight(int lightMode);

int portCOM;
int baudRate;
bool darkMode;
int rates[12] = { 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200 };

char charData0[4];
char charData1[10];
char charData2[3];
char charData3[3];
char charData4[3];

int valRed = 255;
int valGreen = 0;
int valBlue = 0;
int fadeMode = 0;

int oldVal[3] = { 255, 0 ,0 };

int currentMenu = 0; //0: Overview, 1: Lightning, 2: Instructions, 3: Options: 4: Credits
int lightningMenu = 0; //0: Static, 1: Rainbow, 2: Cycle, 3: Fade 
int colorMode = 0;
int speed = 1;
int streamMode = 0;
bool hasTested = false;

sf::Vector2i mousePos;
sf::Sprite logo256Sprite;

int main(){

	getValue();

	sf::RenderWindow window(sf::VideoMode(800, 500), "RGB Strip Color Picker");
	window.setFramerateLimit(60);

	sf::Image icon;
	if (!icon.loadFromFile("Assets/logo256.png")){
		cout << "Error loading 'logo256.png' for icon." << endl;
	}
	window.setIcon(256, 256, icon.getPixelsPtr());

	sf::Texture logo256;
	if (!logo256.loadFromFile("Assets/logo256.png")){
		cout << "Error loading 'logo256.png'." << endl;
	}

	logo256Sprite.setTexture(logo256);
	logo256Sprite.setPosition(sf::Vector2f(662, 10));

	sf::Texture CWB1;
	sf::Texture CWB2;
	
	if (!CWB1.loadFromFile("Assets/CWBWhite.png")){
		cout << "Error loading 'CWBWhite.png'." << endl;
	}
	
	if (!CWB2.loadFromFile("Assets/CWB.png")){
		cout << "Error loading 'CWB.png'." << endl;
	}
	

	sf::Sprite CWBSprite;
	CWBSprite.setPosition(sf::Vector2f(210, 133));

	sf::Texture barHolder1;
	sf::Texture barHolder2;
	
	if (!barHolder1.loadFromFile("Assets/barHolderWhite.png")){
		cout << "Error loading 'barHolderWhite.png'." << endl;
	}
	
	if (!barHolder2.loadFromFile("Assets/barHolder.png")){
		cout << "Error loading 'barHolder.png'." << endl;
	}

	sf::Sprite barHolderSprite;
	barHolderSprite.setPosition(sf::Vector2f(273, 131));

	sf::Texture arrowLeft1;
	sf::Texture arrowLeft2;

	if (!arrowLeft1.loadFromFile("Assets/ArrowLeftWhite.png")){
		cout << "Error loading 'arrowLeftWhite.png'." << endl;
	}

	if (!arrowLeft2.loadFromFile("Assets/ArrowLeft.png")){
		cout << "Error loading 'arrowLeft.png'." << endl;
	}
	


	sf::Sprite arrowLeftSprite;
	arrowLeftSprite.setPosition(sf::Vector2f(305, 177));

	sf::Texture arrowRight1;
	sf::Texture arrowRight2;

	if (!arrowRight1.loadFromFile("Assets/ArrowRightWhite.png")){
		cout << "Error loading 'arrowRightWhite.png'." << endl;
	}

	if (!arrowRight2.loadFromFile("Assets/ArrowRight.png")){
		cout << "Error loading 'arrowRight.png'." << endl;
	}

	sf::Sprite arrowRightSprite;
	arrowRightSprite.setPosition(sf::Vector2f(393, 177));

	sf::Texture arrowRightBlue;
	if (!arrowRightBlue.loadFromFile("Assets/arrowRightBlue.png")){
		cout << "Error loading 'arrowRightBlue.png'." << endl;
	}

	sf::Sprite arrowRightBlueSprite;
	arrowRightBlueSprite.setTexture(arrowRightBlue);
	arrowRightBlueSprite.setPosition(210, 200);

	sf::Texture GHLogo1;
	sf::Texture GHLogo2;
	
	if (!GHLogo1.loadFromFile("Assets/GHLogoWhite.png")){
		cout << "Error loading 'GHLogoWhite.png'." << endl;
	}

	if (!GHLogo2.loadFromFile("Assets/GHLogo.png")){
		cout << "Error loading 'GHLogo.png'." << endl;
	}
	
	sf::Sprite GHLogoSprite;
	GHLogoSprite.setPosition(210, 415);

	sf::Texture TLogo;
	if (!TLogo.loadFromFile("Assets/TLogo.png")){
		cout << "Error loading 'TLogo.png'." << endl;
	}

	sf::Sprite TLogoSprite;
	TLogoSprite.setTexture(TLogo);
	TLogoSprite.setPosition(294, 415);

	sf::Texture InstructablesLogo;
	if (!InstructablesLogo.loadFromFile("Assets/InstructablesLogo.png")){
		cout << "Error loading 'InstructablesLogo.png'." << endl;
	}
	sf::Sprite InstructablesLogoSprite;
	InstructablesLogoSprite.setTexture(InstructablesLogo);
	InstructablesLogoSprite.setPosition(210, 200);

	sf::Font font;
	if (!font.loadFromFile("Assets/Roboto-Light.ttf")){
		cout << "Error loading font 'Roboto-Light'." << endl;
	}

	globalText[0].setPosition(60, 32);
	globalText[1].setPosition(60, 132);
	globalText[2].setPosition(25, 232);
	globalText[3].setPosition(80, 332);
	globalText[4].setPosition(85, 432);

	globalText[5].setPosition(210, 10);

	globalText[6].setPosition(301, 10);
	globalText[7].setPosition(412, 10);
	globalText[8].setPosition(552, 10);
	globalText[9].setPosition(680, 10);
	globalText[10].setPosition(210, 140);
	globalText[11].setPosition(270, 140);
	globalText[12].setPosition(650, 470);
	globalText[13].setPosition(741, 470);
	globalText[14].setPosition(388, 213);
	globalText[15].setPosition(387, 238);
	globalText[16].setPosition(388, 263);
	globalText[17].setPosition(210, 45);
	globalText[18].setPosition(210, 160);
	globalText[19].setPosition(350, 160);

	globalText[20].setPosition(210, 10);
	globalText[21].setPosition(210, 130);
	globalText[22].setPosition(210, 180);
	globalText[23].setPosition(210, 230);
	globalText[24].setPosition(210, 280);
	globalText[25].setPosition(475, 470);

	globalText[26].setPosition(210, 10);

	globalText[27].setPosition(210, 10);

	globalText[0].setString("Overview");
	globalText[1].setString("Lightning");
	globalText[2].setString("Instructions");
	globalText[3].setString("Options");
	globalText[4].setString("Credits");

	globalText[5].setString("Thank you for using RGB Strip Color Picker! \nThis is a tool which you can use to modify your \nRGB color strip attached to an Arduino. Before you \nstart, make sure to read the instructions and check \nout the GitHub page for more information on how \nyou should use this tool. \n\nMore functions and modes will be added by request, so head over \nto the 'Credits' tab and see how you can give feedback and \ncontribute to the project.");

	globalText[6].setString("Static");
	globalText[7].setString("Rainbow");
	globalText[8].setString("Cycle");
	globalText[9].setString("Fade");
	globalText[10].setString("RGB");
	globalText[11].setString("Random");
	globalText[12].setString("Preview");
	globalText[13].setString("Apply");
	globalText[14].setString("R:  " + to_string(valRed));
	globalText[15].setString("G:  " + to_string(valGreen));
	globalText[16].setString("B:  " + to_string(valBlue));
	globalText[17].setString("Sets a static color to the RGB strip. This color will remain unless \nchanged. Choose between white, black or a custom color from \nthe hue bar.");
	globalText[18].setString("Downstream");
	globalText[19].setString("Upstream");

	globalText[20].setString("This is where you can change your options for the program. \nBefore playing around with the different settings, it's important to \nread the instructions to set it up correctly.");
	globalText[21].setString("");
	globalText[22].setString("");
	globalText[23].setString("Dark mode:    ");
	globalText[24].setString("");
	globalText[25].setString("Test connection on current settings");
	globalText[26].setString(L"This software was created by Sigge \"Kladdy\" Stjärnholm with the \nhelp of the Arduino and Stack Overflow communities. SFML was \nused to display graphics as well as handling the window. \nA re-written version of Thierry Schneider's serial library was used \nfor the Arduino communication. \n\nIf you want to contribute to this project, e-mail me at \nsigge.stjarnholm@gmail.com with feature requests or general \nquestions. Issues can be reported the same way or via GitHub. \nThere you can also contribute with your own source code and \nmake any changes for your own version of the program. Click the \nGitHub button below to go to the repository. My Twitter can also \nbe used for communication.");

	globalText[27].setString("If you're not sure on how to use this tool, refer to the guide on \nInstructables. Click the robot below to get to the tutorial. If that \ncouldn't solve your issue, go to the 'Credits' tab to get contact \ninformation.");

	globalText[6].setColor(sf::Color(0, 170, 255));
	globalText[10].setColor(sf::Color(0, 170, 255));
	globalText[18].setColor(sf::Color(0, 170, 255));


	lineBox[0].position = sf::Vector2f(313, 236);
	lineBox[1].position = sf::Vector2f(328, 236);
	lineBox[2].position = sf::Vector2f(328, 236);
	lineBox[3].position = sf::Vector2f(328, 251);
	lineBox[4].position = sf::Vector2f(328, 251);
	lineBox[5].position = sf::Vector2f(313, 251);
	lineBox[6].position = sf::Vector2f(313, 236);
	lineBox[7].position = sf::Vector2f(313, 251);

	lineBox2[0].position = sf::Vector2f(388, 133);
	lineBox2[1].position = sf::Vector2f(458, 133);
	lineBox2[2].position = sf::Vector2f(458, 133);
	lineBox2[3].position = sf::Vector2f(458, 203);
	lineBox2[4].position = sf::Vector2f(458, 203);
	lineBox2[5].position = sf::Vector2f(388, 203);
	lineBox2[6].position = sf::Vector2f(388, 133);
	lineBox2[7].position = sf::Vector2f(388, 203);

	lineBox3[0].position = sf::Vector2f(277, 133);
	lineBox3[1].position = sf::Vector2f(377, 133);
	lineBox3[2].position = sf::Vector2f(377, 133);
	lineBox3[3].position = sf::Vector2f(377, 433);
	lineBox3[4].position = sf::Vector2f(377, 433);
	lineBox3[5].position = sf::Vector2f(277, 433);
	lineBox3[6].position = sf::Vector2f(277, 133);
	lineBox3[7].position = sf::Vector2f(277, 433);

	line[0].position = sf::Vector2f(200, 10);
	line[1].position = sf::Vector2f(200, 490);
	line[2].position = sf::Vector2f(10, 100);
	line[3].position = sf::Vector2f(200, 100);
	line[4].position = sf::Vector2f(10, 200);
	line[5].position = sf::Vector2f(200, 200);
	line[6].position = sf::Vector2f(10, 300);
	line[7].position = sf::Vector2f(200, 300);
	line[8].position = sf::Vector2f(10, 400);
	line[9].position = sf::Vector2f(200, 400);

	menuFade[0].position = sf::Vector2f(10, 10);
	menuFade[1].position = sf::Vector2f(200, 10);
	menuFade[2].position = sf::Vector2f(200, 100);
	menuFade[3].position = sf::Vector2f(10, 100);

	menuFade[4].position = sf::Vector2f(10, 100);
	menuFade[5].position = sf::Vector2f(200, 100);
	menuFade[6].position = sf::Vector2f(200, 200);
	menuFade[7].position = sf::Vector2f(10, 200);

	menuFade[8].position = sf::Vector2f(10, 200);
	menuFade[9].position = sf::Vector2f(200, 200);
	menuFade[10].position = sf::Vector2f(200, 300);
	menuFade[11].position = sf::Vector2f(10, 300);

	menuFade[12].position = sf::Vector2f(10, 300);
	menuFade[13].position = sf::Vector2f(200, 300);
	menuFade[14].position = sf::Vector2f(200, 400);
	menuFade[15].position = sf::Vector2f(10, 400);

	menuFade[16].position = sf::Vector2f(10, 400);
	menuFade[17].position = sf::Vector2f(200, 400);
	menuFade[18].position = sf::Vector2f(200, 490);
	menuFade[19].position = sf::Vector2f(10, 490);

	hueBar[0].position = sf::Vector2f(277, 133);
	hueBar[1].position = sf::Vector2f(377, 133);
	hueBar[2].position = sf::Vector2f(377, 183);
	hueBar[3].position = sf::Vector2f(277, 183);

	hueBar[4].position = sf::Vector2f(277, 183);
	hueBar[5].position = sf::Vector2f(377, 183);
	hueBar[6].position = sf::Vector2f(377, 233);
	hueBar[7].position = sf::Vector2f(277, 233);

	hueBar[8].position = sf::Vector2f(277, 233);
	hueBar[9].position = sf::Vector2f(377, 233);
	hueBar[10].position = sf::Vector2f(377, 283);
	hueBar[11].position = sf::Vector2f(277, 283);

	hueBar[12].position = sf::Vector2f(277, 283);
	hueBar[13].position = sf::Vector2f(377, 283);
	hueBar[14].position = sf::Vector2f(377, 333);
	hueBar[15].position = sf::Vector2f(277, 333);

	hueBar[16].position = sf::Vector2f(277, 333);
	hueBar[17].position = sf::Vector2f(377, 333);
	hueBar[18].position = sf::Vector2f(377, 383);
	hueBar[19].position = sf::Vector2f(277, 383);

	hueBar[20].position = sf::Vector2f(277, 383);
	hueBar[21].position = sf::Vector2f(377, 383);
	hueBar[22].position = sf::Vector2f(377, 433);
	hueBar[23].position = sf::Vector2f(277, 433);

	hueBar[0].color = sf::Color::Red;
	hueBar[1].color = sf::Color::Red;
	hueBar[2].color = sf::Color::Yellow;
	hueBar[3].color = sf::Color::Yellow;

	hueBar[4].color = sf::Color::Yellow;
	hueBar[5].color = sf::Color::Yellow;
	hueBar[6].color = sf::Color::Green;
	hueBar[7].color = sf::Color::Green;

	hueBar[8].color = sf::Color::Green;
	hueBar[9].color = sf::Color::Green;
	hueBar[10].color = sf::Color::Cyan;
	hueBar[11].color = sf::Color::Cyan;

	hueBar[12].color = sf::Color::Cyan;
	hueBar[13].color = sf::Color::Cyan;
	hueBar[14].color = sf::Color::Blue;
	hueBar[15].color = sf::Color::Blue;

	hueBar[16].color = sf::Color::Blue;
	hueBar[17].color = sf::Color::Blue;
	hueBar[18].color = sf::Color::Magenta;
	hueBar[19].color = sf::Color::Magenta;

	hueBar[20].color = sf::Color::Magenta;
	hueBar[21].color = sf::Color::Magenta;
	hueBar[22].color = sf::Color::Red;
	hueBar[23].color = sf::Color::Red;

	currentColor.setPosition(sf::Vector2f(388, 133));
	currentColor.setFillColor(sf::Color(valRed, valGreen, valBlue));

	chooseColor.setPosition(sf::Vector2f(210, 133));
	chooseColor.setFillColor(sf::Color(valRed, valGreen, valBlue));
	chooseWhite.setPosition(sf::Vector2f(210, 233));
	chooseWhite.setFillColor(sf::Color::White);
	chooseBlack.setPosition(sf::Vector2f(210, 333));
	chooseBlack.setFillColor(sf::Color::Black);

	menuOverview();

	cout << "COM: " + to_string(portCOM) + " Baud: " + to_string(rates[baudRate]) + " Dark: " + to_string(darkMode) << endl;



	while (window.isOpen())
	{

		mousePos = sf::Mouse::getPosition(window);
		
		if (darkMode){
			CWBSprite.setTexture(CWB1);
			barHolderSprite.setTexture(barHolder1);
			arrowLeftSprite.setTexture(arrowLeft1);
			arrowRightSprite.setTexture(arrowRight1);
			GHLogoSprite.setTexture(GHLogo1);

			for (int i = 0; i < 5; i++){
				globalText[i].setFont(font);
				globalText[i].setColor(sf::Color::White);
			}
			for (int i = 5; i < 28; i++){
				globalText[i].setFont(font);
				globalText[i].setColor(sf::Color::White);
				globalText[i].setCharacterSize(20);
			}
			for (int i = 0; i < 8; i++){
				lineBox[i].color = sf::Color::White;
				lineBox2[i].color = sf::Color::White;
				lineBox3[i].color = sf::Color::White;
			}
			for (int i = 0; i < 10; i++){
				line[i].color = sf::Color::White;
			}
		}
		else{
			CWBSprite.setTexture(CWB2);
			barHolderSprite.setTexture(barHolder2);
			arrowLeftSprite.setTexture(arrowLeft2);
			arrowRightSprite.setTexture(arrowRight2);
			GHLogoSprite.setTexture(GHLogo2);
			for (int i = 0; i < 5; i++){
				globalText[i].setFont(font);
				globalText[i].setColor(sf::Color::Black);
			}
			for (int i = 5; i < 28; i++){
				globalText[i].setFont(font);
				globalText[i].setColor(sf::Color::Black);
				globalText[i].setCharacterSize(20);
			}
			for (int i = 0; i < 8; i++){
				lineBox[i].color = sf::Color::Black;
				lineBox2[i].color = sf::Color::Black;
				lineBox3[i].color = sf::Color::Black;
			}
			for (int i = 0; i < 10; i++){
				line[i].color = sf::Color::Black;
			}
			
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed){

				if (mousePos.x >= 10 && mousePos.x <= 200 && mousePos.y >= 10 && mousePos.y <= 100){ //Button, Overview
					currentMenu = 0;
					menuOverview();
				}
				else if (mousePos.x >= 10 && mousePos.x <= 200 && mousePos.y >= 100 && mousePos.y <= 200){ //Button, Lightning
					currentMenu = 1;
					menuLightning();
				}
				else if (mousePos.x >= 10 && mousePos.x <= 200 && mousePos.y >= 200 && mousePos.y <= 300){ //Button, Instructions
					currentMenu = 2;
					menuInstructions();
				}
				else if (mousePos.x >= 10 && mousePos.x <= 200 && mousePos.y >= 300 && mousePos.y <= 400){ //Button, Options
					currentMenu = 3;
					hasTested = false;
					menuOptions();
				}
				else if (mousePos.x >= 10 && mousePos.x <= 200 && mousePos.y >= 400 && mousePos.y <= 490){ //Button, Credits
					currentMenu = 4;
					menuCredits();
				}
				else if (mousePos.x >= 313 && mousePos.x <= 328 && mousePos.y >= 236 && mousePos.y <= 251 && currentMenu == 3){ //Checkbox, Dark mode
					if (darkMode){
						darkMode = false;
						setValue(portCOM, baudRate, 0);
					}
					else{
						darkMode = true;
						setValue(portCOM, baudRate, 1);
					}
				}
				else if (mousePos.x >= 314 && mousePos.x <= 326 && mousePos.y >= 183 && mousePos.y <= 201 && currentMenu == 3){ //Arrow, -Baud 
					baudRate--;
					if (baudRate == -1){
						baudRate = 11;
					}
					setValue(portCOM, baudRate, darkMode);
				}
				else if (mousePos.x >= 402 && mousePos.x <= 414 && mousePos.y >= 183 && mousePos.y <= 201 && currentMenu == 3){ //Arrow, +Baud
					baudRate++;
					if (baudRate == 12){
						baudRate = 0;
					}
					setValue(portCOM, baudRate, darkMode);
				}
				else if (mousePos.x >= 314 && mousePos.x <= 326 && mousePos.y >= 133 && mousePos.y <= 151 && currentMenu == 3){ //Arrow, -COM
					portCOM--;
					if (portCOM == 0){
						portCOM = 10;
					}
					setValue(portCOM, baudRate, darkMode);
				}
				else if (mousePos.x >= 356 && mousePos.x <= 368 && mousePos.y >= 133 && mousePos.y <= 151 && currentMenu == 3){ //Arrow, +COM
					portCOM++;
					if (portCOM == 11){
						portCOM = 1;
					}
					setValue(portCOM, baudRate, darkMode);
				}
				else if (mousePos.x >= 210 && mousePos.x <= 284 && mousePos.y >= 415 && mousePos.y <= 491 && currentMenu == 4){ //Link on image, GitHub
					system("start https://github.com/Kladdy/RGBStripColorPicker");
				}
				else if (mousePos.x >= 294 && mousePos.x <= 368 && mousePos.y >= 415 && mousePos.y <= 491 && currentMenu == 4){ //Link on image, Twitter
					system("start https://twitter.com/iKladdy");
				}
				else if (mousePos.x >= 210 && mousePos.x <= 430 && mousePos.y >= 200 && mousePos.y <= 488 && currentMenu == 2){ //Link on image, Instructables
					system("start http://www.instructables.com/id/How-to-use-RGB-Strip-Color-Picker/");
				}
				else if (mousePos.x >= 301 && mousePos.x <= 349 && mousePos.y >= 10 && mousePos.y <= 30 && currentMenu == 1){ //Text, Static
					lightningMenu = 0;
				}
				else if (mousePos.x >= 412 && mousePos.x <= 487 && mousePos.y >= 10 && mousePos.y <= 30 && currentMenu == 1){ //Text, Rainbow
					streamMode = 0;
					speed = 1;
					lightningMenu = 1;
				}
				else if (mousePos.x >= 552 && mousePos.x <= 597 && mousePos.y >= 10 && mousePos.y <= 30 && currentMenu == 1){ //Text, Cycle				
					speed = 1;
					lightningMenu = 2;
				}
				else if (mousePos.x >= 680 && mousePos.x <= 720 && mousePos.y >= 10 && mousePos.y <= 30 && currentMenu == 1){ //Text, Fade
					speed = 1;
					fadeMode = 0;
					lightningMenu = 3;
				}
				else if (mousePos.x >= 650 && mousePos.x <= 716 && mousePos.y >= 470 && mousePos.y <= 490 && currentMenu == 1){ //Text, Preview

					previewLightning(lightningMenu);

				}
				else if (mousePos.x >= 741 && mousePos.x <= 788 && mousePos.y >= 470 && mousePos.y <= 490 && currentMenu == 1){ //Text, Apply

					applyLight(lightningMenu);

				}
				else if (mousePos.x >= 475 && mousePos.x <= 787 && mousePos.y >= 470 && mousePos.y <= 490 && currentMenu == 3){ //Text, Test Connection

					sendData(0);
					hasTested = true;

				}
				else if (mousePos.x >= 210 && mousePos.x <= 268 && mousePos.y >= 133 && mousePos.y <= 233 && currentMenu == 1 && lightningMenu == 0){  //Button, Color

					valRed = oldVal[0];
					valGreen = oldVal[1];
					valBlue = oldVal[2];

					colorMode = 0;
				}
				else if (mousePos.x >= 210 && mousePos.x <= 268 && mousePos.y >= 233 && mousePos.y <= 333 && currentMenu == 1 && lightningMenu == 0){ //Button, White

					if (colorMode == 0){
						oldVal[0] = valRed;
						oldVal[1] = valGreen;
						oldVal[2] = valBlue;
					}

					valRed = 255;
					valGreen = 255;
					valBlue = 255;

					colorMode = 1;
				}
				else if (mousePos.x >= 210 && mousePos.x <= 268 && mousePos.y >= 333 && mousePos.y <= 433 && currentMenu == 1 && lightningMenu == 0){ //Button, Black

					if (colorMode == 0){
						oldVal[0] = valRed;
						oldVal[1] = valGreen;
						oldVal[2] = valBlue;
					}

					valRed = 0;
					valGreen = 0;
					valBlue = 0;

					colorMode = 2;
				}
				else if (mousePos.x >= 219 && mousePos.x <= 231 && mousePos.y >= 206 && mousePos.y <= 224 && currentMenu == 1 && lightningMenu == 1){ //Button, Slow

					speed = 0;
				}
				else if (mousePos.x >= 259 && mousePos.x <= 281 && mousePos.y >= 206 && mousePos.y <= 224 && currentMenu == 1 && lightningMenu == 1){ //Button, Normal

					speed = 1;
				}
				else if (mousePos.x >= 309 && mousePos.x <= 341 && mousePos.y >= 206 && mousePos.y <= 224 && currentMenu == 1 && lightningMenu == 1){ //Button, Fast

					speed = 2;
				}
				else if (mousePos.x >= 219 && mousePos.x <= 231 && mousePos.y >= 121 && mousePos.y <= 139 && currentMenu == 1 && lightningMenu == 2){ //Button, Slow

					speed = 0;
				}
				else if (mousePos.x >= 259 && mousePos.x <= 281 && mousePos.y >= 121 && mousePos.y <= 139 && currentMenu == 1 && lightningMenu == 2){ //Button, Normal

					speed = 1;
				}
				else if (mousePos.x >= 309 && mousePos.x <= 341 && mousePos.y >= 121 && mousePos.y <= 139 && currentMenu == 1 && lightningMenu == 2){ //Button, Fast

					speed = 2;
				}
				else if (mousePos.x >= 219 && mousePos.x <= 231 && mousePos.y >= 191 && mousePos.y <= 209 && currentMenu == 1 && lightningMenu == 3){ //Button, Slow

					speed = 0;
				}
				else if (mousePos.x >= 259 && mousePos.x <= 281 && mousePos.y >= 191 && mousePos.y <= 209 && currentMenu == 1 && lightningMenu == 3){ //Button, Normal

					speed = 1;
				}
				else if (mousePos.x >= 309 && mousePos.x <= 341 && mousePos.y >= 191 && mousePos.y <= 209 && currentMenu == 1 && lightningMenu == 3){ //Button, Fast

					speed = 2;
				}
				else if (mousePos.x >= 210 && mousePos.x <= 320 && mousePos.y >= 160 && mousePos.y <= 180 && currentMenu == 1 && lightningMenu == 1){ //Text, Downstream
					streamMode = 0;
				}
				else if (mousePos.x >= 350 && mousePos.x <= 434 && mousePos.y >= 160 && mousePos.y <= 180 && currentMenu == 1 && lightningMenu == 1){ //Text, Upstream
					streamMode = 1;
				}
				else if (mousePos.x >= 210 && mousePos.x <= 247 && mousePos.y >= 140 && mousePos.y <= 160 && currentMenu == 1 && lightningMenu == 3){ //Text, RGB
					fadeMode = 0;
				}
				else if (mousePos.x >= 270 && mousePos.x <= 343 && mousePos.y >= 140 && mousePos.y <= 160 && currentMenu == 1 && lightningMenu == 3){ //Text, Random
					fadeMode = 1;
				}
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && currentMenu == 1 && lightningMenu == 0 && mousePos.x >= 277 && mousePos.x <= 377 && mousePos.y >= 133 && mousePos.y <= 433){

			if (mousePos.y >= 133 && mousePos.y <= 183){
				valRed = 255;
			}
			else if (mousePos.y >= 183 && mousePos.y <= 233){
				valRed = 255 - ((mousePos.y - 183) * 5.1);
			}
			else if (mousePos.y >= 333 && mousePos.y <= 383){
				valRed = ((mousePos.y - 333) * 5.1);
			}
			else if (mousePos.y >= 383 && mousePos.y <= 433){
				valRed = 255;
			}
			else{
				valRed = 0;
			}

			if (mousePos.y >= 133 && mousePos.y <= 183){
				valGreen = ((mousePos.y - 133) * 5.1);
			}
			else if (mousePos.y >= 183 && mousePos.y <= 283){
				valGreen = 255;
			}
			else if (mousePos.y >= 283 && mousePos.y <= 333){
				valGreen = 255 - ((mousePos.y - 283) * 5.1);
			}
			else{
				valGreen = 0;
			}

			if (mousePos.y >= 283 && mousePos.y <= 383){
				valBlue = 255;
			}
			else if (mousePos.y >= 233 && mousePos.y <= 283){
				valBlue = ((mousePos.y - 233) * 5.1);
			}
			else if (mousePos.y >= 383 && mousePos.y <= 433){
				valBlue = 255 - ((mousePos.y - 383) * 5.1);
			}
			else{
				valBlue = 0;
			}
			chooseColor.setFillColor(sf::Color(valRed, valGreen, valBlue));
			oldVal[0] = valRed;
			oldVal[1] = valGreen;
			oldVal[2] = valBlue;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && currentMenu == 1 && lightningMenu == 0 && mousePos.x >= 277 && mousePos.x <= 377 && mousePos.y >= 53 && mousePos.y <= 513){

			if (mousePos.y <= 134){
				barHolderSprite.setPosition(273, 131);
				valRed = 255;
				valGreen = 0;
				valBlue = 0;
			}
			else if (mousePos.y >= 430){
				barHolderSprite.setPosition(273, 427);
				valRed = 255;
				valGreen = 0;
				valBlue = 0;
			}
			else{
				barHolderSprite.setPosition(273, mousePos.y - 4);
			}
			chooseColor.setFillColor(sf::Color(valRed, valGreen, valBlue));
		}

		if (darkMode){
			window.clear(sf::Color::Black);
		}
		else{
			window.clear(sf::Color::White);
		}
		window.draw(menuFade);
		window.draw(line);
		for (int i = 0; i < 5; i++){
			window.draw(globalText[i]);
		}

		if (currentMenu == 0){
			//Overview
			menuOverview();
			window.draw(globalText[5]);
			window.draw(logo256Sprite);

		}
		else if (currentMenu == 1){
			//Lightning
			menuLightning();
			if (lightningMenu == 0){
				if (darkMode){
					for (int i = 6; i < 10; i++){
						globalText[i].setColor(sf::Color::White);
					}
				}
				else{
					for (int i = 6; i < 10; i++){
						globalText[i].setColor(sf::Color::Black);
					}
				}
				globalText[6].setColor(sf::Color(0, 170, 255));
				globalText[17].setString("Sets a static color to the RGB strip. This color will remain unless \nchanged. Choose between white, black or a custom color from \nthe hue bar.");
				globalText[14].setString("R:  " + to_string(valRed));
				globalText[15].setString("G:  " + to_string(valGreen));
				globalText[16].setString("B:  " + to_string(valBlue));
				currentColor.setFillColor(sf::Color(valRed, valGreen, valBlue));
				window.draw(chooseColor);
				window.draw(chooseWhite);
				window.draw(chooseBlack);
				window.draw(CWBSprite);
				window.draw(hueBar);
				window.draw(lineBox3);
				window.draw(barHolderSprite);
				window.draw(currentColor);
				window.draw(lineBox2);
				for (int i = 14; i < 17; i++){
					window.draw(globalText[i]);
				}

			}
			else if (lightningMenu == 1){
				if (darkMode){
					for (int i = 6; i < 10; i++){
						globalText[i].setColor(sf::Color::White);
					}
				}
				else{
					for (int i = 6; i < 10; i++){
						globalText[i].setColor(sf::Color::Black);
					}
				}
				globalText[7].setColor(sf::Color(0, 170, 255));
				window.draw(globalText[7]);
				globalText[18].setColor(sf::Color(0, 170, 255));
				if (darkMode){
					globalText[19].setColor(sf::Color::White);
				}
				else{
					globalText[19].setColor(sf::Color::Black);
				}

				globalText[17].setString("Sets each LED to cycle between red, yellow, green, cyan, blue \nand magenta at different intervals, creating a rainbow-like feel. \nChoose between going downstream or upstream and set the level \nof speed. ");
				if (streamMode == 0){
					globalText[18].setColor(sf::Color(0, 170, 255));
					if (darkMode){
						globalText[19].setColor(sf::Color::White);
					}
					else{
						globalText[19].setColor(sf::Color::Black);
					}
				}
				else if (streamMode == 1){
					if (darkMode){
						globalText[18].setColor(sf::Color::White);
					}
					else{
						globalText[18].setColor(sf::Color::Black);
					}
					globalText[19].setColor(sf::Color(0, 170, 255));
				}

				if (speed == 0){
					arrowRightBlueSprite.setPosition(210, 200);
					window.draw(arrowRightBlueSprite);
					arrowRightSprite.setPosition(250, 200);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(260, 200);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(300, 200);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(310, 200);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(320, 200);
					window.draw(arrowRightSprite);
				}
				else if (speed == 1){
					arrowRightBlueSprite.setPosition(250, 200);
					window.draw(arrowRightBlueSprite);
					arrowRightBlueSprite.setPosition(260, 200);
					window.draw(arrowRightBlueSprite);
					arrowRightSprite.setPosition(210, 200);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(300, 200);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(310, 200);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(320, 200);
					window.draw(arrowRightSprite);
				}
				else if (speed == 2){
					arrowRightBlueSprite.setPosition(300, 200);
					window.draw(arrowRightBlueSprite);
					arrowRightBlueSprite.setPosition(310, 200);
					window.draw(arrowRightBlueSprite);
					arrowRightBlueSprite.setPosition(320, 200);
					window.draw(arrowRightBlueSprite);
					arrowRightSprite.setPosition(210, 200);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(250, 200);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(260, 200);
					window.draw(arrowRightSprite);
				}
				for (int i = 18; i < 20; i++){
					window.draw(globalText[i]);
				}
			}
			else if (lightningMenu == 2){
				if (darkMode){
					for (int i = 6; i < 10; i++){
						globalText[i].setColor(sf::Color::White);
					}
				}
				else{
					for (int i = 6; i < 10; i++){
						globalText[i].setColor(sf::Color::Black);
					}
				}
				globalText[8].setColor(sf::Color(0, 170, 255));
				globalText[17].setString("Sets all LEDs to cycle between red, yellow, green, cyan, blue and \nmagenta at the same time. Choose the speed of the cycle.");
				if (speed == 0){
					arrowRightBlueSprite.setPosition(210, 115);
					window.draw(arrowRightBlueSprite);
					arrowRightSprite.setPosition(250, 115);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(260, 115);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(300, 115);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(310, 115);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(320, 115);
					window.draw(arrowRightSprite);
				}
				else if (speed == 1){
					arrowRightBlueSprite.setPosition(250, 115);
					window.draw(arrowRightBlueSprite);
					arrowRightBlueSprite.setPosition(260, 115);
					window.draw(arrowRightBlueSprite);
					arrowRightSprite.setPosition(210, 115);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(300, 115);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(310, 115);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(320, 115);
					window.draw(arrowRightSprite);
				}
				else if (speed == 2){
					arrowRightBlueSprite.setPosition(300, 115);
					window.draw(arrowRightBlueSprite);
					arrowRightBlueSprite.setPosition(310, 115);
					window.draw(arrowRightBlueSprite);
					arrowRightBlueSprite.setPosition(320, 115);
					window.draw(arrowRightBlueSprite);
					arrowRightSprite.setPosition(210, 115);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(250, 115);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(260, 115);
					window.draw(arrowRightSprite);
				}
			}
			else if (lightningMenu == 3){
				if (darkMode){
					for (int i = 6; i < 12; i++){
						globalText[i].setColor(sf::Color::White);
					}
				}
				else{
					for (int i = 6; i < 12; i++){
						globalText[i].setColor(sf::Color::Black);
					}
				}

				if (fadeMode == 0){
					if (darkMode){
						globalText[11].setColor(sf::Color::White);
					}
					else{
						globalText[11].setColor(sf::Color::Black);
					}
					globalText[10].setColor(sf::Color(0, 170, 255));
				}
				else if (fadeMode == 1){
					if (darkMode){
						globalText[10].setColor(sf::Color::White);
					}
					else{
						globalText[10].setColor(sf::Color::Black);
					}
					globalText[11].setColor(sf::Color(0, 170, 255));
				}

				globalText[9].setColor(sf::Color(0, 170, 255));
				globalText[17].setString("Sets all LEDs to fade in to a color and then fade out again. Choose \nbetween fading red, yellow, green, cyan, blue and magenta or \nhaving it choose a random color and set the level of speed.");
				if (speed == 0){
					arrowRightBlueSprite.setPosition(210, 185);
					window.draw(arrowRightBlueSprite);
					arrowRightSprite.setPosition(250, 185);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(260, 185);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(300, 185);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(310, 185);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(320, 185);
					window.draw(arrowRightSprite);
				}
				else if (speed == 1){
					arrowRightBlueSprite.setPosition(250, 185);
					window.draw(arrowRightBlueSprite);
					arrowRightBlueSprite.setPosition(260, 185);
					window.draw(arrowRightBlueSprite);
					arrowRightSprite.setPosition(210, 185);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(300, 185);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(310, 185);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(320, 185);
					window.draw(arrowRightSprite);
				}
				else if (speed == 2){
					arrowRightBlueSprite.setPosition(300, 185);
					window.draw(arrowRightBlueSprite);
					arrowRightBlueSprite.setPosition(310, 185);
					window.draw(arrowRightBlueSprite);
					arrowRightBlueSprite.setPosition(320, 185);
					window.draw(arrowRightBlueSprite);
					arrowRightSprite.setPosition(210, 185);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(250, 185);
					window.draw(arrowRightSprite);
					arrowRightSprite.setPosition(260, 185);
					window.draw(arrowRightSprite);
				}
				for (int i = 10; i < 12; i++){
					window.draw(globalText[i]);
				}
			}
			for (int i = 6; i < 10; i++){
				window.draw(globalText[i]);
			}
			for (int i = 12; i < 14; i++){
				window.draw(globalText[i]);
			}
			window.draw(globalText[17]);
		}
		else if (currentMenu == 2){
			//Instructions
			menuInstructions();
			window.draw(globalText[27]);
			window.draw(InstructablesLogoSprite);
		}
		else if (currentMenu == 3){
			//Options
			menuOptions();
			globalText[21].setString("COM-port:      " + to_string(portCOM));
			globalText[22].setString("Baud rate:      " + to_string(rates[baudRate]));
			window.draw(lineBox);
			arrowLeftSprite.setPosition(sf::Vector2f(305, 177));
			window.draw(arrowLeftSprite);
			arrowLeftSprite.setPosition(sf::Vector2f(305, 127));
			window.draw(arrowLeftSprite);
			arrowRightSprite.setPosition(sf::Vector2f(393, 177));
			window.draw(arrowRightSprite);
			arrowRightSprite.setPosition(sf::Vector2f(347, 127));
			window.draw(arrowRightSprite);
			if (hasTested && connFail){
				globalText[25].setColor(sf::Color::Red);
			}
			else if(hasTested && !connFail){
				globalText[25].setColor(sf::Color::Green);
			}

			for (int i = 20; i < 26; i++){
				window.draw(globalText[i]);
			}
			if (darkMode){
				globalText[23].setString("Dark mode: x   ");
			}
			else{
				globalText[23].setString("Dark mode:      ");
			}
		}
		else if (currentMenu == 4){
			//Credits
			menuCredits();
			window.draw(globalText[26]);
			window.draw(GHLogoSprite);
			window.draw(TLogoSprite);
		}

		window.display();
	}

	return 0;
	
}

void menuOverview(){

	if (darkMode){
		for (int i = 0; i < 20; i++){
			menuFade[i].color = sf::Color::Black;
		}
	}
	else{
		for (int i = 0; i < 20; i++){
			menuFade[i].color = sf::Color::White;
		}
	}

	menuFade[1].color = sf::Color(0, 170, 255);
	menuFade[2].color = sf::Color(0, 170, 255);

	logo256Sprite.setScale(sf::Vector2f(0.5, 0.5));

}

void menuLightning(){

	if (darkMode){
		for (int i = 0; i < 20; i++){
			menuFade[i].color = sf::Color::Black;
		}
	}
	else{
		for (int i = 0; i < 20; i++){
			menuFade[i].color = sf::Color::White;
		}
	}

	menuFade[5].color = sf::Color(0, 170, 255);
	menuFade[6].color = sf::Color(0, 170, 255);

	if (darkMode){
		for (int i = 6; i < 10; i++){
			globalText[i].setColor(sf::Color::White);
		}
	}
	else{
		for (int i = 6; i < 10; i++){
			globalText[i].setColor(sf::Color::Black);
		}
	}
	globalText[6].setColor(sf::Color(0, 170, 255));
	globalText[17].setString("Sets a static color to the RGB strip. This color will remain unless \nchanged. Choose between white, black or a custom color from \nthe hue bar.");



}

void menuInstructions(){

	if (darkMode){
		for (int i = 0; i < 20; i++){
			menuFade[i].color = sf::Color::Black;
		}
	}
	else{
		for (int i = 0; i < 20; i++){
			menuFade[i].color = sf::Color::White;
		}
	}

	menuFade[9].color = sf::Color(0, 170, 255);
	menuFade[10].color = sf::Color(0, 170, 255);

}

void menuOptions(){

	if (darkMode){
		for (int i = 0; i < 20; i++){
			menuFade[i].color = sf::Color::Black;
		}
	}
	else{
		for (int i = 0; i < 20; i++){
			menuFade[i].color = sf::Color::White;
		}
	}

	menuFade[13].color = sf::Color(0, 170, 255);
	menuFade[14].color = sf::Color(0, 170, 255);
	if (darkMode){
		globalText[25].setColor(sf::Color::White);
	}
	else{
		globalText[25].setColor(sf::Color::Black);
	}


}

void menuCredits(){

	if (darkMode){
		for (int i = 0; i < 20; i++){
			menuFade[i].color = sf::Color::Black;
		}
	}
	else{
		for (int i = 0; i < 20; i++){
			menuFade[i].color = sf::Color::White;
		}
	}

	menuFade[17].color = sf::Color(0, 170, 255);
	menuFade[18].color = sf::Color(0, 170, 255);

}

void getValue(){
	
	string com;
	string baud;
	string dark;
	ifstream settings("Assets/options.txt");
	if (settings.is_open()){
		getline(settings, com);
		getline(settings, baud);
		getline(settings, dark);
		settings.close();

		portCOM = StringToNumber(com, 0.0);
		baudRate = StringToNumber(baud, 0.0);
		darkMode = StringToNumber(dark, 0.0);

	}
	else{
		cout << "Unable to open file 'options.txt'." << endl;
	}
}

void setValue(int com, int baud, int dark){

	ofstream options("Assets/options.txt");
	if (options.is_open()){
		options << com << endl;
		options << baud << endl;
		options << dark << endl;
		options.close();
		cout << "Successfully saved " + to_string(com) + " to COM, " + to_string(baud) + " to Baud, " + to_string(dark) + " to Dark" << endl;
	}
	else{
		cout << "Unable to open file 'options.txt'." << endl;
	}
}

template <typename T>
T StringToNumber(const string &Text, T defValue = T())
{
	stringstream ss;
	for (string::const_iterator i = Text.begin(); i != Text.end(); ++i)
		if (isdigit(*i) || *i == 'e' || *i == '-' || *i == '+' || *i == '.')
			ss << *i;
	T result;
	return ss >> result ? result : defValue;
}

void sendData(int lightMode){

	cout << "Connecting on port " + to_string(portCOM) + " with a Baud rate of " + to_string(rates[baudRate]) << endl;

	if (portCOM == 1){
		comm.startDevice("COM1", rates[baudRate]);
	}
	else if(portCOM == 2){
		comm.startDevice("COM2", rates[baudRate]);
	}
	else if (portCOM == 3){
		comm.startDevice("COM3", rates[baudRate]);
	}
	else if (portCOM == 4){
		comm.startDevice("COM4", rates[baudRate]);
	}
	else if (portCOM == 5){
		comm.startDevice("COM5", rates[baudRate]);
	}
	else if (portCOM == 6){
		comm.startDevice("COM6", rates[baudRate]);
	}
	else if (portCOM == 7){
		comm.startDevice("COM7", rates[baudRate]);
	}
	else if (portCOM == 8){
		comm.startDevice("COM8", rates[baudRate]);
	}
	else if (portCOM == 9){
		comm.startDevice("COM9", rates[baudRate]);
	}
	else if (portCOM == 10){
		comm.startDevice("COM10", rates[baudRate]);
	}
	else
	{
		cout << "Your port number is higher than the supported numbers. You have to build the source yourself after adding your specific port or ask me to add it for you." << endl;
	}

	if (lightMode == 0){
		
		if (connFail == true){
			globalText[25].setColor(sf::Color::Red);
		}
		else{
			globalText[25].setColor(sf::Color::Green);

			charData0[0] = 'p';
			charData0[1] = 'o';
			charData0[2] = 'o';
			charData0[3] = 'p';
			for (int i = 0; i < 4; i++){
				comm.send_data(charData0[i]);
			}
			comm.stopDevice();

			cout << "Test data was sent, device disconnected." << endl;
		}

	}
	if (connFail == false){
		if (lightMode == 1){
			for (int i = 0; i < 10; i++){
				comm.send_data(charData1[i]);
			}
			comm.stopDevice();
			cout << "Static light set, device disconnected" << endl;
		}
		else if (lightMode == 2){
			for (int i = 0; i < 3; i++){
				comm.send_data(charData2[i]);
			}
			comm.stopDevice();
			cout << "Rainbow light set, device disconnected" << endl;
		}
		else if (lightMode == 3){
			for (int i = 0; i < 2; i++){
				comm.send_data(charData3[i]);
			}
			comm.stopDevice();
			cout << "Cycle light set, device disconnected" << endl;
		}
		else if (lightMode == 4){
			for (int i = 0; i < 3; i++){
				comm.send_data(charData4[i]);
			}
			comm.stopDevice();
			cout << "Fade light set, device disconnected" << endl;
		}
	}

	
}

void previewLightning(int previewMode){
	
	int r = 255, g = 0, b = 0;
	int r1 = 0, g1 = 0, b1 = 0;
	int colorState = 0;
	int roundNum = 0;
	int roundNum1 = 1;
	int nextRound = 1;
	int randNum;

	if (fadeMode == 1){
		srand(time(NULL));
		roundNum1 = rand() % 7 + 1;
	}

	sf::RenderWindow preview(sf::VideoMode(512, 512), "Preview");

	sf::Image iconPre;
	if (!iconPre.loadFromFile("Assets/logo256.png")){
		cout << "Error loading 'logo256.png' for icon in the preview window.";
	}
	preview.setIcon(256, 256, iconPre.getPixelsPtr());

	sf::Texture logo512pre;
	if (!logo512pre.loadFromFile("Assets/logo512pre.png")){
		cout << "Error loading 'logo512pre.png' for the preview window.";
	}

	sf::Texture logo64pre;
	if (!logo64pre.loadFromFile("Assets/logo64pre.png")){
		cout << "Error loading 'logo64pre.png' for the preview window.";
	}


	sf::Sprite logo512preSprite;
	sf::Sprite logo64preSprite;
	
	sf::RectangleShape previewColor1(sf::Vector2f(250, 250));
	
	sf::RectangleShape previewColor2(sf::Vector2f(50, 50));
	sf::RectangleShape previewColor3(sf::Vector2f(50, 50));
	sf::RectangleShape previewColor4(sf::Vector2f(50, 50));
	sf::RectangleShape previewColor5(sf::Vector2f(50, 50));
	sf::RectangleShape previewColor6(sf::Vector2f(50, 50));
	sf::RectangleShape previewColor7(sf::Vector2f(50, 50));

	logo512preSprite.setTexture(logo512pre);

	if (previewMode == 0){
		cout << "Previewing static lightning" << endl;
		previewColor1.setPosition(sf::Vector2f(125, 125));
		previewColor1.setFillColor(sf::Color(valRed, valGreen, valBlue));
	}
	else if (previewMode == 1){
		cout << "Previewing rainbow lightning" << endl;
		logo64preSprite.setTexture(logo64pre);

		previewColor2.setPosition(70, 230);
		previewColor3.setPosition(134, 230);
		previewColor4.setPosition(198, 230);
		previewColor5.setPosition(262, 230);
		previewColor6.setPosition(326, 230);
		previewColor7.setPosition(390, 230);

		if (speed == 0){
			preview.setFramerateLimit(1);
		}
		if (speed == 1){
			preview.setFramerateLimit(2);
		}
		if (speed == 2){
			preview.setFramerateLimit(4);
		}
	}
	else if (previewMode == 2){
		cout << "Previewing cycle lightning" << endl;
		
		previewColor1.setPosition(sf::Vector2f(125, 125));
		previewColor1.setFillColor(sf::Color(r, g, b));
		
		if (speed == 0){
			preview.setFramerateLimit(10);
		}
		if (speed == 1){
			preview.setFramerateLimit(30);
		}
		if (speed == 2){
			preview.setFramerateLimit(60);
		}
	}
	else if (previewMode == 3){
		cout << "Previewing fade lightning" << endl;

		previewColor1.setPosition(sf::Vector2f(125, 125));
		previewColor1.setFillColor(sf::Color(r1, g1, b1));
		
		if (speed == 0){
			preview.setFramerateLimit(50);
		}
		if (speed == 1){
			preview.setFramerateLimit(100);
		}
		if (speed == 2){
			preview.setFramerateLimit(185);
		}
	}

	while (preview.isOpen())
	{
		sf::Event event;
		while (preview.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				preview.close();
		}
		if (previewMode == 1){
			if (roundNum == 0){
				previewColor2.setFillColor(sf::Color::Red);
				previewColor3.setFillColor(sf::Color::Yellow);
				previewColor4.setFillColor(sf::Color::Green);
				previewColor5.setFillColor(sf::Color::Cyan);
				previewColor6.setFillColor(sf::Color::Blue);
				previewColor7.setFillColor(sf::Color::Magenta);

				if (streamMode == 0){
					roundNum = 5;
				}
				else{
					roundNum++;
				}

			}
			else if (roundNum == 1){
				previewColor7.setFillColor(sf::Color::Red);
				previewColor2.setFillColor(sf::Color::Yellow);
				previewColor3.setFillColor(sf::Color::Green);
				previewColor4.setFillColor(sf::Color::Cyan);
				previewColor5.setFillColor(sf::Color::Blue);
				previewColor6.setFillColor(sf::Color::Magenta);

				if (streamMode == 0){
					roundNum--;
				}
				else{
					roundNum++;
				}
			}
			else if (roundNum == 2){
				previewColor6.setFillColor(sf::Color::Red);
				previewColor7.setFillColor(sf::Color::Yellow);
				previewColor2.setFillColor(sf::Color::Green);
				previewColor3.setFillColor(sf::Color::Cyan);
				previewColor4.setFillColor(sf::Color::Blue);
				previewColor5.setFillColor(sf::Color::Magenta);

				if (streamMode == 0){
					roundNum--;
				}
				else{
					roundNum++;
				}
			}
			else if (roundNum == 3){
				previewColor5.setFillColor(sf::Color::Red);
				previewColor6.setFillColor(sf::Color::Yellow);
				previewColor7.setFillColor(sf::Color::Green);
				previewColor2.setFillColor(sf::Color::Cyan);
				previewColor3.setFillColor(sf::Color::Blue);
				previewColor4.setFillColor(sf::Color::Magenta);

				if (streamMode == 0){
					roundNum--;
				}
				else{
					roundNum++;
				}
			}
			else if (roundNum == 4){
				previewColor4.setFillColor(sf::Color::Red);
				previewColor5.setFillColor(sf::Color::Yellow);
				previewColor6.setFillColor(sf::Color::Green);
				previewColor7.setFillColor(sf::Color::Cyan);
				previewColor2.setFillColor(sf::Color::Blue);
				previewColor3.setFillColor(sf::Color::Magenta);

				if (streamMode == 0){
					roundNum--;
				}
				else{
					roundNum++;
				}
			}
			else if (roundNum == 5){
				previewColor3.setFillColor(sf::Color::Red);
				previewColor4.setFillColor(sf::Color::Yellow);
				previewColor5.setFillColor(sf::Color::Green);
				previewColor6.setFillColor(sf::Color::Cyan);
				previewColor7.setFillColor(sf::Color::Blue);
				previewColor2.setFillColor(sf::Color::Magenta);

				if (streamMode == 0){
					roundNum--;
				}
				else{
					roundNum = 0;
				}
			}
		}
		
		if (previewMode == 2){

			if (colorState == 0){
				if (r != 255){
					r++;
				}
				else if (r == 255 && b != 0){
					b--;
				}
				else if (r == 255 && b == 0){
					colorState = 1;
				
				}
			}
			else if (colorState == 1){
				if (g != 255){
					g++;
				}
				else if (g == 255 && r != 0){
					r--;
				}
				else if (g == 255 && r == 0){
					colorState = 2;
				}
			}
			else if (colorState == 2){
				if (b != 255){
					b++;
				}
				else if (b == 255 && g != 0){
					g--;
				}
				else if (b == 255 && g == 0){
					colorState = 0;
				}
			}
			previewColor1.setFillColor(sf::Color(r, g, b));
		}
		else if (previewMode == 3){
			if (roundNum1 == 0){
				if (r1 == 0 && g1 == 0 && b1 == 0){
					if (fadeMode == 1){
						srand(time(NULL));
						roundNum1 = rand() % 6 + 1;

						cout << roundNum1 << endl;
					}
					else{
						if (nextRound == 1){
							roundNum1 = 2;
						}
						else if (nextRound == 2){
							roundNum1 = 3;
						}
						else if (nextRound == 3){
							roundNum1 = 4;
						}
						else if (nextRound == 4){
							roundNum1 = 5;
						}
						else if (nextRound == 5){
							roundNum1 = 6;
						}
						else if (nextRound == 6){
							roundNum1 = 1;
						}
					}
					
				}
				else{
					if (r1 != 0){
						r1--;
					}
					if (g1 != 0){
						g1--;
					}
					if (b1 != 0){
						b1--;
					}
				}

			}
			else if (roundNum1 == 1){
				if (r1 == 255){
					roundNum1 = 0;
					nextRound = 1;
				}
				else{
					r1++;
				}
			}
			else if (roundNum1 == 2){
				if (r1 == 255 && g1 == 255){
					roundNum1 = 0;
					nextRound = 2;
				}
				else{
					r1++;
					g1++;
				}
			}
			else if (roundNum1 == 3){
				if (g1 == 255){
					roundNum1 = 0;
					nextRound = 3;
				}
				else{
					g1++;
				}
			}
			else if (roundNum1 == 4){
				if (g1 == 255 && b1 == 255){
					roundNum1 = 0;
					nextRound = 4;
				}
				else{
					g1++;
					b1++;
				}
			}
			else if (roundNum1 == 5){
				if (b1 == 255){
					roundNum1 = 0;
					nextRound = 5;
				}
				else{
					b1++;
				}
			}
			else if (roundNum1 == 6){
				if (b1 == 255 && r1 == 255){
					roundNum1 = 0;
					nextRound = 6;
				}
				else{
					b1++;
					r1++;
				}
			}
			previewColor1.setFillColor(sf::Color(r1, g1, b1));

		}


		preview.clear(sf::Color::White);
		
		if (previewMode == 0){
			preview.draw(previewColor1);
			preview.draw(logo512preSprite);
		}
		else if (previewMode == 1){

			preview.draw(previewColor2);
			preview.draw(previewColor3);
			preview.draw(previewColor4);
			preview.draw(previewColor5);
			preview.draw(previewColor6);
			preview.draw(previewColor7);

			logo64preSprite.setPosition(65, 224);
			preview.draw(logo64preSprite);
			logo64preSprite.setPosition(129, 224);
			preview.draw(logo64preSprite);
			logo64preSprite.setPosition(193, 224);
			preview.draw(logo64preSprite);
			logo64preSprite.setPosition(257, 224);
			preview.draw(logo64preSprite);
			logo64preSprite.setPosition(321, 224);
			preview.draw(logo64preSprite);
			logo64preSprite.setPosition(385, 224);
			preview.draw(logo64preSprite);
		}
		else if (previewMode == 2){

			preview.draw(previewColor1);
			preview.draw(logo512preSprite);

		}
		else if (previewMode == 3){

			preview.draw(previewColor1);
			preview.draw(logo512preSprite);

		}

		preview.display();
	}
}

void applyLight(int lightMode){
 
	if (lightMode == 0){

		cout << "Applying static lightning" << endl;

		string red = to_string(valRed);
		string green = to_string(valGreen);
		string blue = to_string(valBlue);

		charData1[0] = 'q';

		if (valRed < 10){
			charData1[1] = '0';
			charData1[2] = '0';
			charData1[3] = red[0];
		}
		else if (valRed < 100){
			charData1[1] = '0';
			charData1[2] = red[0];
			charData1[3] = red[1];
		}
		else{
			charData1[1] = red[0];
			charData1[2] = red[1];
			charData1[3] = red[2];
		}

		if (valGreen < 10){
			charData1[4] = '0';
			charData1[5] = '0';
			charData1[6] = green[0];
		}
		else if (valGreen < 100){
			charData1[4] = '0';
			charData1[5] = green[0];
			charData1[6] = green[1];
		}
		else{
			charData1[4] = green[0];
			charData1[5] = green[1];
			charData1[6] = green[2];
		}

		if (valBlue < 10){
			charData1[7] = '0';
			charData1[8] = '0';
			charData1[9] = blue[0];
		}
		else if (valBlue < 100){
			charData1[7] = '0';
			charData1[8] = blue[0];
			charData1[9] = blue[1];
		}
		else{
			charData1[7] = blue[0];
			charData1[8] = blue[1];
			charData1[9] = blue[2];
		}

		sendData(1);
	}
	else if (lightMode == 1){

		cout << "Applying rainbow lightning" << endl;

		string dir = to_string(streamMode);
		string spd1 = to_string(speed);

		charData2[0] = 'w';
		charData2[1] = dir[0];
		charData2[2] = spd1[0];

		sendData(2);

	}
	else if (lightMode == 2){

		cout << "Applying cycle lightning" << endl;

		string spd2 = to_string(speed);

		charData3[0] = 'e';
		charData3[1] = spd2[0];

		sendData(3);
	}
	else if (lightMode == 3){

		cout << "Applying fade lightning" << endl;

		string mode = to_string(fadeMode);
		string spd3 = to_string(speed);

		charData4[0] = 'r';
		charData4[1] = mode[0];
		charData4[2] = spd3[0];

		sendData(4);

	}
	
}