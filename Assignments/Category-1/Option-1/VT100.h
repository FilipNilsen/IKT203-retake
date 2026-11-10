#pragma once
#include <cstdint>
#include <string>

//! Documentation in vt100 is copied from: https://en.wikipedia.org/wiki/ANSI_escape_code#CSIsection
namespace vt100 {
    void CUU(int n);

    /**
     * @brief Moves the cursor to column n. (not ANSI.SYS)
     */
    std::string CHA(uint8_t n);

    /**
     * @brief Clears part of the screen.
     * @param n If n is 0, clear from cursor to end of screen. If n is 1, clear from cursor to beginning of the screen. If n is 2, clear entire screen (and moves cursor to upper left on DOS ANSI.SYS). If n is 3, clear entire screen and delete all lines saved in the scrollback buffer (this feature was added for xterm and is supported by other terminal applications).
     */
    void ED(int n);
    //void CUF(int n);
    void CUD(int n);
}