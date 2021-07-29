#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"

class DropdownMenu : public Menu {
public:
	DropdownMenu(float x, float y, float width, float height, const sf::Font& font);
	DropdownMenu(const DropdownMenu& ref);
	DropdownMenu& operator=(const DropdownMenu& ref);

	void Tick() override;
	void Draw(sf::RenderTarget& target) override;
	void Event(sf::Event& event) override;

	bool AddItem(std::string item) override;
	bool RemoveItem(std::string item) override;
	using Menu::ClearItems;
	void ClearItems() override;

	void SetX(float num) override;
	void SetY(float num) override;
	void SetWidth(float num) override;
	void SetHeight(float num) override;

	void Open() override;
	void Close() override;

	bool MouseInBounds(float x, float y) const override;

private:
	Menu* open_child;

	void copy(const DropdownMenu& ref);
};