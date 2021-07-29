#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Drawable.h"

class TextBox : public Drawable {
public:
	TextBox(float x, float y, float width, float height, sf::RenderWindow& window);
	void Tick() override;
	void Draw(sf::RenderTarget& target) override;

	void Event(sf::Event& event) override;

	void AddLetter(char letter);
	void AddLetters(const char* letters);
	void Backspace();
	void Delete();
	void Clear();

	void ShiftCaretRight();
	void ShiftCaretLeft();

	void SetText(std::string text);
	void SetFont(const sf::Font& font);		//Must be monospaced
	void SetTextColor(sf::Color color);
	void SetColor(sf::Color color);
	void SetBorderColor(sf::Color color);
	void SetBorderWidth(float num);
	void SetCharacterLimit(int num);		//If num < 0, there is no limit
	void SetCaretPosition(unsigned int num);

	void SetX(float num) override;
	void SetY(float num) override;
	void SetWidth(float num) override;
	void SetHeight(float num) override;

	std::string GetText() const;
	const sf::Font* GetFont() const;
	sf::Color GetTextColor() const;
	sf::Color GetColor() const;
	sf::Color GetBorderColor() const;
	float GetBorderWidth() const;
	int GetCharacterLimit() const;
	bool hasCharacterLimit() const;
	unsigned int GetCaretPosition() const;

private:
	std::string text;
	int character_limit;
	float character_width;
	unsigned int caret_position;
	unsigned int text_start_position;
	unsigned int character_display_limit;

	bool blink_caret;
	unsigned tick_count;
	int prev_cursor_pos;
	unsigned int selected_text_start, selected_text_end;

	sf::RenderWindow& window;
	sf::Cursor cursor;
	bool cursor_changed;

	sf::Text visible_text;
	sf::RectangleShape textbox;

	void KeyPressed(int key);
	void KeyReleased(int key);
	void TextEntered(int unicode);

	void calculateSelected();
	void deselectAll();
	int toDiscretePosition(float posX);
	void copyToClipboard() const;
	void cutToClipboard();
};