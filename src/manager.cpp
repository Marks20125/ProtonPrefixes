#include "manager.h"

#include "colored-cout.h"
#include "names.h"
#include "utils.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

void manager()
{
    std::string selectedGameName{selectProcessedPrefix()};
    std::string selectedGameID{getAppIDFromName(selectedGameName)};

    std::cout << clr::green << "0. " << clr::gray << "Delete prefix\n";
    std::cout << clr::green << "1. " << clr::gray << "Shrink prefix\n";

    std::cout << "Select an option: ";
    int option;
    std::cin >> option;

    std::ifstream readJsonFile{paths::jsonPath};
    auto jsonData = nlohmann::json::parse(readJsonFile);

    switch (option)
    {
    case 0:
    {
        std::cout << '\n';
        std::cout << clr::red << "Are you sure you want to delete the prefix? (yes/no): ";
        std::string response;
        std::cin >> response;
        if (response == "yes")
        {
            std::filesystem::path pfxPath{jsonData[selectedGameID]["pfx"]};
            std::filesystem::remove_all(pfxPath);

            std::filesystem::path create{jsonData["create"]};
            clean(create);
        }
        break;
    }
    case 1:
        //TODO: Write shrink prefix function
        break;
    }
}
