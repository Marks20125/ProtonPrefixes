#include "utils.h"

#include "colored-cout.h"
#include "names.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

void printSeparatorWithText(std::string text)
{
    std::cout << clr::cyan << "=====================[" << text << "]=====================" << '\n';
}

void printFolderContents(std::filesystem::path folderPath)
{
    if (!std::filesystem::exists(folderPath))
    {
        std::cout << clr::red << "Folder does not exist\n";
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        std::cout << clr::blue << entry.path().filename().string() << '\n';
    }
}

void writeToJson(const std::string& path, const std::string& value)
{
    nlohmann::json temp;

    std::ifstream readJsonFile{paths::jsonPath};
    if (readJsonFile.is_open())
    {
        readJsonFile >> temp;
        readJsonFile.close();
    }

    temp[nlohmann::json::json_pointer(path)] = value;
    std::ofstream writeJsonFile{paths::jsonPath};
    if (writeJsonFile.is_open())
    {
        writeJsonFile << temp.dump(4);
        writeJsonFile.close();
    }
}

void printHelp()
{
    std::cout << "Parameters available for use: \n";
    std::cout << '\n';
    std::cout << clr::green << "run: " << clr::white << "Reads existing proton prefixes and creates symlinks for them\n";
    std::cout << '\n';
    std::cout << clr::green << "setup: " << clr::white << "By itself, it manages initial configuration. If the program has already been configured, running it again won't do anything. If you want to change a certain setting, you can run 'setup api', 'setup create', 'setup paths' or 'setup yazi'\n";
    std::cout << '\n';
    std::cout << clr::green << "clean: " << clr::white << "Scans the symlink folder for any entry that points to a no longer existing prefix\n";
    std::cout << '\n';
    std::cout << clr::green << "identify: " << clr::white << "If followed by an ID, it prints the game name\n";
    std::cout << '\n';
    std::cout << clr::green << "manager: " << clr::white << "Perform various actions related to a prefix, such as deleting it, or remove everything except the saves\n";
}

void clearCin()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void continuePrompt(std::string message = "")
{

    std::cout << clr::yellow << message;
    clearCin();
    std::cin.get();
}

void clean(std::filesystem::path& create)
{
    std::cout << clr::yellow << "Searching for prefixes that no longer exist\n";

    bool hasBeenFound{false};

    for (const auto& entry : std::filesystem::directory_iterator(create))
    {
        if (!std::filesystem::exists(std::filesystem::read_symlink(entry)))
        {
            hasBeenFound = true;
            std::cout << clr::red << entry.path().filename().string() << " prefix doesn't exist anymore, deleting folder\n";
            std::filesystem::remove(entry.path());
        }
    }
    if (hasBeenFound == false)
    {
        std::cout << clr::green << "No folders were deleted\n";
    }
}

namespace paths
{
std::filesystem::path homeDir{std::getenv("HOME")};
std::filesystem::path jsonPath{homeDir / ".config" / "ProtonPrefixes.json"};
std::filesystem::path tempPath{homeDir / ".config" / "ProtonPrefixesTemp.txt"};

std::filesystem::path getVDFPath()
{
    std::filesystem::path userData{homeDir / ".steam" / "steam" / "userdata"};
    std::filesystem::path vdfPath;

    std::vector<std::filesystem::directory_entry> IDs;
    for (const auto& entry : std::filesystem::directory_iterator(userData))
    {
        if (entry.path().filename().string() != "0")
        {
            IDs.push_back(entry);
        }
    }

    if (IDs.size() == 1)
    {
        vdfPath = userData / IDs[0].path().filename().string() / "config" / "localconfig.vdf";
    }
    else
    {
        std::cout << '\n';
        for (int i = 0; i < IDs.size(); ++i)
        {
            std::cout << clr::green << (i + 1) << clr::gray << ". " << getNameFromAccountID(IDs[i].path().filename().string()) << '\n';
        }

        std::cout << "Select a user (Steam stores launch parameters on different folders for each user): ";
        int selection{};
        std::cin >> selection;

        std::string selectedID{};
        if (selection >= 1 && selection <= IDs.size())
        {
            selectedID = IDs[selection - 1].path().filename().string();
        }
        else
        {
            std::cout << clr::red << "Selection outside of range";
        }

        vdfPath = userData / selectedID / "config" / "localconfig.vdf";
    }

    return vdfPath;
}

} // namespace paths
