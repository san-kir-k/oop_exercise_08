// Киреев Александр Константинович 206
#pragma once
#include <iostream>
#include <fstream>

struct Dot {
    public:
        double X, Y;
        Dot();
        Dot(double x, double y);
        Dot(const Dot& d);
        ~Dot() = default; 
};

std::istream& operator>>(std::istream& is, Dot& d);
std::ostream& operator<<(std::ostream& os, const Dot& d);
std::ofstream& operator<<(std::ofstream& os, const Dot& d);
