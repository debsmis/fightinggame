// Game.cpp

#include "Game.h"

// Implement these and other Game member functions you may have added.

#define MAXLEVELS 5

Game::Game(int goblinSmellDistance) : goblinSmellDistance(goblinSmellDistance)
{
    currLevel = 0; // Start at level 0
    temples.resize(5); // Resize vector to have 5 temples
    for (int i = 0; i < 5; ++i) {
        temples[i] = Temple(i); // Initialize each temple with the corresponding level
    }
    initializeLevel(currLevel);
    

}

void Game::initializeLevel(int level) {
    // Store the player's current attributes if the player exists
    int currentHitPoints = 0;
    int currentArmorPoints = 0;
    int currentStrengthPoints = 0;
    int currentDexterityPoints = 0;
    int currentMaxHit = 0;

    vector<GameObject*> currentInventory;
    
    if (player != nullptr && level != 0) {
        currentHitPoints = player->getHitPoints();
        currentArmorPoints = player->getArmorPoints();
        currentStrengthPoints = player->getStrengthPoints();
        currentDexterityPoints = player->getDexterityPoints();
        currentMaxHit = player->getMaxHit();
        currentInventory = globalInventory;
    }

    // Assign the current temple based on the level
    temples[currLevel] = temples[level];


    // Generate monsters for the new level
    generateMonsters(level);

    // Place the player at a random starting position
    vector<int> loc = temples[currLevel].getRandomEmptyCell();

    player = new Player(loc[0], loc[1]);

    

    // Transfer the player's attributes to the new player
    if (level != 0){
        player->setHitPoints(currentHitPoints);
        player->setArmorPoints(currentArmorPoints);
        player->setStrengthPoints(currentStrengthPoints);
        player->setDexterityPoints(currentDexterityPoints);
        player->changeMaxHit(currentMaxHit);
    }

    temples[currLevel].setCell(loc[0], loc[1], '@');

    player->addToInventory(globalInventory);
    
    globalInventory.clear();

    player->wieldWeapon('a');
    player->actionMessage("");




}



Game::~Game() {
    delete player;
    player = nullptr;
    for (auto item : globalInventory) {
        delete item;  // Delete each GameObject pointer
    }
    globalInventory.clear();
}

void Game::play()
{
    display();
    char input = ' ';

    while (input != 'q') {

        input = getCharacter();

        if (!player->isAsleep()){
        switch (input) {
            // descend stairs, change currLevel, display new temple
        case ARROW_LEFT:
            
            if (temples[currLevel].getCell(player->getXPos(), player->getYPos() - 1) == 'B' ||
                temples[currLevel].getCell(player->getXPos(), player->getYPos() - 1) == 'S' ||
                temples[currLevel].getCell(player->getXPos(), player->getYPos() - 1) == 'D' ||
                temples[currLevel].getCell(player->getXPos(), player->getYPos() - 1) == 'G') {

                if (player->attack(temples[currLevel].getMonster(player->getXPos(), player->getYPos() - 1))) { // kills monster
                    temples[currLevel].removeMonster(player->getXPos(), player->getYPos() - 1);
                }

            }
            else
                move(0, -1);
            break;
        case ARROW_RIGHT:
            if (temples[currLevel].getCell(player->getXPos(), player->getYPos() + 1) == 'B' ||
                temples[currLevel].getCell(player->getXPos(), player->getYPos() + 1) == 'S' ||
                temples[currLevel].getCell(player->getXPos(), player->getYPos() + 1) == 'D' ||
                temples[currLevel].getCell(player->getXPos(), player->getYPos() + 1) == 'G') {

                if (player->attack(temples[currLevel].getMonster(player->getXPos(), player->getYPos() + 1))) { // kills monster
                    temples[currLevel].removeMonster(player->getXPos(), player->getYPos() + 1);
                }
            }
            else
                move(0, 1);
            break;
        case ARROW_DOWN:
            if (temples[currLevel].getCell(player->getXPos() + 1, player->getYPos()) == 'B' ||
                temples[currLevel].getCell(player->getXPos() + 1, player->getYPos()) == 'S' ||
                temples[currLevel].getCell(player->getXPos() + 1, player->getYPos()) == 'D' ||
                temples[currLevel].getCell(player->getXPos() + 1, player->getYPos()) == 'G') {

                if (player->attack(temples[currLevel].getMonster(player->getXPos() + 1, player->getYPos()))) { // kills monster
                    temples[currLevel].removeMonster(player->getXPos() + 1, player->getYPos());
                }
            }
            else
                move(1, 0);
            break;
        case ARROW_UP:
            if (temples[currLevel].getCell(player->getXPos() - 1, player->getYPos()) == 'B' ||
                temples[currLevel].getCell(player->getXPos() - 1, player->getYPos()) == 'S' ||
                temples[currLevel].getCell(player->getXPos() - 1, player->getYPos()) == 'D' ||
                temples[currLevel].getCell(player->getXPos() - 1, player->getYPos()) == 'G') { // kills monster

                if (player->attack(temples[currLevel].getMonster(player->getXPos() - 1, player->getYPos()))) {
                    temples[currLevel].removeMonster(player->getXPos() - 1, player->getYPos());
                }
            }
            else
                move(-1, 0);
            break;
        case 'g':
            // Code to pick up, replace with space, and add to inventory
            // Method to get the item at player's current position
            if (temples[currLevel].getCell(player->getXPos(), player->getYPos()) == '&') {
                player->actionMessage("Player picks up the Golden Idol and wins. Congratulations!");
                cout << player->readActionMessage() << endl;
                input = 'q';
                break;
            }
            if (temples[currLevel].getCell(player->getXPos(), player->getYPos()) == ')') {
                player->pickUpItem(temples[currLevel].getWeapon(player->getXPos(), player->getYPos()));
                removeItemAtPlayerPosition(player->getXPos(), player->getYPos());
            }
            else if (temples[currLevel].getCell(player->getXPos(), player->getYPos()) == '?') {
                player->pickUpItem(temples[currLevel].getScroll(player->getXPos(), player->getYPos()));
                removeItemAtPlayerPosition(player->getXPos(), player->getYPos());
            }
            break;
        case 'w':
            clearScreen();
            player->printInventory();
            std::cout << "Select a weapon to wield: ";
            input = getCharacter();
            player->wieldWeapon(input);
            break;
        case '>': { // last level should not have any staircase

            // check on staircase
            // if on staircase, change temple to next level
            if (temples[currLevel].getCell(player->getXPos(), player->getYPos()) == '>' && currLevel != 4) {
                currLevel++;
                const auto& playerInventory = player->getInventory();
                for (auto item : playerInventory) {
                    globalInventory.push_back(item);
                }
                initializeLevel(currLevel);
                player->actionMessage("You descend to level " + to_string(currLevel));
            }
            else {
                player->actionMessage("You are not standing on a stairway.");
            }
            break;
        }
        case 'r':
            //read scroll, get points back, delete scroll from inventory
            clearScreen();
            player->printInventory();
            std::cout << "Select a scroll to read: "; //need to make it so that user does not have to hit enter afterwards
            input = getCharacter();
            player->readScroll(input);
            break;
        case 'c':
            player->cheat();
            break;
        case 'q':
            break;
        case ' ':
            break;
        case 'i':
            clearScreen();
            player->printInventory();
            while (true) {
                char inventoryInput = getCharacter();
                if (inventoryInput == 'i' || inventoryInput == 'q') {
                    input = inventoryInput;
                    break;
                }
            }
            break;
        default:
            getCharacter();
            break;
        }
        }
        else
            player->decreaseSleepTime();
        if (input == 'q') break;
        //monsters move and/or attack after player moves
        temples[currLevel].moveMonsters(player, goblinSmellDistance);
        clearScreen();
        display();
        cout << player->readActionMessage() << endl;
        if (player->getHitPoints() <= 0) {
            input = 'q';
        }
        player->actionMessage("");

    }

}

void Game::display() {
    temples[currLevel].display();
    cout << endl;
    cout << "Level: " << currLevel << ", Hit points: " << player->getHitPoints() << " , Armor: " << player->getArmorPoints() << ", Strength: " << player->getStrengthPoints() << ", Dexterity: " << player->getDexterityPoints()  << "\n" << endl;
}

void Game::move(int xDirection, int yDirection) {
    char contentsPrevious = temples[currLevel].getCell(player->getXPos(), player->getYPos());
    if (contentsPrevious == '@') {
        contentsPrevious = ' ';
    }
    int row = player->getXPos() + xDirection;
    int col = player->getYPos() + yDirection;
    char contentsNext = temples[currLevel].getCell(row, col);
    switch (contentsNext) {
    case ' ':
    case '>':
    case '?':
    case ')':
    case '&':
        temples[currLevel].setCell(player->getXPos(), player->getYPos(), contentsPrevious);
        contentsPrevious = contentsNext;
        player->setXPos(row);
        player->setYPos(col);
        temples[currLevel].setCell(row, col, '@');
        break;
    case '#':
        // do nothing
        break;
    case 'D':
    case 'S':
    case 'G':
        break;
    default:
        break;
    }
}

void Game::removeItemAtPlayerPosition(int x, int y) {
    // Set the map cell at the player's position to an empty space
    if (temples[currLevel].getCell(x, y) == ')') {
        temples[currLevel].removeWeapon(x, y);
    }
    else if (temples[currLevel].getCell(x, y) == '?') {
        temples[currLevel].removeScroll(x, y);
    }
    else if (temples[currLevel].getCell(x, y) == 'B' ||
        temples[currLevel].getCell(x, y) == 'S' ||
        temples[currLevel].getCell(x, y) == 'D' ||
        temples[currLevel].getCell(x, y) == 'G') {
        temples[currLevel].removeMonster(x, y);
    }

}

void Game::generateMonsters(int level) {
    
   
    int numMonsters = randInt(2, 5 * (level + 1) + 1);


     
    for (int i = 0; i < numMonsters; ++i) {
        Monster* newMonster = nullptr;
        int monsterType = randInt(1, 4);                      
        
        // Ensure the position is empty
        vector<int> emptyCell = temples[currLevel].getRandomEmptyCell();
        int x = emptyCell[0];
        int y = emptyCell[1];
        
        switch (monsterType) {
        case 1:
            if (level >= 2) {
                newMonster = new Bogeyman(x, y);
                newMonster->setXPos(x);
                newMonster->setYPos(y);
                temples[currLevel].placeBogeyman(x, y);
            }
            else {
                newMonster = new Goblin(x, y); // Default to Goblin if level is not deep enough for Bogeyman
                newMonster->setXPos(x);
                newMonster->setYPos(y);
                temples[currLevel].placeGoblin(x, y);
            }
            break;
        case 2:
            //cout << "in snake" << endl;
            newMonster = new Snakewoman(x, y);
            newMonster->setXPos(x);
            newMonster->setYPos(y);
            //cout << "after new snake" << endl;
            temples[currLevel].placeSnakewoman(x, y);
            //cout << "placed??" << endl;
            break;
        case 3:
            if (level >= 3) {
                newMonster = new Dragon(x, y);
                newMonster->setXPos(x);
                newMonster->setYPos(y);
                temples[currLevel].placeDragon(x, y);
            }
            else {
                newMonster = new Snakewoman(x, y); // Default to Snakewoman if level is not deep enough for Dragon
                newMonster->setXPos(x);
                newMonster->setYPos(y);
                temples[currLevel].placeGoblin(x, y);
            }
            break;
        case 4:
            newMonster = new Goblin(x, y);
            newMonster->setXPos(x);
            newMonster->setYPos(y);
            temples[currLevel].placeGoblin(x, y);
            break;
        default:
            break;
        }

    }
}


// You will presumably add to this project other .h/.cpp files for the various
// classes of objects you need to play the game:  player, monsters, weapons, 
// etc.  You might have a separate .h/.cpp pair for each class (e.g., Player.h,
// Bogeyman.h, etc.), or you might put the class declarations for all actors in
// Actor.h, all game objects in GameObject.h, etc.
