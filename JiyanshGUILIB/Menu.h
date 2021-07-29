#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "Drawable.h"
#include "Callback.h"

class Menu : public Drawable, public Callback<std::string> {
public:
	Menu(float x, float y, float width, float height, const sf::Font& font, float text_scale_factor);
	virtual ~Menu();

	virtual void SetMousePosition(float x, float y) override;
	void Event(sf::Event& event) override;

	using Callback::OnClick;
	bool OnClick(std::string item, std::function<void(std::string)> func);

	template <typename T>
	bool OnClick(std::string item, T* object, std::function<void(T*, std::string)> func);

	template <typename U>
	bool OnClick(std::string item, const U* object, std::function<void(const U*, std::string)> func);

	virtual bool AddItem(std::string item) = 0;
	virtual bool RemoveItem(std::string item);
	virtual void ClearItems();
	virtual bool ClearItems(std::string name);
	virtual bool HasItems() const;

	virtual void SetFocus(bool val) override;

	virtual void SetName(std::string name);
	virtual void SetTextColor(sf::Color color);
	virtual void SetColor(sf::Color color);
	virtual void Open();
	virtual void Close();

	std::string GetName() const;
	float GetHeight() const override;
	float GetWidth() const override;
	float GetIndividualHeight() const;
	float GetIndividualWidth() const;
	sf::Color GetTextColor() const;
	sf::Color GetColor() const;
	bool IsOpen() const;
	unsigned int Size() const;
	const Menu* GetSubMenu(std::string item);

protected:
	std::string name;
	std::vector<Menu*> items;
	Menu* parent;

	bool open;
	unsigned int widest_item;

	const sf::Font* font;
	sf::Color color;
	sf::Color text_color;
	sf::Color highlight_color;
	const float TEXT_SCALE_FACTOR;

	float GetTextWidth(std::string str, unsigned int character_size) const;
	int GetItemIndex(std::string str) const;
	Menu* ParseParentMenu(std::string name, std::string& child_name);
	void copy(const Menu& ref);
};

template <typename T>
bool Menu::OnClick(std::string item, T* object, std::function<void(T*, std::string)> func) {
	return OnClick(item, std::bind(func, object, std::placeholders::_1));
}

template <typename U>
bool Menu::OnClick(std::string item, const U* object, std::function<void(const U*, std::string)> func) {
	return OnClick(item, std::bind(func, object, std::placeholders::_1));
}