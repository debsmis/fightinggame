#include "Monster.h"
#include "utilities.h" 
#include "Player.h"

Monster::Monster(const std::string& type, int row, int col, int hit, int armor, int strength, int dexterity, Weapon* weapon)
    : Actor(row, col, hit, armor, strength, dexterity, 99), currentWeapon(weapon) {
    setType(type);
}

// Implement the Bogeyman constructor and move method
Bogeyman::Bogeyman(int row, int col)
    : Monster("Bogeyman", row, col, randInt(5, 10), 2, randInt(2, 3), randInt(2, 3), new Weapon("Short Sword", 0, 2)) {
    //currentWeapon = new shortSword();
}

// Implement the Snakewoman constructor and move method
Snakewoman::Snakewoman(int row, int col)
    : Monster("Snakewoman", row, col, randInt(3, 6), 3, 2, 3, new Weapon("Magic Fangs", 3, 2)) {
}



// Implement the Dragon constructor and move method
Dragon::Dragon(int row, int col)
    : Monster("Dragon", row, col, randInt(20, 25), 4, 4, 4, new Weapon("Long Sword", 2, 4)) {
}


// Implement the Goblin constructor and move method
Goblin::Goblin(int row, int col)
    : Monster("Goblin", row, col, randInt(15, 20), 1, 3, 1, new Weapon("Short Sword", 0, 2)) {
}



string Monster::attack(Player* player) {
    string result = "";
    if (player == nullptr) return result; // Check for null pointer

    // Calculate attack points
    int attackerPoints = getDexterityPoints() + currentWeapon->getDexterityBonus();
    int defenderPoints = player->getDexterityPoints() + player->getArmorPoints();

    // Determine if the attack hits
    if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)) {
        // Attack hits
        int damagePoints = randInt(0, getStrengthPoints() + currentWeapon->getDamageAmount() - 1);
        player->setHitPoints(player->getHitPoints() - damagePoints);

        // Display message
        result = getType() + " swings " + currentWeapon->getName() + " at player and hits. \n";

        if (currentWeapon->getName() == "Magic Fangs") {
            if (randInt(1, 5) == 1) { // 1 in 5 chance to put to sleep
                int sleepTime = randInt(2, 6);
                player->setSleepTime(max(sleepTime, player->getSleepTime()));
                result += "\nPlayer is put to sleep. \n";
                // cout << "player put to sleep" << endl;
            }
        }

        // Check if the player is dead
        if (player->getHitPoints() <= 0) {
            result += "\nPlayer is killed by " + getType() + ".";
            // Quit the game
            //exit(0);
        }
    }
    else {
        // Attack misses
        result = getType() + " attacks Player with " + currentWeapon->getName() + " and misses.";
    }

    return result;
}