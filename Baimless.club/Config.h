#pragma once

#include "Singleton.h"
#include <iostream>
#include <vector>
#include <stdlib.h>  
#include <Shlobj.h>
#include "Utils/Color.h"

class ConfigFile;

class ConfigSys : public Singleton<ConfigSys>
{

public:

    void CreateConfigFolder(std::string path);
    bool FileExists(std::string file);

    void SaveConfig(const std::string path);
    void LoadConfig(const std::string path);
    void LoadColor(Color* col, std::string str);
    std::vector<std::string> GetAllConfigs();

    char my_documents_folder[_MAX_PATH];

private:

    std::vector<ConfigFile> GetAllConfigsInFolder(const std::string path, const std::string ext);

    template<typename T>
    void Load(T& value, std::string str);
    void LoadArray(float_t value[4], std::string str);
    void LoadArray(bool value[14], std::string str);

    template<typename T>
    void Save(T& value, std::string str);
    void SaveColor(Color col, std::string str);
    void SaveArray(float_t value[4], std::string str);
    void SaveArray(bool value[14], std::string str);
};

class ConfigFile
{

public:

    ConfigFile(const std::string name, const std::string path)
    {
        this->name = name;
        this->path = path;
    }

    std::string GetName() { return name; };
    std::string GetPath() { return path; };

private:

    std::string name, path;
};

