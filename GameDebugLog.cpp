#include "GameDebugLog.h"

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

void GameDebugLogStartUp()
{
	AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;

	std::cout << "Debug Window Created" << std::endl;
}
void GameDebugLog(char *string)
{
	//Get the current time
	time_t currentTime = time(0);
	tm *now = localtime(&currentTime);

	int day = now->tm_mday;
	int hour = now->tm_hour;
	int min = now->tm_min;
	int sec = now->tm_sec;

	//Append the time then string to a log file
	std::fstream file;

	if(!file.bad())
		std::cout << hour << ":" << min << ":" << sec << " - " << string << std::endl;

	file.close();
}

void GameDebugLog(const char *string)
{
	//Get the current time
	time_t currentTime = time(0);
	tm *now = localtime(&currentTime);

	int day = now->tm_mday;
	int hour = now->tm_hour;
	int min = now->tm_min;
	int sec = now->tm_sec;

	//Append the time then string to a log file
	std::fstream file;

	if(!file.bad())
		std::cout << hour << ":" << min << ":" << sec << " - " << string << std::endl;

	file.close();
}
