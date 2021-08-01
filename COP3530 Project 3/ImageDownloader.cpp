#include "ImageDownloader.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <WinInet.h>

bool ImageDownloader::DownloadImage(std::string app_name, std::string url, sf::Image& image) {
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
			std::vector<char> data;
			const DWORD dwBytesToRead = 5000;
			DWORD dwBytesRead = 0;

			do {
				const size_t old_size = data.size();
				data.resize(data.size() + dwBytesToRead);

				if (!InternetReadFile(internet_open_handle, &data[old_size], dwBytesToRead, &dwBytesRead)) { 
					InternetCloseHandle(internet_open_handle);
					InternetCloseHandle(session_handle);
					return false;
				}

				data.resize(old_size + dwBytesRead);
			} while (dwBytesRead != 0);

			image.loadFromMemory(data.data(), data.size());

			InternetCloseHandle(internet_open_handle);
			InternetCloseHandle(session_handle);
			return true;
		}

		InternetCloseHandle(session_handle);
	}

	return false;
}