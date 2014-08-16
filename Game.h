#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Player.h"
#include "Deck.h"
#include <vector>
#include <iostream>

class Player;

class Game {
public:
    Game();                         //constructor
    void addCardToTable(Card*);      //updates a card played on the table
    Player* playerQuit(std::vector<int>, std::vector<Card*>, std::vector<Card*>);  //updates game when player quits
    void dealCards();               //deals cards to players
    void printTable() const;        //prints cards on the table
    void printDeck() const;         //prints out deck as it appears
    void startGame();
    void printHand(Player*) const;
    std::vector<Card*> getLegalPlaysofPlayer(Player*) const; //returns legal plays of player
    std::vector<Card*> getLegalPlaysonTable() const;
    void resetDiscards();
private:
    std::vector<Player*> playersingame;
    std::vector<Card*> cardsontable;
    int turnspassed;
    Deck* deck;
};

std::ostream &operator<<(std::ostream &out, const std::vector<Card*> cardlist);
#endif // GAME_H_INCLUDED
