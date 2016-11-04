
#ifndef ANIMATION_INCLUDE
#define ANIMATION_INCLUDE

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Animation
{
public:
	Animation(bool l, std::string n);

	void addFrame(sf::Sprite sp);
	bool getIsLooped() const;
	std::size_t getSize() const;
	std::string getName() const;
	const sf::Sprite getFrame(std::size_t n) const;

private:
	bool loop;
	std::vector<sf::Sprite> m_frames;
	const sf::Texture* m_texture;
	std::string name;
	
};

#endif // ANIMATION_INCLUDE
