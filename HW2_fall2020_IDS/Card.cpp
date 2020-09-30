//
//  Card.cpp
//  HW2_fall2020_IDS
//
//  Created by HoonieMac on 2020/09/28.
//

#include <stdio.h>
using namespace std;

class Card {
private:
    int number = 0;
    int type = 0; //spade: 0; clova: 1; heart: 2, diamond: 3

public:
    Card() {};
    Card(const int number, const int type) {
        this->number = number;
        this->type = type;
    }
    ~Card() {};

    int getNumber() { return number; }
    int getType() { return type; }
};
