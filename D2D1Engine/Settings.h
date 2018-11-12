#pragma once

class Settings
{

public:
	bool isFullscreen;
	bool isResizable;
	size_t windowWidth;
	size_t windowHeight;
	std::string title;
private:
	size_t GetUInt(const std::string& section, const std::string& key);
	std::string GetString(const std::string& section, const std::string& key);
	bool GetBool(const std::string& section, const std::string& key);

	void SetUInt(const std::string& section, const std::string& key, int value);
	void SetString(const std::string& section, const std::string& key, const std::string& value);
	void SetBool(const std::string& section, const std::string& key, bool value);

	std::string GetFullPath();
public:
	Settings();
	~Settings();
	void Load();
	void Save();
};

