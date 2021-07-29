#pragma once
#include <SFML/Graphics.hpp>

class Drawable {
public:
	Drawable(float x, float y, float width, float height);
	virtual ~Drawable();

	virtual void Draw(sf::RenderTarget& target) = 0;

	virtual void Tick();
	virtual void SetX(float num);
	virtual void SetY(float num);
	virtual void SetWidth(float num);
	virtual void SetHeight(float num);
	virtual void SetFocus(bool val);

	virtual float GetX() const;
	virtual float GetY() const;
	virtual float GetWidth() const;
	virtual float GetHeight() const;
	virtual bool HasFocus() const;

	virtual void SetMousePosition(float x, float y);
	virtual void Event(sf::Event& event);

	virtual bool MouseInBounds(float x, float y) const;

protected:
	float x, y;
	float width, height;
	float mouse_x, mouse_y;
	bool mouse_held;
	bool has_focus;
};