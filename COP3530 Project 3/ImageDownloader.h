#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <tuple>
#include <atomic>
#include <thread>
#include "DataReader.h"

class ImageDownloader {
public:
	sf::Texture image_loading_failed_image;

	ImageDownloader(std::string app_name, std::vector<std::vector<std::string>>& vehicles);
	ImageDownloader(const ImageDownloader& ref) = delete;
	ImageDownloader& operator=(const ImageDownloader& ref) = delete;
	~ImageDownloader();

	bool GetImage(std::string image_url, sf::Texture& texture) const;
	static bool DownloadImage(std::string app_name, std::string url, sf::Texture& texture);

private:
	struct HandleWrapper;	//HandleWrapper is needed to pass the InternetOpenURL handle between functions without polluting the header namespace with Windows.h.

	std::unordered_map<std::string, int> url_to_image_map;
	std::vector<std::tuple<std::string, bool, std::vector<char>>> images;

	std::atomic<bool> end_thread;
	std::atomic<int> loaded_index;
	std::thread downloader_thread;

	std::string app_name;	

	void DownloadAllImages();
	static std::string GetHeaderAttribute(const HandleWrapper& handle, int header_attribute);
	static bool GetImage(const HandleWrapper& handle, sf::Texture& texture);

	bool DownloadImage(std::string url, std::vector<char>& image_data);
	bool GetImage(const HandleWrapper& handle, std::vector<char>& image_data);
};