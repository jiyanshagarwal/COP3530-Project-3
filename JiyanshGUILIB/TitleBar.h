#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Drawable.h"
#include "Panel.h"
#include "Button.h"

class TitleBar : public Panel {
public:
	TitleBar(float height, sf::RenderWindow& window);
	~TitleBar();
	void Event(sf::Event& event) override;
	void AddObject(Drawable* object, unsigned int layer) override;

	void ShowCloseButton();
	void HideCloseButton();
	void ShowMinimizeButton();
	void HideMinimizeButton();

	void SetColor(sf::Color color) override;

private:
	Button* close_button;
	Button* minimize_button;
	sf::RenderWindow& window;

	bool window_held;
	sf::Vector2i mouse_grab_offset;

	void Initialize();
};