# Dungeon Crawl
A implementation of dungeon crawl game in c++.<p>
Check this [link](http://www.cplusplus.com/forum/articles/12974/) for more details.

## What does this game do
> Requires:<p>
variables, data types, and numerical operators
basic input/output
logic (if statements, switch statements)
loops (for, while, do-while)
arrays <p>
Make a program that outputs a simple grid based gameboard to the screen using either numbers or characters.
i.e.   <p>
. . . . . . . . . .<p>
. G . . . . . . . .<p>
. . . . . . T . . .<p>
. . . . . . . . . .<p>
. . . . T . . . . .<p>
. . . . . . T . . .<p>
. . . . . . . . . X<p>
or <p>
0 0 0 0 0 0 0 0 0 0 <p>
0 5 0 0 6 0 0 0 0 0 <p>
0 0 0 0 0 0 7 0 0 0 <p>
0 0 0 0 0 0 0 0 0 0 <p>
0 0 0 7 0 0 0 0 0 0 <p>
0 0 0 0 0 0 7 0 0 0 <p>
0 0 0 0 0 0 0 0 0 4 <p>
> Allow the user (marked by G in the example) to move either up, down, left, or right each turn. If the player steps on a trap then they lose. If the make it to the treasure 'X' then they win.<p>
> ★★ Add enemies that move randomly in any direction once per turn. (enemies just like traps cause the player to lose if touched)<p>
> HINT: Don't let the player move off the gameboard! You program will crash if they move off the top or bottom of the board!<p>
(the same holds true for enemies)<p>

## How to play this game
```bash
mkdir build
cd build
cmake ..
make -j4
```
Then you will find a executable file named dungeon_crawl in this build folder.

## License
MIT.See [LICENSE](LICENSE) for more details.