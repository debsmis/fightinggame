#include "Player.h"

Player::Player(int row, int col) : Actor(row, col, 20, 2, 2, 2, 20) {}


void Player::cheat() {
    setStrengthPoints(9);
    setHitPoints(50);
}

void Player::result() {}

// clear inventory
Player::~Player() {
    for (auto item : playerObj) {
        delete item;
    }
}

// pick up scroll
void Player::pickUpItem(Scroll* item) {
    if (playerObj.size() > 25) {  
        std::cout << "Inventory is full. Cannot pick up the item." << std::endl;
        return;
    }

    playerObj.push_back(new Scroll(item->getName()));
    actionMessage("You pick up " + item->getName() + ".");
}

// pick up weapon
void Player::pickUpItem(Weapon* item) {
    if (playerObj.size() > 25) {  
        std::cout << "Inventory is full. Cannot pick up the item." << std::endl;
        return;
    }

    playerObj.push_back(new Weapon(item->getName(), item->getDexterityBonus(), item->getDamageAmount()));
    actionMessage("You pick up " + item->getName() + ".");
}

// pick up golden idol
void Player::pickUpItem(GameObject* item) {
    if (playerObj.size() > 25) {  //inventory.size()
        std::cout << "Inventory is full. Cannot pick up the item." << std::endl;
        return;
    }

    if (item->getName() == "Golden Idol") {
        actionMessage("You pick up the Golden Idol. You win!");
    }
    playerObj.push_back(item);
}

vector<GameObject*> Player::getInventory() const {
    return playerObj;
}

void Player::printInventory() {
    for (size_t i = 0; i < playerObj.size(); ++i) {
        cout << static_cast<char>('a' + i) << ". " << playerObj[i]->getName() << endl;
    }
}

void Player::addToInventory(const vector<GameObject*>& globalInventory) {
    for (GameObject* item : globalInventory) {
        playerObj.push_back(item);
    }
}

void Player::clearInventory() {
    for (GameObject* item : playerObj) {
        delete item;
    }
    playerObj.clear();
}

void Player::readScroll(char scrollLabel) {
    int index = scrollLabel - 'a';
    if (index >= 0 && index < playerObj.size()) {
        Scroll* scroll = dynamic_cast<Scroll*>(playerObj[index]);
        if (scroll) {
            scroll->performAction();
            actionMessage("You read a " + scroll->getName() + ". The scroll magically self-destructs.");
            applyScrollEffect(scroll->getName());
            delete scroll;
            playerObj.erase(playerObj.begin() + index);
        }
        else {
            actionMessage("This item is not a scroll.");
        }
    }
    else {
        actionMessage("Invalid selection.");
    }
}

void Player::applyScrollEffect(const std::string& scrollType) {
    if (scrollType == "Scroll of armor") {
        int increase = randInt(1, 3); // Random integer from 1 to 3
        setArmorPoints(getArmorPoints() + increase);
    }
    else if (scrollType == "Scroll of strength") {
        int increase = randInt(1, 3); // Random integer from 1 to 3
        setStrengthPoints(getStrengthPoints() + increase);
    }
    else if (scrollType == "Scroll of health") {
        int increase = randInt(3, 8); // Random integer from 3 to 8
        changeMaxHit(increase);  
    }
    else if (scrollType == "Scroll of enhance dexterity") {
        setDexterityPoints(getDexterityPoints() + 1);
    }
}

void Player::wieldWeapon(char weaponLabel) {
    int index = weaponLabel - 'a';
    if (index >= 0 && index < playerObj.size()) {
        Weapon* weapon = dynamic_cast<Weapon*>(playerObj[index]);
        if (weapon) {
            currentWeapon = weapon;
            actionMessage("You wield the " + weapon->getName() + ".");
        }
        else {
            actionMessage("This item is not a weapon.");
        }
    }
    else {
        actionMessage("Invalid selection.");
    }
}

bool Player::attack(Monster* monster) {
   
    if (monster->getHitPoints() <= 0) {
        return true;
    }
    
    if (currentWeapon != nullptr && monster != nullptr) {

        int attackerPoints = getDexterityPoints() + currentWeapon->getDexterityBonus();
        int defenderPoints = monster->getDexterityPoints() + monster->getArmorPoints();
        if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)) {
            
            int damagePoints = randInt(0, getStrengthPoints() + currentWeapon->getDamageAmount() - 1);
            monster->setHitPoints(monster->getHitPoints() - damagePoints);
            actionMessage("Player swings " + currentWeapon->getName() + " at " + monster->getType() + " and hits.\n");
            
            if (monster->getHitPoints() <= 0) {
                actionMessage(readActionMessage() + "\n" + monster->getType() + " dies.");
                return true;
            }
            if (currentWeapon->getName() == "Magic Fangs") {
                if (randInt(1, 5) == 1) { // 1 in 5 chance to put to sleep
                    int sleepTime = randInt(2, 6);
                    monster->setSleepTime(max(sleepTime, monster->getSleepTime()));
                    actionMessage(readActionMessage() + "\n" + monster->getType() + " is put to sleep. \n");
                    return false;
                }
            }
            
        }
        else {
            actionMessage("Player swings " + currentWeapon->getName() + " at " + monster->getType() + " and misses.\n");
        }
    }
    return false;
}

void Player::actionMessage(const std::string& message) {
    actionMsg = message;
}

string Player::readActionMessage() const {
    return actionMsg;
}
