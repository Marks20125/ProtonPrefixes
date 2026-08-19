#include "manager.h"

#include "names.h"

#include <string>

void manager()
{
    std::string selectedGameName{selectProcessedPrefix()};
    std::string selectedGameID{getAppIDFromName(selectedGameName)};
}
