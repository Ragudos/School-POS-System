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

    void next() { currentChoice = (currentChoice + 1) % choices.size(); }
    void prev() {
        if (currentChoice == 0) {
            currentChoice = choices.size() - 1;
        } else {
            currentChoice -= 1;
        }
    }
    void reset() { currentChoice = 0; }

    size_t getCurrentChoice() { return currentChoice; }

   private:
    size_t currentChoice;
};

#endif
