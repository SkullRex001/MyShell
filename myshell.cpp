#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <map>
#include <cstring>
namespace fs = std::filesystem;



std::map<std::string , std::string> printPath(const std::string &allPath){
  std::map<std::string , std::string> pathMap;
  char* path_copy = strdup(allPath.c_str());
  char* path = strtok(path_copy , ":");
  while (path!=NULL) {
    if(fs::exists(path) && fs::is_directory(path))
      for(auto &entry : std::filesystem::directory_iterator(path))
        if(access(entry.path().c_str(), X_OK) == 0)
          pathMap.insert({entry.path().filename() , entry.path()});
    path = strtok(NULL , ":");
  }
  free(path_copy);
  return pathMap;
}


std::vector<std::string> extractPath(std::string str)

{

  std::vector<std::string> allPath;

  int start = 0;

  for (int i = 0; i < str.length(); i++)

  {

    if (str[i] == ':')

    {

      int end = i;

      std::string path = str.substr(start, end - start);

      allPath.push_back(path);

      start = end + 1;
    }
  }

  allPath.push_back(str.substr(start));

  return allPath;
}

std::string execuitableExist(std::vector<std::string> &allPath, std::string exe)
{

  for (auto dir : allPath)
    if (fs::exists(dir) && fs::is_directory(dir))
      for (auto &entry : std::filesystem::directory_iterator(dir))
        if (access(entry.path().c_str(), X_OK) == 0)
          if (entry.path().filename() == exe)
            return entry.path();
  return "Not Found";
}

std::set<std::string> setOfExecutable(std::vector<std::string> allPath)
{
  std::set<std::string> allexecutable;
  for (auto dir : allPath)
    if (fs::exists(dir) && fs::is_directory(dir))
      for (auto &entry : std::filesystem::directory_iterator(dir))
        if (access(entry.path().c_str(), X_OK) == 0)
          allexecutable.insert(entry.path().filename());

  return allexecutable;
}

int main()

{

  std::srand(std::time(0));
  // Flush after every std::cout / std:cerr

  std::cout << std::unitbuf;

  std::cerr << std::unitbuf;

  std::string path = std::getenv("PATH");

//  std::vector<std::string> allPath = extractPath(path);
//  std::set<std::string> listOfAllExecutable = setOfExecutable(allPath);

  std::map<std::string , std::string> pathMap = printPath(path);
  std::set<std::string> commands;

  commands.insert({"echo", "exit", "type"});

  // wite space may cause some unexpected behaviour

  while (true)
  {

    std::cout << "$ ";

    std::string input;

    std::getline(std::cin, input);

    std::stringstream ss(input);

    std::string word;
    std::vector<std::string> words;

    while (ss >> word)

    {

      words.push_back(word);
    }

    if (input == "exit 0")

      return 0;

    else if (words[0] == "type")

    {

      std::string str = input.substr(5); //white space may cause bugs //will fix later
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
      else

        std::cout << str << ": not found" << '\n';
    }

    else if (words[0] == "echo")

      std::cout << input.substr(5) << "\n";

    else if ((words.size() != 0) && (pathMap.find(words[0]) != pathMap.end()))
    {
        std::system(input.c_str());
    }

    else

      std::cout << input << ": not found" << std::endl;
  }
}
