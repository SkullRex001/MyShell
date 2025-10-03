#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include "utils.h"
#include <iostream>
#include <set>

void handleTypeBuiltin(std::string &inputString , std::unordered_map<std::string , std::string> pathMap , std::set<std::string> commands){

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



void handleEchoBuiltin(std::string &inputString){
      
      std::string strText = inputString.substr(5);
//      std::string strText = extractArgumentString(inputString);
      std::string finalInput = takeInput(strText);
      std::vector<std::string> str = tokenize(finalInput);
      for (int i = 0; i < str.size(); i++)
      {
        std::cout << str[i];
        if (i != str.size() - 1)
          std::cout << " ";
      }
      std::cout << std::endl;
}


void handleChangeDirectoryBuiltin(std::string &inputString){

      std::string path = extractArgumentString(inputString);
      int pos = path.find("~");
      if (pos != std::string::npos)
      {
        std::string home(std::getenv("HOME"));
        path.replace(pos, 1, home);
      }
      if (path.empty())
        std::cout << "path is required" << '\n';
      else
      {
        int result = chdir(path.c_str());
        if (result != 0)
          std::cout << "cd: " << path << ": No such file or directory" << '\n';
      }


}
