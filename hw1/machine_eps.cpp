#include <iostream>

int main()
{
    float epsf = 1;
    double epsd = 1;

    float ansf;
    double ansd;

    while (1 - (1 + epsf) != 0)
    {
        ansf = epsf;
        epsf /= 2;
    }

    while (1 - (1 + epsd) != 0)
    {
        ansd = epsd;
        epsd /= 2;
    }

    std::cout << "SP Machine Eps: " << ansf << std::endl;
    std::cout << "DP Machine Eps: " << ansd << std::endl;

    return 0;
}
