Mason Prosser
I worked alone on this project.

For my final project, I recreated the classic arcade game Tetris. As rows are
cleared, the game gets progressively faster, and the game ends when a new
block cannot be generated without being placed above the top of the playing
board. Blocks cannot be moved or rotated past any edge, and two blocks cannot
overlap in any way.

The controls are very simple:

[LEFT ARROW KEY]: Moves the active block to the left
[RIGHT ARROW KEY]: Moves the active block to the right
[DOWN ARROW KEY]: Moves the active block down

[Z]: Rotates the active block to the left (counterclockwise)
[X]: Rotates the active block to the right (clockwise)

RULES FOR ROTATING:
A block can only be rotated if there are no blocks or edges preventing it
from doing so properly. If the active block is against an edge and cannot be
rotated properly, the rotate button will move the block away from the edge
and then rotate the block. If another block is preventing the active block
from rotating, then the active block will not rotate. However, a clever player
can still slide a block into a tight space if he/she knows how the block will
rotate.

[SPACE]: Moves the active block straight down as far as it can, then generates
	 a new block

[Q]: The q key can be pressed at any time to quit the game
