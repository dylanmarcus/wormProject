TO PLAY THE GAME (in a linux terminal):
	run "./worm.x"
	if that doesn't work:
	1) run "make"
	2) run "./worm.x"
	Old school vi controls:
		h : left
		j: down
		k: up
		l: right

If number of rows and columns are given as arguments in the run command
(i.e: ./worm.x 'rows' 'columns'), the size of the playing area will
be set accordingly.

If an out of range value is entered for either row size or column size,
it will be set to a default value.

If only one in-range argument is given, the number of rows will be
set to that given value, and the number of columns will be set to a
default value.

And if no arguments are given, the number of rows and columns will
be set to default values.


When the game starts, only the worm's head appears. And then the worm
grows by 7 as it moves to make the worm 8 segments long to start the game.

When directional keys (h, j, k, l) are hit, the worm moves accordingly.
Otherwise, any other key input will stop the worm from moving until
one of the directional keys are hit again.