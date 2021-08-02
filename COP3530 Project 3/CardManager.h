#pragma once
#include <unordered_map>
#include <vector>
#include <Panel.h>
#include "CarData.h"
#include "SearchCard.h"
#include "ResourceManager.h"
#include "ImageDownloader.h"

class CardManager : public Panel {
public:
	const static unsigned int NUM_CARDS_TO_SHOW = 25;

	CardManager(float x, float y, float width, float height, float card_height, 
		const std::vector<std::string>& cards_to_show, const ResourceManager<ImageDownloader>& resources);
	~CardManager();

	void AddCard(CarData data);
	void UpdateCardsToShow();
	void ShowMoreCards(unsigned int num);

	void SetWidth(float num) override;

	void Clear();

private:
	std::unordered_map<std::string, SearchCard*> cards;
	const std::vector<std::string>& cards_to_show;
	const ResourceManager<ImageDownloader>& resources;

	float card_height;
	float prev_card_y;
	unsigned int num_cards_to_show, index_shown;
};