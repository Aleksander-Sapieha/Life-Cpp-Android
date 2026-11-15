
#include "game.h"

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <algorithm>

using namespace std;

Player::Player(const string& n) {
    name = n;
}

bool Player::isAlive() const {
    return health > 0 && happiness > 0;
}

string Player::showStatus() const {
    ostringstream out;

    out << "--- Status ---\n";
    out << "Name: " << name << "\n";
    out << "Age: " << age << "\n";
    out << "Health: " << health << "\n";
    out << "Happiness: " << happiness << "\n";
    out << "Money: $" << money << "\n";
    out << "Social: " << social << "\n";
    out << "Job: " << job << "\n";
    out << "Married: " << (married ? "Yes" : "No") << "\n";
    out << "Children: " << children << "\n";
    out << "Owns House: " << (ownsHouse ? "Yes" : "No") << "\n";
    out << "Retired: " << (retired ? "Yes" : "No");

    return out.str();
}

Game::Game(const string& name) : player(name) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

static void clampStats(Player& p) {
    p.health = clamp(p.health, 0, 100);
    p.happiness = clamp(p.happiness, 0, 100);
    p.social = clamp(p.social, 0, 100);
}

static string randomEvent(Player& p) {
    ostringstream out;

    switch(rand() % 5) {
        case 0:
            out << "You found a small gift!\nMoney +10, Happiness +5";
            p.money += 10;
            p.happiness += 5;
            break;

        case 1:
            out << "You caught a cold.\nHealth -10, Happiness -5";
            p.health -= 10;
            p.happiness -= 5;
            break;

        case 2:
            out << "Your friend invited you out.\nSocial +10, Happiness +10, Money -5";
            p.social += 10;
            p.happiness += 10;
            p.money -= 5;
            break;

        case 3:
            out << "Unexpected car repair!\nMoney -20, Happiness -5";
            p.money -= 20;
            p.happiness -= 5;
            break;

        case 4:
            out << "You exercised and ate healthy today.\nHealth +10, Happiness +5";
            p.health += 10;
            p.happiness += 5;
            break;
    }

    clampStats(p);
    return out.str();
}

string Game::getStatus() const {
    return player.showStatus();
}

bool Game::isFinished() const {
    return finished;
}

string Game::startDay() {
    if(finished) {
        return "The game has already ended.";
    }

    ostringstream out;

    out << player.showStatus() << "\n\n";
    out << "Random Event:\n";
    out << randomEvent(player);

    return out.str();
}

string Game::chooseAction(int choice) {
    if(finished) {
        return "The game has already ended.";
    }

    ostringstream out;

    switch(choice) {
        case 1:
            if(player.job == "Unemployed" || player.retired) {
                out << "You are unemployed or retired, so you cannot work.";
            } else {
                out << "You worked at your job.\n"
                    << "Money +50, Happiness -10, Health -5";
                player.money += 50;
                player.happiness -= 10;
                player.health -= 5;
            }
            break;

        case 2:
            out << "You relaxed.\nHappiness +15, Health +5";
            player.happiness += 15;
            player.health += 5;
            player.social -= 5;
            break;

        case 3:
            out << "You exercised.\nHealth +15, Happiness +5";
            player.health += 15;
            player.happiness += 5;
            break;

        case 4:
            out << "You socialized.\nSocial +20, Happiness +10, Money -10";
            player.social += 20;
            player.happiness += 10;
            player.money -= 10;
            break;

        case 5:
            out << "Choose a job:\n"
                << "1. Engineer\n"
                << "2. Artist\n"
                << "3. Teacher";
            return out.str();

        case 6:
            if(!player.ownsHouse && player.money >= 100) {
                out << "You bought a house!\nMoney -100, Happiness +20";
                player.ownsHouse = true;
                player.money -= 100;
                player.happiness += 20;
            } else if(player.ownsHouse) {
                out << "You already own a house.";
            } else {
                out << "Not enough money to buy a house.";
            }
            break;

        case 7:
            if(player.married) {
                out << "You had a child!\nHappiness +20, Social +10, Money -20";
                player.children++;
                player.happiness += 20;
                player.social += 10;
                player.money -= 20;
            } else {
                out << "You must be married to have a child.";
            }
            break;

        default:
            out << "You did nothing today.";
            break;
    }

    clampStats(player);

    return finishDay(out.str());
}

string Game::finishDay(const string& message) {
    ostringstream out;
    out << message;

    clampStats(player);

    if(player.age >= 65 && !player.retired) {
        player.retired = true;
        player.job = "Retired";
        out << "\n\nYou reached retirement age. You are now retired.";
    }

    player.age++;

    if(!player.isAlive() || player.age > 100) {
        finished = true;

        out << "\n\nGame Over!";
        out << "\n" << player.name << " lived until age " << player.age;
        out << "\n\nFinal Stats:";
        out << "\nHappiness: " << player.happiness;
        out << "\nHealth: " << player.health;
        out << "\nMoney: $" << player.money;
        out << "\nSocial: " << player.social;
        out << "\nChildren: " << player.children;
        out << "\nHouse: " << (player.ownsHouse ? "Yes" : "No");

        return out.str();
    }

    out << "\n\nNext day begins!\n\n";
    out << startDay();

    return out.str();
}

string Game::chooseJob(int choice) {
    switch(choice) {
        case 1:
            player.job = "Engineer";
            return finishDay("You are now an Engineer!");

        case 2:
            player.job = "Artist";
            return finishDay("You are now an Artist!");

        case 3:
            player.job = "Teacher";
            return finishDay("You are now a Teacher!");

        default:
            return "Invalid job choice. Choose again.";
    }
}
