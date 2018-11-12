#include "pch.h"
#include "Settings.h"


size_t Settings::GetUInt(const std::string & section, const std::string & key)
{
	return GetPrivateProfileIntA(section.c_str(), key.c_str(), 0, GetFullPath().c_str());
}

std::string Settings::GetString(const std::string & section, const std::string & key)
{
	char buf[C_STR_BUFFER_SIZE];
	GetPrivateProfileStringA(section.c_str(), key.c_str(), "undefined", buf, C_STR_BUFFER_SIZE, GetFullPath().c_str());
	return buf;
}

bool Settings::GetBool(const std::string & section, const std::string & key)
{
	return GetString(section, key) == "true" ? true : false;
}

void Settings::SetUInt(const std::string & section, const std::string & key, int value)
{
	WritePrivateProfileStringA(section.c_str(), key.c_str(),std::to_string(value).c_str(), GetFullPath().c_str());
}

void Settings::SetString(const std::string & section, const std::string & key, const std::string & value)
{
	WritePrivateProfileStringA(section.c_str(), key.c_str(), value.c_str(), GetFullPath().c_str());
}

void Settings::SetBool(const std::string & section, const std::string & key, bool value)
{
	std::string str = value ? "true": "false";
	WritePrivateProfileStringA(section.c_str(), key.c_str(), str.c_str(), GetFullPath().c_str());
}

std::string Settings::GetFullPath()
{
	char buf[C_STR_BUFFER_SIZE];
	GetFullPathNameA("settings.ini", C_STR_BUFFER_SIZE, buf, nullptr);
	return buf;
}

Settings::Settings()
{
	Load();
}


Settings::~Settings()
{
}

void Settings::Load()
{
	isFullscreen = GetBool("window", "isFullscreen");
	isResizable = GetBool("window", "isResizable");
	/*windowWidth = GetUInt("window", "width");
	windowHeight = GetUInt("window", "height");*/
	windowWidth = 1280;
	windowHeight = 720;
	title = GetString("window", "title");
}
void Settings::Save()
{
	SetBool("window", "isFullscreen", isFullscreen);
	SetBool("window", "isResizable", isResizable);
	//SetUInt("window", "width", windowWidth);
	//SetUInt("window", "height", windowHeight);
	SetString("window", "title", title);
}