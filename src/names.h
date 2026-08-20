#include <filesystem>
#include <string>

std::string getNameFromAppID(unsigned long appID, std::filesystem::path& gamePath);
std::string getNewName(std::string& gameName, std::filesystem::path& gamePath);
std::string getAppIDFromName(const std::string& name);
std::string getNameFromAccountID(std::string ID32);
std::string selectProcessedPrefix();
