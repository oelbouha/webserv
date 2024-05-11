/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:33 by oelbouha          #+#    #+#             */
/*   Updated: 2024/05/11 14:42:44 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <algorithm>
#include <sys/errno.h>

#include "src/Server/Result.hpp"
#include "src/Response/BufferResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Server/ErrorPages.hpp"
#include "src/Route/Upload.hpp"
#include "src/CGI/CGIHandler.hpp"
#include "Utils.hpp"

class Upload;

struct DirLisingItem {
	std::string name;
	bool		isDir;

	bool	operator<(const DirLisingItem& o) const {
		if (isDir && !o.isDir)
			return true;
		if (!isDir && o.isDir)
			return false;
		return name < o.name;
	}
};

class Route {
	std::vector<string> 	allowedMethods;
	std::vector<string> 	CGIExtensions;
	std::vector<Config*> 	redirect;
	ErrorPages&				error_pages;
	bool					autoIndex;
	string					uri;
	string					location;
	string					root;
	string					indexFile;
	string  				uploadPath;
	unsigned int			code;
	unsigned int			max_body_size;

public:
	Route(Config * config, ErrorPages& pages);
	Route&	operator=( const Route& s );
	~Route();
	
	const string&				getURI() const;
	std::string 				getAbsolutePath(std::string requri);
	bool						isMethodAllowed(const std::string& method);

	Result  					handle(Request&);

private:
	IResponse* 					makeFileResponseFromPath(const Request&, const std::string& );
	IResponse*					handleRequestToFile(const Request&);
	Result						handleRequestToCgi(Request&);
	IResponse*					makeDirectoryListingResponse(const Request&, const std::string&);
	bool						isRequestToCgi(const std::string & );
	bool						isMethodImplemented(const std::string& );
	bool						isUpload(const Request&);
	std::string					getAllowedMethods() const;
	void						printMethods();
	std::vector<DirLisingItem> 	readDirectory(const std::string&);
};







#define DIR_LISTING_START "\
<!Doctype html>\
<html>\
	<head>\
		<title>Directory Listing</title>\
		<style>\
			body { \
				background: #282828;\
				color: white;\
				display: flex;\
				flex-direction: column;\
				justify-content: center;\
				align-items: center;\
			}\
			h1 {\
				font-size: 40px;\
			}\
			main {\
				display: grid;\
				grid-template-columns: 1fr 1fr 1fr 1fr 1fr;\
				width: 1600px;\
				max-width: 100%;\
				gap: 16px;\
			}\
			main a {\
				color: white;\
				text-decoration: none;\
				font-size: 24px;\
				padding: 12px;\
				background: #f5deb385;\
				text-align: center;\
				border-radius: 8px;\
			}\
			.item {\
				display: flex;\
				flex-direction: column;\
				padding: 12px;\
				padding-top: 20px;\
				border-radius: 8px;\
				align-items: center;\
				cursor: pointer;\
				background: none;\
				border: 2px solid;\
				transition: all 0.3s;\
			}\
			.dir {\
				color: gold;\
				border: 2px solid gold;\
			}\
			.item:hover {\
				background: #a2a2a2;\
				border: none;\
				box-shadow: 0px 0px 65px -25px black;\
				color: black;\
			}\
			.dir:hover {\
				background: gold;\
				color: #282828;\
			}\
			.item:hover svg {\
				fill: #000;\
			}\
			.dir:hover svg {\
				fill: #282828;\
			}\
			.item svg {\
				fill: #fff;\
				height: 64px;\
			}\
			.dir svg {\
				fill: gold;\
			}\
			.item span {\
				background: none;\
				margin-top: 8px;\
			}\
			@media (max-width: 1180px) {\
				main {\
					grid-template-columns: 1fr 1fr 1fr 1fr;\
				}\
			}\
			@media (max-width: 860px) {\
				main {\
					grid-template-columns: 1fr 1fr 1fr;\
				}\
			}\
		</style>\
	</head>\
	<body>\
		<h1>Directory Listing</h1>\
		<main>\
"
#define DIR_LISTING_END "\
		</main>\
	</body>\
</html>"

#define FILE_ITEM "\
<a class=\"item\" href=\"::path::\">\
	<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\" viewBox=\"0 0 256 256\" xml:space=\"preserve\">\
		<g transform=\"translate(1.4065934065934016 1.4065934065934016) scale(2.81 2.81)\">\
			<path d=\"M 63.089 1.483 H 15.755 c -1.551 0 -2.808 1.257 -2.808 2.808 v 81.418 c 0 1.551 1.257 2.808 2.808 2.808 h 58.49 c 1.551 0 2.808 -1.257 2.808 -2.808 V 14.927 \
			L 63.089 1.483 z M 65.896 14.927 c -1.551 0 -2.808 -1.257 -2.808 -2.808 V 1.483 l 13.964 13.444 H 65.896 z\" style=\"fill: none;\" transform=\" matrix(1 0 0 1 0 0) \" stroke-linecap=\"round\"></path>\
			<path d=\"M 75.569 83.678 v 2.031 c 0 0.729 -0.594 1.323 -1.324 1.323 h -58.49 c -0.73 0 -1.324 -0.594 -1.324 -1.323 V 4.291 c 0 -0.73 0.594 -1.324 1.324 -1.324 h 45.851 v 9.153 \
			c 0 2.366 1.925 4.291 4.29 4.291 h 9.673 V 83.678 c 1.021 0.094 1.601 1.553 2.967 -0.566 V 14.94 c 0.001 -0.084 -0.006 -0.166 -0.018 -0.248 c 0 0 -0.001 0 -0.001 -0.001 c 0 -0.001 0 \
			-0.001 0 -0.003 v 0 c 0 -0.001 -0.001 -0.002 -0.001 -0.003 c -0.007 -0.042 -0.015 -0.082 -0.026 -0.123 v 0 c -0.001 -0.001 0.002 -0.003 -0.001 -0.003 c 0 -0.002 0 -0.003 -0.001 -0.004 \
			v 0 c -0.066 -0.253 -0.199 -0.489 -0.392 -0.68 l -0.001 0 c 0 0 -0.001 -0.001 -0.001 -0.001 c -0.007 -0.001 -0.002 -0.001 -0.003 -0.002 c 0.003 -0.005 -0.002 -0.002 -0.002 -0.002 c -0.002 \
			-0.001 0.001 -0.001 -0.003 -0.002 c 0.001 -0.001 -0.002 -0.001 -0.002 -0.002 c -0.001 -0.001 -0.002 -0.001 -0.003 -0.002 L 64.118 0.415 c -0.001 -0.001 -0.002 -0.002 -0.003 -0.003 c -0.001 \
			-0.001 -0.002 -0.002 -0.003 -0.003 c -0.201 -0.19 -0.444 -0.317 -0.703 -0.374 c -0.038 -0.008 -0.075 -0.015 -0.113 -0.02 c -0.001 0 -0.001 0 -0.001 0 c -0.002 -0.001 -0.003 0 -0.005 -0.001 c \
			-0.002 0 -0.004 -0.001 -0.004 0 c -0.001 0 -0.003 0 -0.005 0 c -0.002 0 -0.003 -0.001 -0.005 0 c 0 0 -0.001 0 -0.005 -0.001 C 63.207 0.003 63.131 0.003 63.079 0 H 15.755 c -2.366 0 -4.291 1.925 \
			-4.291 4.291 V 85.71 c 0 2.365 1.925 4.29 4.291 4.29 h 58.49 c 2.366 0 4.291 -1.925 4.291 -4.29 v -2.598 C 77.58 83.39 76.59 83.584 75.569 83.678 z M 64.573 4.971 l 8.8 8.473 h -7.477 c -0.729 0 \
			-1.323 -0.594 -1.323 -1.324 V 4.971 z\"  transform=\" matrix(1 0 0 1 0 0) \" stroke-linecap=\"round\"></path>\
			<path d=\"M 21.924 66.401 c -0.82 0 -1.484 0.664 -1.484 1.484 s 0.664 1.484 1.484 1.484 h 22.668 c 0.819 0 1.484 -0.664 1.484 -1.484 s -0.664 -1.484 -1.484 -1.484 H 21.924 z\"  transform=\" matrix(1 0 0 1 0 0)\"\
			stroke-linecap=\"round\"></path>\
			<path d=\"M 21.924 39.698 h 47.294 c 0.819 0 1.484 -0.664 1.484 -1.484 s -0.664 -1.484 -1.484 -1.484 H 21.924 c -0.82 0 -1.484 0.664 -1.484 1.484 S 21.105 39.698 21.924 39.698 z\"  \
			transform=\" matrix(1 0 0 1 0 0)\" stroke-linecap=\"round\"></path>\
			<path d=\"M 70.702 57.995 c 0 -0.819 -0.664 -1.484 -1.484 -1.484 H 21.924 c -0.82 0 -1.484 0.664 -1.484 1.484 s 0.664 1.484 1.484 1.484 h 47.294 C 70.038 59.478 70.702 58.814 70.702 57.995 z\"\
			transform=\" matrix(1 0 0 1 0 0)\" stroke-linecap=\"round\"></path>\
			<path d=\"M 21.924 29.808 h 22.668 c 0.819 0 1.484 -0.664 1.484 -1.484 c 0 -0.82 -0.664 -1.484 -1.484 -1.484 H 21.924 c -0.82 0 -1.484 0.664 -1.484 1.484 C 20.441 29.144 21.105 29.808 21.924 29.808 z\"\
			transform=\" matrix(1 0 0 1 0 0) \" stroke-linecap=\"round\"></path>\
			<path d=\"M 21.924 49.588 h 47.294 c 0.819 0 1.484 -0.664 1.484 -1.484 c 0 -0.819 -0.664 -1.484 -1.484 -1.484 H 21.924 c -0.82 0 -1.484 0.664 -1.484 1.484 C 20.441 48.924 21.105 49.588 21.924 49.588 z\"\
			transform=\" matrix(1 0 0 1 0 0) \" stroke-linecap=\"round\"></path>\
		</g>\
	</svg>\
  	<span>::name::</span>\
</a>\
"

#define DIR_ITEM "\
<a class=\"item dir\" href=\"::path::\">\
	<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\" viewBox=\"0 0 256 256\" xml:space=\"preserve\">\
		<g transform=\"translate(1.4065934065934016 1.4065934065934016) scale(2.81 2.81)\">\
			<path d=\"M 89.235 40.459 c -0.729 -0.976 -1.847 -1.536 -3.065 -1.536 H 74.085 v -8.511 c 0 -2.856 -2.324 -5.181 -5.181 -5.181 h -4.874 V 9.175 c 0 -1.96 -1.594 -3.554 -3.554 -3.554 H 13.819 c -1.96 0 \
			-3.554 1.594 -3.554 3.554 v 6.312 H 5.18 c -2.856 0 -5.181 2.324 -5.181 5.181 v 49.46 v 7.593 v 2.517 c 0 2.284 1.858 4.143 4.142 4.143 h 70.042 c 0.011 0 0.021 0 0.033 0 c 2.389 0 4.245 -1.737 5.093 \
			-4.753 l 10.531 -35.793 C 90.186 42.665 89.965 41.435 89.235 40.459 z M 68.905 28.231 c 1.202 0 2.181 0.978 2.181 2.181 v 8.511 h -7.055 V 28.231 H 68.905 z M 13.819 8.621 h 46.658 c 0.306 0 0.554 0.249 \
			0.554 0.554 v 29.749 H 21.036 c -2.338 0 -4.42 1.57 -5.065 3.817 l -2.706 9.441 V 9.175 C 13.265 8.869 13.514 8.621 13.819 8.621 z M 4.142 81.379 c -0.63 0 -1.142 -0.513 -1.142 -1.143 V 77.72 v -7.593 v \
			-49.46 c 0 -1.203 0.978 -2.181 2.181 -2.181 h 5.085 v 44.161 L 5.341 79.827 C 4.9 81.379 4.388 81.379 4.142 81.379 z M 86.963 42.986 L 76.426 78.797 c -0.331 1.179 -0.983 2.582 -2.222 2.582 c -0.003 0 -0.007 \
			0 -0.011 0 H 7.985 c 0.086 -0.227 0.167 -0.469 0.241 -0.729 l 4.925 -17.184 h 0.114 v -0.397 l 5.59 -19.502 c 0.277 -0.968 1.174 -1.644 2.181 -1.644 h 65.133 c 0.361 0 0.57 0.208 0.663 0.332 S 87.066 42.639 \
			86.963 42.986 z\" transform=\" matrix(1 0 0 1 0 0) \" stroke-linecap=\"round\"/>\
			<path d=\"M 53.125 20.484 H 20.287 c -0.829 0 -1.5 -0.671 -1.5 -1.5 s 0.671 -1.5 1.5 -1.5 h 32.838 c 0.828 0 1.5 0.671 1.5 1.5 S 53.954 20.484 53.125 20.484 z\" transform=\"matrix(1 0 0 1 0 0)\" stroke-linecap=\"round\"/>\
			<path d=\"M 53.125 29.484 H 20.287 c -0.829 0 -1.5 -0.671 -1.5 -1.5 s 0.671 -1.5 1.5 -1.5 h 32.838 c 0.828 0 1.5 0.671 1.5 1.5 S 53.954 29.484 53.125 29.484 z\" transform=\"matrix(1 0 0 1 0 0)\" stroke-linecap=\"round\"/>\
		</g>\
	</svg>\
  	<span>::name::</span>\
</a>\
"

#endif
