#include "utils.h"

#include "colored-cout.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

void printSeparatorWithText(std::string text)
{
    std::cout << clr::cyan << "=====================[" << text << "]=====================" << '\n';
}

void printFolderContents(std::string folderPath)
{
    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        std::cout << clr::blue << entry.path().filename().string() << '\n';
    }
}

void writeToJson(std::string& path, std::string& value)
{
    nlohmann::json temp;

    std::ifstream readJsonFile{"games.json"};
    if (readJsonFile.is_open())
    {
        readJsonFile >> temp;
        readJsonFile.close();
    }

    temp[nlohmann::json::json_pointer(path)] = value;
    std::ofstream writeJsonFile{"games.json"};
    if (writeJsonFile.is_open())
    {
        writeJsonFile << temp.dump(4);
        writeJsonFile.close();
    }
}

void printHelp()
{
    std::cout << "Parameters available for use: \n";
    std::cout << clr::green << "run: " << clr::white << "Reads existing proton prefixes and creates symlinks for them\n";
    std::cout << '\n';
    std::cout << clr::green << "setup: " << clr::white << "By itself, it manages initial configuration. It the program has already been configured, running it again won't do anything. If you want to change a certain setting, you can run 'setup api', 'setup create' or 'setup paths'\n";
    std::cout << '\n';
    std::cout << clr::green << "clean: " << clr::white << "Scans the symlink folder for any entry that points to a no longer existing prefix";
}
