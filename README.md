# Fighting Game

# Game Overview:
The player navigates through a multi-level temple in search of a golden idol. Upon locating the idol, the player is instantly transported out of the temple and wins the game. Upon starting a new game, you the player are placed on the top level of the temple. From here, you must work your way through the rooms on that level, battling monsters, finding treasures, and using staircases to descend ever deeper into the temple's depths. When you reach the bottom level of the temple, you must reach the golden idol in order to win the game.

As you move through a level, you will encounter a number of different types of monsters that you will have to battle, including:
-	Bogeymen (shown on screen as a B)
-	Dragons (shown on screen as a D)
-	Goblins (shown on screen as a G)
-	Snakewomen (shown on screen as an S)
  
Each of these monsters has a different behavior and reacts differently to the player (who is shown on screen as an @). 

In addition to monsters, you'll find many different objects during your adventure:
-	Impenetrable walls (shown on screen as #)
-	Stairways down to the next level (shown on screen as >)
-	The golden idol (shown on screen as &)
-	Weapons (all weapons are shown on screen as a ) character)
-	Maces
-	Short swords
-	Long swords
-	Magic fangs of sleep (that put an opponent to sleep)
-	Magic axes (that hit an opponent more often than regular weapons)
-	Scrolls (all scrolls are shown on screen as ?)
  - A scroll of teleportation (when read, randomly moves the player)
  - A scroll of improve armor (when read, makes it harder for monsters to hit the player)
  - A scroll of raise strength (when read, makes a player's blows more effective)
  - A scroll of enhance health (when read, raises maximum hit points)
  - A scroll of enhance dexterity (when read, makes it more likely the player will hit an opponent)

In addition to moving with the arrow keys/hlkj keys, you can:
•	Attack a monster next to you by moving in its direction.
•	Pick up an object by standing on it and typing g.
•	Wield a weapon by typing w and then selecting a weapon from your inventory.
•	Read a scroll by typing r and then selecting a scroll from your inventory.
•	See an inventory of your items by typing i.
•	When standing on a stairway, descend deeper into the temple by typing >.
•	Quit the game by typing q.
•	Cheat by typing c. This command sets the player's characteristics to make defeating monsters easy, and exists solely to let us test some aspects of your program without our having to slog through a regular game.
If you type something not on this list, you do nothing for this turn, but the monsters take their turn

# More details about the code:

Name of goblin movement function: moveGoblinRecursive

Helper functions:
- findDistanceRecursive  
- findPathRecursive (recursive implementation)
- isValidMove
- calculateHeuristicRecursion

These functions can be found in Temple.cpp 

High-level description of goblin movement:
- This function leverages a recursive pathfinding approach to determine the shortest and most efficient path from the goblin's current position to the player's position, avoiding obstacles along the way.
- The moveGoblinRecursive function is designed to move goblins towards the player if the player is reachable within a specified number of steps. The function uses a recursive pathfinding approach based on the A* algorithm to determine the shortest path from the goblin's current position to the player's position, taking into account obstacles. If a valid path is found, the goblin moves one step towards the player.
- The findDistanceRecursive function initializes the pathfinding process by setting up a visited matrix and a path vector. It calls the findPathRecursive function, which performs the actual recursive A* search. This function checks if the current distance exceeds the maximum allowed distance or if the current position matches the player's position. If the path is found, it returns the path coordinates; otherwise, it returns an empty path.
- The findPathRecursive function explores possible moves (up, down, left, right) from the Monster’s current position, marking cells as visited and backtracking if no valid path is found. It sorts potential moves based on their total cost (current distance plus heuristic) to prioritize shorter paths.
- The isValidMove function ensures that a move does not go out of bounds or into a cell occupied by obstacles. It verifies that the target cell is not a wall or another obstacle. The calculateHeuristicRecursion function calculates the Manhattan distance between two points, which is then inputted as the heuristic in the A* search algorithm to estimate the cost of reaching the player's position.



High-level description of level generation:
- The Temple constructor initializes the temple with a grid of cells (MAXROWS (18) x MAXCOLS (70)), initially filled with all wall characters (#).
- The generateRandomBoxes function generates a random number of rooms (boxes) within the grid. Each room's dimensions and position are randomly determined, ensuring they fit within the grid and do not overlap with other rooms.
- Once the rooms are generated, the connectBoxes function sorts the rooms and connects each room to the next using horizontal corridors, ensuring all rooms are accessible.
- The rooms were still being separated, but this time only by one column of #, so I implemented the function replaceHashBetweenRooms to solve this issue. It replaces wall characters between connected rooms to ensure there are clear pathways.
- The player is placed at a random empty location within the temple. If the player is transitioning from a previous level, their attributes/points and inventory are carried over.
- Depending on the level, different game objects are placed in the temple:
  - Stairs are placed on the first four levels, while an idol is placed on the fifth level.
  - A random number of weapons and scrolls are placed at empty locations within the grid, between 2 and 3 (usually 3).
  - The placeWeapons and placeScrolls functions randomly select a type of weapon or scroll and place it in the designated cell.
  - The generateMonsters function determines the number and types of monsters to be placed in the temple based on the current level. The placeBogeyman, placeSnakewoman, placeGoblin, and placeDragon functions place specific types of monsters at random empty cells within the grid. Monsters are also added to a list for easy management and deletion.
- Each position in the temple grid is represented by a cell structure, which indicates if it is occupied by a monster, weapon, scroll, or staircase (and what the display/visual output is) for organized control of adding and removing items/actors on the level.
- The levels of the temple are created dynamically during gameplay, not at compilation time. They are stored in a vector of Temple objects within the Game class and are accessed and modified as the player transitions between levels. This ensures that there is a unique but working layout of the temple levels each time the game is played and that the rooms are always connected and populated with the proper actors and game objects.
