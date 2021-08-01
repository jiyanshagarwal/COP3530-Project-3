#include "ImageDownloader.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <WinInet.h>

bool ImageDownloader::DownloadImage(std::string app_name, std::string url, sf::Image& image) {
	/*
	* The following pages were helpful for this:
	* https://docs.microsoft.com/en-us/windows/win32/wininet/http-sessions
	* https://stackoverflow.com/questions/20726723/download-file-from-the-internet-httpsendrequest-fails-with-error-code
	*/

	std::string host_name;
	std::string filepath;
	const char* accept_types[] = { "image/jpeg", "image/jpg", "image/png", NULL }; //Must be NULL terminated array or HTTPOpenRequest will crash.

	//URL must be split into host and file section for proper HTTPS request.
	if (URLSplitter(url, host_name, filepath)) {
		//See https://docs.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetopena
		HINTERNET session_handle = InternetOpenA(
			app_name.c_str(),						//Name of the calling application
			INTERNET_OPEN_TYPE_PRECONFIG,			//Access type. INTERNET_OPEN_TYPE_PRECONFIG is the default option
			NULL,									//Name of proxy server. Only used when access type is INTERNET_OPEN_TYPE_PROXY.
			NULL,									//Proxy bypass: Names of hosts and IP addresses to not route through when using proxies. Only used when access type is INTERNET_OPEN_TYPE_PROXY.
			0										//Any flags
		);

		if (session_handle) {
			//See https://docs.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetconnecta
			HINTERNET connection_handle = InternetConnectA(
				session_handle,						//Handle to result from InternetOpen
				host_name.c_str(),					//Name of host.
				INTERNET_DEFAULT_HTTPS_PORT,		//TCP/IP port on server. Port 443 for HTTPS
				NULL,								//Username
				NULL,								//Password
				INTERNET_SERVICE_HTTP,				//Service Type (FTP/Gopher/HTTP)
				0,									//Any flags
				0									//Application context
			);

			if (connection_handle) {
				//See https://docs.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-httpopenrequesta
				HINTERNET http_open_handle = HttpOpenRequestA(
					connection_handle,				//Handle to result from InternetConnect
					"GET",							//HTTP verb to use
					filepath.c_str(),				//Name of resource to get
					"HTTP/1.1",						//HTTP version
					host_name.c_str(),				//Name of host from which filepath is requested
					accept_types,					//Media types accepted by client (this program)
					INTERNET_FLAG_RELOAD |			//Any Flags. Reload forces download of requested file from server, not cache.
					INTERNET_FLAG_SECURE,			//Secure uses SSL so that HTTPS is used not HTTP.
					0								//Application context
				);

				if (http_open_handle) {
					//See https://docs.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-httpsendrequesta
					if (HttpSendRequestA(http_open_handle, NULL, 0, NULL, 0)) {
						DWORD dwSize = 0;	//Stores the size of the requested resource.

						while (true) {
							if (!InternetQueryDataAvailable(http_open_handle, &dwSize, 0, 0)) { break; }
							else {

							}
						}
					}
					
					InternetCloseHandle(http_open_handle);
				}

				InternetCloseHandle(connection_handle);
			}

			InternetCloseHandle(session_handle);
		}
	}

	return false;
}

bool ImageDownloader::URLSplitter(std::string url, std::string& host, std::string& filepath) {

}