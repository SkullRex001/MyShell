#include <string>
#include <sstream>
#include <unordered_map>

void handleTypeBuiltin(std::string &inputString , std::unordered_map<std::string , std::string> pathMap){

      std::string str = extractArgumentString(inputString);
      auto it = pathMap.find(str);
      if (commands.find(str) != commands.end())
      {
        std::cout << str << " is a shell builtin" << "\n";
      }
      else if (it != pathMap.end())
      {

        std::string res = it->second;
        std::cout << str << " is " << res << '\n';
      }
      else {
        std::cout << str << ": not found" << '\n';
      }
}
