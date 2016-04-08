#ifndef GAME_DEBUG_LOG_H
#define GAME_DEBUG_LOG_H

#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <time.h>
#include <iostream>

//Creates a console window
void GameDebugLogStartUp();

void GameDebugLog(char *string);
void GameDebugLog(const char *string);

#endif