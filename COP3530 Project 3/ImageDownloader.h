#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ImageDownloader {
public:
	static bool DownloadImage(std::string app_name, std::string url, sf::Image& image);
private:
	struct HandleWrapper;	//HandleWrapper is needed to pass the InternetOpenURL handle between functions without polluting the header namespace with Windows.h.

	static std::string GetHeaderAttribute(const HandleWrapper& handle, int header_attribute);
	static bool GetImage(const HandleWrapper& handle, sf::Image& image);
};