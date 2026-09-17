#pragma once
#include <string>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <conio.h>
#else
int _getch();
#endif

#define wait(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

std::string repeatStr(const std::string& str, int count);
int visibleLength(const std::string& str);
int getTerminalWidth();
std::string formatNumber(int num);
void clear();
void resetCursor();
void flushInput();
bool readInt(int& out);
std::string sanitizeName(const std::string& input);
void typewriterPrint(const std::string& text, int delayMs = 40);
void setTextDelay(bool enabled);
bool isTextDelayEnabled();