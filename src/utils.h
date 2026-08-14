#include <filesystem>
#include <string>

void printSeparatorWithText(std::string text);
void printFolderContents(std::filesystem::path folderPath);
void writeToJson(std::string& path, std::string& value);
void printHelp();
void clearCin();
void continuePrompt(std::string message);

namespace paths
{
extern std::filesystem::path homeDir;
extern std::filesystem::path jsonPath;
extern std::filesystem::path vdfPath;
extern std::filesystem::path tempPath;

std::filesystem::path getVDFPath();
} // namespace paths
