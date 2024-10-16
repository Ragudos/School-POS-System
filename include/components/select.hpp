#ifndef SELECT
#define SELECT

#include <string>
#include <vector>

using namespace std;

struct SelectChoice {
    string name;
    string val;
};

class Select {
   public:
    const vector<SelectChoice> choices;

    Select(const vector<SelectChoice> choices)
        : choices(choices), currentChoice(0){};

    void next() { int currentChoice = (currentChoice + 1) % choices.size(); }
    void prev() {
        if (currentChoice == 0) {
            currentChoice = choices.size() - 1;
        } else {
            currentChoice -= 1;
        }
    }

   private:
    int currentChoice;
};

#endif
