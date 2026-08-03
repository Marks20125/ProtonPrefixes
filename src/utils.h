#include <filesystem>
#include <string>

void printSeparatorWithText(std::string text);
void printFolderContents(std::filesystem::path folderPath);
void writeToJson(std::string& path, std::string& value);
void printHelp();

namespace paths
{
extern std::string homeDir;
extern std::filesystem::path jsonPath;
} // namespace paths
