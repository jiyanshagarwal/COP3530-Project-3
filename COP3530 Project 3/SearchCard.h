#pragma once
#include <SFML/Graphics.hpp>
#include <Drawable.h>
#include "CarData.h"

class SearchCard : public Drawable {
public:
	SearchCard(float x, float y, float width, float height, CarData data, const sf::Font& font, sf::RenderWindow* window);

	void Tick() override;
	void Event(sf::Event& event) override;
	void Draw(sf::RenderTarget& target) override;

private:
	CarData data;
	int border_width;
	sf::Image photo_image;
	sf::Texture photo_texture;

	sf::RenderWindow* window;
	sf::Font font;
	sf::Cursor cursor;
	bool cursor_changed;

	float GetTextWidth(std::string str, unsigned int character_size) const;
	std::string StringWrap(std::string str, float width, unsigned int character_size) const;
};