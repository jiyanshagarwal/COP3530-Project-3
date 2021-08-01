#pragma once
#include <SFML/Graphics.hpp>

template <typename T>
class ResourceManager {
public:
	ResourceManager(sf::RenderWindow& window, sf::Font& font, const T& resouce);

	sf::RenderWindow& window; 
	sf::Font& font;
	const T& resource;
};

template<typename T>
ResourceManager<T>::ResourceManager(sf::RenderWindow& window, sf::Font& font, const T& resource) : window(window), font(font), resource(resource) {}