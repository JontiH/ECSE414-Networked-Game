#include <SFML/Graphics.hpp>
#include <string>


#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"
#include <iostream>


enum State { walkLeft, walkRight, crouchLeft, crouchRight , idleLeft, idleRight, jumpLeft, jumpRight, chuckLeft, chuckRight , die };
float FLOOR_HEIGHT = 100.0;
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


sf::Vector2i screenDimensions(800, 600);
sf::RectangleShape floorBox(sf::Vector2f(screenDimensions.x, FLOOR_HEIGHT));
sf::RectangleShape halfLine(sf::Vector2f(10, FLOOR_HEIGHT));


State getCurrentState(AnimatedSprite sprite, State previousState, sf::Event event, sf::Time frameTime)  {

	bool noKeyWasPressed = true;
	State currentState = previousState;
	// if a key was pressed set the correct animation and move correctly

	
	if (sprite.getTeam() == 0) {

		return currentState;

		/*
		if (sprite.getVelocityX > 0) {
			if (sprite.getIsOnGround()) {

			}
		}
		*/
	}
	
	if (currentState == die) {
		return currentState;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
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
Animation getCurrentAnimation(AnimatedSprite sprite, State current, State previous) {
	Animation* tmp(NULL);

	if (sprite.getTeam() == 0) {
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
			tmp = &crouchingAnimationRight;
	}
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
std::pair<float, float> updateSim(State current, AnimatedSprite sprite, sf::Time frameTime) {

	float fr = frameTime.asSeconds();
	float x = sprite.getVelocityX() / fr;
	float y = sprite.getVelocityY() / fr;

	

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
			printf("LOL\n");
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

	else {

		if (spriteRight > screenDimensions.x - 1) {
			x = -x;
		}
		if (spriteLeft < 1) {
			x = -x;
		}
		
	}
	
	
	//printf("HERE YOU GO %i \n\n", isOnGround);
	//printf("here is x -> %f, here is y -> %f \n\n,", sprite.getPosX(), sprite.getPosY());
	
	return std::pair<float,float>(x*fr, y*fr);
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
	AnimatedSprite animatedSprite(sf::seconds(1/15.f),1);
	animatedSprite.changePos(screenDimensions.x/2, screenDimensions.y/2);
	animatedSprite.setAnimation(idleAnimationRight,true);


	AnimatedSprite hammer(sf::seconds(1 / 15.f), 0);
	hammer.changePos(screenDimensions.x / 2 + 200, screenDimensions.y / 2);
	hammer.setAnimation(hammerIdleRight, true);

	State currentStatePlayer = idleRight;
	State previousStatePlayer;

	State currentStateHammer = idleRight;
	State previousStateHammer;

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
		previousStatePlayer = currentStatePlayer;
		currentStatePlayer = getCurrentState(animatedSprite, previousStatePlayer, event, frameTime);


		bool hasChanged = (!(currentStatePlayer == previousStatePlayer));
		Animation playerAnimation = getCurrentAnimation(animatedSprite, currentStatePlayer, previousStatePlayer);



		animatedSprite.setAnimation(playerAnimation, hasChanged);
		animatedSprite.update(frameTime);
		std::pair<float, float> newVelocity = updateSim(currentStatePlayer, animatedSprite, frameTime);
		animatedSprite.setVelocity(newVelocity.first, newVelocity.second);
		animatedSprite.movePosition();



		previousStateHammer = currentStateHammer;
		currentStateHammer = getCurrentState(hammer, previousStateHammer, event, frameTime);

		hasChanged = (!(currentStateHammer == previousStateHammer));
		Animation hammerAnimation = getCurrentAnimation(hammer, currentStateHammer, previousStateHammer);



		hammer.setAnimation(hammerAnimation, hasChanged);
		hammer.update(frameTime);
		newVelocity = updateSim(currentStateHammer, hammer, frameTime);
		hammer.setVelocity(newVelocity.first, newVelocity.second);
		hammer.movePosition();



		// draw
		window.clear();
		window.draw(animatedSprite);
		window.draw(hammer);
		window.draw(floorBox);
		window.draw(halfLine);


		window.display();
	}

	return 0;
}


