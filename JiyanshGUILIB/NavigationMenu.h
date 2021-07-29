#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"

class NavigationMenu : public Menu {
public:
	NavigationMenu(float x, float y, float width, float height, const sf::Font& font);
	NavigationMenu(const NavigationMenu& ref);
	NavigationMenu& operator=(const NavigationMenu& ref);

	void Draw(sf::RenderTarget& target) override;
	void Event(sf::Event& event) override;

	bool AddItem(std::string item) override;

	void SetX(float num) override;
	void SetY(float num) override;
	void SetHeight(float num) override;

	void Open() override;
	void Close() override;
	bool MouseInBounds(float x, float y) const override;

private:
	sf::CircleShape triangle;

	void OffsetSiblings(float offset);
	void copy(const NavigationMenu& ref);
};