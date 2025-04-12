#include <ctime>
#include <cstdlib>
#include <iostream>

// template for any game where players take 
// turns to make moves
// and there is a winner
class Game {
public:
    Game():playersCount_(0), movesCount_(0), playerWon_(noWinner){}

    // template method
    void playGame(const int playersCount = 0) {
        playersCount_ = playersCount;
        movesCount_ = 0;

        initializeGame();

        for(int i = 0; !endOfGame(); i = (i + 1) % playersCount_ ){
	        makeMove(i);
	        if (i == playersCount_ - 1) 
	            ++movesCount_; 
            }
        printWinner();
    }

    virtual ~Game(){}

protected:
    // primitive operations
    virtual void initializeGame() = 0;
    virtual void makeMove(int player) = 0;
    virtual void printWinner() = 0;
    virtual bool endOfGame() { return playerWon_ != noWinner;} // this is a hook
                   // returns true if winner is decided
    static const int noWinner = -1;

    int playersCount_;
    int movesCount_;
    int playerWon_;
};

// Monopoly - a concrete game implementing primitive 
// operations for the template method
class Monopoly: public Game {
public:  
    // implementing concrete methods
    void initializeGame(){
        playersCount_ = rand() % numPlayers_ + 1 ; // initialize players
    }

    void makeMove(int player) {
        if (movesCount_ > minMoves_){ 
	        const int chance = minMoves_ + rand() % (maxMoves_ - minMoves_);
	        if (chance < movesCount_) playerWon_= player;
        }
    }

    void printWinner(){
        std::cout << "Monopoly, player "<< playerWon_<< " won in "
	    << movesCount_<< " moves." << std::endl;
    }

private:
    static const int numPlayers_ = 8; // max number of players
    static const int minMoves_ = 20; // nobody wins before minMoves_
    static const int maxMoves_ = 200; // somebody wins before maxMoves_
};

// Chess - another game implementing
// primitive operations
class Chess: public Game {
public:
    void initializeGame() {
        playersCount_ = numPlayers_; // initalize players
        for(int i=0; i < numPlayers_; ++i) {
	        experience_[i] = rand() % maxExperience_ + 1 ; 
        }
    }

    void makeMove(int player) {
        if (movesCount_ > minMoves_){
	        const int chance = (rand() % maxMoves_) / experience_[player];
	        if (chance < movesCount_) playerWon_= player;
        }
    }

    void printWinner(){
        std::cout << "Chess, player " << playerWon_ 
	    << " with experience " << experience_[playerWon_]
	    << " won in "<< movesCount_ << " moves over"
	    << " player with experience " << experience_[playerWon_== 0 ? 1:0] 
        << std::endl;
    }

private:
    static const int numPlayers_ = 2;
    static const int minMoves_ = 5; // nobody wins before minMoves_
    static const int maxMoves_ = 100; // somebody wins before maxMoves_
    static const int maxExperience_ = 5; // player's experience
                              // the higher, the greater probability of winning
    int experience_[numPlayers_]; 
};


class Dice: public Game {
public:
    void initializeGame() {
        playersCount_ = 2;
        computerScore = 0;
        playerScore = 0;
        round = 1;
        playerPass = false;
        computerPass = false;
    }

    void makeMove(int player) {
        if(round <= maxRounds && !(playerPass && computerPass)) { // round is round 3 and both did not pass
            if(player == 0) {
                std::cout << "Round " << round << ": ";
                std::cout << "Computer rolled: ";
                int roundScore = 0;
                if(!computerPass) { // computer did not pass
                    int computerValues[numberOfRolls];
                    int score = 0;

                    for(int i = 0; i < numberOfRolls; ++i) { // get the random rolls
                        computerValues[i] = rand() % 6 + 1;
                        score = score + computerValues[i];
                        std::cout << computerValues[i] << " ";
                    } 
                    computerRoundScore = score;
                    computerScore = computerScore + score;  
                    std::cout << ", computer's score = " << score << std::endl;

                    int computerDecision = rand() % 2; // 0 meaning do not pass and 1 meaning pass
                    if(computerDecision == 1) {
                        tempComputerPass = true;
                    }
                }
                else { // computer did pass
                    std::cout << "passed, computer's score = " << computerRoundScore << std::endl ;
                }
            }

            else { // player is the player
                std::cout << "You rolled: ";
                if(!playerPass) {
                    int playerValues[numberOfRolls];
                    int score = 0;
                    for(int i = 0; i < numberOfRolls; ++i) {
                        playerValues[i] = rand() % 6 + 1; // random roll number
                        score = score + playerValues[i];
                        std::cout << playerValues[i] << " ";
                    }
                    playerRoundScore = score;
                    playerScore = playerScore + score;
                    std::cout << ", your score = " << score << std::endl;

                    if(!playAnotherRound(round)) {
                        playerPass = true;
                    }
                }
                else { // player passes
                    std::cout << "passed, your score = " << playerRoundScore << std::endl;
                }
                
                ++round;
                if(tempComputerPass) { // set the computerPass for next round and after players turn
                    computerPass = true;
                }
            }
        }
        else if(round <= maxRounds && (computerPass && playerPass)) { // both passed
            std::cout << "Round " << round << ": ";
            std::cout << "Computer rolled: passed, computer's score = " << computerRoundScore << std::endl;
            std::cout << "You rolled: passed, your score = " << playerRoundScore << std::endl; 
            playerWon_ = 0;
        }
        else {
            playerWon_ = 0;
        }
    }


    void printWinner() {
        if(computerScore >= playerScore) {
            std::cout << "You lose!" << std::endl;
        }
        else {
            std::cout << "You win!" << std::endl;
        }
    }


private:
    static const int numberOfPlayers;
    static const int numberOfRolls = 5;
    static const int maxRounds = 3;
    int computerScore;
    int playerScore;
    int playerRoundScore;
    int computerRoundScore;
    int round;
    bool playerPass;
    bool computerPass;
    bool tempComputerPass;


    bool playAnotherRound(int round) {
        if(round < maxRounds) {
            char input;
            std::cout << "Roll again? [y/n] ";
            std::cin >> input;
            std::cout << std::endl;
            return input == 'y'; // return true if y false otherwise
        }
        else {
            return false;
        }
    }
};


int main() {
    srand(time(nullptr));

    Game* gp = nullptr;

    // play chess 7 times
    for (int i = 0; i < 7; ++i){ 
        gp = new Chess;
        gp -> playGame(); 
        delete gp;
    }
      

    // play monopoly 7 times
    for (int i = 0; i < 7; ++i) {
        gp = new Monopoly;
        gp -> playGame(); 
        delete gp;
    }
    
    for(int i = 0; i < 1; ++i) {
        gp = new Dice; 
        gp -> playGame();
        delete gp;
    }
}