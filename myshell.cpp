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
#include <unordered_map>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <fcntl.h>
#include <cctype>
#include <readline/readline.h>
#include <readline/history.h>
namespace fs = std::filesystem;



// Generator function that returns one completion at a time
char* command_generator(const char* text, int state) {
    static int list_index, len;
    static const char* commands[] = {"echo", "exit", "export", "env", nullptr};

    if (state == 0) {
        list_index = 0;
        len = std::strlen(text);
    }

    while (commands[list_index]) {
        const char* cmd = commands[list_index++];
        if (std::strncmp(cmd, text, len) == 0) {
            return strdup(cmd); // readline expects malloc'd strings
        }
    }

    return nullptr; // No more matches
}

// Completion function that readline will call
char** my_completion(const char* text, int start, int end) {
    (void)start; // unused
    (void)end;   // unused

    // Only complete the first word (command position)
    if (start == 0) {
        return rl_completion_matches(text, command_generator);
    }

    // Otherwise, fallback to default filename completion
    return nullptr;
}


std::string trim(const std::string &str)
{
  size_t start = 0;
  while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])))
  {
    start++;
  }

  size_t end = str.size();
  while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
  {
    end--;
  }

  return str.substr(start, end - start);
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

void handleCat(std::string inputPath)
{
  //  std::string path;
  // std::vector<std::string> paths;
  // std::stringstream ss(inputPath);
  // while(ss >> path){
  //   paths.push_back(path);
  //}
  //
  std::vector<std::string> paths = tokenizeForCat(inputPath);
  int size = paths.size();
  if (size > 1)
  {

    for (int i = 1; i < size; i++)
    {
      std::ifstream file(paths[i]);
      if (!file)
      {
        std::cout << "Error in opening file" << std::endl;
        break;
      }
      std::string line;
      while (getline(file, line))
      {
        std::cout << line << std::endl;
      }
      file.close();
    }
    std::cout << std::endl;
  }
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

void parseEcho(std::string input)
{

  int quotes = std::count(input.begin(), input.end(), '\'');
  while (quotes % 2 != 0)
  {
    std::string str2;
    std::cout << "quote> ";
    std::getline(std::cin, str2);
    input += str2;
    quotes = std::count(input.begin(), input.end(), '\'');
  }

  std::vector<std::string> args;
  std::string current;
  bool inSingleQuote = false;

  for (size_t i = 0; i < input.size(); i++)
  {
    char c = input[i];

    if (c == '\'')
    {
      inSingleQuote = !inSingleQuote;
      continue;
    }

    if (std::isspace(c) && !inSingleQuote)
    {
      if (!current.empty())
      {
        args.push_back(current);
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
    args.push_back(current);
  }

  for (int i = 0; i < args.size(); i++)
  {
    std::cout << args[i];
    if (i != args.size() - 1)
      std::cout << " ";
  }

  std::cout << std::endl;
}

std::unordered_map<std::string, std::string> printPath(const std::string &allPath)
{
  std::unordered_map<std::string, std::string> pathMap;
  char *path_copy = strdup(allPath.c_str());
  char *path = strtok(path_copy, ":");
  while (path != NULL)
  {
    if (fs::exists(path) && fs::is_directory(path))
      for (auto &entry : std::filesystem::directory_iterator(path))
        if (access(entry.path().c_str(), X_OK) == 0)
          pathMap.insert({entry.path().filename(), entry.path()});
    path = strtok(NULL, ":");
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

int main()

{


  rl_attempted_completion_function = my_completion;

  std::srand(std::time(0));
  // Flush after every std::cout / std:cerr

  std::cout << std::unitbuf;

  std::cerr << std::unitbuf;

  std::string path = std::getenv("PATH");

  //  std::vector<std::string> allPath = extractPath(path);
  //  std::set<std::string> listOfAllExecutable = setOfExecutable(allPath);

  std::unordered_map<std::string, std::string> pathMap = printPath(path);
  std::set<std::string> commands;

  commands.insert({"echo", "exit", "type", "pwd", "cd", "cat"});

  // wite space may cause some unexpected behaviour

  while (true)
  {

   // std::cout << "$ ";


    //std::getline(std::cin, input);
    
    char * getInput = readline("$ ");

    std::string input(getInput);

    input = takeInput(input);

    std::vector<std::string> words = tokenizeForCat(input);
    int saved_stdout;
    auto it1 = std::find(words.begin(), words.end(), "1>");
    auto it2 = std::find(words.begin(), words.end(), ">");
    auto it3 = std::find(words.begin(), words.end(), "2>");
    auto it4 = std::find(words.begin(), words.end(), "1>>");
    auto it5 = std::find(words.begin(), words.end(), ">>");
    auto it6 = std::find(words.begin(), words.end(), "2>>");

    if (it1 != words.end() || it2 != words.end() || it3 != words.end() || it4 != words.end() || it5 != words.end() || it6 != words.end())
    {
      int index;
      int flags = O_WRONLY | O_CREAT;

      if (it1 != words.end())
      {
        index = it1 - words.begin();
        saved_stdout = dup(1);
        flags |= O_TRUNC;
      }
      else if (it2 != words.end())
      {

        index = it2 - words.begin();
        saved_stdout = dup(1);
        flags |= O_TRUNC;
      }
      else if (it3 != words.end())
      {
        index = it3 - words.begin();
        saved_stdout = dup(2);
        flags |= O_TRUNC;
      }
      else if (it4 != words.end())
      {
        index = it4 - words.begin();
        saved_stdout = dup(1);
        flags |= O_APPEND;
      }
      else if (it5 != words.end())
      {
        index = it5 - words.begin();
        saved_stdout = dup(1);
        flags |= O_APPEND;
      }
      else if (it6 != words.end())
      {
        index = it6 - words.begin();
        saved_stdout = dup(2);
        flags |= O_APPEND;
      }

      int pathIndex = index + 1;
      if (words.size() < pathIndex + 1)
      {
        std::cout << "File path not provided";
        continue;
      }
      int file = open(words[pathIndex].c_str(), flags, 0644);
      if (file < 0)
      {
        std::cout << "Error in opening file" << std::endl;
        continue;
      }

      if (it1 != words.end() || it2 != words.end() || it4 != words.end() || it5 != words.end())
        if (dup2(file, 1) < 0)
        {
          std::cout << "dup2 failed";
          continue;
        }

      if (it3 != words.end() || it6 != words.end())
        if (dup2(file, 2) < 0)
        {
          std::cout << "dup2 failed";
          continue;
        }

      // removing "> path" from string and array and input string

      int operatorPostion = input.find(words[index]);
      int pathPostion = input.find(words[pathIndex]);
      // std::cout << operatorPostion << " " << pathPostion <<std::endl;
      if (operatorPostion != std::string::npos && operatorPostion != std::string::npos)
      {
        input.erase(operatorPostion, words[index].length());
        int stringIndexAfterDeletingOperator;
        if (it1 != words.end() || it3 != words.end() || it5 != words.end())
          stringIndexAfterDeletingOperator = 2;
        else if (it4 != words.end() || it6 != words.end())
          stringIndexAfterDeletingOperator = 3;
        else if (it2 != words.end())
          stringIndexAfterDeletingOperator = 1;
        input.erase(pathPostion - stringIndexAfterDeletingOperator, words[pathIndex].length());
      }

      words[index] = "";
      words[pathIndex] = "";

      input = trim(input);

      close(file);
    }

    // std::stringstream ss(input);

    // std::string word;
    // std::vector<std::string> words;

    // while (ss >> word)

    //{

    // words.push_back(word);
    //}

    if (input == "exit 0")

      return 0;

    else if (words[0] == "type")

    {
      // std::string str = input.substr(5);
      std::string str = extractArgumentString(input);
      //    std::cout << str << std::endl;
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
    {
      std::string strText = input.substr(5);
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
    else if (words[0] == "cd")
    {
      std::string path = extractArgumentString(input);
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
    else if (words[0] == "cat")
    {
      // handleCat(input);
      std::system(input.c_str());
    }
    else if ((words.size() != 0) && (pathMap.find(words[0]) != pathMap.end()))
    {
      std::system(input.c_str());
    }
    else
      std::cout << input << ": not found" << std::endl;

      
    if (it1 != words.end() || it2 != words.end() || it4 != words.end() || it5 != words.end())
    {
      if (dup2(saved_stdout, 1) < 0)
      {
        std::cout << "Unable to restore stdout";
        return 1;
      }
      close(saved_stdout);
    }
    else if (it3 != words.end() || it6 != words.end())
    {
      if (dup2(saved_stdout, 2) < 0)
      {
        std::cout << "Unable to restore stdout";
        return 1;
      }
      close(saved_stdout);
    }

    // std::cout <<input <<std::endl;
    // for(auto it : words)
    // std::cout << it <<" " <<std::endl;
  }
}
