#pragma once
#include <Panel.h>
#include <vector>
#include "SearchCard.h"
#include "CarData.h"

class CardManager : public Panel {
public:
	CardManager(float x, float y, float width, float height, float card_height, const sf::Font& font, sf::RenderWindow* window);
	~CardManager();

	void AddCard(CarData data);
	void Clear();

private:
	sf::RenderWindow* window;
	sf::Font font;

	int card_height;
	int prev_card_y;
};