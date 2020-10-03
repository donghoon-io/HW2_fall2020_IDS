//
//  Solitaire.cpp
//  HW2_fall2020_IDS
//
//  Created by DONGHOON SHIN (elecle iOS dev) on 2020/09/30.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

class Card {
private:
    int number = -1; // assigned from A: 1 to K: 13
    int type = 0; // spade: 0; clova: 1; heart: 2, diamond: 3
    bool isHidden = true; // this indicates if the card is open by the user
public:
    // constructors
    Card() {};
    Card(const int number, const int type) {
        this->number = number;
        this->type = type;
    }
    Card(const int number, const int type, bool isHidden) {
        this->number = number;
        this->type = type;
        this->isHidden = isHidden;
    }
    // default destructor term
    ~Card() {};
    
    int getNumber() { return number; } // get card number
    int getType() { return type; } // get card type
    void revealCard() { isHidden = false; } // change 'isHidden' flag of card
    bool isRevealed() { return !isHidden; } // get if the card has been open
    bool isBlack() { return (type == 0 || type == 1); } // check if the card is black or red
};

class CardPile {
private:
    Card* arr; // array for assigning card
    int capacity; // capacity of the cardpile stack
    int top = -1; // indicates the top index of cardpile
public:
    // constructor
    CardPile(int capacity = 10) {
        this->arr = new Card [capacity];
        this->capacity = capacity;
    };
    // default destructor term
    ~CardPile() {};
    
    bool isEmpty() { return (top == -1); } // get if the pile is empty
    Card* getArray() { return arr; } // function of getting the array of card
    int getCardNum() { return top + 1; } // return the number (of cards)
    
    // push card
    void push(Card card) {
        if (capacity == top+1) {
            doubleSize1D();
            capacity = capacity*2;
        }
        arr[++top] = card;
    }
    
    // pop card
    void pop() {
        if (isEmpty()) throw "Stack is empty";
        arr[top--].~Card();
    }
    
    // return the top card
    Card getTopCard() {
        if (isEmpty()) throw "Stack is empty";
        return arr[top];
    }
    
    // doubling the capacity of stack
    void doubleSize1D() {
        if (capacity == 0) {
            arr = new Card [1];
            capacity = 1;
            return;
        }
        
        Card* temp = new Card[capacity*2];
        for (int i = 0; i < capacity; i++) {
            temp[i] = arr[i];
        }
        delete [] arr;
        arr = temp;
    }
    
    // check consecutive cards among revealed ones
    std::tuple<Card*, int> checkConsecutives() {
        Card* temp = new Card [13];
        int topIdx = top;
        int addIdx = 0;
        while (arr[topIdx].isRevealed() && topIdx > -1) {
            if (addIdx == 0) temp[addIdx++] = arr[topIdx--];
            else if (arr[topIdx+1].isBlack() != arr[topIdx].isBlack() && arr[topIdx].getNumber() - arr[topIdx+1].getNumber() == 1) temp[addIdx++] = arr[topIdx--];
            else break;
        }
        return std::make_tuple(temp, addIdx);
    }
    
    // reveal the top card
    void revealLast() {
        if (top != -1) {
            this->arr[top].revealCard();
        }
    }
    
    // remove {{ number }} cards and reveal the last card
    void removeCards(int number) {
        for (int i=0; i<number; i++) {
            this->pop();
        }
        this->revealLast();
    }
};

class Solitaire {
private:
    // below are defined stock, waste, plays, and outputs
    CardPile *stock = new CardPile(), *waste = new CardPile();
    CardPile *play1 = new CardPile(), *play2 = new CardPile(), *play3 = new CardPile(), *play4 = new CardPile(), *play5 = new CardPile(), *play6 = new CardPile(), *play7 = new CardPile();
    CardPile *plays[7] = {play1, play2, play3, play4, play5, play6, play7};
    CardPile *output1 = new CardPile(), *output2 = new CardPile(), *output3 = new CardPile(), *output4 = new CardPile();
    CardPile *outputs[4] = {output1, output2, output3, output4};
    
    int point = -52; // define initial point as -52
    bool isDone = false; // check if the game cannot proceed anymore
public:
    Solitaire() {}; // constructor
    ~Solitaire() {}; // destructor
    
    //function for generating an initial card sets
    Card* generate52Cards() {
        Card* arr = new Card [52];
        int idx = 0;
        for (int i=1; i<14; i++) {
            for (int j=0; j<4; j++) {
                arr[idx++] = Card(i, j);
            }
        }
        shuffle(arr, 52); // after defining the cards, shuffle them
        return arr;
    }
    
    // swapping function of two cards
    void swapCard(Card *card1, Card *card2) {
        Card temp = *card1;
        *card1 = *card2;
        *card2 = temp;
    }
    
    // shuffle card order using srand
    void shuffle(Card card[], int cardNum) {
        srand(time(NULL));
        
        for (int i = cardNum-1; i > 0; i--) {
            int j = rand() % (i + 1);
            swapCard(&card[i], &card[j]);
        }
    }
    
    // initialize the game
    void initialize() {
        Card *a;
        a = generate52Cards(); //generate cards
        
        //assign cards to each piles
        for (int i=0; i<52; i++) {
            if (i == 0 || i == 2 || i == 5 || i == 9 || i == 14 || i == 20 || i == 27 || i == 51) a[i].revealCard();
            if (i<1) play1->push(a[i]);
            else if (i<3) play2->push(a[i]);
            else if (i<6) play3->push(a[i]);
            else if (i<10) play4->push(a[i]);
            else if (i<15) play5->push(a[i]);
            else if (i<21) play6->push(a[i]);
            else if (i<28) play7->push(a[i]);
            else stock->push(a[i]);
        }
        delete [] a;
    }
    
    // check if the card(s) are able to move from one pile to another
    // isSingle indicates if the mobility of only one card should be checked
    std::tuple<bool, int> checkMobility(CardPile* from, CardPile* to, bool isSingle = false) {
        if (isSingle) {
            if (from->getCardNum() == 0) {
                return std::make_tuple(false, 0); // if origin is empty, return false
            }
            else if (to->getCardNum() == 0 && from->getTopCard().getNumber() == 13) {
                return std::make_tuple(true, 0);  // if destination is empty and available card is 13, return true
            }
            else if (to->getCardNum() == 0) {
                return std::make_tuple(false, 0); // else if destination is empty, return false
            }
            return std::make_tuple(from->getArray()[from->getCardNum()-1].getNumber() + 1 == to->getTopCard().getNumber() && from->getArray()[from->getCardNum()-1].isBlack() != to->getTopCard().isBlack(), 1); // else, check if a card can move
        } else {
            Card* possibles = std::get<0>(from->checkConsecutives()); // check the possible card sets
            int possibleNum = std::get<1>(from->checkConsecutives()); // check the possible number of cards
            
            if (from->getCardNum() == 0) {
                return std::make_tuple(false, 0); // if origin is empty, return false
            }
            else if (to->getCardNum() == 0 && possibles[possibleNum-1].getNumber() == 13) {
                if (from->getArray()[0].getNumber() == 13 && from->getArray()[0].isRevealed()) return std::make_tuple(false, 0); // if destination is empty and the head of available cards is 13, return true
                return std::make_tuple(true, possibleNum);
            }
            else if (to->getCardNum() == 0) {
                return std::make_tuple(false, 0); // else if destination is empty, return false
            }
            
            if (to->getTopCard().getNumber() == possibles[possibleNum-1].getNumber() + 1 && to->getTopCard().isBlack() != possibles[possibleNum-1].isBlack()) {
                return std::make_tuple(true, possibleNum); // else, check if cards can move
            }
            return std::make_tuple(false, 0);
        }
    }
    
    // check if the card(s) are able to move from one pile to output pile
    bool checkMobilityToOutput(CardPile* from) {
        if (from->getCardNum() == 0) return false; // if destination is empty, return false
        else {
            if (from->getTopCard().getNumber() == 1 && outputs[from->getTopCard().getType()]->getCardNum() == 0) {
                return true; // if the available card is A, return true
            }
            else if (outputs[from->getTopCard().getType()]->getCardNum() != 0) {
                if (outputs[from->getTopCard().getType()]->getTopCard().getNumber()+1 == from->getTopCard().getNumber()) {
                    return true; // else, check if the card can move to the output
                }
            }
        }
        return false;
    }
    
    // function to move the number {{ cardNum }} of card(s) to another pile
    void move(CardPile* from, CardPile* to, int cardNum) {
        for (int i = from->getCardNum()-cardNum; i < from->getCardNum(); i++) {
            to->push(from->getArray()[i]);
        }
        from->removeCards(cardNum);
    }
    
    // return the current point of game
    int getPoint() {
        return point;
    }
    
    // add 5 points
    void addPoint(int point = 5) {
        this->point += point;
    }
    
    // check if the game should be terminated
    bool shouldBeTerminated() {
        return isDone;
    }
    
    // a function to print all the piles
    void printPiles() {
        cout << "===========================" << endl << endl << "[Initial card sets]" << endl << endl;
        for (int i=0; i<7; i++) {
            cout << "Playing #" << i+1 << ": ";
            printPile(plays[i]);
        }
        for (int i=0; i<4; i++) {
            cout << "Output #" << i+1 << ": ";
            printPile(outputs[i]);
        }
        cout << "Stock: ";
        printPile(stock);
        cout << "Waste: ";
        printPile(waste);
        cout << endl << "===========================" << endl << endl << "[Game sequences]" << endl << endl;
    }
    
    // function for sequence (a)-1: Move a card to output pile from the top of the playing pile
    bool a1() {
        // for Playing 1 to 7, check if the card can move to output
        for (int i=0; i<7; i++) {
            if (checkMobilityToOutput(plays[i])) {
                int type = plays[i]->getTopCard().getType();
                
                printCard(plays[i]->getTopCard());
                cout << " from Playing #" << i+1 << " to Output #" << type+1 << endl;
                move(plays[i], outputs[type], 1);
                addPoint();
                return true;
            }
        }
        return false;
    }
    
    // function for sequence (a)-2: Move a card to output pile from the top of the waste pile
    bool a2() {
        // same as a1 except that this tackles waste case
        if (waste->isEmpty()) {
            return false; // if the waste pile is empty, return false
        }
        if (checkMobilityToOutput(waste)) {
            int type = waste->getTopCard().getType();
            
            printCard(waste->getTopCard());
            cout << " from Waste to Output #" << type+1 << endl;
            move(waste, outputs[type], 1);
            addPoint();
            return true;
        }
        return false;
    }
    
    // function for sequence (b): Move a card from the top of the waste pile to the leftmost playing pile available. If the waste pile is empty, draw a card from the stock pile.
    bool b() {
        // if waste is empty and stock is not, move a card from stock to waste
        if (waste->isEmpty() && !stock->isEmpty()) {
            printCard(stock->getTopCard());
            cout << " from Stock to Waste" << endl;
            move(stock, waste, 1);
        }
        // for playing 1 to 7, check if the card can move from stack to the playing pile
        for (int i=0; i<7; i++) {
            if (std::get<0>(checkMobility(waste, plays[i], true))) {
                
                printCard(waste->getTopCard());
                cout << " from Waste to Playing #" << i+1 << endl;
                move(waste, plays[i], 1);
                return true;
            }
        }
        return false;
    }
    // function for sequence (c): Find the leftmost playing pile that can be moved and place it on top of the leftmost playing pile available
    bool c() {
        for (int i=0; i<7; i++) {
            for (int j=0; j<7; j++) {
                if (std::get<0>(checkMobility(plays[i], plays[j], false)) && (i != j)) {
                    // below expresses the condition for avoiding infinite loop: the possible origin and destination should not have the same 'head' number and color -> this avoids such redundancy
                    if (!(plays[j]->getCardNum() != 0 &&
                          std::get<0>(plays[i]->checkConsecutives())[std::get<1>(plays[i]->checkConsecutives())-1].getNumber() == std::get<0>(plays[j]->checkConsecutives())[std::get<1>(plays[j]->checkConsecutives())-1].getNumber() &&
                          std::get<0>(plays[i]->checkConsecutives())[std::get<1>(plays[i]->checkConsecutives())-1].isBlack() == std::get<0>(plays[j]->checkConsecutives())[std::get<1>(plays[j]->checkConsecutives())-1].isBlack())) {
                        // print cards to move
                        for (int a = plays[i]->getCardNum()-1; a >= plays[i]->getCardNum()-std::get<1>(checkMobility(plays[i], plays[j], false)); a--) {
                            printCard(plays[i]->getArray()[a]);
                            cout << " ";
                        }
                        
                        cout << "from Playing #" << i+1 << " to Playing #" << j+1 << endl;
                        move(plays[i], plays[j], std::get<1>(checkMobility(plays[i], plays[j], false)));
                        
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    // running cycle from (a) to (c)
    // each step returns true if there are movement made by card, returns false if not
    void cycle() {
        if (a1() || a2() || b() || c()) {
        } else {
            // if stock is not empty, waste one card
            // else, terminate the game
            if (!stock->isEmpty()) {
                printCard(stock->getTopCard());
                cout << " from Stock to Waste" << endl;
                move(stock, waste, 1);
            } else {
                cout << endl << "===========================" << endl << endl << "[End of game]" << endl << endl << "Point: " << getPoint() << endl << endl << "===========================" << endl;
                isDone = true;
            }
        }
    }
    
    // print a card (e.g., ◇12)
    // R follows if the card is "R"evealed (in other word, open previously)
    void printCard(Card card) {
        if (card.getType() == 0) cout << "♠" << card.getNumber();
        else if (card.getType() == 1) cout << "♣" << card.getNumber();
        else if (card.getType() == 2) cout << "♡" << card.getNumber();
        else cout << "◇" << card.getNumber();
        if (card.isRevealed()) {
            cout << "R";
        } else {
            cout << "";
        }
    }
    
    // print a pile of cards
    void printPile(CardPile* pile) {
        for (int i = 0; i < pile->getCardNum(); i++) {
            printCard(pile->getArray()[i]);
            cout << " ";
        }
        cout << endl;
    }
};

int main(int argc, const char * argv[]) {
    
    Solitaire solitaire = Solitaire(); // initialize Solitaire class

    
    solitaire.initialize(); // call initialize() function in the class
    solitaire.printPiles(); // first, print the initial card sets
    
    while (!solitaire.shouldBeTerminated()) {
        solitaire.cycle(); // while the solitare should not be terminated so far, keep running the cycle
    }
    
    return 0;
}
