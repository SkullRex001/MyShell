#include <string>
#include <sys/wait.h>
#include <vector>
#include <unistd.h>
#include <iostream>

void handleRunningSingleExecutable(std::vector<std::string> words){
      std::vector<char *> argv;
      for (auto &t : words)
        if (t != "")
          argv.push_back(&t[0]);
      argv.push_back(nullptr);
      pid_t pid = fork();
      if (pid == 0)
      {
        execvp(argv[0], argv.data());
        std::cout << "There was an error \n";
      }
      else
      {
        wait(NULL);
      }
}
