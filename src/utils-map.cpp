#include "../include/header.hpp"
static void insert_in_map(std::string line, _MAP &types_map);

_MAP get_map()
{

	// std::string tmp = "text/html                             html htm shtml;text/css                              css;text/xml                              xml rss;image/gif                             gif;image/jpeg                            jpeg jpg;application/x-javascript              js;text/plain                            txt;text/x-component                      htc;text/mathml                           mml;image/png                             png;image/x-icon                          ico;image/x-jng                           jng;image/vnd.wap.wbmp                    wbmp;application/java-archive              jar war ear;application/mac-binhex40              hqx;application/pdf                       pdf;application/x-cocoa                   cco;application/x-java-archive-diff       jardiff;application/x-java-jnlp-file          jnlp;application/x-makeself                run;application/x-perl                    pl pm;application/x-pilot                   prc pdb;application/x-rar-compressed          rar;application/x-redhat-package-manager  rpm;application/x-sea                     sea;application/x-shockwave-flash         swf;application/x-stuffit                 sit;application/x-tcl                     tcl tk;application/x-x509-ca-cert            der pem crt;application/x-xpinstall               xpi;application/zip                       zip;application/octet-stream              deb;application/octet-stream              bin exe dll;application/octet-stream              dmg;application/octet-stream              eot;application/octet-stream              iso img;application/octet-stream              msi msp msm;audio/mpeg                            mp3;audio/x-realaudio                     ra;video/mpeg                            mpeg mpg;video/quicktime                       mov;video/x-flv                           flv;video/x-msvideo                       avi;video/x-ms-wmv                        wmv;video/x-ms-asf                        asx asf;video/x-mng                           mng;";
	std::string mimeTypes[] = {"text/html                                        												html htm shtml",
					  "text/css                                         														css",
					  "text/xml                                         														xml",
					  "image/vnd.adobe.photoshop    																			psd",
					  "image/gif                                        														gif",
					  "image/jpeg                                       														jpeg jpg",
					  "application/javascript                           														js",
					  "application/atom+xml                             														atom",
					  "application/rss+xml                              														rss",
					  "text/mathml                                      														mml",
					  "text/plain                                       														txt",
					  "text/vnd.sun.j2me.app-descriptor                 														jad",
					  "text/vnd.wap.wml                                 														wml",
					  "text/x-component                                 														htc",
					  "image/avif                                       														avif",
					  "image/png                                        														png",
					  "image/svg+xml                                    														svg svgz",
					  "image/tiff                                       														tif tiff",
					  "image/vnd.wap.wbmp                               														wbmp",
					  "image/webp                                       														webp",
					  "image/x-icon                                     														ico",
					  "image/x-jng                                      														jng",
					  "image/x-ms-bmp                                   														bmp",
					  "font/woff                                        														woff",
					  "font/woff2                                       														woff2",
					  "application/java-archive                         														jar war ear",
					  "application/json                                 														json",
					  "application/mac-binhex40                         														hqx",
					  "application/msword                               														doc",
					  "application/pdf                                  														pdf",
					  "application/postscript                           														ps eps ai",
					  "application/rtf                                  														rtf",
					  "application/vnd.apple.mpegurl                    														m3u8",
					  "application/vnd.google-earth.kml+xml             														kml",
					  "application/vnd.google-earth.kmz                 														kmz",
					  "application/vnd.ms-excel                         														xls",
					  "application/vnd.ms-fontobject                    														eot",
					  "application/vnd.ms-powerpoint                    														ppt",
					  "application/vnd.oasis.opendocument.graphics      														odg",
					  "application/vnd.oasis.opendocument.presentation  														odp",
					  "application/vnd.oasis.opendocument.spreadsheet   														ods",
					  "application/vnd.oasis.opendocument.text          														odt",
					  "application/vnd.openxmlformats-officedocument.presentationml.presentation                                pptx",
					  "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet                                        xlsx",
					  "application/vnd.openxmlformats-officedocument.wordprocessingml.document                                  docx",
					  "application/vnd.wap.wmlc                         														wmlc",
					  "application/wasm                                 														wasm",
					  "application/x-7z-compressed                      														7z",
					  "application/x-cocoa                              														cco",
					  "application/x-java-archive-diff                  														jardiff",
					  "application/x-java-jnlp-file                     														jnlp",
					  "application/x-makeself                           														run",
					  "application/x-perl                               														pl pm",
					  "application/x-pilot                              														prc pdb",
					  "application/x-rar-compressed                     														rar",
					  "application/x-redhat-package-manager             														rpm",
					  "application/x-sea                                														sea",
					  "application/x-shockwave-flash                    														swf",
					  "application/x-stuffit                            														sit",
					  "application/x-tcl                                														tcl tk",
					  "application/x-x509-ca-cert                       														der pem crt",
					  "application/x-xpinstall                          														xpi",
					  "application/xhtml+xml                            														xhtml",
					  "application/xspf+xml                             														xspf",
					  "application/zip                                  														zip",
					  "application/octet-stream                         														bin exe dll",
					  "application/octet-stream                         														deb",
					  "application/octet-stream                         														dmg",
					  "application/octet-stream                         														iso img",
					  "application/octet-stream                         														msi msp msm",
					  "audio/midi                                       														mid midi kar",
					  "audio/mpeg                                       														mp3",
					  "audio/ogg                                        														ogg",
					  "audio/x-m4a                                      														m4a",
					  "audio/x-realaudio                                														ra",
					  "video/3gpp                                       														3gpp 3gp",
					  "video/mp2t                                       														ts",
					  "video/mp4                                        														mp4",
					  "video/mpeg                                       														mpeg mpg",
					  "video/quicktime                                  														mov",
					  "video/webm                                       														webm",
					  "video/x-flv                                      														flv",
					  "video/x-m4v                                      														m4v",
					  "video/x-mng                                      														mng",
					  "video/x-ms-asf                                   														asx asf",
					  "video/x-ms-wmv                                   														wmv",
					  "video/x-msvideo                                  														avi"};
	_MAP types_map;

	for (size_t i = 0; i < 87; i++)
	{
		insert_in_map(mimeTypes[i], types_map);
	}
	return types_map;
}

static void insert_in_map(std::string line, _MAP &types_map)
{

	char *line_tmp;
	char *token;

	line_tmp = strdup(line.c_str());
	token = strtok(line_tmp, " \t");
	if (!token)
	{
		return;
	}
	std::string tmp2(token);
	token = strtok(NULL, " \t");
	if (!token)
	{
		return;
	}
	while (token)
	{
		std::string tmp1(token);
		size_t pos = tmp1.find('/');
		if (pos == std::string::npos)
		{
			types_map.insert(std::pair<std::string, std::string>(tmp1, tmp2));
		}
		token = strtok(NULL, " \t");
	}
}

std::string	getContentTypeOfExt(_MAP& types_map, std::string &ext)
{
	_MAP::iterator it = types_map.find(ext);

	if (it != types_map.end())
		return it->second;
	return types_map.find("html")->second;
}
