#include <string>

#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"
#include <iostream>
#include <math.h>  


enum State { walkLeft, walkRight, crouchLeft, crouchRight , idleLeft, idleRight, jumpLeft, jumpRight, chuckLeft, chuckRight , die };
float FLOOR_HEIGHT = 100.0;

#define PI 3.14159265
sf::Texture texture;

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

int carryList[1] = { 0 };
int chuckList[2] = { 0 };
int teamList[1] = { 0 };

std::pair<float, float> p1V, p2V;


sf::Vector2i screenDimensions(800, 600);
sf::RectangleShape floorBox(sf::Vector2f(screenDimensions.x, FLOOR_HEIGHT));
sf::RectangleShape halfLine(sf::Vector2f(10, FLOOR_HEIGHT));


State getCurrentState(AnimatedSprite sprite, State previousState, sf::Event event)  {

	bool noKeyWasPressed = true;
	State currentState = previousState;
	// if a key was pressed set the correct animation and move correctly

	
	if (currentState == die) {
		return currentState;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		
		if (previousState == chuckLeft || previousState == chuckRight) {
			chuckList[sprite.getTeam() - 1] = 0;
			currentState = previousState;
		}
		else {
			
			if ((currentState == idleRight) || (currentState == walkRight) || (currentState == crouchRight) || (currentState == idleRight) || (currentState == jumpRight) ) {
				
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

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{

		if ((currentState == walkLeft) || (currentState == idleLeft) || (currentState == chuckLeft) || (currentState == jumpLeft) || (currentState == crouchLeft)) {
			currentState = crouchLeft;
		}
		else {
			currentState = crouchRight;
		}

		noKeyWasPressed = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
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
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
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
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if ((currentState == idleRight) || (currentState == walkRight) || (currentState == crouchRight) || (currentState == idleRight) ||(currentState == jumpRight)) {
			currentState = jumpRight;
		}
		else
			currentState = jumpLeft;
		noKeyWasPressed = false;
		
	}
	
	// if no key was pressed stop the animation
	else if (noKeyWasPressed)
	{
		

		if ((currentState == walkLeft) || (currentState == idleLeft) || (currentState == chuckLeft) || (currentState == jumpLeft) || (currentState == crouchLeft)) {
			currentState = idleLeft;
		}
		else
			currentState = idleRight;
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
void checkCollision(AnimatedSprite player, AnimatedSprite hammer) {


	if (hammer.getGlobalBounds().intersects(player.getGlobalBounds())) {

		if (hammer.getTeam() == 0) {

			teamList[0] =player.getTeam();
			carryList[0] = 1;


		}
		else if (hammer.getTeam() != player.getTeam()) {
			//die
		}
	}
}


std::pair<float, float> updateHammer(AnimatedSprite sprite, sf::Time frameTime) {

	float fr = frameTime.asSeconds();
	float x = sprite.getVelocityX() / fr;
	float y = sprite.getVelocityY() / fr;

	float throwingVelocity = 500;
	float angle = -45;
	std::pair<float, float> outputPair;

	State current = static_cast<State>(sprite.getCurrState());

	y += gravity;
	
	 if (carryList[0] == 1) {

		if (chuckList[sprite.getTeam() - 1] > 0) {

			carryList[0] = 0;
			y = throwingVelocity*sin(angle*PI / 180);
			x = throwingVelocity*cos(angle*PI / 180);
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
		x = -x;
	}
	if (spriteLeft < 1) {
		x = -x;
	}
	if (sprite.getGlobalBounds().intersects(floorBox.getGlobalBounds())) {
		if (y > 0) {
			y = 0;
		}
		
		if (carryList[0] == 0) {
			
			if (y == 0) {
				x = 0;
				teamList[0] = 0;
			}
			
		}

	}


	outputPair.first = x*fr;
	outputPair.second = y*fr;
	return outputPair;

}

int main()
{ 
	
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

	// set up the animations for all four directions (set spritesheet and push frames)
	setupAnimations();

	// set up AnimatedSprite
	AnimatedSprite player1(sf::seconds(1/25.f),1);
	player1.changePos(screenDimensions.x/2 -200, screenDimensions.y/2);

	AnimatedSprite player2(sf::seconds(1 / 25.f), 2);
	player2.changePos(screenDimensions.x / 2 + 200, screenDimensions.y / 2);

	AnimatedSprite hammer1(sf::seconds(1 / 25.f), 0);
	hammer1.changePos(screenDimensions.x / 2 - 150, screenDimensions.y / 2);

	sf::Clock frameClock;
	

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

		sf::Time frameTime = frameClock.restart();
		
		player1.setAnimation(getCurrentAnimation(player1,false));
		
		player1.setState(getCurrentState(player1, static_cast<State>(player1.getCurrState()), event));
		player1.update(frameTime);
		newVelocity = updatePlayer(player1, frameTime);
		player1.setVelocity(newVelocity.first, newVelocity.second);
		player1.movePosition();
		p1V = newVelocity;

		player2.setAnimation(getCurrentAnimation(player2,false));
		player2.setState(getCurrentState(player2, static_cast<State>(player2.getCurrState()), event));
		player2.update(frameTime);
		newVelocity = updatePlayer(player2, frameTime);
		player2.setVelocity(newVelocity.first, newVelocity.second);
		player2.movePosition();
		p2V = newVelocity;


		
		hammer1.setAnimation(getCurrentAnimation(hammer1,true));
		hammer1.setState(getCurrentState(hammer1, static_cast<State>(hammer1.getCurrState()), event));
		hammer1.update(frameTime);
		newVelocity = updateHammer(hammer1, frameTime);

		hammer1.setVelocity(newVelocity.first, newVelocity.second);
		hammer1.movePosition();

		checkCollision(player1,hammer1);
		checkCollision(player2, hammer1);

		hammer1.setTeam(teamList[0]);
		
		//printf("%i", carryList[2]);

	

		// draw
		window.clear();
		window.draw(player1);
		window.draw(player2);
		window.draw(hammer1);
		window.draw(floorBox);
		window.draw(halfLine);
		

		window.display();
	}

	return 0;
}


