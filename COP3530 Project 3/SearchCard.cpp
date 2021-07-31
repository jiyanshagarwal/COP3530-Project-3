#include "SearchCard.h"
#include <Windows.h>

SearchCard::SearchCard(float x, float y, float width, float height, CarData data, sf::RenderWindow* window)
	: Drawable(x, y, width, height), window(window) {
	border_width = width / 50;
	photo_texture.setSmooth(true);

	this->data = data;
	cursor_changed = false;
}

void SearchCard::Tick() {
	if (this->MouseInBounds(mouse_x, mouse_y) && cursor.loadFromSystem(sf::Cursor::Hand)) {
		window->setMouseCursor(cursor);
		cursor_changed = true;
	}
	else if (cursor_changed) {
		cursor.loadFromSystem(sf::Cursor::Arrow);
		window->setMouseCursor(cursor);
		cursor_changed = false;
	}
}

void SearchCard::Event(sf::Event& event) {
	Drawable::Event(event);

	if (event.type == sf::Event::MouseButtonReleased && MouseInBounds(mouse_x, mouse_y) && has_focus && !data.page_url.empty()) {
		ShellExecuteA(0, 0, data.page_url.c_str(), 0, 0, 1);
	}
}

void SearchCard::Draw(sf::RenderTarget& target) {
	sf::RectangleShape card(sf::Vector2f(width, height));
	card.setPosition(sf::Vector2f(x, y));
	card.setFillColor(sf::Color(50, 50, 50));
	card.setOutlineColor(sf::Color::Black);
	card.setOutlineThickness(3);

	sf::RectangleShape photo(sf::Vector2f(width / 3, height - 2 * border_width));
	photo.setTexture(&photo_texture);
	photo.setPosition(x + border_width, y + border_width);

	//------------------Change Color when Clicked------------------//
	sf::Color original_color = card.getFillColor();

	if (MouseInBounds(mouse_x, mouse_y)) {
		if (has_focus && mouse_held) {
			float shade_factor = 0.1f;
			sf::Color darker = sf::Color((int)(original_color.r * (1 - shade_factor)),
				(int)(original_color.g * (1 - shade_factor)), (int)(original_color.b * (1 - shade_factor)));
			card.setFillColor(darker);
		}
	}
	//------------------------------------------------------------//

	sf::Text name(data.car_name, font, 32);
	sf::Text VIN(data.car_VIN, font, 12);
	sf::Text price("$" + data.car_price, font, 24);
	sf::Text description(data.car_description, font, 16);
	
	name.setPosition(photo.getPosition().x + photo.getSize().x + border_width, y + border_width);
	VIN.setPosition(photo.getPosition().x + photo.getSize().x + border_width, y + border_width + 40);
	price.setPosition(x + width - 130, y + border_width);
	description.setPosition(photo.getPosition().x + photo.getSize().x + border_width, y + border_width + 90);

	price.setFillColor(sf::Color(252, 127, 0));

	target.draw(card);
	target.draw(photo);
	target.draw(name);
	target.draw(VIN);
	target.draw(price);
	target.draw(description);
}