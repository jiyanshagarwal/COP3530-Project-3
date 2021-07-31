#pragma once
#include <SFML/Graphics.hpp>
#include <Drawable.h>
#include "CarData.h"

class SearchCard : public Drawable {
public:
	CarData data;
	sf::Font font;

	SearchCard(float x, float y, float width, float height, CarData data, sf::RenderWindow* window);

	void Tick() override;
	void Event(sf::Event& event) override;
	void Draw(sf::RenderTarget& target) override;

private:
	int border_width;
	sf::Texture photo_texture;

	sf::RenderWindow* window;
	sf::Cursor cursor;
	bool cursor_changed;
};