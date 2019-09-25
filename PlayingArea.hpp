#include <queue>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <curses.h>

struct cursor {
    int row;
    int column;

    friend bool operator == ( const cursor &l, const cursor &r ) {
        return l.row == r.row && l.column == r.column;
    }
};

class PlayingArea {
public:
    PlayingArea( int numRows, int numColumns );
    ~PlayingArea();
    cursor move( char direction, bool grow, bool &alive );
    cursor insertWorm( cursor wormStart );
    cursor insertMunchie( int &size );
    
private:
    int rows, columns;
    std::queue<cursor> worm;
    int **board;
    int lastIndex;
    cursor *freePool;
    int fpSize;
};
