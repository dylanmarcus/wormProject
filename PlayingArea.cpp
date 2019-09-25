/*
The PlayingArrea class uitilizes: a two-dimensional array for
the game board, a queue for the worm, and an array for the free pool,
and a cursor struct which holds row and column integers.

The game board 2d array has the number of rows and columns specified
by the client then fills the array with '-1's as its border, and
integers sequentially ranging from 0 to the number of game cells
within the borders (minus 1). These values are references to indecies
in the free pool.

The free pool is an array of cursors representing which cells are
available so a munchies can be placed in cells that are not taken
up by the worm. And the worm is a queue of cursors representing
cells that the worm occupies.

When the game starts, the worm is only a head until it starts moving.
To make the worm 8 segments long, the main function gives the worm a
munchie of value 7. This does not add to the score, but simply
provides a way to start the worm with 8 segments.
When the worm moves and is not growing from a munchie, the correspoding
board value that the worm head is moving into is replaced by the
soon-to-be old last worm tail segment in the freepool. In the case that
the worm is growing, that space in the freepool is replaced by lastIndex
which is then decremented to take one spot out of the free pool.
Then, the board's values are changed to make sure the cursor values in
the free pool provide the coordinates to the correct value on the board.

The worm queue reflects these changes by pushing the new head value
onto the queue and if the worm is not growing, its last tail segment
is popped off (if the worm is growing, simply don't pop).

While the initialization of all of the game presentation in curses
(along with the changing of the score) is dealt with in the
main function, any curses placement regarding the worm is managed
within this class. This way, all worm changing logic is together.
*/

#include "PlayingArea.hpp"

PlayingArea::PlayingArea( int numRows, int numColumns ) {
    rows = numRows;
    columns = numColumns;
    
    // allocate space for board and freePool
    board = new int *[rows];
    freePool = new cursor[( (rows-2) * (columns-2) ) + 1];
    
    for( int i = 0; i < rows; i++ ) {
        board[i] = new int [columns];
    }
    
    // fill board and freePool
    int square = 0;
    cursor currentCursor;
    for( int r = 0; r < rows; r++ ) {
        for( int c = 0; c < columns; c++ ) {
            if( r == 0 || r == rows-1 || c == 0 || c == columns-1 ) {
                board[r][c] = -1;
            } else {
                board[r][c] = square;
                currentCursor.row = r;
                currentCursor.column = c;
                freePool[square] = currentCursor;
                square++;
            }
        }
    }
    lastIndex = square;
    fpSize = square + 1;
}


PlayingArea::~PlayingArea() {
    for( int i = 0; i < rows; i++ ) {
        delete [] board[i];
    }
    delete [] board;

    delete [] freePool;
}


cursor PlayingArea::insertWorm( cursor wormStart ) {	
	// The new head calls for one element being removed from the freePool
	// So the index that matches that value on the the gameboard is
	// replaced by the last legal element in the freePool
	// lastIndex is decremented to account for the new worm head
	lastIndex--;
	freePool[ board[wormStart.row][wormStart.column] ] = 
		freePool[lastIndex];

	// Update the gameboard to represent the changes
	board[ freePool[lastIndex].row ][ freePool[lastIndex].column ] =
		board[wormStart.row][wormStart.column];
	board[wormStart.row][wormStart.column] = -1;

	// add head segment to the worm
	worm.push(wormStart);

    // Place worm head in curses
    mvaddch( wormStart.row + 1, wormStart.column, '@' );

	return wormStart;
}


cursor PlayingArea::insertMunchie( int &size ) {
	// Generate random index in the freePool and return that cell
	srand( time( NULL ) );
	int position = rand() % (lastIndex - 1);
	size = rand() % 9 + 1;

    // Place munchie in curses
    std::string sizeString = std::to_string(size);
    mvaddstr( freePool[position].row + 1, freePool[position].column,
        sizeString.c_str() );

	return freePool[position];
}


cursor PlayingArea::move( char direction, bool grow, bool &alive ) {
    cursor newCell = worm.back();
    cursor oldCell = worm.back();
    if( direction == 'h' ) { // move left
        newCell.column -= 1;
    }
    else if( direction == 'j' ) { // move down
        newCell.row += 1;
    }
    else if( direction == 'k' ) { // move up
        newCell.row -= 1;
    }
    else if( direction == 'l' ) { // move right
        newCell.column += 1;
    }

    // Kill the worm if user moves to a -1 board cell
    if( board[newCell.row][newCell.column] == -1 ) {
    	alive = false;
        return newCell;
    }

    // If the snake is growing, simply take its new position
    // out of the freepool and decrement lastIndex
    if( grow ) {
    	lastIndex--;
    	freePool[ board[newCell.row][newCell.column] ] =
    		freePool[lastIndex];
    }
    // Otherwise, the new head of the worm will no longer be in the freePool
    // So take the head's value out of the free pool and replace
    // it with the newly free'd up cell from the worm's old tail
    else {
    	freePool[ board[newCell.row][newCell.column] ] = worm.front();
	}

    // update the board to reflect the correct changes
    if( !grow ) {
    	board[worm.front().row][worm.front().column] =
    		board[newCell.row][newCell.column];
        // Remove worm tail in curses
        mvaddch( worm.front().row + 1, worm.front().column, ' ' );
    }
    board[newCell.row][newCell.column] = -1;
    // Add worm head and body segment in curses
    mvaddch( newCell.row + 1, newCell.column, '@' );
    mvaddch( oldCell.row + 1, oldCell.column, 'o' );
    // add the newCell to the worm's head and remove its backmost tail
    worm.push(newCell);
    if( !grow ) {
    	worm.pop();
    }

    return newCell;
}
