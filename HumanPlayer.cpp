#include "HumanPlayer.h"
#include "Command.h"
#include <iostream>

using namespace std;

HumanPlayer::HumanPlayer() {
    
}

Card* HumanPlayer::play(std::vector<Card*> legalPlays, Game* game, bool& discarded) {
    game->printTable();
    game->printHand( this );
    
    cout << "Legal plays:";
    cout << legalPlays;
    
    //Card* played;
    bool ValidPlay = false;
    // loops until a valid play is entered
    while( !ValidPlay) {
        Command command;
        cout << ">";
        cin >> command;
        Card* targetCard = new Card(command.card.getSuit(), command.card.getRank());
        // checks for command type, executes respective operation 
        if (command.type == QUIT) {
            exit(0);
        }
        else if (command.type == DECK) {
            game->printDeck();
        }
        else if (command.type == PLAY) {
            ValidPlay = match(targetCard, legalPlays );
            if (ValidPlay) {
                game->addCardToTable(targetCard);
                removeFromHand(targetCard);
                discarded = false;
                return targetCard;
            }
            else {
                cout << "This is not a legal play." << endl;
            }
        }
        else if (command.type == DISCARD) {
            if (legalPlays.size() != 0) {
                cout << "You have a legal play. You may not discard." << endl;
            }
            else {
                ValidPlay = match(&command.card, cardsinhand_ );
                if (ValidPlay) {
                    discardFromHand(targetCard);
                    discarded = true;
                    return targetCard;
                }
                else {
                    exit(0);//cout << "You do not have that card" << endl;
                    //interaction not specified in in program requirements TO DO
                }
            }
        }
        else if (command.type == RAGEQUIT) {
            return NULL;
        }
    }
    return NULL;
}
