// ok so here are the rules that the AI told me to do:
/*

Theme: High-risk gambling simulation. Goal: Turn $100 into $1000 without going bankrupt. Flow:

Player bets a specific amount.

Player chooses a game mode via Switch: Even/Odd, Exact Number (1-10), or High/Low.

Math Requirement: The payout multiplier should be calculated using math logic (higher risk = higher reward).

Logic: If the user guesses right, add money. If wrong, subtract.

Variable Constraint: Use typedef for the currency.

Loop: Use a for loop to simulate the "spinning" of the wheel (printing numbers to the screen) before showing the result.

*/
#include <iostream>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>


namespace gameStates{
    typedef bool gameState;
    gameState running = true;
    gameState validBet = true;
    enum hlModes {parlay, normal};
    enum hlModes currentMode = normal;
}

namespace playerStats{
    typedef float Currency;
    Currency money = 100.00;
    Currency betAmount = 0.00;
}

void moneyCheck(playerStats::Currency money, playerStats::Currency betAmount);
void evenOddGame(playerStats::Currency betAmount, int numberSpun, char guess);
void exactNumberGame(playerStats::Currency betAmount);
void highLowGame(playerStats::Currency betAmount);

void clearScreen();

int main (){
    using namespace gameStates;
    using namespace playerStats;

    char gamemodeSelection;
    srand(time(0)); // seed for random number generation
    do
    {
        std::cout << "select mode: \n" << "(1) even or odd \n" << "(2) exact number \n" << "(3) higher or lower \n";
        std::cin >> gamemodeSelection;

        switch (gamemodeSelection)
        {
        // even or odd
        case '1':
            std::cout << "you selected even or odd \n";
            std::cout << "wager: $";
            std::cin >> betAmount;
            std::cout << "you are betting $" << betAmount << "\n";

            moneyCheck(money, betAmount); // checks if the bet is valid, if not, warns the user

            if (validBet == false){
                break;
            } else {
                clearScreen();
                int numberSpun = rand() % 100 + 1; // random number between 1 and 100
                std::cout << "spinning the wheel...\n";
                for (int i = 0; i < 10; i++){
                    std::cout << "\r" << "-> even" << std::flush;
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    std::cout << "\r" << "-> odd " << std::flush;
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }
                std::cout << "\r" << "-> ?? " << "\n";
                std::cout << "\n please enter your guess (e for even, o for odd): ";
                
                char evenOddGuess;
                std::cin >> evenOddGuess;
                // game logic would go here
                evenOddGame(betAmount, numberSpun, evenOddGuess);
                std::this_thread::sleep_for(std::chrono::seconds(1));

            }
            break;

        // exact number
        case '2':
            
            std::cout << "wager: $";
            std::cin >> betAmount;
            std::cout << "you are betting $" << betAmount << "\n";

            moneyCheck(money, betAmount); // checks if the bet is valid, if not, warns the user
            exactNumberGame(betAmount);

            break;
        // higher or lower
        case '3':
            std::cout << "which mode do you wanna do?" << '\n';
            std::cout << "[P] parlay mode" << '\n';
            std::cout << "[N] normal mode" << '\n';

            char hlMode;
            std::cin >> hlMode;

            if (hlMode == 'P' || hlMode == 'p'){
                std::cout << "you selected parlay mode!" << '\n';
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                std::cout << "in parlay mode, each correct guess will up your winning multiplier by 0.25x!" << '\n';
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                std::cout << "however, one wrong guess and you lose it all!" << '\n';
                currentMode = parlay;

            } else {
                std::cout << "you selected normal mode!" << '\n';
            }
            
            std::cout << "wager: $";
            std::cin >> betAmount;
            std::cout << "you are betting $" << betAmount << "\n";

            moneyCheck(money, betAmount); // checks if the bet is valid, if not, warns the user
            highLowGame(betAmount);
            /*
            for (int i = 0; i < 10; i++){
                int spin = rand() % 100 + 1;
                std::cout << "\r" << "-> " << spin << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                spin = rand() % 100 + 1;
                std::cout << "\r" << "-> " << spin << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            */
            break;
        // invalid input
        default:
            std::cout << "invalid gamemode! please try again" << '\n';
        }

    } while (running);
    
    return 0;
}

void moneyCheck(playerStats::Currency money, playerStats::Currency betAmount){
    clearScreen();
    std::cout << "checking funds...\n";
    if (money > betAmount){
        std::cout << "bet accepted! you are betting $" << betAmount << "\n";
        gameStates::validBet = true;
        
    } else if (money == betAmount){
        std::cout << "bet accepted! you're going ALL IN: $" << betAmount << "\n";
        gameStates::validBet = true;
    } 
    else {
        std::cout << "insufficent funds! your current balance is: " << money << " (attempted to bet $" << betAmount - money << " above limit)\n";
        gameStates::validBet = false;
    }
}

void clearScreen(){
    // clears the console screen
    std::cout << "\033[2J\033[1;1H"; // ANSI escape codes to clear screen and move cursor to top-left
}

void evenOddGame(playerStats::Currency betAmount, int numberSpun, char guess){
    // FOR THE TIME BEING, it'll be 2:1 payout
    bool isEven;
    numberSpun % 2 == 0 ? isEven = true : isEven = false;
    if (isEven == true && guess == 'e') {
        std::cout << "you win! the number was " << numberSpun << "\n";
        playerStats::money += betAmount;
    } else if (isEven == false && guess == 'o') {
        std::cout << "you win! the number was " << numberSpun << "\n";
        playerStats::money += betAmount;
    } else {
        std::cout << "you lose! the number was " << numberSpun << "\n";
        playerStats::money -= betAmount;
    }
    std::cout << "current balance: $" << playerStats::money << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    clearScreen();
}

void exactNumberGame(playerStats::Currency betAmount){
    std::cout << "what range will you generate?\n" << "[E] 1-10 (10:1 payout)\n" << "[M] 1-50 (50:1 payout)\n" << "[H] 1-100 (100:1 payout)\n";
    char rangeSelection;
    std::cin >> rangeSelection;
    int maxRange;
    int payoutMultiplier;
    switch (rangeSelection){
        case 'E':
        case 'e': {
            maxRange = 10;
            payoutMultiplier = 10;
            int numberSpun = rand() % maxRange + 1;
            std::cout << "what's your guess? (1-" << maxRange << "): ";
            int playerGuess;
            std::cin >> playerGuess;
            if (playerGuess == numberSpun){
                std::cout << "you win! the number was " << numberSpun << "\n";
                playerStats::money += betAmount * payoutMultiplier;
            } else {
                std::cout << "you lose! the number was " << numberSpun << "\n";
                playerStats::money -= betAmount;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            std::cout << "current balance: $" << playerStats::money << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));

            break;
        }
        case 'M':
        case 'm': {
            maxRange = 50;
            payoutMultiplier = 50;
            int numberSpun = rand() % maxRange + 1;
            std::cout << "what's your guess? (1-" << maxRange << "): ";
            int playerGuess;
            std::cin >> playerGuess;
            if (playerGuess == numberSpun){
                std::cout << "you win! the number was " << numberSpun << "\n";
                playerStats::money += betAmount * payoutMultiplier;
            } else {
                std::cout << "you lose! the number was " << numberSpun << "\n";
                playerStats::money -= betAmount;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            std::cout << "current balance: $" << playerStats::money << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));

            break;
        }
        case 'H':
        case 'h': {
            maxRange = 100;
            payoutMultiplier = 100;
            int numberSpun = rand() % maxRange + 1;
            std::cout << "what's your guess? (1-" << maxRange << "): ";
            int playerGuess;
            std::cin >> playerGuess;
            if (playerGuess == numberSpun){
                std::cout << "you win! the number was " << numberSpun << "\n";
                playerStats::money += betAmount * payoutMultiplier;
            } else {
                std::cout << "you lose! the number was " << numberSpun << "\n";
                playerStats::money -= betAmount;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            std::cout << "current balance: $" << playerStats::money << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));

            break;
        }
        default: {
            std::cout << "invalid selection, defaulting to 1-10 range\n";
            maxRange = 10;
            payoutMultiplier = 10;
            int numberSpun = rand() % maxRange + 1;
            std::cout << "what's your guess? (1-" << maxRange << "): ";
            int playerGuess;
            std::cin >> playerGuess;
            if (playerGuess == numberSpun){
                std::cout << "you win! the number was " << numberSpun << "\n";
                playerStats::money += betAmount * payoutMultiplier;
            } else {
                std::cout << "you lose! the number was " << numberSpun << "\n";
                playerStats::money -= betAmount;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            std::cout << "current balance: $" << playerStats::money << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        clearScreen();
    }
}

void highLowGame(playerStats::Currency betAmount){
    // to be implemented
    using namespace gameStates;
    using namespace playerStats;
    // parlay mode 
    std::string progress[] {"[X]", "[X]", "[X]", "[X]", "[X]", "[X]", "[X]", "[X]", "[X]", "[X]"};
    int numbers[11];
    int currentMultiplierTimes100 = 100; // starts at 1.00x

    if (currentMode == parlay){
        for (int i = 0; i < 11; i++){
            numbers[i] = rand() % 10 + 1;
        }
        for (int round = 0; round < 10; round++){
            clearScreen();
            std::cout << "~| round " << round + 1 <<  " |~ ";
            std::cout << "current progress: ";
            for (std::string p : progress){
                std::cout << p << " ";
            }
            std::cout << "\n" << "current multiplier: " << currentMultiplierTimes100 / 100.0 << "x\n";
            std::cout << "cash out? (y/n): ";

            char cashOutChoice;
            std::cin >> cashOutChoice;

            if (cashOutChoice == 'y' || cashOutChoice == 'Y'){
                playerStats::Currency winnings = betAmount * (currentMultiplierTimes100 / 100.0);
                std::cout << "you cashed out with a multiplier of " << currentMultiplierTimes100 / 100.0 << "x!\n";
                if (round == 0) {
                    std::cout << "you made it through 0 rounds, so you get your bet back only.\n";
                    return;
                } else {
                    std::cout << "you won $" << winnings << "!\n";
                    playerStats::money += winnings;
                    return; 
                }
            }
            std::cout << "will the next number be higher, lower, or the same as " << numbers[round] << "? (h/l/=): ";
            char hlGuess;
            std::cin >> hlGuess;

            int nextNumber = numbers[round + 1];

            if ((hlGuess == 'h' || hlGuess == 'H') && nextNumber > numbers[round]){ // when the next number is higher

                std::cout << "you guessed correctly! the next number was " << nextNumber << "\n";
                currentMultiplierTimes100 += 25; // increase multiplier by 0.25x
                progress[round] = "[O]";

            } else if ((hlGuess == 'l' || hlGuess == 'L') && nextNumber < numbers[round]){ // when the next number is lower

                std::cout << "you guessed correctly! the next number was " << nextNumber << "\n";
                currentMultiplierTimes100 += 25; // increase multiplier by 0.25x
                progress[round] = "[O]";

            } else if ((hlGuess == '=' || hlGuess == 'e') && nextNumber == numbers[round]){ // when the numbers are the same

                std::cout << "nice! the next number was the same! " << nextNumber << "\n";
                currentMultiplierTimes100 += 25; // increase multiplier by 0.25x
                progress[round] = "[O]";

            } else {
                std::cout << "you guessed wrong! the next number was " << nextNumber << "\n";
                std::cout << "you lose all your winnings from this parlay!\n";
                playerStats::money -= betAmount;
                return; // exit the function since the player lost
            }
            std::cout << "current balance: $" << playerStats::money << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    } else { // normal mode
        int numbers[2];
        numbers[0] = rand() % 10 + 1;
        numbers[1] = rand() % 10 + 1;
        clearScreen();
        std::cout << "the first number is: " << numbers[0] << "\n";
        std::cout << "will the next number be higher, lower, or the same? (h/l/=): ";
        char hlGuess;  
        std::cin >> hlGuess;
        int nextNumber = numbers[1];
        if ((hlGuess == 'h' || hlGuess == 'H') && nextNumber > numbers[0]){ // when the next number is higher

            std::cout << "you guessed correctly! the next number was " << nextNumber << "\n";
            money += betAmount * 1.5; // 1.5x payout

        } else if ((hlGuess == 'l' || hlGuess == 'L') && nextNumber < numbers[0]){ // when the next number is lower

            std::cout << "you guessed correctly! the next number was " << nextNumber << "\n";
            money += betAmount * 1.5;

        } else if ((hlGuess == '=' || hlGuess == 'e') && nextNumber == numbers[0]){ // when the numbers are the same
            
            std::cout << "you guessed correctly! the next number was " << nextNumber << "\n";
            money += betAmount * 1.5;

        } else {
            std::cout << "you guessed wrong! the next number was " << nextNumber << "\n";
            std::cout << "you lose all your winnings from this parlay!\n";
            playerStats::money -= betAmount;
            return; // exit the function since the player lost
        }
        std::cout << "current balance: $" << playerStats::money << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    clearScreen();

}