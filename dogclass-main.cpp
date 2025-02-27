/** L2: game_of_war.cpp
FIXME: Add name here //Rue Haile
FIXME: username: hailer
Purpose: To learning aboutstacks,queues and vector uses in C++.


Acknowledgements:
Modified from original stub code written by Dr. Jan Pearce
Overloading the << Operator for Your Own Classes (https://learn.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=msvc-170&utm_source=chatgpt.com) // CHANGE THIS!!
*/
//randome number generator from stackoverflow  https://stackoverflow.com/questions/37039682/shuffle-deck-of-cards-using-stdrandom-device


#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <random>

using namespace std;

class War {
private:
    int myCurrent;
    int otherCurrent;
    vector<int> newdecks;
    stack<int> myPlayingPile;
    queue<int> myStoragePile;
    stack<int> otherPlayingPile;
    queue<int> otherStoragePile;
    queue<int> lootPile;
    stack<int> tempStack; // Temporary stack for moving cards

public:
    War() {
        myPlayingPile = stack<int>();
        myStoragePile = queue<int>();
        otherPlayingPile = stack<int>();
        otherStoragePile = queue<int>();
        lootPile = queue<int>();
        newdecks = vector<int>();
    }

    void create_dealingPile() {
        for (int count = 0; count < 5; count++) { // 5 suits
            for (int i = 0; i <= 9; i++) { // 10 cards per suit
                newdecks.push_back(i);
            }
        }
        shuffle_deck(); // shuffle the deck
    }

    void shuffle_deck() {
        random_device rd;
        mt19937  rando(rd());
        shuffle(newdecks.begin(), newdecks.end(), rando);
    }

    void deal() {
        for (int i = 0; i < 25; i++) { // Each player should get 25 cards
            if (!newdecks.empty()) {
                myPlayingPile.push(newdecks.back());
                newdecks.pop_back();
            }
            if (!newdecks.empty()) {  // Move inside the loop so both players get equal cards
                otherPlayingPile.push(newdecks.back());
                newdecks.pop_back();
            }
        }
    }


    bool make_move() {
        if (myPlayingPile.empty() && myStoragePile.empty()) {
            cout << "I am out of cards! You win!" << endl;
            return false;
        }
        if (otherPlayingPile.empty() && otherStoragePile.empty()) {
            cout << "You are out of cards! I win!" << endl;
            return false;
        }
        if (myPlayingPile.empty()) move_my_storage();
        if (otherPlayingPile.empty()) move_other_storage();

        myCurrent = remove_my_card();
        otherCurrent = remove_other_card();

        cout << "My card: " << display_card(myCurrent) << " Opponent's card: " << display_card(otherCurrent) << endl;

        compare_cards();
        return true;
    }


    int remove_my_card() {
        if (myPlayingPile.empty()) {
            move_my_storage();
            if (myPlayingPile.empty())
                return 0; // Return 0 to indicate no cards left
        }
        int card = myPlayingPile.top();
        myPlayingPile.pop();
        return card;
    }

    int remove_other_card() {
        if (otherPlayingPile.empty()) {
            move_other_storage();
            if (otherPlayingPile.empty())
                return 0; // Return 0 to indicate no cards left
        }
        int card = otherPlayingPile.top();
        otherPlayingPile.pop();
        return card;
    }

    int display_card(int card) {
        return card;
    }

    void compare_cards() {


        if (myCurrent > otherCurrent) {
            cout << "I win!!" << endl;
            myStoragePile.push(myCurrent);
            myStoragePile.push(otherCurrent);
            while (!lootPile.empty()) {
                myStoragePile.push(lootPile.front());
                lootPile.pop();
            }
        }
        else if (myCurrent < otherCurrent) {
            cout << "You win!" << endl;
            otherStoragePile.push(myCurrent);
            otherStoragePile.push(otherCurrent);
            while (!lootPile.empty()) {
                otherStoragePile.push(lootPile.front());
                lootPile.pop();
            }
        }
        else {
            cout << "It's a tie!" << endl;
            war_out();
        }
    }

    void war_out() {
        cout << "War!" << endl;

        if (myPlayingPile.empty()) move_my_storage();
        // Move from storage if playing pile is empty
        if (otherPlayingPile.empty()) move_other_storage();

        // Check again after moving storage cards
        if (myPlayingPile.size() < 2 || otherPlayingPile.size() < 2) {
            cout << "Not enough cards, Game Over!" << endl;
            return;
        }

        lootPile.push(myCurrent);
        lootPile.push(otherCurrent);

        // Add one hidden card from each player
        lootPile.push(remove_my_card());
        lootPile.push(remove_other_card());

        // Face-up war card
        myCurrent = remove_my_card();
        otherCurrent = remove_other_card();

        cout << "My card: " << display_card(myCurrent) << " Opponent's card: " << display_card(otherCurrent) << endl;

        compare_cards();
    }


    void move_my_storage() {
        if (myStoragePile.empty()) return; // No need to move if storage is empty



        //Move everything from queue (FIFO) to a temporary stack (LIFO)
        while (!myStoragePile.empty()) {//from queue to stack
            tempStack.push(myStoragePile.front()); // Oldest element goes on top
            myStoragePile.pop();
        }

        //Move everything from temp stack (LIFO) to the playing stack (LIFO)
        while (!tempStack.empty()) {
            myPlayingPile.push(tempStack.top()); // Now order is preserved
            tempStack.pop();
        }
    }


    void move_other_storage() {

        if (otherStoragePile.empty()) return; // No need to move if storage is empty

        //Move everything from queue (FIFO) to a temporary stack (LIFO)
        while (!otherStoragePile.empty()) {//from queue to stack
            tempStack.push(otherStoragePile.front()); // Oldest element goes on top
            otherStoragePile.pop();
        }

        //Move everything from temp stack (LIFO) to the playing stack (LIFO)
        while (!tempStack.empty()) {
            otherPlayingPile.push(tempStack.top()); // Now order is preserved
            tempStack.pop();
        }
    }

};

int main() {
    War game;
    game.create_dealingPile();
    game.deal();

    while (game.make_move()) {
        game.move_my_storage();
        game.move_other_storage();
    }

    return 0;
}