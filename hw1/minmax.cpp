#include <iostream>
#include <cmath>

int main()
{
    float Vf = std::powf(2, 104) * (std::powf(2, 24) - 1);
    float vf = -Vf;
    float nvf = std::powf(2, -126);
    float epsf = 1.19209e-07f;

    std::cout << "Largest float V = " << Vf << std::endl;
    std::cout << "Smallest float v = " << vf << "\n\n";

    std::cout << "Overflow\n";

    std::cout << "V + (2^102 * (2 - eps)) = "
              << Vf + (std::powf(2, 102) * (2.0f - epsf)) << "\n\n";
    std::cout << "V + 2^103 = " << Vf + (std::powf(2, 102) * 2.0f) << "\n\n";

    std::cout << "v - (2^102 * (2 - eps)) = "
              << vf - (std::powf(2, 102) * (2.0f - epsf)) << "\n\n";
    std::cout << "v - 2^103 = " << vf - (std::powf(2, 102) * 2.0f) << "\n\n";

    std::cout << "Underflow\n";

    std::cout << "Smallest positive normal float = " << nvf << "\n\n";
    std::cout << "Smallest positive normal float - eps = " << nvf - epsf << "\n\n";

    double Vd = std::pow(2, 971) * (std::pow(2, 53) - 1);
    double vd = -Vd;
    double nvd = std::pow(2, -1022);
    double epsd = 2.22045e-16;

    std::cout << "Largest double: " << Vd << std::endl;
    std::cout << "Smallest double: " << vd << "\n\n";

    std::cout << "Overflow\n";
    std::cout << "V + (2^969 * (2 - eps)) = "
              << Vd + (std::pow(2, 969) * (2.0 - epsd)) << "\n\n";
    std::cout << "V + 2^970 = " << Vd + (std::pow(2, 969) * 2.0) << "\n\n";

    std::cout << "v - (2^969 * (2 - eps)) = "
              << vd - (std::pow(2, 969) * (2.0 - epsd)) << "\n\n";
    std::cout << "v - 2^970 = " << vd - (std::pow(2, 969) * 2.0) << "\n\n";

    std::cout << "Underflow\n";
    std::cout << "Smallest positive normal double = " << nvd << "\n\n";
    std::cout << "Smallest positive normal double - eps = " << nvd - epsd << "\n\n";

    return 0;
}