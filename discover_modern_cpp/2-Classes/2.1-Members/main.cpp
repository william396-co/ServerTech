#include <iostream>

class complex
{
public:
    double r, i;
};

int main()
{
    complex z, c;
    z.r = 3.5, z.i = 2;
    c.r = 2;
    z.i = -3.5;
    std::cout << "z is (" << z.r << "," << z.i << ")\n";

    return 0;
}

