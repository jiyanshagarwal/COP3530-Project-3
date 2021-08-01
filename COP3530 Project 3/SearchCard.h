#pragma once
#include <SFML/Graphics.hpp>
#include <Drawable.h>
#include "ResourceManager.h"
#include "CarData.h"

class SearchCard : public Drawable {
public:
	SearchCard(float x, float y, float width, float height, CarData data, const ResourceManager<sf::Texture>& resources);

	void Tick() override;
	void Event(sf::Event& event) override;
	void Draw(sf::RenderTarget& target) override;

private:
	CarData data;
	int border_width;
	sf::RectangleShape photo;
	sf::Texture photo_texture;

	const ResourceManager<sf::Texture>& resources;
	sf::Cursor cursor;
	bool cursor_changed;

	float GetTextWidth(std::string str, unsigned int character_size) const;
	std::string StringWrap(std::string str, float width, unsigned int character_size) const;
};