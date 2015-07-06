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


using namespace std;

serial comm;

sf::Vertex vertex(sf::Vector2f(100, 0), sf::Color::Black, sf::Vector2f(100, 100));

sf::VertexArray line(sf::Lines, 10);
sf::VertexArray lineBox(sf::Lines, 8);
sf::VertexArray menuFade(sf::Quads, 20);
sf::VertexArray hueBar(sf::Quads, 24);

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
void sendData(int charData[]);


int portCOM;
int baudRate;
bool darkMode;
int rates[12] = { 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200 };

int valRed;
int valGreen;
int valBlue;

int currentMenu = 0; //0: Overview, 1: Lightning, 2: Instructions, 3: Options: 4: Credits
int lightningMenu = 0; //0: Static, 2: Rainbow, 3: Cycle, 4: Fade RGB, 5: Fade Random

sf::Sprite logo256Sprite;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 500), "RGB Strip Color Picker");
	window.setFramerateLimit(60);

	sf::Image icon;
	if (!icon.loadFromFile("Assets/logo256.png")){
		cout << "Error loading 'logo256.png' for icon.";
	}
	window.setIcon(256, 256, icon.getPixelsPtr());

	sf::Texture logo256;
	if (!logo256.loadFromFile("Assets/logo256.png")){
		cout << "Error loading 'logo256.png'.";
	}

	logo256Sprite.setTexture(logo256);
	logo256Sprite.setPosition(sf::Vector2f(662, 10));

	sf::Texture CWB;
	if (!CWB.loadFromFile("Assets/CWB.png")){
		cout << "Error loading 'CWB.png'.";
	}

	sf::Sprite CWBSprite;
	CWBSprite.setTexture(CWB);
	CWBSprite.setPosition(sf::Vector2f(210, 133));

	sf::Texture barHolder;
	if (!barHolder.loadFromFile("Assets/barHolder.png")){
		cout << "Error loading 'barHolder.png'.";
	}

	sf::Sprite barHolderSprite;
	barHolderSprite.setTexture(barHolder);
	barHolderSprite.setPosition(sf::Vector2f(274, 130));

	sf::Sprite arrowLeftSprite;
	sf::Sprite arrowRightSprite;
	sf::Sprite arrowLeftSprite2;
	sf::Sprite arrowRightSprite2;

	sf::Texture arrowLeft;
	if (!arrowLeft.loadFromFile("Assets/arrowLeft.png")){
		cout << "Error loading 'logo256.png'.";
	}

	arrowLeftSprite.setTexture(arrowLeft);
	arrowLeftSprite.setPosition(sf::Vector2f(305, 177));
	arrowLeftSprite2.setTexture(arrowLeft);
	arrowLeftSprite2.setPosition(sf::Vector2f(305, 127));

	sf::Texture arrowRight;
	if (!arrowRight.loadFromFile("Assets/arrowRight.png")){
		cout << "Error loading 'logo256.png'.";
	}

	arrowRightSprite.setTexture(arrowRight);
	arrowRightSprite.setPosition(sf::Vector2f(393, 177));
	arrowRightSprite2.setTexture(arrowRight);
	arrowRightSprite2.setPosition(sf::Vector2f(347, 127));



	sf::Font font;
	if (!font.loadFromFile("Assets/Roboto-Light.ttf")){
		cout << "Error loading font 'Roboto-Light'." << endl;
	}

	sf::Text textMenu[5];

	for (int i = 0; i < 5; i++){
		textMenu[i].setFont(font);
		textMenu[i].setColor(sf::Color::Black);
	}

	textMenu[0].setPosition(60, 32);
	textMenu[1].setPosition(60, 132);
	textMenu[2].setPosition(25, 232);
	textMenu[3].setPosition(80, 332);
	textMenu[4].setPosition(85, 432);

	textMenu[0].setString("Overview");
	textMenu[1].setString("Lightning");
	textMenu[2].setString("Instructions");
	textMenu[3].setString("Options");
	textMenu[4].setString("Credits");

	sf::Text textOverview;

	textOverview.setFont(font);
	textOverview.setColor(sf::Color::Black);
	textOverview.setPosition(210, 10);
	textOverview.setCharacterSize(20);
	textOverview.setString("Thank you for using RGB Strip Color Picker! \nThis is a tool which you can use to modify your \nRGB color strip attached to an Arduino. Before you \nstart, make sure to read the instructions and check \nout the GitHub page for more information on how \nyou should use this tool. \n\nMore functions and modes will be added by request, so head over \nto the 'Credits' tab and see how you can give feedback and \ncontribute to the project.");

	sf::Text textLightning[8];

	for (int i = 0; i < 8; i++){
		textLightning[i].setFont(font);
		textLightning[i].setColor(sf::Color::Black);
		textLightning[i].setCharacterSize(20);
	}

	textLightning[0].setColor(sf::Color(0, 170, 255));

	textLightning[0].setPosition(301, 10);
	textLightning[1].setPosition(412, 10);
	textLightning[2].setPosition(552, 10);
	textLightning[3].setPosition(680, 10);
	textLightning[4].setPosition(650, 35);
	textLightning[5].setPosition(710, 35);
	textLightning[6].setPosition(650, 470);
	textLightning[7].setPosition(741, 470);

	textLightning[0].setString("Static");
	textLightning[1].setString("Rainbow");
	textLightning[2].setString("Cycle");
	textLightning[3].setString("Fade");
	textLightning[4].setString("RGB");
	textLightning[5].setString("Random");
	textLightning[6].setString("Preview");
	textLightning[7].setString("Apply");

	sf::Text textOptions[5];

	for (int i = 0; i < 5; i++){
		textOptions[i].setFont(font);
		textOptions[i].setColor(sf::Color::Black);
		textOptions[i].setCharacterSize(20);
	}

	textOptions[0].setPosition(210, 10);
	textOptions[1].setPosition(210, 130);
	textOptions[2].setPosition(210, 180);
	textOptions[3].setPosition(210, 230);
	textOptions[4].setPosition(210, 280);

	textOptions[0].setString("This is where you can change your options for the program. \nBefore playing around with the different settings, it's important to \nread the instructions to set it up correctly.");
	textOptions[3].setString("Dark mode: x");
	textOptions[4].setString("");

	lineBox[0].position = sf::Vector2f(313, 236);
	lineBox[1].position = sf::Vector2f(328, 236);
	lineBox[2].position = sf::Vector2f(328, 236);
	lineBox[3].position = sf::Vector2f(328, 251);
	lineBox[4].position = sf::Vector2f(328, 251);
	lineBox[5].position = sf::Vector2f(313, 251);
	lineBox[6].position = sf::Vector2f(313, 236);
	lineBox[7].position = sf::Vector2f(313, 251);

	for (int i = 0; i < 8; i++){
		lineBox[i].color = sf::Color::Black;
	}

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

	for (int i = 0; i < 10; i++){
		line[i].color = sf::Color::Black;
	}
	
	for (int i = 0; i < 20; i++){
		menuFade[i].color = sf::Color::White;
	}

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

	sf::Vector2i mousePos;

	menuOverview();

	getValue();

	cout << "COM: " + to_string(portCOM) + " Baud: " + to_string(baudRate) + " Dark: " + to_string(darkMode) << endl;

	while (window.isOpen())
	{

		mousePos = sf::Mouse::getPosition(window);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed){

				if (mousePos.x >= 10 && mousePos.x <= 200 && mousePos.y >= 10 && mousePos.y <= 100){
					currentMenu = 0;
					menuOverview();
				}
				else if (mousePos.x >= 10 && mousePos.x <= 200 && mousePos.y >= 100 && mousePos.y <= 200){
					currentMenu = 1;
					menuLightning();
				}
				else if (mousePos.x >= 10 && mousePos.x <= 200 && mousePos.y >= 200 && mousePos.y <= 300){
					currentMenu = 2;
					menuInstructions();
				}
				else if (mousePos.x >= 10 && mousePos.x <= 200 && mousePos.y >= 300 && mousePos.y <= 400){
					currentMenu = 3;
					menuOptions();
				}
				else if (mousePos.x >= 10 && mousePos.x <= 200 && mousePos.y >= 400 && mousePos.y <= 490){
					currentMenu = 4;
					menuCredits();
				}
				else if (mousePos.x >= 313 && mousePos.x <= 328 && mousePos.y >= 236 && mousePos.y <= 251 && currentMenu == 3){
					if (darkMode){
						darkMode = false;
						setValue(portCOM, baudRate, 0);
					}
					else{
						darkMode = true;
						setValue(portCOM, baudRate, 1);
					}
				}
				else if (mousePos.x >= 314 && mousePos.x <= 326 && mousePos.y >= 183 && mousePos.y <= 201 && currentMenu == 3){
					baudRate--;
					if (baudRate == -1){
						baudRate = 11;
					}
					setValue(portCOM, baudRate, darkMode);
				}
				else if (mousePos.x >= 402 && mousePos.x <= 414 && mousePos.y >= 183 && mousePos.y <= 201 && currentMenu == 3){
					baudRate++;
					if (baudRate == 12){
						baudRate = 0;
					}
					setValue(portCOM, baudRate, darkMode);
				}
				else if (mousePos.x >= 314 && mousePos.x <= 326 && mousePos.y >= 133 && mousePos.y <= 151 && currentMenu == 3){
					portCOM--;
					if (portCOM == 0){
						portCOM = 10;
					}
					setValue(portCOM, baudRate, darkMode);
				}
				else if (mousePos.x >= 356 && mousePos.x <= 368 && mousePos.y >= 133 && mousePos.y <= 151 && currentMenu == 3){
					portCOM++;
					if (portCOM == 11){
						portCOM = 1;
					}
					setValue(portCOM, baudRate, darkMode);
				}
				else if (mousePos.x >= 301 && mousePos.x <= 349 && mousePos.y >= 10 && mousePos.y <= 25 && currentMenu == 1){
					textLightning[1].setColor(sf::Color::Black);
					textLightning[2].setColor(sf::Color::Black);
					textLightning[3].setColor(sf::Color::Black);
					textLightning[0].setColor(sf::Color(0, 170, 255));

					lightningMenu = 0;
				}
				else if (mousePos.x >= 412 && mousePos.x <= 487 && mousePos.y >= 10 && mousePos.y <= 25 && currentMenu == 1){
					textLightning[0].setColor(sf::Color::Black);
					textLightning[2].setColor(sf::Color::Black);
					textLightning[3].setColor(sf::Color::Black);
					textLightning[1].setColor(sf::Color(0, 170, 255));

					lightningMenu = 1;
				}
				else if (mousePos.x >= 552 && mousePos.x <= 597 && mousePos.y >= 10 && mousePos.y <= 25 && currentMenu == 1){
					textLightning[0].setColor(sf::Color::Black);
					textLightning[1].setColor(sf::Color::Black);
					textLightning[3].setColor(sf::Color::Black);
					textLightning[2].setColor(sf::Color(0, 170, 255));

					lightningMenu = 2;
				}
				else if ((mousePos.x >= 680 && mousePos.x <= 720 && mousePos.y >= 10 && mousePos.y <= 25 && currentMenu == 1) || (mousePos.x >= 650 && mousePos.x <= 687 && mousePos.y >= 35 && mousePos.y <= 50 && currentMenu == 1 && (lightningMenu == 3 || lightningMenu == 5))){
					textLightning[0].setColor(sf::Color::Black);
					textLightning[1].setColor(sf::Color::Black);
					textLightning[2].setColor(sf::Color::Black);
					textLightning[5].setColor(sf::Color::Black);
					textLightning[3].setColor(sf::Color(0, 170, 255));
					textLightning[4].setColor(sf::Color(0, 170, 255));

					lightningMenu = 4;
				}
				else if (mousePos.x >= 710 && mousePos.x <= 783 && mousePos.y >= 35 && mousePos.y <= 50 && currentMenu == 1 && (lightningMenu == 3 || lightningMenu == 4)){
					textLightning[0].setColor(sf::Color::Black);
					textLightning[1].setColor(sf::Color::Black);
					textLightning[2].setColor(sf::Color::Black);
					textLightning[4].setColor(sf::Color::Black);
					textLightning[5].setColor(sf::Color(0, 170, 255));


					lightningMenu = 5;
				}
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && currentMenu == 1 && lightningMenu == 0 && mousePos.x >= 277 && mousePos.x <= 377 && mousePos.y >= 133 && mousePos.y <= 433){
		
			if (mousePos.y >= 133 && mousePos.y <= 233){
				valRed = 255 - ((mousePos.y - 133) * 2.55);
			}
			else if (mousePos.y >= 333 && mousePos.y <= 433){
				valRed = ((mousePos.y - 333) * 2.55);
			}
			else{
				valRed = 0;
			}
			if (mousePos.y >= 133 && mousePos.y <= 233){
				valGreen = ((mousePos.y - 133) * 2.55);
			}
			else if (mousePos.y >= 233 && mousePos.y <= 333){
				valGreen = 255 - ((mousePos.y - 233) * 2.55);
			}
			else{
				valGreen = 0;
			}
			if (mousePos.y >= 233 && mousePos.y <= 333){
				valBlue = ((mousePos.y - 233) * 2.55);
			}
			else if (mousePos.y >= 333 && mousePos.y <= 433){
				valBlue = 255 - ((mousePos.y - 333) * 2.55);
			}
			else{
				valBlue = 0;
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && currentMenu == 1 && lightningMenu == 0 && mousePos.x >= 277 && mousePos.x <= 377){
		
			if (mousePos.y <= 134){
				barHolderSprite.setPosition(274, 130);
			}
			else if (mousePos.y >= 430){
				barHolderSprite.setPosition(274, 427);
			}
			else{
				barHolderSprite.setPosition(274, mousePos.y - 4);
			}
		}
		

		window.clear(sf::Color::White);
		window.draw(menuFade);
		window.draw(line);
		for (int i = 0; i < 5; i++){
			window.draw(textMenu[i]);
		}

		if (currentMenu == 0){
		//Overview
			window.draw(textOverview);
			window.draw(logo256Sprite);

		}else if (currentMenu == 1){
		//Lightning
			if (lightningMenu == 0){
				window.draw(CWBSprite);
				window.draw(hueBar);
				window.draw(barHolderSprite);
			}
			else if (lightningMenu == 1){
				
			}
			else if (lightningMenu == 2){

			}
			else if(lightningMenu == 3){
				window.draw(textLightning[4]);
				window.draw(textLightning[5]);
			}
			else if (lightningMenu == 4){
				window.draw(textLightning[4]);
				window.draw(textLightning[5]);
			}
			else if (lightningMenu == 5){
				window.draw(textLightning[4]);
				window.draw(textLightning[5]);
			}
			
			for (int i = 0; i < 4; i++){
				window.draw(textLightning[i]);
			}
			for (int i = 6; i < 8; i++){
				window.draw(textLightning[i]);
			}
			
		}
		else if (currentMenu == 2){
		//Instructions

		}
		else if (currentMenu == 3){
		//Options
			textOptions[1].setString("COM-port:      " + to_string(portCOM));
			textOptions[2].setString("Baud rate:      " + to_string(rates[baudRate]));
			window.draw(lineBox);
			window.draw(arrowLeftSprite);
			window.draw(arrowRightSprite);
			window.draw(arrowLeftSprite2);
			window.draw(arrowRightSprite2);
			for (int i = 0; i < 5; i++){
				window.draw(textOptions[i]);
			}
			if (darkMode){
				textOptions[3].setString("Dark mode: x");
			}
			else{
				textOptions[3].setString("Dark mode: ");
			}
		}
		else if (currentMenu == 4){
		//Credits

		}

		window.display();
	}

	return 0;
}

void menuOverview(){

	for (int i = 0; i < 20; i++){
		menuFade[i].color = sf::Color::White;
	}

	menuFade[1].color = sf::Color(0, 170, 255);
	menuFade[2].color = sf::Color(0, 170, 255);

	logo256Sprite.setScale(sf::Vector2f(0.5, 0.5));

}

void menuLightning(){

	for (int i = 0; i < 20; i++){
		menuFade[i].color = sf::Color::White;
	}

	menuFade[5].color = sf::Color(0, 170, 255);
	menuFade[6].color = sf::Color(0, 170, 255);

}

void menuInstructions(){

	for (int i = 0; i < 20; i++){
		menuFade[i].color = sf::Color::White;
	}

	menuFade[9].color = sf::Color(0, 170, 255);
	menuFade[10].color = sf::Color(0, 170, 255);

}

void menuOptions(){

	for (int i = 0; i < 20; i++){
		menuFade[i].color = sf::Color::White;
	}

	menuFade[13].color = sf::Color(0, 170, 255);
	menuFade[14].color = sf::Color(0, 170, 255);

}

void menuCredits(){

	for (int i = 0; i < 20; i++){
		menuFade[i].color = sf::Color::White;
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

void sendData(int charData[]){
	
	char port = (char)portCOM;
	char* com = &port;

	comm.startDevice(com, baudRate);
	for (int i = 0; i < numChars; i++){
		comm.send_data(charData[i]);
	}
	comm.stopDevice(); 
	_getch();
}