#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ImageDownloader {
public:
	static bool DownloadImage(std::string app_name, std::string url, sf::Image& image);

private:
	static bool URLSplitter(std::string url, std::string& host, std::string& filepath);
};