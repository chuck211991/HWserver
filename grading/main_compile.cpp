#include <unistd.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include <set>
#include <fstream>
#include <dirent.h>

#include "TestCase.h"


#include "execute.h"

#include "default_config.h"

#define DIR_PATH_MAX 1000


// =====================================================================
// =====================================================================

void LoadDisallowedWords(const std::string &filename,
			 std::set<std::string> &disallowed_words,
			 std::set<std::string> &warning_words) {
  std::ifstream istr(filename.c_str());
  if (!istr) {
    system("pwd");
    system("whoami");
    system("ls -lta");
    std::cout << "file is " << filename << std::endl;
  }
  assert (istr);
  std::string token1, token2;
  while (istr >> token1 >> token2) {
    if (token1 == "disallow") {
      assert (warning_words.find(token2) == warning_words.end());
      disallowed_words.insert(token2);
    } else if (token1 == "warning") {
      assert (disallowed_words.find(token2) == disallowed_words.end());
      warning_words.insert(token2);
    } else {
      std::cout << "UNKNOWN TOKEN " << token1 << std::endl;
      exit(1);
    }
  }
}

void SearchForDisallowedWords(std::set<std::string> &disallowed_words,
			      std::set<std::string> &warning_words) {

  system ("ls -lta");
  system ("whoami");

  char buf[DIR_PATH_MAX];
  getcwd( buf, DIR_PATH_MAX );
  DIR* dir = opendir(buf);
  assert (dir != NULL);
  struct dirent *ent;

  bool disallowed = false;
  bool warning = false;

  while (1) {
    ent = readdir(dir);
    if (ent == NULL) break;
    if (ent->d_type != DT_REG) continue;
    std::string filename = ent->d_name;
    if (filename == "disallowed_words.txt") continue;
    if (filename == "my_compile.out") continue;

    for (std::set<std::string>::iterator itr = disallowed_words.begin(); itr != disallowed_words.end(); itr++) {
      int success = system ( (std::string("grep ")+(*itr)+" "+filename+" 1> /dev/null 2> /dev/null").c_str());
      if (success == 0) {
	std::cout << "DISALLOWED: " << filename << " contains '" << (*itr) << "'" << std::endl;
	disallowed = true;
      }
    }

    for (std::set<std::string>::iterator itr = warning_words.begin(); itr != warning_words.end(); itr++) {
      int success = system ( (std::string("grep ")+(*itr)+" "+filename+" 1> /dev/null 2> /dev/null").c_str());
      if (success == 0) {
	std::cout << "WARNING: " << filename << " contains '" << (*itr) << "'" << std::endl;
	warning = true;
      }
    }

  }
  closedir(dir);

  if (disallowed) {
    exit(1);
  }
}

// =====================================================================

int main(int argc, char *argv[]) {


  assert (assignment_limits.size() == 16);

  // Make sure arguments are entered correctly
  if (argc != 1) {
    // Pass in the current working directory to run the programs
    std::cout << "Incorrect # of arguments:" << argc << std::endl;
    std::cout << "Usage : " << std::endl << "     ./compile" << std::endl;
    return 2;
  }

  /*
  std::cout << "Scanning User Code..." << std::endl;

  std::set<std::string> disallowed_words;
  std::set<std::string> warning_words;
  LoadDisallowedWords("disallowed_words.txt",disallowed_words,warning_words);
  SearchForDisallowedWords(disallowed_words,warning_words);
  */

  std::cout << "Compiling User Code..." << std::endl;


  // Run each COMPILATION TEST
  for (unsigned int i = 0; i < testcases.size(); i++) {

    if (testcases[i].isCompilationTest()) {
      
      assert (testcases[i].numFileGraders() == 0);
      
      std::cout << "========================================================" << std::endl;
      std::cout << "TEST " << i+1 << " " << testcases[i].command() << " IS COMPILATION!" << std::endl;
      
      std::string cmd = testcases[i].command();
      assert (cmd != "");
      
      // run the command, capturing STDOUT & STDERR
      int exit_no = execute(cmd +
			    " 1>" + testcases[i].prefix() + "_STDOUT.txt" +
			    " 2>" + testcases[i].prefix() + "_STDERR.txt",
			    testcases[i].prefix() + "_execute_logfile.txt",
			    testcases[i].get_test_case_limits());
      
      std::cout<< "Exited with exit_no: "<<exit_no<<std::endl;
    }
  }
  std::cout << "========================================================" << std::endl;
  std::cout << "FINISHED ALL TESTS" << std::endl;

  return 0;
}

// ----------------------------------------------------------------

