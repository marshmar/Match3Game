#include "Utils.h"

int Debug::nDebugLine;
char Debug::whiteSpaces[160] = { '\0' };

void Debug::Log(const char* fmt, ...) {
	char dest[1024];
	static clock_t startTime = clock();


	if (whiteSpaces[0] == '\0') {
		memset(whiteSpaces, ' ', 160);
		whiteSpaces[159] = '\0';
	}

	va_list argPtr;
	va_start(argPtr, fmt);


	Borland::GotoXY(47, nDebugLine++ % 49);

	vsprintf(dest, fmt, argPtr);
	va_end(argPtr);
	printf("[%5d: %.3f] %s", nDebugLine, (clock() - startTime) / 1000.0f, dest);
	printf("\n%s", whiteSpaces);
}
