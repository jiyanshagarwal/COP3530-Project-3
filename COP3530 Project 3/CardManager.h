#pragma once
#include <Panel.h>
#include "CarData.h"
#include "ResourceManager.h"
#include "ImageDownloader.h"

class CardManager : public Panel {
public:
	CardManager(float x, float y, float width, float height, float card_height, const ResourceManager<ImageDownloader>& resources);
	~CardManager();

	void AddCard(CarData data);
	void Clear();

private:
	const ResourceManager<ImageDownloader>& resources;
	float card_height;
	float prev_card_y;
};