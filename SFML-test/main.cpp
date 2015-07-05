#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <sstream> 
#include <string>



using namespace std;

sf::Vertex vertex(sf::Vector2f(100, 0), sf::Color::Black, sf::Vector2f(100, 100));

sf::VertexArray line(sf::Lines, 10);
sf::VertexArray menuFade(sf::Quads, 20);

void menuOverview();
void menuLightning();
void menuInstructions();
void menuOptions();
void menuCredits();
void getValue();
void setValue(int com, int baud);
template <typename T>
T StringToNumber(const string &Text, T defValue);

int portCOM;
int baudRate;
int currentMenu = 0; //0: Overview, 1: Lightning, 2: Instructions, 3: Options: 4: Credits

sf::Sprite logo256Sprite;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 500), "RGB Strip Color Picker");
	window.setFramerateLimit(60);
	
	sf::Image icon;
	if (!icon.loadFromFile("logo256.png"))
	{
		cout << "Error loading 'logo256.png' for icon.";
	}
	window.setIcon(256, 256, icon.getPixelsPtr());

	sf::Texture logo256;
	if (!logo256.loadFromFile("logo256.png"))
	{
		cout << "Error loading 'logo256.png'.";
	}
	
	logo256Sprite.setTexture(logo256);
	logo256Sprite.setPosition(sf::Vector2f(662, 10));

	sf::Font font;
	if (!font.loadFromFile("Roboto-Light.ttf")){
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

	sf::Vector2i mousePos;

	menuOverview();

	setValue(3, 9600);

	getValue();

	cout << "COM: " + to_string(portCOM) + " Baud: " + to_string(baudRate) << endl;

	while (window.isOpen())
	{

		mousePos = sf::Mouse::getPosition(window);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				
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

		}
		else if (currentMenu == 2){
		//Instructions

		}
		else if (currentMenu == 3){
		//Options

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
	int resultCom;
	int resultBaud;
	ifstream settings("options.txt");
	if (settings.is_open()){
		getline(settings, com);
		getline(settings, baud);
		settings.close();

		portCOM = StringToNumber(com, 0.0);
		baudRate = StringToNumber(baud, 0.0);

	}
	else{
		cout << "Unable to open file 'options.txt'." << endl;
	}
}

void setValue(int com, int baud){

	ofstream options("options.txt");
	if (options.is_open()){
		options << com << endl;
		options << baud << endl;
		options.close();
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