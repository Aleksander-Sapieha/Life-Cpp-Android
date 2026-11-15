
#pragma once

#include <string>
#include <vector>

class Player {
public:
    std::string name;
    int age = 18;
    int health = 100;
    int happiness = 50;
    int money = 100;
    int social = 50;

    std::string job = "Unemployed";

    bool married = false;
    int children = 0;
    bool ownsHouse = false;
    bool retired = false;

    Player(const std::string& n);

    bool isAlive() const;
    std::string showStatus() const;
};



class Game {
private:
    Player player;
    bool finished = false;

    std::string finishDay(const std::string& message);

public:
    Game(const std::string& name);

    std::string startDay();
    std::string chooseAction(int choice);
    std::string chooseJob(int choice);
    std::string getStatus() const;

    bool isFinished() const;
};
