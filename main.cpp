#include "PlayingArea.hpp"
#include "getChar.hpp"

#include <iostream>

void startup();
void setupBoard( int rows, int columns );
void terminate();

int main( int argc, const char * const argv[] ) {
    int rows = 15;
    int columns = 40;
    // Verify command line arguments
    if( argc > 1 )
        rows = atoi( argv[1] );
    if( argc > 2 )
        columns = atoi( argv[2] );
    // Verifty number of rows and columns
    if( rows < 10 || rows > 25 ) {
        // std::cout << "Number of rows must be 10-25. ";
        // std::cout << "Setting rows to default (15)\n";
        rows = 15;
    }
    if( columns < 10 || columns > 80 ) {
        // std::cout << "Number of columns must be 10-80. ";
        // std::cout << "Setting columns to default (40)\n";
        columns = 40;
    }

	int score = 0;
    std::string scoreString;
	int munchieSize;
	int growSize = 7;
	bool grow = true;
    bool alive = true;
    
    PlayingArea game( rows, columns );

    cursor wormStart;
    wormStart.row = (rows-1) / 2;
    wormStart.column = (columns-1) / 2;

    startup();
    cursor worm = game.insertWorm(wormStart);
    cursor munchie = game.insertMunchie( munchieSize );
    setupBoard(rows, columns);

    char direction = ' ';
    
    while( alive ) {
    	if( growSize < 1 )
    		grow = false;

        direction = get_char();

        if( direction == 'h' || direction == 'j' ||
            direction == 'k' || direction == 'l' ) {
    	   worm = game.move( direction, grow, alive );
           if( growSize > 0 )
               growSize--;
        }

    	if( worm == munchie ) {
    		score += munchieSize;
            scoreString = std::to_string(score);
    		grow = true;
    		growSize = munchieSize;
            mvaddstr(0, columns-3, scoreString.c_str() );
    		munchie = game.insertMunchie( munchieSize );
    	}

        refresh();
    }

    terminate();

    std::cout << "The worm died since it ran into something!\n";
    std::cout << "Your score was: " << score << std::endl;
    
    return 0;
}


void startup() {
     initscr();  /* activate the curses */
     curs_set(0);/* do not display the cursor */
     clear();    /* clear the screen that curses provides */
     noecho();   /* prevent the input chars to be echoed to the screen */
     cbreak();   /* change the stty so that characters are delivered to the
            program as they are typed--no need to hit the return key!*/
}

void setupBoard( int rows, int columns ) {
    mvaddstr(0, 0, "Worm");
    mvaddstr(0, columns-9, "Score");
    mvaddstr(0, columns-3, "0");
    for( int r = 1; r <= rows; r++ ) {
        for( int c = 0; c < columns; c++ ) {
            move(r, c);
            if( r == 1 || r == rows || c == 0 || c == columns-1 ) {
                addch('*');
            }
        }
    }

    refresh();
}


void terminate() {
     mvcur( 0, COLS - 1, LINES - 1, 0 );
     clear();
     refresh();
     endwin();
}
