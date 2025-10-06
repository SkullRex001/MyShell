#include <unordered_map>
#include <vector>
std::unordered_map<std::string, std::string> printPath(const std::string &allPath);
std::string extractArgumentString(std::string &inputString);
bool isQuotesBalanced(std::string &input);
std::string takeInput(std::string &input);
std::vector<std::string> tokenizeForCat(const std::string input);
std::vector<std::string> tokenize(const std::string input);
std::vector<char *> extractPipedInput(std::string input);
std::vector<char*> splitCommand(char * cmd);
std::string trim(const std::string &str);
