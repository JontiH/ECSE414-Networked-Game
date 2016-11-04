////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include "AnimatedSprite.hpp"

AnimatedSprite::AnimatedSprite(sf::Time frameTime, int team ) :
	m_animation(NULL), m_frameTime(frameTime), m_currentFrame(0), xPos(0), yPos(0), team(team)
{

}

void AnimatedSprite::setAnimation(const Animation& animation, bool hasChanged)
{
	printf("has it changed POST?  %i\n", hasChanged);
	m_animation = &animation;
	
	if (hasChanged)
		m_currentFrame = 0;		

}

void AnimatedSprite::setFrameTime(sf::Time time)
{
	m_frameTime = time;
}

Animation AnimatedSprite::getAnimation() 
{
	
	return *m_animation;

}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
	sf::Sprite sprite = m_animation->getFrame(m_currentFrame);
	sprite.setPosition(xPos, yPos);
	return sf::FloatRect(sprite.getLocalBounds());
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const
{
	sf::Sprite sprite = m_animation->getFrame(m_currentFrame);
	sprite.setPosition(xPos, yPos);
	return sf::FloatRect(sprite.getGlobalBounds());
}

sf::Time AnimatedSprite::getFrameTime() const
{
	return m_frameTime;
}
void AnimatedSprite::movePosition() {
	xPos += xVelocity;
	yPos += yVelocity;
	//printf("velocity?  %f, %f\n", xPos, yPos);
}
void AnimatedSprite::setVelocity(float x, float y) {
	xVelocity = x;
	yVelocity = y;

}
float AnimatedSprite::getVelocityX() {
	return xVelocity;
}
float AnimatedSprite::getVelocityY() {
	return yVelocity;
}
void AnimatedSprite::changePos(float x, float y) {
	xPos = x;
	yPos = y;
}
float AnimatedSprite::getPosY() {
	return yPos;
}
float AnimatedSprite::getPosX() {
	return xPos;
}
void AnimatedSprite::setTeam(int b) {
	team = b;
}
int  AnimatedSprite::getTeam() {
	return team;
}
bool AnimatedSprite::getIsJumping() {
	return isJumping;
}
bool AnimatedSprite::getIsOnGround() {
	return isOnGround;
}
void AnimatedSprite::setIsOnGround(bool g) {
	isOnGround = g;
}
void AnimatedSprite::setIsJumping(bool j) {
	isJumping = j;
}


void AnimatedSprite::update(sf::Time deltaTime) 
{
	//printf("the frame is --> %i \n\n", m_currentFrame);

	// if not paused and we have a valid animation
	if ( m_animation)
	{
		// add delta time
		m_currentTime += deltaTime;

		// if current time is bigger then the frame time advance one frame
		if (m_currentTime >= m_frameTime)
		{
			// reset time, but keep the remainder
			m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

			// get next Frame index
			
			if (m_currentFrame + 1 < m_animation->getSize()) {
				m_currentFrame++;
			}
			else
			{
				// animation has ended
				 // reset to start
				if (getAnimation().getIsLooped()) {
					m_currentFrame = 0;
				}

			}

			// set the current frame, not reseting the time
		}
	}
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//printf("FFFUUU %i \n", m_currentFrame);
	if (m_animation->getSize()>0) {

		sf::Sprite currSprite = m_animation->getFrame(m_currentFrame);
		currSprite.setPosition(xPos, yPos);

		//printf("xd %f, %f \n\n", );
		target.draw(currSprite, states);
	}
	
}