#include <cstdio>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "util.h"

int util_getch(void)
{
	struct termios oldt, newt;
	int ch=-1;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	fcntl(STDIN_FILENO, F_SETFL, FNDELAY);
	ch = getchar();
	fcntl(STDIN_FILENO, F_SETFL, 0);
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}
