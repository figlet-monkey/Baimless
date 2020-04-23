#include "GrenadeHelperAPI.h"
CGrenadeAPI cGrenade;

std::string CGrenadeAPI::parseString(std::string szBefore, std::string szAfter, std::string szSource)
{
	if (!szBefore.empty() && !szAfter.empty() && !szSource.empty() && (szSource.find(szBefore) != std::string::npos) && (szSource.find(szAfter) != std::string::npos))
	{
		std::string t = strstr(szSource.c_str(), szBefore.c_str());
		t.erase(0, szBefore.length());
		std::string::size_type loc = t.find(szAfter, 0);
		t = t.substr(0, loc);
		return t;
	}
	else
		return "";
}

std::string CGrenadeAPI::HttpGet(std::string hostaddr, std::string api)
{
	std::string request;
	std::string response;
	int resp_leng;
	char buffer[1024];
	struct sockaddr_in serveraddr;
	int sock;
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		printf("WSAStartup() failed\r\n");

	struct hostent* host;
	host = gethostbyname(hostaddr.c_str());

	int port = 80;
	request += "GET " + api + " HTTP/1.0\r\n";
	request += "Host: " + hostaddr + "\r\n";
	request += "User-Agent: GRENADE API [Build: 1.0.0] \r\n";
	request += "\r\n";

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("socket() failed\r\n");

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;

	serveraddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	serveraddr.sin_port = htons((unsigned short)port);

	struct in_addr** addr_list;

	addr_list = (struct in_addr**) host->h_addr_list;

	response = "";

	if (connect(sock, (struct sockaddr*) & serveraddr, sizeof(serveraddr)) < 0)
		printf("connect() failed\r\n");

	if (send(sock, request.c_str(), request.length(), 0) != request.length())
		printf("send() sent a different number of bytes than expected\r\n");

	resp_leng = 1024;

	while (resp_leng > 0)
	{

		resp_leng = recv(sock, (char*)&buffer, 1024, 0);

		if (resp_leng > 0)
			response += std::string(buffer).substr(0, resp_leng);
	}

	closesocket(sock);
	WSACleanup();
	return response;
}

bool CGrenadeAPI::bUpdateGrenadeInfo(std::string szMapName)
{

	this->GrenadeInfo.clear();

	std::string responce = this->HttpGet(this->szHost, this->szApi + szMapName);
	if (responce.empty())
		return false;

	if (!this->parseString("<error>", "</error>", responce).empty())
		return false;

	int i = 0;

	while (true)
	{
		if (responce.find("<id>" + std::to_string(i) + "</id>") == std::string::npos)
			break;

		std::string szParseId = this->parseString("<id>", "</id>", responce);
		std::string szParseName = this->parseString("<name>", "</name>", responce);
		std::string szParseDescription = this->parseString("<description>", "</description>", responce);
		std::string szParseWeapon = this->parseString("<grenade>", "</grenade>", responce);

		std::string szParseOriginx = this->parseString("<player_x>", "</player_x>", responce);
		std::string szParseOriginy = this->parseString("<player_y>", "</player_y>", responce);
		std::string szParseOriginz = this->parseString("<player_z>", "</player_z>", responce);

		std::string szParseViewx = this->parseString("<view_x>", "</view_x>", responce);
		std::string szParseViewy = this->parseString("<view_y>", "</view_y>", responce);

		if (szParseId.empty() || szParseName.empty() || szParseDescription.empty() || szParseWeapon.empty())
			break;

		GrenadeInfo_t info;

		info.id = atoi(szParseId.c_str());
		info.szName = szParseName;
		info.szDescription = szParseDescription;
		info.szWeapon = szParseWeapon;
		info.vecOrigin = Vector(atof(szParseOriginx.c_str()), atof(szParseOriginy.c_str()), atof(szParseOriginz.c_str()));
		info.vecViewangles = Vector(atof(szParseViewx.c_str()), atof(szParseViewy.c_str()), 0);
		info.vecOrigin.z -= 64.f;

		GrenadeInfo.push_back(info);

		size_t nPos = responce.find("</view_y>");
		responce.erase(0, nPos + 7);

		i++;

	}
	return true;
}

bool CGrenadeAPI::GetInfo(int iNum, GrenadeInfo_t* info)
{
	if (iNum < 0 || iNum > GrenadeInfo.size())
		return false;

	if (GrenadeInfo.at(iNum).id > 0 && !GrenadeInfo.at(iNum).szDescription.empty() && !GrenadeInfo.at(iNum).szName.empty() && !GrenadeInfo.at(iNum).szWeapon.empty())
	{
		*info = GrenadeInfo.at(iNum);
		return true;
	}
	else return false;
}



