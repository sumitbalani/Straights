#include "Game.h"
#include "Command.h"
#include "ComputerPlayer.h"
#include "HumanPlayer.h"
#include <string>
#include <cassert>
#include <algorithm>

//RM #define endl "|" << endl
using namespace std;

//helper fcn TO DO relocate this fcn?
ostream &operator<<(ostream &out, const vector<Card*> cardlist)
{
    if (cardlist.size() == 0) {
        cout << " ";
    }
    for (int i = 0; i < cardlist.size(); i++) {
        cout << " " << *cardlist[i];
    }
    out << endl;
    
    return out;
}


Game::Game() {
    deck = new Deck;
    
    string input;
    for (int i = 0; i < 4; i++) {
        cout << "Is player " << i+1 << " a human(h) or a computer(c)?" << endl << ">";
        cin >> input;
        assert( input=="c" || input=="C" || input=="h" || input=="H" );
        
        if (input=="h" || input=="H") {
            HumanPlayer* playerptr = new HumanPlayer;
            playersingame.push_back(playerptr);
        }
        else {
            ComputerPlayer* playerptr = new ComputerPlayer;
            playersingame.push_back(playerptr);
        }
    }
}

void Game::printDeck() const {
    deck->printDeck();
}

void Game::dealCards() {
    for (int i = 0; i < 4; i++) {
        int begin = i*13;
        for (int j = begin; j < begin+13 ; j++) {
            //playersingame[i]->cardsinhand.push_back(deck->dealDeck(j));
            playersingame[i]->addToHand(deck->dealDeck(j));
        }
    }
}
// simple implementation that traverses through list and prints in order specified in design rubric
void Game::printTable() const{
    cout << "Cards on the table: " << endl;
    
    for (int i = 0; i < SUIT_COUNT; i++) {
        switch (i) {
            case 0:
                cout << "Clubs: ";
                break;
            case 1:
                cout << "Diamonds: ";
                break;
            case 2:
                cout << "Hearts: ";
                break;
            case 3:
                cout << "Spades: ";
                break;
        }
        bool first = true;
        for (int j = 0; j < RANK_COUNT; j++ ) {
            for (int k =0; k < cardsontable.size(); k++) {
                if (cardsontable[k]->getRank() == j && cardsontable[k]->getSuit() == i){
                    int value = j+1;
                    if (first){
                        first = false;
                    }
                    else {
                        cout << " ";
                    }
                    if (value == 10) cout << "J";
                    else if (value == 11) cout << "Q";
                    else if (value == 12) cout << "K";
                    else if (value == 1) cout << "A";
                    else cout << value;
                }
            }
        }
        cout << endl;
    }
}

void Game::startGame() {
    bool gameQuit = false;
    bool haveWinner = false;
    int oldScore[4] = {0, 0, 0, 0};
    deck->resetDeck();
    while ( !gameQuit && !haveWinner ) {
        //start of round init
        
        deck->shuffle();
        dealCards();
        resetDiscards();
        
        //find 7S to decide who goes first and announce
        int startPlayer;
        for (int i = 0; i < playersingame.size(); i++ ) {
            if (match( new Card(SPADE, SEVEN), playersingame[i]->getHand() )) {
                startPlayer = i;
                break;
            }
        }
        cout << "A new round begins. It's player " << startPlayer+1 << "'s turn to play." << endl;
        cardsontable.clear(); // clear table
        for (int turn = 0; turn < 13; turn++) {
            int currentPlayer = startPlayer;
            for (int plays = 0 ; plays < 4; plays++ ) {
                //game logic of a players turn
                Player* curPlayer = playersingame[currentPlayer];
                vector<Card*> playerLegalPlays = getLegalPlaysofPlayer( curPlayer );
                
                bool discarded;
                Card* played = curPlayer->play( playerLegalPlays , this, discarded);
                if (played != NULL) {
                    if ( discarded ) {
                        cout << "Player " << currentPlayer+1 << " discards " << *played << "." << endl;
                    }
                    else{
                        cout << "Player " << currentPlayer+1 << " plays " << *played << "." << endl;
                    }
                }
                else {
                    //player ragequit, replace him with a computer
                    cout << "Player " << currentPlayer+1 << " ragequits. A computer will now take over." << endl;
                    playersingame[currentPlayer] = playerQuit(curPlayer->getScore(), curPlayer->getHand(), curPlayer->getDiscards() );
                    
                    //let computer take players turn
                    played = playersingame[currentPlayer]->play( playerLegalPlays , this, discarded);
                    cout << "Player " << currentPlayer+1 << (discarded ? " discards " : " plays ") << *played << "." << endl;
                }
                
                if(currentPlayer !=3){
                    currentPlayer++;
                }
                else{
                    currentPlayer = 0;
                }
            }
            
        }
        
        //print out player discards and score
        for (int i = 0; i < playersingame.size(); i++ ) {
            cout << "Player " << i+1 << "'s discards:" << playersingame[i]->getDiscards();
            int totalscore = 0;
            vector<int> playerscore = playersingame[i]->getScore();
            for (int j = 0; j < playerscore.size(); j++) {
                totalscore += playerscore[j];
            }
            cout << "Player " << i+1 << "'s score: " << oldScore[i] << " + " << totalscore-oldScore[i] << " = " << totalscore << endl;
            oldScore[i] = totalscore;
            if (oldScore[i] >= 80) {
                haveWinner = true;
            }
        }
    }
    
    //find lowest score
    int lowestscore = oldScore[0];
    if (haveWinner) {
        for(int i = 0; i<4; i++) {
            if (oldScore[i] < lowestscore) {
                lowestscore = oldScore[i];
            }
        }
    }
    
    //print all people with lowest score as winners
    for (int i = 0; i<playersingame.size(); i++) {
        if(oldScore[i]==lowestscore){
            cout<<"Player "<< i+1<<" wins!" << endl;
        }
    }
    
}

void Game::resetDiscards(){
    //resets every player's discards list
    for (int i = 0; i < playersingame.size(); i++) {
        playersingame[i]->resetDiscards();
    }
}

Player* Game::playerQuit(vector<int> score, vector<Card*> hand, vector<Card*> discards) {
    //construct computer player with ^ properties
    ComputerPlayer* playerptr = new ComputerPlayer(score, hand, discards);
    return playerptr;
}

void Game::printHand(Player* playerptr) const {
    //prints out hand
    vector<Card*> playerCards = playerptr->getHand();
    
    cout << "Your hand:";
    cout << playerCards;
}
// determines legal plays for player given possible plays and his hand
vector<Card*> Game::getLegalPlaysofPlayer(Player* playerptr) const {
    vector<Card*> playersHand = playerptr->getHand();
    vector<Card*> legalTablePlays = getLegalPlaysonTable();
    vector<Card*> legalPlayerPlays;
    
    Card* SevenSpade = new Card(SPADE, SEVEN);
    if (match( SevenSpade, playersHand)) {
        legalPlayerPlays.push_back(SevenSpade);
        return legalPlayerPlays;
    }
    
    for (int i = 0; i < playersHand.size() ; i++) {
        if ( match(playersHand[i], legalTablePlays)) {
            legalPlayerPlays.push_back(playersHand[i]);
        }
    }
    return legalPlayerPlays;
}
// populates vectors with possible plays
vector<Card*> Game::getLegalPlaysonTable() const {
    vector<Card*> possiblePlays;
    
    if (cardsontable.size() == 0) {
        possiblePlays.push_back( new Card(SPADE, SEVEN) );
        return possiblePlays;
    }
    //fills deck with 7's as possible plays
    possiblePlays.push_back( new Card(HEART, SEVEN) );
    possiblePlays.push_back( new Card(CLUB, SEVEN) );
    possiblePlays.push_back( new Card(DIAMOND, SEVEN) );
    
    
    //takes care of case of A,K not being adjacent ranks
    //loops through to find every card on table
    for(int j =0; j<cardsontable.size(); j++){
    Card* lastCard = cardsontable[j];
    if (lastCard->getRank() != 0 && lastCard->getRank() != 12 ) {
        possiblePlays.push_back(new Card(lastCard->getSuit(), Rank(lastCard->getRank()+1)) );
        possiblePlays.push_back(new Card(lastCard->getSuit(), Rank(lastCard->getRank()-1)) );
    }
    else if (lastCard->getRank() == 0) {
        possiblePlays.push_back(new Card(lastCard->getSuit(), Rank(lastCard->getRank()+1)) );
    }
    else {
        possiblePlays.push_back(new Card(lastCard->getSuit(), Rank(lastCard->getRank()-1)) );
    }
    }
    return possiblePlays;
}

void Game::addCardToTable(Card* card) {      //updates a card played on the table
    cardsontable.push_back(card);
}
