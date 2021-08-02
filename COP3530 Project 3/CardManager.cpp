#include "CardManager.h"
#include "SearchCard.h"

CardManager::CardManager(float x, float y, float width, float height, float card_height, 
	const std::vector<std::string>& cards_to_show, const ResourceManager<ImageDownloader>& resources)
	: Panel(x, y, width, height), cards_to_show(cards_to_show), resources(resources) {
	prev_card_y = -card_height - 15;
	this->card_height = card_height;
	num_cards_to_show = NUM_CARDS_TO_SHOW;
	index_shown = 0;
}

CardManager::~CardManager() {
	this->Clear();
}

void CardManager::AddCard(CarData data) {
	cards.emplace(data.car_VIN, new SearchCard(5, 0, width - 30, card_height, data, resources));
}

void CardManager::UpdateCardsToShow() {
	objectsToDraw.clear();
	focused_object = nullptr;
	prev_card_y = -card_height - 15;

	unsigned int cards_added = 0;

	for (unsigned int i = 0; i < cards_to_show.size() && cards_added < num_cards_to_show; i++) {
		if (cards.find(cards_to_show[i]) != cards.end()) {
			SearchCard* card = cards[cards_to_show[i]];
			card->SetY(prev_card_y + card_height + 20);
			prev_card_y = prev_card_y + card_height + 20;

			AddObject(card, 0);
			cards_added++;
		}
		index_shown = i;
	}
}

void CardManager::ShowMoreCards(unsigned int num) {
	unsigned int cards_added = 0;

	for (unsigned int i = index_shown + 1; i < cards_to_show.size() && cards_added < num; i++) {
		if (cards.find(cards_to_show[i]) != cards.end()) {
			SearchCard* card = cards[cards_to_show[i]];
			card->SetY(prev_card_y + card_height + 20);
			prev_card_y = prev_card_y + card_height + 20;

			AddObject(card, 0);
			cards_added++;
		}
		index_shown = i;
	}
}

void CardManager::SetWidth(float num) {
	Panel::SetWidth(num);

	for (auto iter = cards.begin(); iter != cards.end(); iter++) {
		iter->second->SetWidth(num - 30);
	}
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