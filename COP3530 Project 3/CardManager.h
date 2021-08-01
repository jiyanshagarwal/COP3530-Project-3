#pragma once
#include <Panel.h>
#include <vector>
#include "SearchCard.h"
#include "CarData.h"
#include "ResourceManager.h"

class CardManager : public Panel {
public:
	CardManager(float x, float y, float width, float height, float card_height, const ResourceManager<sf::Texture>& resources);
	~CardManager();

	void AddCard(CarData data);
	void Clear();

private:
	const ResourceManager<sf::Texture>& resources;
	float card_height;
	float prev_card_y;
};