#include "names.h"

#include "colored-cout.h"
#include "utils.h"

#include <cpr/cpr.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

std::string getNameFromAppID(unsigned long appID, std::filesystem::path& gamePath)
{
    std::string apiKey{};
    std::string url{"https://api.steampowered.com/IStoreService/GetAppList/v1/"};
    std::string appName{};

    std::ifstream readJsonFile{paths::jsonPath};
    auto jsonNames = nlohmann::json::parse(readJsonFile);
    apiKey = jsonNames["api"];
    std::string appIDstr{std::to_string(appID)};

    if (!jsonNames[appIDstr]["name"].empty() && !gamePath.string().empty())
    {
        std::cout << clr::yellow << appIDstr << " is already on json file as " << jsonNames[appIDstr]["name"] << '\n';
        return jsonNames[appIDstr]["name"];
    }

    if (appID == 0)
    {
        appName = "invalid";
        return appName;
    }

    auto response = cpr::Get(
        cpr::Url{url},
        cpr::Parameters{
            {"key", apiKey},
            {"last_appid", std::to_string(appID - 1)},
            {"max_results", "1"},
            {"include_software", "true"},
        }
    );

    if (response.status_code == 200)
    {
        auto jsonResponse = nlohmann::json::parse(response.text);

        if (!jsonResponse["response"]["apps"].empty())
        {
            if (jsonResponse["response"]["apps"][0]["appid"] == appID) // App exists on steam
            {
                appName = jsonResponse["response"]["apps"][0]["name"];
                std::string path{"/" + appIDstr + "/name"};
                writeToJson(path, appName);

                path = "/" + appIDstr + "/pfx";
                std::string pfx{gamePath.parent_path().parent_path()};
                writeToJson(path, pfx);
            }
            else // ID doesn't exist on steam, so it sends the next valid one, which we don't want
            {
                appName = "invalid";
            }
        }
        else // This ID can't correspond to any steam game
        {
            if (gamePath.string().empty())
            {
                appName = "This game is not on Steam";
                return appName;
            }
            appName = getNewName(appIDstr, gamePath);
            std::string path{"/" + appIDstr + "/name"};
            writeToJson(path, appName);

            path = "/" + appIDstr + "/pfx";
            std::string pfx{gamePath.parent_path().parent_path()};
            writeToJson(path, pfx);
        }
    }

    return appName;
}

std::string getNewName(std::string& gameName, std::filesystem::path& gamePath)
{
    std::string newGameName{};

    std::ifstream readJsonFile{paths::jsonPath};
    auto jsonNames = nlohmann::json::parse(readJsonFile);
    if (!jsonNames[gameName]["name"].empty())
    {
        std::cout << clr::yellow << gameName << " is already on json file as " << jsonNames[gameName]["name"] << '\n';
        return jsonNames[gameName]["name"];
    }

    std::cout << gameName << " is not on Steam, so you must specify a name for it\n";
    std::cout << "Folders that might help identify the game: \n";
    std::vector<std::string> folders{"Documents", "AppData/Local", "AppData/LocalLow", "AppData/Roaming", "Saved Games"};

    for (int i = 0; i < folders.size(); ++i)
    {
        printSeparatorWithText(folders[i]);
        printFolderContents(gamePath / "users" / "steamuser" / folders[i]);
    }

    std::cout << clr::white << "Game name? (If you couldn't identify it, type 'N', a file exporer will open): ";
    std::getline(std::cin, newGameName);

    if (newGameName == "N")
    {
        std::string command{};

        if (jsonNames["yazi"] == "true" && !jsonNames["yazi"].empty())
        {
            if (system("which yazi > /dev/null 2>&1") == 0)
            {
                command = "yazi " + gamePath.string() + "/users/steamuser";
            }
            else
            {
                std::cout << clr::red << "You wanted to use Yazi, but you haven't installed it. Another file explorer will be used as a fallback\n";
                command = "xdg-open " + gamePath.string() + "/users/steamuser";
            }
        }
        else
        {
            command = "xdg-open " + gamePath.string() + "/users/steamuser";
        }

        system(command.c_str());

        std::cout << clr::white << "Game name?: ";
        std::getline(std::cin, newGameName);

        return newGameName;
    }
    return newGameName;
}

std::string getAppIDFromName(const std::string& name)
{
    std::ifstream readJsonFile{paths::jsonPath};
    auto jsonNames = nlohmann::json::parse(readJsonFile);

    for (const auto& [key, value] : jsonNames.items())
    {
        if (value.contains("name") && value["name"] == name)
        {
            return key;
        }
    }
    return "";
}

std::string getNameFromAccountID(std::string ID32)
{
    std::string apiKey{};
    std::string url{"https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/"};
    std::string accountName{};

    std::string ID64{std::to_string(std::stoi(ID32) + 76561197960265728)};

    std::ifstream readJsonFile{paths::jsonPath};
    auto jsonNames = nlohmann::json::parse(readJsonFile);
    apiKey = jsonNames["api"];

    auto response = cpr::Get(
        cpr::Url{url},
        cpr::Parameters{
            {"key", apiKey},
            {"steamids", ID64},
        }
    );

    if (response.status_code == 200)
    {
        auto jsonResponse = nlohmann::json::parse(response.text);
        return jsonResponse["response"]["players"][0]["personaname"];
    }

    return ID32;
}

std::string selectProcessedPrefix()
{
    std::ifstream readJsonFile{paths::jsonPath};
    auto jsonNames = nlohmann::json::parse(readJsonFile);

    std::filesystem::path create{jsonNames["create"]};

    std::vector<std::filesystem::directory_entry> games;
    for (const auto& entry : std::filesystem::directory_iterator(create))
    {
        games.push_back(entry);
    }

    if (games.empty())
    {
        std::cout << clr::red << "Before using the manager run the program with './ProtonPrefixes run'";
        std::exit(0);
    }

    for (int i = 0; i < games.size(); ++i)
    {
        std::cout << clr::green << (i + 1) << clr::gray << ". " << games[i].path().filename().string() << '\n';
    }

    std::cout << "Select a number: ";
    int selection{};
    std::cin >> selection;

    std::string selectedGameName{};
    if (selection >= 1 && selection <= games.size())
    {
        selectedGameName = games[selection - 1].path().filename().string();
    }
    else
    {
        std::cout << clr::red << "Selection outside of range";
    }

    return selectedGameName;
}
