#include <string>
#include <sys/wait.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include "utils.h"
#include "builtin.h"
#include <set>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <unordered_map>



std::set<std::string> builtInCommands = {"echo",  "type", "cd", "cat"};


/*

std::set <std::string> builtInCommands = { "one" , "two" , "three"};


//This will mimic the builtin commands
void sampleBuiltinOne(){
  std::cout << "sampleBuiltinOne" << '\n';
}


void sampleBuiltinTwo(){
  std::cout << "sampleBuiltinTwo" << '\n';
}


void sampleBuiltinThree(){
  std::cout << "sampleBuiltinThree" << '\n';
}

*/


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



void hadndleSinglePipe(std::string input , std::unordered_map<std::string , std::string> &pathMap , std::set<std::string> commands){
  std::vector<char *> pipedInput = extractPipedInput(input);
  if(pipedInput.size() !=2) {
    std::cout << "This function hadndleSinglePipe";
    return;
  }
  int fd[2];
  if(pipe(fd)==-1){
    std::cout << "Error in pipe";
    return;
  }
  int pid1 = fork();
  if(pid1==0){
    // first child
    close(fd[0]);
    dup2(fd[1] , STDOUT_FILENO);
    close(fd[1]);
    auto args1 = splitCommand(pipedInput[0]);
    auto it = builtInCommands.find(args1[0]);
    if(it == builtInCommands.end()){
    execvp(args1[0] , args1.data());
    std::cout << "Error\n";
    exit(1);
    }
    else{
      std::string inputAfterPipe(pipedInput[0]);
      if(strcmp(args1[0], "echo") == 0) handleEchoBuiltin(inputAfterPipe);
      else if(strcmp(args1[0], "type") == 0) handleTypeBuiltin(inputAfterPipe , pathMap , commands);
      else if(strcmp(args1[0], "cd") == 0) handleChangeDirectoryBuiltin(inputAfterPipe);
      exit(0);
    }
  }else{
    int pid2 = fork();
    if(pid2 == 0){
      //second child
      close(fd[1]);
      dup2(fd[0] , STDIN_FILENO);
      close(fd[0]);
      auto args2 = splitCommand(pipedInput[1]);
      auto it = builtInCommands.find(args2[0]);
      if(it == builtInCommands.end()){
      execvp(args2[0] , args2.data());
      std::cout << "Error\n";
      exit(1);
    }
    else{

      std::string inputAfterPipe(pipedInput[1]);
      if(strcmp(args2[0], "echo") == 0) handleEchoBuiltin(inputAfterPipe);
      else if(strcmp(args2[0], "type") == 0) handleTypeBuiltin(inputAfterPipe , pathMap , commands);
      else if(strcmp(args2[0], "cd") == 0) handleChangeDirectoryBuiltin(inputAfterPipe);
      exit(0);
    }
    }else{
      //parent
      close(fd[0]);
      close(fd[1]);
      while(wait(NULL)>0);
      for(auto p: pipedInput) free(p);

    }
      
  }  
}


