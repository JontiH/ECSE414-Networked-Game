/*
 * This is the client source code.
 */

#include <SFML/Graphics.hpp>
#include <string>
#include "./UDP/UDPSystem.hpp"
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"
#include <iostream>
#include <math.h>  
#include "json.hpp"

#define TIMEOUT 10000 //timeout value for recvPacket()
#define HALF_SEC_TIMEOUT 500000 //unique timeout for udp handshake
#define TO_SERVER 1 //used as param for sendPacket()

using json = nlohmann::json;

enum State { walkLeft, walkRight, crouchLeft, crouchRight , idleLeft, idleRight, jumpLeft, jumpRight, chuckLeft, chuckRight , die };
enum Input { none, up, left, right, down, action, enter};

Input inputBuffer[60] = { none };
float FLOOR_HEIGHT = 100.0;

#define PI 3.14159265
sf::Texture texture;
sf::Font font;

Animation idleAnimationLeft(false,"idleLeft");
Animation crouchingAnimationLeft(false,"crouchLeft");
Animation walkingAnimationLeft(true,"walkLeft");


Animation idleAnimationRight(false, "idleRight");
Animation crouchingAnimationRight(false,"crouchRight");
Animation walkingAnimationRight(true,"walkRight");

Animation hammerMovingRight(true, "hammerMoveRight");
Animation hammerMovingLeft(true, "hammerMoveLeft");
Animation hammerIdleRight(true, "hammerIdleRight");
Animation hammerIdleLeft(true, "hammerIdleLeft");

float scalingFactor = 3.5;
sf::Sprite s;
float walkSpeed = 200;
float stopFactor = 0.10 ;
float gravity = 9.8;
float jumpSpeed = 500;

int carryList[2] = { 0 };
int chuckList[2] = { 0 };
int teamList[2] = { 0 };

int victory = 0;
bool playerSideLeft = true;
bool playerTeamRight = true;
std::pair<float, float> p1V, p2V, h1V, h2V;




sf::Vector2i screenDimensions(800, 600);
sf::RectangleShape floorBox(sf::Vector2f(screenDimensions.x, FLOOR_HEIGHT));
sf::RectangleShape halfLine(sf::Vector2f(10, FLOOR_HEIGHT));


State interpolateState() {

	return idleRight;
}


Input getCurrentInput(sf::Event event) {

	Input currInput = none;


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		
		currInput = action;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
	
		currInput = down;

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		currInput = left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		currInput = right;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		currInput = up;
	}

	return currInput;
}

State getCurrentState(AnimatedSprite sprite, Input input)  {

	bool noKeyWasPressed = true;
	State previousState = static_cast<State>(sprite.getCurrState());
	State currentState;
	// if a key was pressed set the correct animation and move correctly

	if (sprite.getTeam() == 1) {
		if (currentState == die) {
			return currentState;
		}
		if (input == action)
		{

			if (previousState == chuckLeft || previousState == chuckRight) {
				chuckList[sprite.getTeam() - 1] = 0;
				currentState = previousState;
			}
			else {

				if ((currentState == idleRight) || (currentState == walkRight) || (currentState == crouchRight) || (currentState == idleRight) || (currentState == jumpRight)) {

					currentState = chuckRight;

					chuckList[sprite.getTeam() - 1] = 2;
				}
				else {

					currentState = chuckLeft;
					chuckList[sprite.getTeam() - 1] = 1;
				}
			}

			noKeyWasPressed = false;
		}

		else if (input == down)
		{

			if ((currentState == walkLeft) || (currentState == idleLeft) || (currentState == chuckLeft) || (currentState == jumpLeft) || (currentState == crouchLeft)) {
				currentState = crouchLeft;
			}
			else {
				currentState = crouchRight;
			}

			noKeyWasPressed = false;
		}
		else if (input == left)
		{
			if ((currentState == jumpLeft) || (currentState == jumpRight)) {
				currentState = jumpLeft;
			}
			else if (currentState == crouchRight) {
				currentState = crouchLeft;
			}
			else
				currentState = walkLeft;

			noKeyWasPressed = false;
		}
		else if (input == right)
		{
			if ((currentState == jumpRight) || (currentState == jumpRight)) {
				currentState = jumpRight;
			}
			else if (currentState == crouchLeft) {
				currentState = crouchRight;
			}
			else
				currentState = walkRight;
			noKeyWasPressed = false;
		}
		else if (input == up)
		{
			if ((currentState == idleRight) || (currentState == walkRight) || (currentState == crouchRight) || (currentState == idleRight) || (currentState == jumpRight)) {
				currentState = jumpRight;
			}
			else
				currentState = jumpLeft;
			noKeyWasPressed = false;

		}

		// if no key was pressed stop the animation
		else if (input == none)
		{


			if ((currentState == walkLeft) || (currentState == idleLeft) || (currentState == chuckLeft) || (currentState == jumpLeft) || (currentState == crouchLeft)) {
				currentState = idleLeft;
			}
			else
				currentState = idleRight;
		}
	}
	
	return currentState;
}
const Animation* getCurrentAnimation(AnimatedSprite sprite,bool isHammer) {
	const Animation* tmp(NULL);

	State current = static_cast<State>(sprite.getCurrState());

	if (isHammer) {
		tmp = &hammerIdleRight;
	}
	else {
		if (current == idleLeft) {
			tmp = &idleAnimationLeft;
		}
		else if (current == idleRight) {
			tmp = &idleAnimationRight;
		}
		else if (current == walkLeft) {
			tmp = &walkingAnimationLeft;
		}
		else if (current == walkRight) {
			tmp = &walkingAnimationRight;
		}
		else if (current == crouchLeft) {
			tmp = &crouchingAnimationLeft;
		}
		else
			tmp = &crouchingAnimationLeft;
	}
	return tmp;

}

void setupAnimations() {
	s = sf::Sprite(texture, sf::IntRect(231 + 18, 62, -18, 32));
	s.scale(scalingFactor, scalingFactor);
	idleAnimationLeft.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(14 + 17, 59, -18, 32));
	s.scale(scalingFactor, scalingFactor);
	idleAnimationLeft.addFrame(s);



	s = sf::Sprite(texture, sf::IntRect(231 + 18, 62, -18, 32));
	s.scale(scalingFactor, scalingFactor);
	crouchingAnimationLeft.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(232, 62, -18, 32));
	s.scale(scalingFactor, scalingFactor);
	crouchingAnimationLeft.addFrame(s);



	s = sf::Sprite(texture, sf::IntRect(30, 59, -18, 32));
	s.scale(scalingFactor, scalingFactor);
	walkingAnimationLeft.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(30 + 18, 59, -18, 32));
	s.scale(scalingFactor, scalingFactor);
	walkingAnimationLeft.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(47 + 18, 59, -18, 32));
	s.scale(scalingFactor, scalingFactor);
	walkingAnimationLeft.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(30 + 18, 59, -18, 32));
	s.scale(scalingFactor, scalingFactor);
	walkingAnimationLeft.addFrame(s);


	s = sf::Sprite(texture, sf::IntRect(13, 59, 18, 32));
	s.scale(scalingFactor, scalingFactor);
	walkingAnimationRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(30, 59, 18, 32));
	s.scale(scalingFactor, scalingFactor);
	walkingAnimationRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(47, 59, 18, 32));
	s.scale(scalingFactor, scalingFactor);
	walkingAnimationRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(30, 59, 18, 32));
	s.scale(scalingFactor, scalingFactor);
	walkingAnimationRight.addFrame(s);


	s = sf::Sprite(texture, sf::IntRect(231, 62, 18, 32));
	s.scale(scalingFactor, scalingFactor);
	crouchingAnimationRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(232 - 18, 62, 18, 32));
	s.scale(scalingFactor, scalingFactor);
	crouchingAnimationRight.addFrame(s);

	s = sf::Sprite(texture, sf::IntRect(231, 62, 18, 32));
	s.scale(scalingFactor, scalingFactor);
	idleAnimationRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(13, 59, 18, 32));
	s.scale(scalingFactor, scalingFactor);
	idleAnimationRight.addFrame(s);
	

	s = sf::Sprite(texture, sf::IntRect(331, 343, 17, 18));
	s.scale(scalingFactor, scalingFactor);
	hammerIdleRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(331 - 17-17, 343, 17, 18));
	s.scale(scalingFactor, scalingFactor);
	hammerIdleRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(331-17, 343, 17, 18));
	s.scale(scalingFactor, scalingFactor);
	hammerIdleRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(331 - 17 -17 , 343+18, 17, -18));
	s.scale(scalingFactor, scalingFactor);
	hammerIdleRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(331, 343+18, 17, -18));
	s.scale(scalingFactor, scalingFactor);
	hammerIdleRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(331 - 17, 343 +18 , -17, -18));
	s.scale(scalingFactor, scalingFactor);
	hammerIdleRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(331 , 343, -17, 18));
	s.scale(scalingFactor, scalingFactor);
	hammerIdleRight.addFrame(s);
	s = sf::Sprite(texture, sf::IntRect(331 - 17 , 343, -17, 18));
	s.scale(scalingFactor, scalingFactor);
	hammerIdleRight.addFrame(s);
}
std::pair<float, float> updatePlayer(AnimatedSprite  sprite, sf::Time frameTime) {

	float fr = frameTime.asSeconds();
	float x = sprite.getVelocityX() / fr;
	float y = sprite.getVelocityY() / fr;
	State current = static_cast<State>(sprite.getCurrState());
	

	if (current == walkRight) {
		if (x > walkSpeed) {
			x = walkSpeed;
		}
		x = walkSpeed;
	}
	else if (current == walkLeft) {
		x = -walkSpeed;
	}
	else if (current == crouchLeft) {
		x += walkSpeed*stopFactor;
		if (x > 0) {
			x = 0;
		}
	}
	else if (current == crouchRight) {
		x -= walkSpeed*stopFactor;
		if (x < 0) {
			x = 0;
		}
	}
	else if (current == idleLeft || current == idleRight) {
x = 0;
	}
	y += gravity;
	if (sprite.getGlobalBounds().intersects(floorBox.getGlobalBounds())) {
		sprite.setIsOnGround(true);
		sprite.setIsJumping(false);
		y = 0;

	}
	else {
		sprite.setIsOnGround(false);
	}

	if (current == jumpLeft || current == jumpRight) {

		if (!sprite.getIsJumping() && sprite.getIsOnGround() && abs(y) < 1) {
			y = -jumpSpeed;
			sprite.setIsJumping(true);

		}


	}


	float spriteLeft = sprite.getGlobalBounds().left;
	float spriteRight = sprite.getGlobalBounds().width + spriteLeft;

	float lineLeft = halfLine.getGlobalBounds().left;
	float lineRight = halfLine.getGlobalBounds().width + lineLeft;
	if (sprite.getTeam() == 1) {



		if (spriteLeft < 1) {
			if (current == walkLeft || current == jumpLeft || current == crouchLeft || current == idleLeft) {
				x = 0;
			}
		}
		if (spriteRight > lineLeft - 1) {
			if (current == walkRight || current == jumpRight || current == crouchRight || current == idleRight) {
				x = 0;
			}
		}
	}
	else if (sprite.getTeam() == 2) {

		if (spriteLeft - lineRight < 1) {
			if (current == walkLeft || current == jumpLeft || current == crouchLeft || current == idleLeft) {
				x = 0;
			}
		}
		if (spriteRight > screenDimensions.x - 1) {
			if (current == walkRight || current == jumpRight || current == crouchRight || current == idleRight) {
				x = 0;
			}
		}
	}

	return std::pair<float, float>(x*fr, y*fr);
}
void checkCollision(AnimatedSprite player, AnimatedSprite hammer, int id) {


	if (hammer.getGlobalBounds().intersects(player.getGlobalBounds())) {

		if (hammer.getTeam() == 0) {

			teamList[id-1] =player.getTeam();
			carryList[player.getTeam()-1] = 1;


		}
		else if (hammer.getTeam() != player.getTeam()) {
			victory = hammer.getTeam();
		}
	}
}


std::pair<float, float> updateHammer(AnimatedSprite sprite, sf::Time frameTime, int id) {

	float fr = frameTime.asSeconds();
	float x = sprite.getVelocityX() / fr;
	float y = sprite.getVelocityY() / fr;

	float throwingVelocity = 500;
	float angle = -30;
	std::pair<float, float> outputPair;

	State current = static_cast<State>(sprite.getCurrState());

	y += gravity;
	int team = teamList[id - 1];
	 if (carryList[team-1] == 1) {


		if (chuckList[sprite.getTeam() - 1] > 0) {

			carryList[team - 1] = 0;
			y = throwingVelocity*sin(angle*PI / 180);
			x = throwingVelocity*cos(angle*PI / 180);
			if (sprite.getTeam() == 1) {

				x += abs(p1V.first / fr);
				y += p1V.second / fr;
			}
			else {
				x += abs(p2V.first / fr);
				y += p2V.second / fr;
			}
			
			//printf("%i", chuckList[sprite.getTeam() - 1]);

			if (chuckList[sprite.getTeam() - 1] == 1)
				
				x = -x;
		}
		else {
			if (sprite.getTeam() == 1){
				x = p1V.first / fr;
				y = p1V.second / fr;
			}
			else {
				x = p2V.first / fr;
				y = p2V.second / fr;
			
			}
			
			//printf("x = %f, y = %f ", x, y);
		}
	}

		

	float spriteLeft = sprite.getGlobalBounds().left;
	float spriteRight = sprite.getGlobalBounds().width + spriteLeft;

	
	if (spriteRight > screenDimensions.x - 1) {
		
		if (x > 0) {

			x = -x;
		}
		
		
	}
	if (spriteLeft < 1) {
		if (x < 0) {

			x = -x;
		}
	}
	if (sprite.getGlobalBounds().intersects(floorBox.getGlobalBounds())) {
		if (y > 0) {
			y = 0;
		}
		
		if (carryList[team - 1] == 0) {
			
			if (y == 0) {
				x = 0;
				teamList[id - 1] = 0;
			}
			
		}

	}


	outputPair.first = x*fr;
	outputPair.second = y*fr;
	return outputPair;

}

void correctErrors() {

}

int main(int argc, char *argv[])
{ 
    //initialize UDPSystem
    char *serverIP = argv[1];
    char serverPort[] = "6767";
    UDPSystem udpClient(serverIP, serverPort);
	udpClient.init();
    messageContainer serverMessage;

    //UDP "handshake" with server
    do{
        serverMessage.msg = NULL;
	    udpClient.connect();
        serverMessage = udpClient.recvPacket(HALF_SEC_TIMEOUT);
    }while(serverMessage.msg == NULL);
    //here the serverMessage.msg contains either 1 or 2
    //for player 1 or 2

	// setup window
	floorBox.setFillColor(sf::Color(100, 250, 50));
	floorBox.setPosition(0, screenDimensions.y - FLOOR_HEIGHT);
	halfLine.setFillColor(sf::Color(250, 0, 0));
	halfLine.setPosition(screenDimensions.x/2, screenDimensions.y - FLOOR_HEIGHT);

	sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Animations!");
	window.setFramerateLimit(60);

	// load texture (spritesheet)
	if (!texture.loadFromFile("spritesheet.png"))
	{
		std::cout << "Failed to load player spritesheet!" << std::endl;
		return 1;
	}

	
	if (!font.loadFromFile("smb.ttf"))
	{
		// error...
	}

	sf::Text sideText;

	// select the font
	sideText.setFont(font); // font is a sf::Font						// set the string to display
	sideText.setString("Your Side");
	// set the character size
	sideText.setCharacterSize(24); // in pixels, not point
	sideText.setFillColor(sf::Color(255, 0, 0));
	// set the text style
	sideText.setStyle(sf::Text::Bold);

	sideText.setPosition(screenDimensions.x / 6, screenDimensions.y * 7 / 8);





	// set up the animations for all four directions (set spritesheet and push frames)
	setupAnimations();

	// set up AnimatedSprite
	AnimatedSprite player1(sf::seconds(1 / 25.f), 1 + playerTeamRight);
	player1.changePos(screenDimensions.x/2 -300 + 600*(playerSideLeft), screenDimensions.y*2/3);

	AnimatedSprite player2(sf::seconds(1 / 25.f), 1 + playerTeamRight);
	player2.changePos(screenDimensions.x / 2 + 300 + 600 * (playerSideLeft), screenDimensions.y *2/3);

	AnimatedSprite hammer1(sf::seconds(1 / 25.f), 0);
	hammer1.changePos(screenDimensions.x / 2 - 100, screenDimensions.y* 2/3);

	AnimatedSprite hammer2(sf::seconds(1 / 25.f), 0);
	hammer2.changePos(screenDimensions.x / 2 + 100, screenDimensions.y * 2 / 3);

	sf::Clock frameClock;
	sf::Text endText;

	sf::Clock updateClock;
	int ackCounter = 0;

	while (window.isOpen())
	{
		sf::Event event;
		std::pair<float, float> newVelocity;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}
		
		while (updateClock.getElapsedTime().asMilliseconds() < 1000 / 60) {
		}
			updateClock.restart();
			sf::Time frameTime = sf::milliseconds(1000/60);
			if (victory == 0) {
				player1.setAnimation(getCurrentAnimation(player1, false));

				Input p1Input = getCurrentInput(event);

				json output = 
				{
					{"ack", ackCounter},
					{"input" , p1Input }

				};
                std::string jsonString = output.dump();
                char *inputString = &jsonString[0];
				udpClient.sendPacket(TO_SERVER,inputString);

				player1.setState(getCurrentState(player1,p1Input));
				player1.update(frameTime);
				newVelocity = updatePlayer(player1, frameTime);
				player1.setVelocity(newVelocity.first, newVelocity.second);
				player1.movePosition();
				p1V = newVelocity;

				player2.setAnimation(getCurrentAnimation(player2, false));
				State p2State = interpolateState();
				player2.setState(p2State);
				player2.update(frameTime);
				newVelocity = updatePlayer(player2, frameTime);
				player2.setVelocity(newVelocity.first, newVelocity.second);
				player2.movePosition();
				p2V = newVelocity;

				

				hammer1.setTeam(teamList[0]);

                //yer1.changePos(output["p1X"], output["p1Y"]);
                //              player1.setVelocity(output["p1VX"], output["p1VY"]);player1.changePos(output["p1X"], output["p1Y"]);
                //                              player1.setVelocity(output["p1VX"], output["p1VY"]);send input to server
				hammer1.setAnimation(getCurrentAnimation(hammer1, true));
				//hammer1.setState(getCurrentState(hammer1, static_cast<State>(hammer1.getCurrState()), event));
				hammer1.update(frameTime);
				newVelocity = updateHammer(hammer1, frameTime,1);

				hammer1.setVelocity(newVelocity.first, newVelocity.second);
				hammer1.movePosition();
                h1V = newVelocity;

				checkCollision(player1, hammer1,1);
				checkCollision(player2, hammer1,1);




				hammer2.setTeam(teamList[1]);
				hammer2.setAnimation(getCurrentAnimation(hammer2, true));
				//hammer2.setState(getCurrentState(hammer2, static_cast<State>(hammer2.getCurrState()), event));
				hammer2.update(frameTime);
				newVelocity = updateHammer(hammer2, frameTime,2);

				hammer2.setVelocity(newVelocity.first, newVelocity.second);
				hammer2.movePosition();
                h2V = newVelocity;

				checkCollision(player1, hammer2,2);
				checkCollision(player2, hammer2,2);

                //receive server's game state
				serverMessage = udpClient.recvPacket(TIMEOUT);

	            //player1.changePos(output["p1X"], output["p1Y"]);
                //player1.setVelocity(output["p1VX"], output["p1VY"]);
			}
			else {


				// select the font
				endText.setFont(font); // font is a sf::Font						// set the string to display
				if (victory == 1) {
					endText.setString("YOU WIN!!");

				}
				else {
					endText.setString("YOU LOSE :(");
				}
				// set the character size
				endText.setCharacterSize(100); // in pixels, not point
				endText.setFillColor(sf::Color(255, 0, 0));
				// set the text style
				endText.setStyle(sf::Text::Bold);

				endText.setPosition(screenDimensions.x / 6, screenDimensions.y / 8);

			}

			//printf("%i", carryList[2]);



			// draw
			window.clear();





			// inside the main loop, between window.clear() and window.display()

			window.draw(player1);
			window.draw(player2);
			window.draw(hammer1);
			window.draw(hammer2);
			window.draw(floorBox);
			window.draw(halfLine);
			window.draw(sideText);
			if (victory != 0) {
				window.draw(endText);
			}

			window.display();
		}
		

    udpClient.closeSocket();		

	return 0;
}

