#include <fstream>
#include <sstream>
#include <map>
#include <cassert>

#include "iclicker.h"
#include "student.h"

extern std::vector<std::string> ICLICKER_QUESTION_NAMES;
extern float MAX_ICLICKER_TOTAL;

std::map<int,Date> LECTURE_DATE_CORRESPONDENCES;

Student* GetStudent(const std::vector<Student*> &students, const std::string& name);


Date dateFromFilename(const std::string& filename_with_directory) {
  
  std::string filename = filename_with_directory;
  while (true) {
    int pos = filename.find('/');
    if (pos == std::string::npos) break;
    filename = filename.substr(pos+1,filename.size()-pos-1);
  }

  assert (filename.size() == 14);
  assert (filename[4] == '_');
  assert (filename[7] == '_');
  assert (filename.substr(10,4) == ".csv");

  Date answer;

  answer.year  = atoi(filename.substr(0,4).c_str());
  answer.month = atoi(filename.substr(5,2).c_str());
  answer.day   = atoi(filename.substr(8,2).c_str());

  return answer;
}


std::string ReadQuoted(std::istream &istr) {
  char c;
  std::string answer;
  //bool success = true;
  if (  !(istr >> c) || c != '"') {
    //std::cout << success << " OOPS not quote '" << c << "'" << std::endl;
  }

  while (istr >> c) {
    if (c == '"') break;
    answer.push_back(c);
  }
  //std::cout << "FOUND " << answer <<std::endl;
  return answer;
}

std::map<std::string, std::string> GLOBAL_CLICKER_MAP;


void MatchClickerRemotes(std::vector<Student*> &students, const std::string &remotes_filename) {
  if (remotes_filename == "") return;
  std::cout << "READING CLICKER REMOTES FILE: " << remotes_filename << std::endl;

  std::ifstream istr(remotes_filename.c_str());
  if (!istr) return; 

  char c;
  while (1) {
    std::string remote = ReadQuoted(istr);
    bool success = true;
    istr >> c;
    if (!success || c != ',') {
      //std::cout << success << " OOPS-not comma '" << c << "'" << std::endl;
    }
    std::string username = ReadQuoted(istr);
    if (remote == "" || username == "") break;
    //std::cout << "tokens: " << remote << " " << username << std::endl;

    Student *s = GetStudent(students,username);
    if (s == NULL) {
      std::cout << "BAD USERNAME FOR CLICKER MATCHING " << username << std::endl;
      continue;
    }
    assert (s != NULL);
    s->setRemoteID(remote);

    if (GLOBAL_CLICKER_MAP.find(remote) != GLOBAL_CLICKER_MAP.end()) {
      std::cout << "ERROR!  already have this clicker assigned " << remote << " " << s->getUserName() << std::endl;
    }


    assert (GLOBAL_CLICKER_MAP.find(remote) == GLOBAL_CLICKER_MAP.end());
    GLOBAL_CLICKER_MAP[remote] = username;
  }
}


std::string getItem(const std::string &line, int which) {
  int comma_before = 0;
  for (int i = 0; i < which; i++) {
    comma_before = line.find(',',comma_before)+1;
    assert (comma_before != std::string::npos);
  }
  int comma_after = line.find(',',comma_before);
  return line.substr(comma_before,comma_after-comma_before);
}


void AddClickerScores(std::vector<Student*> &students, std::vector<std::vector<iClickerQuestion> > iclicker_questions) {

  for (int which_lecture = 0; which_lecture < iclicker_questions.size(); which_lecture++) {
    std::vector<iClickerQuestion>& lecture = iclicker_questions[which_lecture];
    for (int which_question = 0; which_question < lecture.size(); which_question++) {
      iClickerQuestion& question = lecture[which_question];

      std::stringstream ss;
      ss << which_lecture << "." << which_question+1;

      ICLICKER_QUESTION_NAMES.push_back(ss.str());
MAX_ICLICKER_TOTAL += 1.0;

      std::ifstream istr(question.getFilename());

      if (LECTURE_DATE_CORRESPONDENCES.find(which_lecture) ==
          LECTURE_DATE_CORRESPONDENCES.end()) {
        Date date = dateFromFilename(question.getFilename());
        LECTURE_DATE_CORRESPONDENCES[which_lecture] = date;
      }

      assert (istr);
      char line_helper[5000];
      // ignore first 5 lines
      for (int i = 0; i < 5; i++) {
        istr.getline(line_helper,5000);
      }

      while (istr.getline(line_helper,5000)) {
        std::string line = line_helper;
        std::string remoteid = getItem(line,0);
        std::string item = getItem(line,question.getColumn()-1);
        bool participate = (item != "");
        
        if (!participate) continue;
        //std::cout << "ITEM " << item << " " << item.size() << std::endl;
        assert (item.size() == 1);

        //bool correct = question.participationQuestion() || question.isCorrectAnswer(item[0]);

        //std::cout << "ITEM " << remoteid << " " << item << "  " << participate << " " << correct << std::endl;
        std::map<std::string,std::string>::iterator itr = GLOBAL_CLICKER_MAP.find(remoteid);
        if (itr == GLOBAL_CLICKER_MAP.end()) {
          //std::cout << "UNKNOWN CLICKER: " << remoteid << "  " << std::endl;
          std::cout << " " << remoteid;
          continue;
        }
        assert (itr != GLOBAL_CLICKER_MAP.end());
        std::string username = itr->second;
        Student *s = GetStudent(students,username);
        assert (s != NULL);

iclicker_answer_enum grade = ICLICKER_NOANSWER;
        if (question.participationQuestion()) 
          grade = ICLICKER_PARTICIPATED;
        else {
          if (question.isCorrectAnswer(item[0])) {
            grade = ICLICKER_CORRECT;
          } else {
            grade = ICLICKER_INCORRECT;
          }
        }

        s->addIClickerAnswer(ss.str(),item[0],grade);
        //s.seti

      }
      std::cout << std::endl;
    }
  }
}
