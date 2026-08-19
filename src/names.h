#include <filesystem>
#include <string>

std::string getNameFromAppID(unsigned long appID, std::filesystem::path& gamePath);
std::string getNewName(std::string& gameName, std::filesystem::path& gamePath);
std::string getAppIDFromName(std::string& name);
std::string getNameFromAccountID(std::string ID);
std::string selectProcessedPrefix();
