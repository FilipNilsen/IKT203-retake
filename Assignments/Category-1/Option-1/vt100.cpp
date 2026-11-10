//
// Created by filip on 07.07.25.
//
#include <iostream>
#include <string>
#include "VT100.h"

std::string vt100::CHA(uint8_t n) {
    return "\033["+std::to_string(n)+"G";
}

void vt100::CUU(int n) {
    std::cout<<"\033["<<n<<"A";
}

void vt100::ED(int n) {
    std::cout<<"\033["<<n<<"J";
}

void vt100::CUD(int n) {
    std::cout<<"\033["<<n<<"B";
}