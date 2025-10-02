#include <string>
#include <sstream>
#include <unordered_map>
#include <filesystem>
#include <unistd.h>


std::string extractArgumentString(std::string &inputString)
{
  std::stringstream ss(inputString);
  std::string cmd, arg;
  ss >> cmd;
  std::getline(ss, arg);
  size_t start = arg.find_first_not_of(" \t\n\r\f\v");
  if (start != std::string::npos)
  {
    arg.erase(0, start);
  }
  else
  {
    arg.clear();
  }
  return arg;
}


std::unordered_map<std::string, std::string> printPath(const std::string &allPath)
{
  std::unordered_map<std::string, std::string> pathMap;
  char *path_copy = strdup(allPath.c_str());
  char *path = strtok(path_copy, ":");
  while (path != NULL)
  {
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
      for (auto &entry : std::filesystem::directory_iterator(path))
        if (access(entry.path().c_str(), X_OK) == 0)
          pathMap.insert({entry.path().filename(), entry.path()});
    path = strtok(NULL, ":");
  }
  free(path_copy);
  return pathMap;
}


