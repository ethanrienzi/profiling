#include <iostream>
#include <list>
#include <string>

using namespace std;

class Player
{

  private:
    list<string> sentence;

  public:

  Player()
  {
    // this->sentenceLength = sentenceLength;
  }

  void addWord(string word)
  {
    sentence.push_back(word);
  }

  string getSentence()
  {
    string result = "";
    int len = sentence.size();

    for (auto it = sentence.begin(); it != sentence.end(); ++it)
    {
      result += " " + *it;
    }

    return result;
  }
};
