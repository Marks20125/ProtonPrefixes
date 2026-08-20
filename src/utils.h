#include <filesystem>
#include <string>

void printSeparatorWithText(std::string text);
void printFolderContents(const std::filesystem::path& folderPath);
void writeToJson(const std::string& path, const std::string& value);
void printHelp();
void clearCin();
void continuePrompt(const std::string& message);
void clean(const std::filesystem::path& create);

namespace paths
{
extern std::filesystem::path homeDir;
extern std::filesystem::path jsonPath;
extern std::filesystem::path vdfPath;
extern std::filesystem::path tempPath;

std::filesystem::path getVDFPath();
} // namespace paths
