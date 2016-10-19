#include <SFML/Graphics.hpp>
#include <string>


#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"
#include <iostream>

enum State { walkLeft, walkRight, crouchLeft, crouchRight , idleLeft, idleRight, jumpLeft, jumpRight, chuckLeft, chuckRight , die };

sf::Texture texture;

Animation idleAnimationLeft(false,"idleLeft");
Animation crouchingAnimationLeft(false,"crouchLeft");
Animation walkingAnimationLeft(true,"walkLeft");


Animation idleAnimationRight(false, "idleRight");
Animation crouchingAnimationRight(false,"crouchRight");
Animation walkingAnimationRight(true,"walkRight");

float scalingFactor = 3.5;
sf::Sprite s;
float walkSpeed = 200;
float stopFactor = 0.10 ;
float gravity = 9.8;
float jumpSpeed = 500;
bool isJumping;

State getCurrentState(State previousState, sf::Event event, sf::Time frameTime)  {

	bool noKeyWasPressed = true;
	State currentState = previousState;
	// if a key was pressed set the correct animation and move correctly

	if (currentState == die) {
		return currentState;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{

		if ((currentState == walkLeft) || (currentState == idleLeft) || (currentState == chuckLeft) || (currentState == jumpLeft) || (currentState == crouchLeft)) {
			currentState = crouchLeft;
		}
		else {
			currentState = crouchRight;
		}

		noKeyWasPressed = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
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
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
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
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if ((currentState == idleRight) || (currentState == walkRight) || (currentState = crouchRight) || (currentState == idleRight) ||(currentState == jumpRight)) {
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
Animation getCurrentAnimation(State current, State previous) {
	Animation* tmp(NULL);
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
		tmp = &crouchingAnimationRight;
	return *tmp;

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

}
std::pair<float, float> updateSim(State current, AnimatedSprite sprite, sf::Time frameTime) {
	
	float fr = frameTime.asSeconds();
	float x = sprite.getVelocityX()/fr;
	float y = sprite.getVelocityY()/fr;

	if (current == walkRight) {
		if (x > walkSpeed){
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
	
	if (current == jumpLeft || current == jumpRight) {

		
		if (!isJumping) {
			y -= jumpSpeed;
			isJumping = true;
		}
		//printf("pre y is here %f \n", y);
	}
	y += gravity;
	if ((sprite.getPosY()+y*fr) > 400.f) {
	

		isJumping = false;
		y = 0; 

	}
	
	

	return std::pair<float,float>(x*fr, y*fr);
}
int main()
{ 
	
	// setup window
	sf::Vector2i screenDimensions(800, 600);
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
	AnimatedSprite animatedSprite(sf::seconds(1/20.f));
	animatedSprite.changePos(screenDimensions.x/2, screenDimensions.y/2);
	animatedSprite.setAnimation(idleAnimationRight,true);
	State currentState = State::idleRight;
	State previousState;
	sf::Clock frameClock;
	

	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		sf::Time frameTime = frameClock.restart();
		previousState = currentState;
		currentState = getCurrentState(previousState,event, frameTime);

		//printf("here is the current state %i\n", isJumping);
		bool hasChanged = (!(currentState == previousState));
		Animation currentAnimation = getCurrentAnimation(currentState, previousState);
		
		
		std::pair<float,float> newVelocity = updateSim(currentState, animatedSprite, frameTime);

		animatedSprite.setVelocity(newVelocity.first , newVelocity.second);
		animatedSprite.movePosition();


		animatedSprite.setAnimation(currentAnimation, hasChanged);
		animatedSprite.update(frameTime);


		// draw
		window.clear();
		window.draw(animatedSprite);
		window.display();
	}

	return 0;
}


