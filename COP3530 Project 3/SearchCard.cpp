#include "SearchCard.h"

SearchCard::SearchCard(float x, float y, float width, float height, std::string photo_filename, sf::RenderWindow& window) 
	: Drawable(x, y, width, height), window(window) {
	border_width = width / 50;

	photo_texture.loadFromFile(photo_filename);
	photo_texture.setSmooth(true);

	cursor_changed = false;
}

void SearchCard::Tick() {
	if (this->MouseInBounds(mouse_x, mouse_y) && cursor.loadFromSystem(sf::Cursor::Hand)) {
		window.setMouseCursor(cursor);
		cursor_changed = true;
	}
	else if (cursor_changed) {
		cursor.loadFromSystem(sf::Cursor::Arrow);
		window.setMouseCursor(cursor);
		cursor_changed = false;
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

	sf::Color original_color = card.getFillColor();

	if (MouseInBounds(mouse_x, mouse_y)) {
		if (has_focus && mouse_held) {
			float shade_factor = 0.1f;
			sf::Color darker = sf::Color((int)(original_color.r * (1 - shade_factor)),
				(int)(original_color.g * (1 - shade_factor)), (int)(original_color.b * (1 - shade_factor)));
			card.setFillColor(darker);
		}
	}

	target.draw(card);
	target.draw(photo);
}