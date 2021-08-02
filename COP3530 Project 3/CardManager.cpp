#include "CardManager.h"
#include "SearchCard.h"

CardManager::CardManager(float x, float y, float width, float height, float card_height, const ResourceManager<ImageDownloader>& resources)
	: Panel(x, y, width, height), resources(resources) {
	prev_card_y = -card_height - 15;
	this->card_height = card_height;
}

CardManager::~CardManager() {
	this->Clear();
}

void CardManager::AddCard(CarData data) {
	AddObject(new SearchCard(5, prev_card_y + card_height + 20, width - 30, card_height, data, resources), 0);
	prev_card_y = prev_card_y + card_height + 20;
}

void CardManager::Clear() {
	for (auto iter = objectsToDraw.begin(); iter != objectsToDraw.end(); iter++) {
		if (iter->second != nullptr) {
			delete iter->second;
		}
	}

	objectsToDraw.clear();

	if (focused_object != nullptr) {
		delete focused_object;
	}
}