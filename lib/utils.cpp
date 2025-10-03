#include <string>
#include <sstream>
#include <unordered_map>
#include <filesystem>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <cstring>
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


bool isQuotesBalanced(std::string &input)
{
  bool singleQuote = false;
  bool doubleQuote = false;

  for (int i = 0; i < (int)input.size(); i++)
  {

    if (input[i] == '\\' && i + 1 < input.size())

    {
      i++;
      continue;
    }

    if (input[i] == '\'' && !doubleQuote)
    {
      singleQuote = !singleQuote;
    }
    else if (input[i] == '"' && !singleQuote)
    {
      doubleQuote = !doubleQuote;
    }
  }
  return !singleQuote && !doubleQuote;
}

std::string takeInput(std::string &input)
{

  while (!isQuotesBalanced(input))
  {
    std::string str;
    std::cout << "> ";
    std::getline(std::cin, str);
    input += "\n" + str;
  }
  return input;
}


std::vector<std::string> tokenizeForCat(const std::string input)
{
  std::vector<std::string> tokens;
  std::string current;
  bool inSingle = false, inDouble = false;

  for (size_t i = 0; i < input.size(); i++)
  {
    char c = input[i];

    if (c == '\'' && !inDouble)
    { // toggle single quotes
      inSingle = !inSingle;
      continue;
    }
    if (c == '"' && !inSingle)
    { // toggle double quotes
      inDouble = !inDouble;
      continue;
    }

    if (std::isspace(c) && !inSingle && !inDouble)
    {
      if (!current.empty())
      {
        tokens.push_back(current);
        current.clear();
      }
    }
    else
    {
      current.push_back(c);
    }
  }

  if (!current.empty())
  {
    tokens.push_back(current);
  }

  return tokens;
}



std::vector<std::string> tokenize(const std::string input)
{
  std::vector<std::string> tokens;
  std::string current;
  bool inSingle = false, inDouble = false;

  for (size_t i = 0; i < input.size(); i++)
  {
    char c = input[i];

    if (c == '\'' && !inDouble)
    { // toggle single quotes
      inSingle = !inSingle;
      continue;
    }
    if (c == '"' && !inSingle)
    { // toggle double quotes
      inDouble = !inDouble;
      continue;
    }

    if (input[i] == '\\' && i + 1 < input.size() && !inSingle)
    {
      current.push_back(input[i + 1]);
      i++;
      continue;
    }

    if (std::isspace(c) && !inSingle && !inDouble)
    {
      if (!current.empty())
      {
        tokens.push_back(current);
        current.clear();
      }
    }
    else
    {
      current.push_back(c);
    }
  }

  if (!current.empty())
  {
    tokens.push_back(current);
  }

  return tokens;
}

