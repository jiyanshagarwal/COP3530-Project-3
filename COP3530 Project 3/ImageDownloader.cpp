#include "ImageDownloader.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <Windows.h>
#include <WinInet.h>

struct ImageDownloader::HandleWrapper {
	HINTERNET& handle;
	HandleWrapper(HINTERNET& handle);
};

ImageDownloader::HandleWrapper::HandleWrapper(HINTERNET& handle) : handle(handle) {}

ImageDownloader::ImageDownloader(std::string app_name, std::vector<std::vector<std::string>>& vehicles) : app_name(app_name), loaded_index(0) {
	int index = 0;

	for (auto vehicle : vehicles) {
		std::string image_url = vehicle[DataReader::IMAGE_URL];

		url_to_image_map.emplace(image_url, index++);
		images.push_back(std::make_tuple(image_url, false, std::vector<char>()));
	}

	image_loading_failed_image.loadFromFile("res\\Image_Failed_To_Load.png");
	image_loading_failed_image.setSmooth(true);

	downloader_thread = std::thread(&ImageDownloader::DownloadAllImages, this);
}

ImageDownloader::~ImageDownloader() {
	end_thread = true;
	downloader_thread.join();
}

bool ImageDownloader::GetImage(std::string image_url, sf::Texture& texture) const {
	try {
		int index = url_to_image_map.at(image_url);

		//This index check is very important to prevent race conditions where the main thread reads data that the downloader_thread could be modifying.
		if (index < loaded_index) {
			bool image_loaded = std::get<1>(images[index]);

			if (image_loaded) {
				const std::vector<char>& image_data = std::get<2>(images[index]);
				texture.loadFromMemory(image_data.data(), image_data.size());
				return true;
			}
		}
	}
	catch (const std::out_of_range& e) {
		return false;
	}

	return false;
}

bool ImageDownloader::DownloadImage(std::string app_name, std::string url, sf::Texture& texture) {
	/*
	* The following pages were helpful for this:
	* https://docs.microsoft.com/en-us/windows/win32/wininet/http-sessions
	*/

	//See https://docs.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetopena
	HINTERNET session_handle = InternetOpenA(
		app_name.c_str(),						//Name of the calling application
		INTERNET_OPEN_TYPE_PRECONFIG,			//Access type. INTERNET_OPEN_TYPE_PRECONFIG is the default option
		NULL,									//Name of proxy server. Only used when access type is INTERNET_OPEN_TYPE_PROXY.
		NULL,									//Proxy bypass: Names of hosts and IP addresses to not route through when using proxies. Only used when access type is INTERNET_OPEN_TYPE_PROXY.
		0										//Any flags.
	);

	if (session_handle) {
		//See https://docs.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetopenurla
		HINTERNET internet_open_handle = InternetOpenUrlA(
			session_handle,						//Handle to result from InternetOpen.
			url.c_str(),						//URL to open.
			NULL,								//Any additional headers to send.
			0,									//Size of additional headers.
			INTERNET_FLAG_RELOAD |				//Any Flags. Reload forces download of requested file from server, not cache.
			INTERNET_FLAG_SECURE,				//Secure uses SSL so that HTTPS is used not HTTP.
			0									//Application context.
		);

		if (internet_open_handle) {
			std::string status_code = GetHeaderAttribute(HandleWrapper(internet_open_handle), HTTP_QUERY_STATUS_CODE);
			std::string content_type = GetHeaderAttribute(HandleWrapper(internet_open_handle), HTTP_QUERY_CONTENT_TYPE);
			
			if ((status_code == "200" || status_code == "304")
				&& (content_type == "image/jpeg" || content_type == "image/jpg" || content_type == "image/png")) {

				if (GetImage(HandleWrapper(internet_open_handle), texture)) {
					InternetCloseHandle(internet_open_handle);
					InternetCloseHandle(session_handle);
					return true;
				}
			}

			InternetCloseHandle(internet_open_handle);
		}

		InternetCloseHandle(session_handle);
	}

	return false;
}

void ImageDownloader::DownloadAllImages() {
	for (unsigned int i = 0; i < images.size(); i++) {
		if (end_thread) return;
		std::string image_url = std::get<0>(images[i]);
		std::get<1>(images[i]) = DownloadImage(image_url, std::get<2>(images[i]));
		loaded_index++;
	}
}

std::string ImageDownloader::GetHeaderAttribute(const HandleWrapper& handle, int header_attribute) {
	/*
	* Because of the stupid way that HttpQueryInfo works.You have to first call it so that it can put the buffer size in dwSize.
	* Then size your buffer correctly and re - call it. Thus, the goto statement is needed to run the code twice.
	*/
	std::vector<char> header_buffer;
	DWORD dwSize = 0;

retry:
	if (!HttpQueryInfoA(handle.handle, header_attribute, header_buffer.data(), &dwSize, NULL)) {
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			header_buffer.resize(dwSize);
			goto retry;
		}
		else {
			return "";
		}
	}

	if (header_buffer.back() == '\0') { header_buffer.pop_back(); }

	return std::string(header_buffer.data(), header_buffer.size());
}

bool ImageDownloader::GetImage(const HandleWrapper& handle, sf::Texture& texture) {
	std::vector<char> data;
	const DWORD dwBytesToRead = 5000;
	DWORD dwBytesRead = 0;

	do {
		const size_t old_size = data.size();
		data.resize(data.size() + dwBytesToRead);

		if (!InternetReadFile(handle.handle, &data[old_size], dwBytesToRead, &dwBytesRead)) {
			return false;
		}

		data.resize(old_size + dwBytesRead);
	} while (dwBytesRead != 0);

	texture.loadFromMemory(data.data(), data.size());
	return true;
}

bool ImageDownloader::DownloadImage(std::string url, std::vector<char>& image_data) {
	/*
	* The following pages were helpful for this:
	* https://docs.microsoft.com/en-us/windows/win32/wininet/http-sessions
	*/

	//See https://docs.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetopena
	HINTERNET session_handle = InternetOpenA(
		app_name.c_str(),						//Name of the calling application
		INTERNET_OPEN_TYPE_PRECONFIG,			//Access type. INTERNET_OPEN_TYPE_PRECONFIG is the default option
		NULL,									//Name of proxy server. Only used when access type is INTERNET_OPEN_TYPE_PROXY.
		NULL,									//Proxy bypass: Names of hosts and IP addresses to not route through when using proxies. Only used when access type is INTERNET_OPEN_TYPE_PROXY.
		0										//Any flags.
	);

	if (session_handle) {
		//See https://docs.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetopenurla
		HINTERNET internet_open_handle = InternetOpenUrlA(
			session_handle,						//Handle to result from InternetOpen.
			url.c_str(),						//URL to open.
			NULL,								//Any additional headers to send.
			0,									//Size of additional headers.
			INTERNET_FLAG_RELOAD |				//Any Flags. Reload forces download of requested file from server, not cache.
			INTERNET_FLAG_SECURE,				//Secure uses SSL so that HTTPS is used not HTTP.
			0									//Application context.
		);

		if (internet_open_handle) {
			std::string status_code = GetHeaderAttribute(HandleWrapper(internet_open_handle), HTTP_QUERY_STATUS_CODE);
			std::string content_type = GetHeaderAttribute(HandleWrapper(internet_open_handle), HTTP_QUERY_CONTENT_TYPE);

			if ((status_code == "200" || status_code == "304")
				&& (content_type == "image/jpeg" || content_type == "image/jpg" || content_type == "image/png")) {

				if (GetImage(HandleWrapper(internet_open_handle), image_data)) {
					InternetCloseHandle(internet_open_handle);
					InternetCloseHandle(session_handle);
					return true;
				}
			}

			InternetCloseHandle(internet_open_handle);
		}

		InternetCloseHandle(session_handle);
	}

	return false;
}

bool ImageDownloader::GetImage(const HandleWrapper& handle, std::vector<char>& image_data) {
	image_data.clear();
	const DWORD dwBytesToRead = 5000;
	DWORD dwBytesRead = 0;

	do {
		const size_t old_size = image_data.size();
		image_data.resize(image_data.size() + dwBytesToRead);

		if (!InternetReadFile(handle.handle, &image_data[old_size], dwBytesToRead, &dwBytesRead)) {
			image_data.clear();
			return false;
		}

		image_data.resize(old_size + dwBytesRead);
	} while (dwBytesRead != 0);

	return true;
}