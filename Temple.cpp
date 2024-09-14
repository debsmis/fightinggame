#include "Temple.h"

Temple::Temple() : Temple(0) {};

Temple::Temple(int level) : m_level(level), m_rows(MAXROWS), m_cols(MAXCOLS) {

    // create a maze of all hashtags
    vector<Cell> row(MAXCOLS, Cell('#'));

    temple.resize(MAXROWS, row);

    // generate traversable spaces
    generateRandomBoxes();
    connectBoxes(rooms);
    replaceHashBetweenRooms();

    // add stairs if first 4 levels
    if (level < 4) {
        vector<int> loc = getRandomEmptyCell();
        placeStairs(loc[0], loc[1]);
    }
    if (level == 4) {
        vector<int> loc = getRandomEmptyCell();
        placeIdol(loc[0], loc[1]);
    }

    int numWeapons = randInt(1, 3);
    int numScrolls = 3 - numWeapons;

    for (int i = 1; i <= numWeapons; i++) {
        vector<int> weaponloc = getRandomEmptyCell();
        placeWeapons(weaponloc[0], weaponloc[1]);
        objectPos[i - 1][0] = weaponloc[0];
        objectPos[i - 1][1] = weaponloc[1];
        objectPos[i - 1][2] = 44; //weapon
        // save positions
    }

    for (int i = 1; i <= numScrolls; i++) {
        vector<int> scrollloc = getRandomEmptyCell();
        placeScrolls(scrollloc[0], scrollloc[1]);
        objectPos[i - 1][0] = scrollloc[0];
        objectPos[i - 1][1] = scrollloc[1];
        objectPos[i - 1][2] = 55; // scroll
        // save positions
    }

}

Temple::~Temple() {
    for (auto monster : monsters) {
        delete monster;
    }
}

void Temple::connectBoxes(const vector<Room>& rooms) {
    vector<Room> sortedBoxes = rooms;
    sort(sortedBoxes.begin(), sortedBoxes.end(), [](const Room& a, const Room& b) {
        return a.startCol < b.startCol;
        });

    // Connect each box with the next one in the sorted order
    for (size_t i = 0; i < sortedBoxes.size() - 1; ++i) {
        Room currentBox = sortedBoxes[i];

        // Find the row and column to connect
        int connectRow = currentBox.startRow + currentBox.height / 2;
        int connectCol = currentBox.startCol + currentBox.width;

        // Ensure connectRow is within bounds
        if (connectRow >= m_rows || connectRow < 0) {
            continue; // skip to next iteration if out of bounds
        }

        // Connect horizontally from the end of current box to the start of next box
        while (connectCol < m_cols && temple[connectRow][connectCol].display != ' ') {
            temple[connectRow][connectCol].display = ' ';
            connectCol++;
        }
    }
}

void Temple::generateRandomBoxes() {
    int numBoxes = randInt(3, 5);  // Adjust the range as needed
    int attempts = 0;

    for (int i = 0; i < numBoxes; i++) {
        bool placed = false;

        while (!placed && attempts < 100) {  // Limit attempts to avoid infinite loop
            int minWidth = 10, minHeight = 10;
            int maxWidth = 15;
            int maxHeight = 15;

            int boxWidth = randInt(minWidth, maxWidth);
            int boxHeight = randInt(minHeight, maxHeight);

            int startRow = randInt(1, m_rows - boxHeight - 1);
            int startCol = randInt(1, m_cols - boxWidth - 1);

            // Check if the area around the box is clear
            bool areaClear = true;
            for (int r = startRow - 2; r < startRow + boxHeight + 2 && areaClear; r++) {
                for (int c = startCol - 2; c < startCol + boxWidth + 2 && areaClear; c++) {
                    if (r >= 0 && r < m_rows && c >= 0 && c < m_cols && temple[r][c].display != '#') {
                        areaClear = false;
                    }
                }
            }

            if (areaClear) {
                placeBox(startRow, startCol, boxWidth, boxHeight);

                // Store box coordinates
                Room room = { startRow, startCol, boxWidth, boxHeight };
                rooms.push_back(room);

                placed = true;
            }
            attempts++;
        }
    }
}

bool Temple::isAreaClear(int startRow, int startCol, int boxWidth, int boxHeight) const {
    for (int r = startRow; r < startRow + boxHeight; r++) {
        for (int c = startCol; c < startCol + boxWidth; c++) {
            if (temple[r][c].display != '#') {
                return false;
            }
        }
    }
    return true;
}

void Temple::placeBox(int startRow, int startCol, int boxWidth, int boxHeight) {
    for (int r = startRow + 1; r < startRow + boxHeight - 1; r++) {
        for (int c = startCol + 1; c < startCol + boxWidth - 1; c++) {
            temple[r][c].display = ' ';
        }
    }
}

void Temple::replaceHashBetweenRooms() {
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 1; c < m_cols - 1; ++c) {
            if (temple[r][c].display == '#' && temple[r][c - 1].display == ' ' && temple[r][c + 1].display == ' ') {
                temple[r][c].display = ' ';
            }
        }
    }
}

void Temple::display() const {
    for (int r = 0; r < m_rows; r++) {
        for (int c = 0; c < m_cols; c++) {
            cout << temple[r][c].display;
        }
        cout << endl;
    }
}

char Temple::getCell(int row, int col) const {
    if (temple[row][col].isScroll) {
        return '?';
    }
    if (temple[row][col].isWeapon) {
        return ')';
    }
    if (temple[row][col].isStair) {
        return '>';
    }
    if (temple[row][col].isIdol) {
        return '&';
    }
    return temple[row][col].display;
}

void Temple::setCell(int row, int col, char contents) {
    if (temple[row][col].isScroll && contents == ' ') {
        temple[row][col].isScroll = false;
    }
    if (temple[row][col].isWeapon && contents == ' ') {
        temple[row][col].isWeapon = false;
    }
    temple[row][col].display = contents;
}

void Temple::placeIdol(int row, int col) {
    temple[row][col].display = '&';
    temple[row][col].isIdol = true;
}

void Temple::placeStairs(int row, int col) {
    temple[row][col].display = '>';
    temple[row][col].isStair = true;
}

void Temple::placeWeapons(int row, int col) {
    temple[row][col].display = ')';
    temple[row][col].isWeapon = true;
    int n = randInt(1, 3);
    if (n == 1) {
        temple[row][col].w = new Mace();
    }
    else if (n == 2) {
        temple[row][col].w = new shortSword();
    }
    else {
        temple[row][col].w = new longSword();
    }
}

void Temple::placeScrolls(int row, int col) {
    temple[row][col].display = '?';
    temple[row][col].isScroll = true;
    int n = randInt(1, 4);
    if (n == 1) {
        temple[row][col].s = new armor();
    }
    else if (n == 2) {
        temple[row][col].s = new strength();
    }
    else if (n == 3) {
        temple[row][col].s = new health();
    }
    else {
        temple[row][col].s = new dexterity();
    }
}

void Temple::placeBogeyman(int row, int col) {
    temple[row][col].display = 'B';
    temple[row][col].isMonster = true;
    temple[row][col].m = new Bogeyman(row, col);
    monsters.push_back(temple[row][col].m);
}

void Temple::placeSnakewoman(int row, int col) {
    temple[row][col].display = 'S';
    temple[row][col].isMonster = true;
    temple[row][col].m = new Snakewoman(row, col);
    monsters.push_back(temple[row][col].m);
}

void Temple::placeGoblin(int row, int col) {
    temple[row][col].display = 'G';
    temple[row][col].isMonster = true;
    temple[row][col].m = new Goblin(row, col);
    monsters.push_back(temple[row][col].m);
}

void Temple::placeDragon(int row, int col) {
    temple[row][col].display = 'D';
    temple[row][col].isMonster = true;
    temple[row][col].m = new Dragon(row, col);
    monsters.push_back(temple[row][col].m);
}

vector<int> Temple::getRandomEmptyCell() {
    int row = 0;
    int col = 0;
    while (getCell(row, col) == '#') {
        row = randInt(1, MAXROWS - 1);
        col = randInt(1, MAXCOLS - 1);
    }
    return { row,col };
}

Monster* Temple::getMonster(int row, int col) {
    return temple[row][col].m;
}

void Temple::removeMonster(int row, int col) {
    if (temple[row][col].m != nullptr) {
        // Remove the monster from the monsters vector
        auto it = std::find(monsters.begin(), monsters.end(), temple[row][col].m);
        if (it != monsters.end()) {
            monsters.erase(it);
        }
        string type = temple[row][col].m->getType();
        delete temple[row][col].m;
        temple[row][col].isMonster = false;
        if (type == "Bogeyman") {
            if (trueWithProbability(0.1)) {
                setCell(row, col, ')');
                temple[row][col].isWeapon = true;
                temple[row][col].w = new magicAxe();
                // TO DO: print that this is dropped
            }
            else
                setCell(row, col, ' ');
        }
        else if (type == "Snakewoman") {
            if (trueWithProbability(3.0 / 10)) {
                setCell(row, col, ')');
                temple[row][col].isWeapon = true;
                temple[row][col].w = new magicFangs();
                // TO DO: print that this is dropped
            }
            else
                setCell(row, col, ' ');
        }
        else if (type == "Dragon") {
            setCell(row, col, '?');
            temple[row][col].isScroll = true;
            int n = randInt(1, 5);
            if (n == 1) {
                temple[row][col].s = new teleportation();
                // TO DO: print that this is dropped
            }
            else if (n == 2) {
                temple[row][col].s = new armor();
                // TO DO: print that this is dropped
            }
            else if (n == 3) {
                temple[row][col].s = new strength();
                // TO DO: print that this is dropped
            }
            else if (n == 4) {
                temple[row][col].s = new health();
                // TO DO: print that this is dropped
            }
            else {
                temple[row][col].s = new dexterity();
                // TO DO: print that this is dropped
            }
        }
        else {
            //cout << "at goblin" << endl;
            if (trueWithProbability(3.0 / 10)) {
                setCell(row, col, ')');
                temple[row][col].isWeapon = true;
                if (trueWithProbability(0.5))
                    temple[row][col].w = new magicFangs();
                // TO DO: print that this is dropped
                else
                    temple[row][col].w = new magicAxe();
                // TO DO: print that this is dropped
            }
            else {
                setCell(row, col, ' ');
            }
        }
    }
}

Weapon* Temple::getWeapon(int row, int col) {
    return temple[row][col].w;
}

Scroll* Temple::getScroll(int row, int col) {
    return temple[row][col].s;
}

void Temple::removeWeapon(int row, int col) {
    if (temple[row][col].isWeapon) {
        delete temple[row][col].w;
        temple[row][col].isWeapon = false;
    }
}

void Temple::removeScroll(int row, int col) {
    if (temple[row][col].isScroll) {
        delete temple[row][col].s;
        temple[row][col].isScroll = false;
    }
}

bool Temple::isAdjacent(int x1, int y1, int x2, int y2) {
    return (abs(x1 - x2) + abs(y1 - y2)) == 1;
}

void Temple::moveMonsters(Player* p, int goblinSmellDistance) {
    for (Monster* m : monsters) {
        if (isAdjacent(m->getXPos(), m->getYPos(), p->getXPos(), p->getYPos())) {
            // Attack the player if next to it
            string attackResult = m->attack(p);
            p->actionMessage(p->readActionMessage() + "\n" + attackResult);
        }
        else {
            // Move the monster if not adjacent
            if (m->getType() == "Goblin") {
                moveGoblinRecursive(m, p, goblinSmellDistance);
            }
            else if (m->getType() == "Bogeyman") {
                moveBogeyman(m, p);
            }
            else if (m->getType() == "Snakewoman") {
                moveSnakewoman(m, p);
            }
        }
    }
}



void Temple::moveBogeyman(Monster* m, Player* p) {

    if (m->isAsleep()) {
        m->decreaseSleepTime();
        return;
    }
    pathCoord path = findDistance(m, p, 5);

    // Check if the path is not empty and if it contains more than one coordinate
    if (path.totalPath.size() > 1) {
        // The first coordinate in the path is the goblin's current position
        // The second coordinate in the path is the next position to move to
        coord currentPos(m->getXPos(), m->getYPos());

        // The second coordinate in the path is the next position to move to
        coord nextMove = path.totalPath[path.totalPath.size() - 2]; // move towards the player

        // Get references to the current and next cells
        Cell& nextCell = temple[nextMove.x][nextMove.y];

        // Check if the next cell has an actor
        if (nextCell.display == '@' || nextCell.display == 'D' || nextCell.display == 'S' ||
            nextCell.display == 'B' || nextCell.display == 'G' || nextCell.display == '#' 
            //|| nextCell.display == '>' || nextCell.display == '?' || nextCell.display == ')' || nextCell.display == '&'
            ) {
            // IF EXTRA TIME, CHANGE >, ?, ), and & stuff 
            return; // The goblin does not move if the next cell is occupied
        }

        // Update the current cell to remove the bogeyman
        Cell& currentCell = temple[currentPos.x][currentPos.y];

        //char previousDisplay = currentCell.display;
        bool previousIsScroll = currentCell.isScroll;
        bool previousIsWeapon = currentCell.isWeapon;
        bool previousIsStair = currentCell.isStair;
        bool previousIsIdol = currentCell.isIdol;
        Weapon* previousWeapon = currentCell.w;
        Scroll* previousScroll = currentCell.s;

        currentCell.isMonster = false;
        currentCell.m = nullptr;
        currentCell.display = ' ';

        // Update the next cell to add the bogeyman
        nextCell.isMonster = true;
        nextCell.m = m;
        nextCell.display = 'B';

        // Update the bogeyman's position
        m->setXPos(nextMove.x);
        m->setYPos(nextMove.y);

        // Restore the previous state of the cell if it had an item
        if (previousIsScroll) {
            currentCell.display = '?';
            currentCell.isScroll = true;
            currentCell.s = previousScroll;
        }
        else if (previousIsWeapon) {
            currentCell.display = ')';
            currentCell.isWeapon = true;
            currentCell.w = previousWeapon;
        }
        else if (previousIsStair) {
            currentCell.display = '>';
            currentCell.isStair = true;
        }
        else if (previousIsIdol) {
            currentCell.display = '&';
            currentCell.isIdol = true;
        }

        //cout << "Goblin moved to: x = " << nextMove.x << ", y = " << nextMove.y << endl;
    }
    else {
        // << "Goblin did not move. No valid path or already at the player." << endl;
        return;
    }
}

void Temple::moveSnakewoman(Monster* m, Player* p) {
    if (m->isAsleep()) {
        m->decreaseSleepTime();
        return;
    }
    pathCoord path = findDistance(m, p, 3);

    // Check if the path is not empty and if it contains more than one coordinate
    if (path.totalPath.size() > 1) {
        // The first coordinate in the path is the goblin's current position
        coord currentPos(m->getXPos(), m->getYPos());

        // The second coordinate in the path is the next position to move to
        coord nextMove = path.totalPath[path.totalPath.size() - 2]; // move towards the player

        // Get references to the current and next cells
        Cell& nextCell = temple[nextMove.x][nextMove.y];

        // Check if the next cell has an actor
        if (nextCell.display == '@' || nextCell.display == 'D' || nextCell.display == 'S' ||
            nextCell.display == 'B' || nextCell.display == 'G' || nextCell.display == '#' 
            ) {
            return; // The snakewoman does not move if the next cell is occupied
        }

        // Update the current cell to remove the snakewoman
        Cell& currentCell = temple[currentPos.x][currentPos.y];

        //char previousDisplay = currentCell.display;
        bool previousIsScroll = currentCell.isScroll;
        bool previousIsWeapon = currentCell.isWeapon;
        bool previousIsStair = currentCell.isStair;
        bool previousIsIdol = currentCell.isIdol;
        Weapon* previousWeapon = currentCell.w;
        Scroll* previousScroll = currentCell.s;

        currentCell.isMonster = false;
        currentCell.m = nullptr;
        currentCell.display = ' ';

        // Update the next cell to add the snakewoman
        nextCell.isMonster = true;
        nextCell.m = m;
        nextCell.display = 'S';

        // Update the snakewoman's position
        m->setXPos(nextMove.x);
        m->setYPos(nextMove.y);

        // Restore the previous state of the cell if it had an item
        if (previousIsScroll) {
            currentCell.display = '?';
            currentCell.isScroll = true;
            currentCell.s = previousScroll;
        }
        else if (previousIsWeapon) {
            currentCell.display = ')';
            currentCell.isWeapon = true;
            currentCell.w = previousWeapon;
        }
        else if (previousIsStair) {
            currentCell.display = '>';
            currentCell.isStair = true;
        }
        else if (previousIsIdol) {
            currentCell.display = '&';
            currentCell.isIdol = true;
        }

    }
    else {
        return;
    }
}

int Temple::calculateHeuristic(Monster* m, Player* p) {
    return abs(m->getXPos() - p->getXPos()) + abs(m->getYPos() - p->getYPos());
}

int Temple::calculateHeuristic(int x, int y, Player* p) {
    if (p == nullptr) return 0;
    return abs(x - p->getXPos()) + abs(y - p->getYPos());
}


pathCoord Temple::findDistance(Monster* m, Player* p, int maxDistance) {
    deque<coord> openList;  // visited but children not processed yet
    deque<coord> closedList; // visited and children processed 

    coord temp(m->getXPos(), m->getYPos(), 0, calculateHeuristic(m, p));
    openList.push_front(temp);

    while (!openList.empty() && openList.front().height <= maxDistance) { 
        //cout << "inside while loop" << endl;
        coord top = openList.front();
        openList.pop_front();
        closedList.push_back(top);
        if (top.x == p->getXPos() && top.y == p->getYPos()) {
            // base case -> found player
            pathCoord path;
            temp = top;
            while (temp.x != -1 && temp.y != -1) {
                path.addCoordinate(temp);

                auto itCoordinate = find(closedList.begin(), closedList.end(), coord(temp.parentX, temp.parentY));

                if (itCoordinate != closedList.end()) {
                    temp = *itCoordinate;
                }
                else {
                    break;
                }
            }

            return path;
        }
        
    // Generate valid moves (up, down, left, right)

        vector<coord> neighbors = {
            coord(top.x + 1, top.y, top.height + 1, calculateHeuristic(top.x + 1, top.y, p), top.x, top.y),
            coord(top.x - 1, top.y, top.height + 1, calculateHeuristic(top.x - 1, top.y, p), top.x, top.y),
            coord(top.x, top.y + 1, top.height + 1, calculateHeuristic(top.x, top.y + 1, p), top.x, top.y),
            coord(top.x, top.y - 1, top.height + 1, calculateHeuristic(top.x, top.y - 1, p), top.x, top.y)
        };

        for (auto& neighbor : neighbors) {
            // Check if the neighbor is within bounds and not a wall
            if (neighbor.x >= 0 && neighbor.x < MAXROWS && neighbor.y >= 0 && neighbor.y < MAXCOLS && getCell(neighbor.x, neighbor.y) != '#') {
                // Check if the neighbor is already in closedList
                if (find(closedList.begin(), closedList.end(), neighbor) == closedList.end()) {
                    // Check if the neighbor is already in openList
                    auto it = find(openList.begin(), openList.end(), neighbor);
                    if (it == openList.end()) {
                        openList.push_back(neighbor);
                    }
                }
            }
        }
    }

    return pathCoord(); 
}


//trying recursive from here

pathCoord Temple::findDistanceRecursive(Monster* m, Player* p, int maxDistance) {
    vector<vector<bool>> visited(m_rows, vector<bool>(m_cols, false));
    vector<coord> path;

    if (findPathRecursive(m->getXPos(), m->getYPos(), p->getXPos(), p->getYPos(), maxDistance, 0, path, visited)) {
        pathCoord result;
        for (const auto& step : path) {
            result.addCoordinate(step);
        }
        return result;
    }

    return pathCoord(); // Return an empty pathCoord if no path is found
}

bool Temple::findPathRecursive(int x, int y, int playerX, int playerY, int maxDistance, int currentDistance, vector<coord>& path, vector<vector<bool>>& visited) {
    
    if (currentDistance > maxDistance) {
        return false;
    }
    if (x == playerX && y == playerY) {
        path.push_back(coord(x, y, currentDistance, calculateHeuristicRecursion(x, y, playerX, playerY)));
        return true;
    }

    visited[x][y] = true;
    path.push_back(coord(x, y, currentDistance, calculateHeuristicRecursion(x, y, playerX, playerY)));

    vector<coord> directions = {
        coord(x + 1, y, currentDistance + 1, calculateHeuristicRecursion(x + 1, y, playerX, playerY)),
        coord(x - 1, y, currentDistance + 1, calculateHeuristicRecursion(x - 1, y, playerX, playerY)),
        coord(x, y + 1, currentDistance + 1, calculateHeuristicRecursion(x, y + 1, playerX, playerY)),
        coord(x, y - 1, currentDistance + 1, calculateHeuristicRecursion(x, y - 1, playerX, playerY))
    };

    // Sort directions based on their total cost (g + h)
    sort(directions.begin(), directions.end(), [](const coord& a, const coord& b) {
        return (a.height + a.heuristic) < (b.height + b.heuristic);
        });

    for (const auto& dir : directions) {
        if (isValidMove(dir.x, dir.y) && !visited[dir.x][dir.y]) {
            if (findPathRecursive(dir.x, dir.y, playerX, playerY, maxDistance, currentDistance + 1, path, visited)) {
                return true;
            }
        }
    }

    path.pop_back(); // Backtrack if no valid path is found
    visited[x][y] = false; // Unmark the cell as visited to allow other paths
    return false;
}
bool Temple::isValidMove(int x, int y) {
    if (x < 0 || x >= m_rows || y < 0 || y >= m_cols) {
        return false;
    }
    char cell = temple[x][y].display;
    return cell != '#' && cell != 'B' && cell != 'D' && cell != 'S' && cell != 'G'; // removed @ symbol condition
}

void Temple::moveGoblinRecursive(Monster* m, Player* p, int goblinSmellDistance) {
    if (m->isAsleep()) {
        m->decreaseSleepTime();
        return;
    }
    pathCoord path = findDistanceRecursive(m, p, goblinSmellDistance); 
    
    // Check if the path is not empty and if it contains more than one coordinate
    if (path.totalPath.size() > 1) {
        // The second coordinate in the path is the next position to move to

        // Remove the first coordinate from the path (current position of the goblin)
        path.totalPath.erase(path.totalPath.begin());
        coord nextMove = path.totalPath.front();
        

        // Get references to the current and next cells

        Cell& nextCell = temple[nextMove.x][nextMove.y];

        // Check if the next cell has an actor
        if (nextCell.display == '@' || nextCell.display == 'D' || nextCell.display == 'S' ||
            nextCell.display == 'B' || nextCell.display == 'G' || nextCell.display == '#'
            //  || nextCell.display == '>' || nextCell.display == '?' || nextCell.display == ')' || nextCell.display == '&'
            ) {
            return; // The goblin does not move if the next cell is occupied
        }

        // Update the current cell to remove the goblin
        Cell& currentCell = temple[m->getXPos()][m->getYPos()];

        //char previousDisplay = currentCell.display;
        bool previousIsScroll = currentCell.isScroll;
        bool previousIsWeapon = currentCell.isWeapon;
        bool previousIsStair = currentCell.isStair;
        bool previousIsIdol = currentCell.isIdol;
        Weapon* previousWeapon = currentCell.w;
        Scroll* previousScroll = currentCell.s;

        currentCell.isMonster = false;
        currentCell.m = nullptr;
        currentCell.display = ' ';

        // Update the next cell to add the goblin
        nextCell.isMonster = true;
        nextCell.m = m;
        nextCell.display = 'G';

        // Update the goblin's position
        m->setXPos(nextMove.x);
        m->setYPos(nextMove.y);

        // Restore the previous state of the cell if it had an item
        if (previousIsScroll) {
            currentCell.display = '?';
            currentCell.isScroll = true;
            currentCell.s = previousScroll;
        }
        else if (previousIsWeapon) {
            currentCell.display = ')';
            currentCell.isWeapon = true;
            currentCell.w = previousWeapon;
        }
        else if (previousIsStair) {
            currentCell.display = '>';
            currentCell.isStair = true;
        }
        else if (previousIsIdol) {
            currentCell.display = '&';
            currentCell.isIdol = true;
        }
    }
}

int Temple::calculateHeuristicRecursion(int x, int y, int pX, int pY) {
    return abs(x - pX) + abs(y - pY);
}
