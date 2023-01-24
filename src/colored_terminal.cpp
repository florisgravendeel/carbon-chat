//
// Created by Floris Gravendeel on 21/01/2023.
//
// Copied from (https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal)
#include <ostream>
namespace Color {
    enum Code {
        RESET_ALL_ATTRIBUTES = 0,
        BOLD        = 1,
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49,

        FG_BLACK    = 30,
        FG_YELLOW   = 33,
        FG_MAGENTA  = 95,
        FG_CYAN     = 36,
        FG_LIGHT_GRAY   = 37,
        FG_DARK_GRAY    = 90,
        FG_LIGHT_RED    = 91,
        FG_LIGHT_GREEN  = 92,
        FG_LIGHT_YELLOW = 93,
        FG_LIGHT_BLUE   = 94,
        FG_LIGHT_CYAN = 96,
    };
    std::ostream& operator<<(std::ostream& os, Code code) {
        return os << "\033[" << static_cast<int>(code) << "m";
    }

//    cout << Color::FG_LIGHT_YELLOW << "User joined the chat. Total online users: " << Color::FG_MAGENTA << "2" << endl;
//    cout << Color::FG_BLUE << "Bob: " << Color::FG_LIGHT_BLUE << "How was your day?" << endl;
}