#include <iostream>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <sys/time.h>

#include "Body/Body.h"

using namespace std;

int main(int argc, char * const argv[]) {
	Body* mBody;

	Body::BodyType bodyType = Body::Regular;
#ifdef DEBUG_X86
	mBody = new Body("/dev/ttyUSB0", bodyType);
#else
	mBody = new Body("/dev/ttySS0", bodyType);
#endif

	mBody->startBootloader();

	delete mBody;
	return 0;
}
