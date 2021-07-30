#pragma once
#include <SFML/Graphics.hpp>
#include <Drawable.h>

class SearchCard : public Drawable {
public:
	SearchCard(float x, float y, float width, float height, std::string photo_filename, sf::RenderWindow& window);

	void Tick() override;
	void Draw(sf::RenderTarget& target) override;

private:
	int border_width;
	sf::Texture photo_texture;

	sf::RenderWindow& window;
	sf::Cursor cursor;
	bool cursor_changed;
};