#include "parameters.h"

#include "names.h"
#include "utils.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vdf_parser.hpp>

void parameters()
{
    std::string selectedGameName{selectProcessedPrefix()};
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
