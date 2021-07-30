#pragma once
#include <SFML/Graphics.hpp>
#include <Drawable.h>

class SearchCard : public Drawable {
public:
	SearchCard(float x, float y, float width, float height, std::string photo_filename);

	void Draw(sf::RenderTarget& target) override;

private:
	int border_width;
	sf::Texture photo_texture;
};