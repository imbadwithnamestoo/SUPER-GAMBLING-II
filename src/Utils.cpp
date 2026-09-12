#include "Utils.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>

int _getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

static bool textDelayEnabled = true;

void setTextDelay(bool enabled) {
    textDelayEnabled = enabled;
}

bool isTextDelayEnabled() {
    return textDelayEnabled;
}

void flushInput() {
#ifdef _WIN32
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
    tcflush(STDIN_FILENO, TCIFLUSH);
#endif
}

void typewriterPrint(const std::string& text, int delayMs) {
    if (!textDelayEnabled || delayMs <= 0) {
        std::cout << text << std::flush;
        return;
    }

    for (char c : text) {
        std::cout << c << std::flush;
        wait(delayMs);
    }
}

std::string formatSuffix(double value, const std::string& suffix) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.2f", value);
    std::string s(buffer);

    if (s.find('.') != std::string::npos) {
        while (!s.empty() && s.back() == '0') {
            s.pop_back();
        }
        if (!s.empty() && s.back() == '.') {
            s.pop_back();
        }
    }

    return s + suffix;
}

std::string formatNumber(int num) {
    if (num < 0) {
        return "-" + formatNumber(-num);
    }

    if (num >= 1000000000) {
        return formatSuffix(num / 1000000000.0, "B");
    } else if (num >= 1000000) {
        return formatSuffix(num / 1000000.0, "M");
    } else if (num >= 1000) {
        return formatSuffix(num / 1000.0, "K");
    } else {
        return std::to_string(num);
    }
}

std::string repeatStr(const std::string& str, int count) {
    if (count <= 0) return "";
    std::string result;
    result.reserve(str.length() * count);
    for (int i = 0; i < count; i++) {
        result += str;
    }
    return result;
}

int visibleLength(const std::string& str) {
    int length = 0;
    for (size_t i = 0; i < str.length();) {
        unsigned char ch = static_cast<unsigned char>(str[i]);

        if (ch == '\033') {
            if (i + 1 < str.length() && str[i + 1] == '[') {
                i += 2;
                while (i < str.length()) {
                    unsigned char code = static_cast<unsigned char>(str[i++]);
                    if (code >= 0x40 && code <= 0x7E)
                        break;
                }
            } else {
                ++i;
            }
            continue;
        }

        if ((ch & 0x80) == 0) {
            ++i;
        } else if ((ch & 0xE0) == 0xC0) {
            i += 2;
        } else if ((ch & 0xF0) == 0xE0) {
            i += 3;
        } else if ((ch & 0xF8) == 0xF0) {
            i += 4;
        } else {
            ++i;
        }
        ++length;
    }
    return length;
}

int getTerminalWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
}

void clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void resetCursor() {
    std::cout << "\033[H";
}