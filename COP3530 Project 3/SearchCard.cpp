#include "SearchCard.h"
#include "ImageDownloader.h"
#include <Windows.h>

SearchCard::SearchCard(float x, float y, float width, float height, CarData data, const ResourceManager<ImageDownloader>& resources)
	: Drawable(x, y, width, height), resources(resources) {
	this->data = data;
	border_width = width / 50;
	cursor_changed = false;
	image_downloaded = false;

	//This part adds newline characters to the string to make it fit in the box.
	float description_width = (2 * width / 3) - 3 * border_width;
	float description_height = height - 2 * border_width - 90;
	this->data.car_description = StringWrap(data.car_description, description_height / 16 - 1, description_width, 16);

	float name_width = width - 3 * border_width - (width / 3) - 100;
	this->data.car_name = StringWrap(data.car_name, 2, name_width, 32);

	photo.setTexture(&resources.resource.image_loading_failed_image);
}

void SearchCard::Tick() {
	if (this->MouseInBounds(mouse_x, mouse_y) && cursor.loadFromSystem(sf::Cursor::Hand)) {
		resources.window.setMouseCursor(cursor);
		cursor_changed = true;
	}
	else if (cursor_changed) {
		cursor.loadFromSystem(sf::Cursor::Arrow);
		resources.window.setMouseCursor(cursor);
		cursor_changed = false;
	}

	if (!image_downloaded) {
		if (resources.resource.GetImage(data.image_url, photo_texture)) { 
			photo_texture.setSmooth(true);
			photo.setTexture(&photo_texture, true);
			image_downloaded = true; 
		}
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

	photo.setSize(sf::Vector2f(width / 3, height - 2 * border_width));
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

	sf::Text name(data.car_name, resources.font, 32);
	sf::Text VIN(data.car_VIN, resources.font, 12);
	sf::Text price("$" + data.car_price, resources.font, 24);
	sf::Text description(data.car_description, resources.font, 16);

	name.setPosition(photo.getPosition().x + photo.getSize().x + border_width, y + border_width);
	price.setPosition(x + width - 100, y + border_width);
	description.setPosition(photo.getPosition().x + photo.getSize().x + border_width, y + border_width + 90);

	price.setFillColor(sf::Color(252, 127, 0));

	if (data.car_name.find('\n') != std::string::npos) {
		VIN.setPosition(photo.getPosition().x + photo.getSize().x + border_width, y + border_width + 72);
	}
	else {
		VIN.setPosition(photo.getPosition().x + photo.getSize().x + border_width, y + border_width + 40);
	}

	target.draw(card);
	target.draw(photo);
	target.draw(name);
	target.draw(VIN);
	target.draw(price);
	target.draw(description);
}

float SearchCard::GetTextWidth(std::string str, unsigned int character_size) const {
	sf::Text text(str, resources.font, character_size);
	return text.getGlobalBounds().width;
}

std::string SearchCard::StringWrap(std::string str, int max_lines, float width, unsigned int character_size) const {
	float str_width = GetTextWidth(str, character_size);

	if (str_width > width) {
		int line_length = std::floor(width * str.size() / str_width);

		for (int i = 1; i * line_length < str.size(); i++) {
			if (i == max_lines) {
				str = str.substr(0, i * line_length);
				return str;
			}

			int newline_pos = str.rfind(" ", i * line_length);

			if (newline_pos == std::string::npos) {
				newline_pos = i * line_length;
				str.insert(newline_pos + 1, "\n");
			}
			else {
				str[newline_pos] = '\n';
			}

		}
	}

	return str;
}