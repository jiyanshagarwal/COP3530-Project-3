#pragma once
#include <SFML/Graphics.hpp>
#include <Drawable.h>
#include "CarData.h"
#include "ResourceManager.h"
#include "ImageDownloader.h"

class SearchCard : public Drawable {
public:
	SearchCard(float x, float y, float width, float height, CarData data, const ResourceManager<ImageDownloader>& resources);

	void Tick() override;
	void Event(sf::Event& event) override;
	void Draw(sf::RenderTarget& target) override;

private:
	CarData data;
	int border_width;
	sf::RectangleShape photo;
	sf::Texture photo_texture;

	const ResourceManager<ImageDownloader>& resources;
	sf::Cursor cursor;
	bool cursor_changed;
	bool image_downloaded;

	float GetTextWidth(std::string str, unsigned int character_size) const;
	std::string StringWrap(std::string str, int max_lines, float width, unsigned int character_size) const;
};