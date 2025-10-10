#include <unordered_map>
#include <set>
void handleTypeBuiltin(std::string &inputString, std::unordered_map<std::string, std::string> pathMap, std::set<std::string> commands);

void handleEchoBuiltin(std::string &inputString);

void handleChangeDirectoryBuiltin(std::string &inputString);


void handlePwdBuiltin();
