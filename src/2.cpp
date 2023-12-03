// --- Day 2: Cube Conundrum ---
// You're launched high into the atmosphere! The apex of your trajectory just 
// barely reaches the surface of a large island floating in the sky. You 
// gently land in a fluffy pile of leaves. It's quite cold, but you don't see 
// much snow. An Elf runs over to greet you.

// The Elf explains that you've arrived at Snow Island and apologizes for the 
// lack of snow. He'll be happy to explain the situation, but it's a bit of a 
// walk, so you have some time. They don't get many visitors up here; would 
// you like to play a game in the meantime?

// As you walk, the Elf shows you a small bag and some cubes which are either 
// red, green, or blue. Each time you play this game, he will hide a secret 
// number of cubes of each color in the bag, and your goal is to figure out 
// information about the number of cubes.

// To get information, once a bag has been loaded with cubes, the Elf will 
// reach into the bag, grab a handful of random cubes, show them to you, and 
// then put them back in the bag. He'll do this a few times per game.

// You play several games and record the information from each game (your 
// puzzle input). Each game is listed with its ID number (like the 11 in 
// Game 11: ...) followed by a semicolon-separated list of subsets of cubes 
// that were revealed from the bag (like 3 red, 5 green, 4 blue).

// For example, the record of a few games might look like this:

// Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
// Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
// Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
// Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
// Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
// In game 1, three sets of cubes are revealed from the bag (and then put back 
// again). The first set is 3 blue cubes and 4 red cubes; the second set is 1 
// red cube, 2 green cubes, and 6 blue cubes; the third set is only 2 green cubes.

// The Elf would first like to know which games would have been possible if 
// the bag contained only 12 red cubes, 13 green cubes, and 14 blue cubes?

// In the example above, games 1, 2, and 5 would have been possible if the bag 
// had been loaded with that configuration. However, game 3 would have been 
// impossible because at one point the Elf showed you 20 red cubes at once; 
// similarly, game 4 would also have been impossible because the Elf showed 
// you 15 blue cubes at once. If you add up the IDs of the games that would 
// have been possible, you get 8.

// Determine which games would have been possible if the bag had been loaded 
// with only 12 red cubes, 13 green cubes, and 14 blue cubes. What is the sum 
// of the IDs of those games?

// --- Part Two ---
// The Elf says they've stopped producing snow because they aren't getting any 
// water! He isn't sure why the water stopped; however, he can show you how to 
// get to the water source to check it out for yourself. It's just up ahead!

// As you continue your walk, the Elf poses a second question: in each game 
// you played, what is the fewest number of cubes of each color that could 
// have been in the bag to make the game possible?

// Again consider the example games from earlier:

// Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
// Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
// Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
// Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
// Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green

//  -  In game 1, the game could have been played with as few as 4 red, 2 green, and 6 blue cubes. 
//     If any color had even one fewer cube, the game would have been impossible.
//  -  Game 2 could have been played with a minimum of 1 red, 3 green, and 4 blue cubes.
//  -  Game 3 must have been played with at least 20 red, 13 green, and 6 blue cubes.
//  -  Game 4 required at least 14 red, 3 green, and 15 blue cubes.
//  -  Game 5 needed no fewer than 6 red, 3 green, and 2 blue cubes in the bag.

// The power of a set of cubes is equal to the numbers of red, green, and blue 
// cubes multiplied together. The power of the minimum set of cubes in game 1 
// is 48. In games 2-5 it was 12, 1560, 630, and 36, respectively. Adding up 
// these five powers produces the sum 2286.

// For each game, find the minimum set of cubes that must have been present. 
// What is the sum of the power of these sets?

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

const std::string INPUT_FILE = "etc/2.txt";

const int RED_CUBES = 12;
const int GREEN_CUBES = 13;
const int BLUE_CUBES = 14;

class Round
{
public:
    Round(int red = 0, int green = 0, int blue = 0) : red_(red), green_(green), blue_(blue) {}
    Round(std::string oneRoundSummary) {
        std::stringstream oneRoundStream(oneRoundSummary);

        std::cout << oneRoundSummary << std::endl;

        for (std::string grab; std::getline(oneRoundStream, grab, ',');) {
            if (grab.find('d') != std::string::npos) {
                red_ = std::stoi(grab.substr(grab.find(' ') + 1));
            }
            if (grab.find('g') != std::string::npos) {
                green_ = std::stoi(grab.substr(grab.find(' ') + 1));
            }
            if (grab.find('b') != std::string::npos) {
                blue_ = std::stoi(grab.substr(grab.find(' ') + 1));
            }
        }
    }

    bool isPossible() {
        return red_ <= RED_CUBES && green_ <= GREEN_CUBES && blue_ <= BLUE_CUBES;
    }

    int getRed() { return red_; }
    int getBlue() { return blue_; }
    int getGreen() { return green_; }


private:
    int red_ = 0;
    int green_ = 0;
    int blue_ = 0;
};

class Game
{
public:
    Game(std::string gameSummary) {
        gameSummary.erase(0, 5);

        std::stringstream gameStream(gameSummary);

        std::string gameId;
        std::string allRoundsSummary;
        std::getline(std::getline(gameStream, gameId, ':'), allRoundsSummary, ':');

        std::stringstream allRoundsStream(allRoundsSummary);

        std::vector<Round> rounds = {};

        for (std::string oneRoundSummary; std::getline(allRoundsStream, oneRoundSummary, ';');) {

            Round round(oneRoundSummary);
            rounds.push_back(round);
        }

        gameId_ = std::stoi(gameId);
        rounds_ = rounds;
    };

    bool isPossible() {
        return std::all_of(rounds_.cbegin(), rounds_.cend(), [](Round round) { return round.isPossible(); });
    }

    int power() {
        int max_red = std::accumulate(rounds_.cbegin(), rounds_.cend(), 0, [](int max_r, Round r) { return std::max(max_r, r.getRed()); });
        int max_green = std::accumulate(rounds_.cbegin(), rounds_.cend(), 0, [](int max_g, Round r) { return std::max(max_g, r.getGreen()); });
        int max_blue = std::accumulate(rounds_.cbegin(), rounds_.cend(), 0, [](int max_b, Round r) { return std::max(max_b, r.getBlue()); });

        return max_red * max_green * max_blue;
    }

    int getGameId() { return gameId_; }
    std::vector<Round> getRounds() { return rounds_; }

private:
    int gameId_;
    std::vector<Round> rounds_;
};

int main() {
    std::ifstream file(INPUT_FILE);
    int sum_ids = 0;
    int sum_power = 0;

    for (std::string gameSummary; std::getline(file, gameSummary);)
    {
        Game game(gameSummary);
        if (game.isPossible()) {
            sum_ids += game.getGameId();
        }
        sum_power += game.power();
    }

    std::cout << sum_ids << std::endl;
    std::cout << "Power: " << sum_power << std::endl;

    return 0;
}