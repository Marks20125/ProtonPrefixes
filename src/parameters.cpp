#include "parameters.h"

#include "colored-cout.h"
#include "names.h"
#include "utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vdf_parser.hpp>
#include <vector>

std::vector<int> searchForNumbersInString(const std::string& str)
{
    std::vector<int> nums {};

    std::istringstream ss {str};
    int number;

    while (ss >> number)
    {
        nums.push_back(number);
    }

    return nums;
}

std::vector<std::string> searchForTextInString(const std::string& str)
{
    std::vector<std::string> texts {};

    std::istringstream ss {str};
    std::string text;

    while (ss >> text)
    {
        texts.push_back(text);
    }

    return texts;
}

void parameters()
{
    std::string selectedGameName{selectProcessedPrefix()};
    std::string appID = getAppIDFromName(selectedGameName);
    std::ifstream readVDF{paths::getVDFPath()};
    auto root = tyti::vdf::read(readVDF);

    auto paramPath = root.childs["Software"]->childs["Valve"]->childs["Steam"]->childs["apps"]->childs[appID];

    std::string launchParameters;
    std::string newLaunchParameters;
    if (paramPath->attribs.contains("LaunchOptions"))
    {
        launchParameters = paramPath->attribs["LaunchOptions"];
    }
    else
    {
        launchParameters = "";
    }

    std::vector<std::string> params {
        "DLL Overrides",
        "Share prefix",
        "PROTON_ENABLE_NVAPI=1 ",
        "mangohud "
    };
    std::cout << "\nAvailable launch parameters:\n";

    for (int i = 0; i < params.size(); i++)
    {
        std::cout << clr::green << i+1 << clr::gray <<". " << params[i] << "\n";
    }

    clearCin();
    std::cout << "Enter the ones you want separated by a space: ";
    std::string selections {};
    std::getline(std::cin, selections);

    std::vector<int> paramNums {searchForNumbersInString(selections)};

    for (int i = 0; i < paramNums.size(); i++)
    {
        std::string selectedParam {params[paramNums[i] - 1]};

        if (selectedParam == "DLL Overrides")
        {
            std::cout << "Enter the DLLs you want to override separated by a space: ";
            std::string dllsStr {};
            std::getline(std::cin, dllsStr);
            std::vector<std::string> dlls {searchForTextInString(dllsStr)};

            selectedParam = "WINEDLLOVERRIDES=\"";

            for (int j = 0; j < dlls.size(); j++)
            {
                if (j == dlls.size() - 1)
                {
                    selectedParam += dlls[j];
                }
                else
                {
                    selectedParam += dlls[j] + ",";
                }
            }

            selectedParam += "=n,b\" ";
        }
        else if (selectedParam == "Share prefix")
        {
            std::ifstream readJsonFile{paths::jsonPath};
            auto jsonData = nlohmann::json::parse(readJsonFile);

            std::cout << "\nWhere do you want " << selectedGameName << "'s prefix to be?\n";
            std::string pfxID {getAppIDFromName(selectProcessedPrefix())};
            std::filesystem::path pfxPath {jsonData[pfxID]["pfx"]};
            selectedParam = "STEAM_COMPAT_DATA_PATH=\"" + pfxPath.string() + "\" ";
        }

        newLaunchParameters += selectedParam;
    }

    newLaunchParameters += "%command%";

    continuePrompt("Press enter to continue. Make sure Steam is closed as it will overwrite the parameters otherwise");

    paramPath->attribs["LaunchOptions"] = newLaunchParameters;

    std::ofstream writeVDF{paths::getVDFPath()};
    tyti::vdf::write(writeVDF, root);
    std::cout << clr::green << "Parameters updated";
    writeVDF.close();
}
