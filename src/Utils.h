#pragma once
#include <string>
#include <thread>
#include <chrono>

#define wait(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

std::string repeatStr(const std::string& str, int count);
int visibleLength(const std::string& str);
int getTerminalWidth();
std::string formatNumber(int num);
void clear();
void resetCursor();
int _getch();
void flushInput();
void typewriterPrint(const std::string& text, int delayMs = 40);
void setTextDelay(bool enabled);
bool isTextDelayEnabled();