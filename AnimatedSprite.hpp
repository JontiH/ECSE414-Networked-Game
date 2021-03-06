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

#ifndef ANIMATEDSPRITE_INCLUDE
#define ANIMATEDSPRITE_INCLUDE

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Animation.hpp"

class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:
	explicit AnimatedSprite(sf::Time frameTime = sf::seconds(0.2f),int team = 1);
	void update(sf::Time deltaTime);
	void setAnimation(const Animation* animation);
	void setFrameTime(sf::Time time);

	Animation getAnimation();
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	sf::Time getFrameTime() const;

	float getVelocityX();
	float getVelocityY();
	void changePos(float x, float y);
	float getPosY();
	float getPosX();
	int getTeam();
	bool getIsJumping();
	bool getIsOnGround();
	int getCurrState();
	int getPrevState();

	void setTeam(int b);
	void setIsJumping(bool j);
	void setIsOnGround(bool g);
	void setState(int s);
	void movePosition();
	void setVelocity(float x, float y);


private:
	const Animation * m_animation;
	sf::Time m_frameTime;
	sf::Time m_currentTime;
	std::size_t m_currentFrame;
	sf::Sprite currentSprite;
	float xPos;
	float yPos;
	float xVelocity;
	float yVelocity;
	int currState;
	int prevState;
	bool isJumping;
	bool isOnGround;
	bool changedState;

	int team;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif // ANIMATEDSPRITE_INCLUDE