#include "manager.h"

#include "colored-cout.h"
#include "names.h"
#include "utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vdf_parser.hpp>

void manager()
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

    std::string appID = getAppIDFromName(selectedGameName);
    std::ifstream readVDF{paths::getVDFPath()};
    auto root = tyti::vdf::read(readVDF);

    auto paramPath = root.childs["Software"]->childs["Valve"]->childs["Steam"]->childs["apps"]->childs[appID];

    std::string launchParameters;
    std::string newLaunchParameters;
    if (paramPath->attribs.count("LaunchOptions"))
    {
        launchParameters = paramPath->attribs["LaunchOptions"];
    }
    else
    {
        launchParameters = "";
    }

    std::ofstream writeTempFile{paths::tempPath};
    writeTempFile << launchParameters;
    writeTempFile.close();

    std::string command{"xdg-open " + paths::tempPath.string()};
    system(command.c_str());

    continuePrompt("Press enter to continue. Make sure steam is closed as it will overwrite the parameters otherwise");

    std::ifstream readTempFile(paths::tempPath);
    std::getline(readTempFile, newLaunchParameters);

    // TODO: Write newLaunchParameters to localconfig.vdf
}
