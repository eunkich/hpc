#include <iostream>
#include <random>
#include <vector>
#include <set>

double polynomial(double x, const std::vector<double> &coef)
{
    double out = 0;
    double x_power = 1;
    for (int i = 0; i < coef.size(); i++)
    {
        out += coef[i] * x_power;
        x_power *= x;
    }
    return out;
};

double polynomial_prime(double x, const std::vector<double> &coef)
{
    double out = 0;
    double x_power = 1;
    for (int i = 1; i < coef.size(); i++)
    {
        out += i * coef[i] * x_power;
        x_power *= x;
    }
    return out;
};

void newton_step(double &point, double &val, const std::vector<double> &coef)
{
    double f = polynomial(point, coef);
    double f_prime = polynomial_prime(point, coef);

    val = f;
    point -= f / f_prime;
}

int main()
{
    const int MAX_ITER = 1e8;
    const double TOL = 1e-6;

    int degree, iter = 0;
    double start, end;

    std::cout << "Enter the degree of the polynomial: ";
    std::cin >> degree;

    std::vector<double> coef(degree + 1, 0);

    for (int i = degree; i > -1; i--)
    {
        std::cout << "Enter coefficient " << i << ": ";
        std::cin >> coef[i];
    }

    std::cout << "Enter the start of the domain: ";
    std::cin >> start;
    std::cout << "Enter the start of the domain: ";
    std::cin >> end;

    assert(start < end);

    // Start at the middle
    double point = (start + end) / 2;
    double val = polynomial(point, coef);

    std::vector<double> roots;

    while (iter < MAX_ITER)
    {
        iter++;

        if (point > end or point < start)
        {
            // std::cout << "point out of domain" << std::endl;
            // Uniform draw from domain
            double draw = static_cast<double>(rand()) / RAND_MAX;
            point = start + abs(end - start) * draw;
            continue;
        }

        newton_step(point, val, coef);

        if (abs(val) < TOL)
        {
            bool is_new = true;
            for (auto &e : roots)
            {
                if (abs(e - point) < TOL)
                {
                    is_new = false;
                    break;
                }
            }
            if (is_new)
            {
                roots.push_back(point);
                // std::cout << "Roots found: " << point << std::endl;
                if (roots.size() == degree)
                {
                    std::cout << "Found all solutions" << std::endl;
                    break;
                }
            }

            // Add perturbation for starting point
            double step = static_cast<double>(rand()) / RAND_MAX;
            double draw = static_cast<double>(rand()) / RAND_MAX;
            (draw > 0.5) ? point -= step * abs(start - point) : point += step * abs(end - point);

            continue;
        }
    }

    // std::cout << iter << "/" << MAX_ITER << std::endl;

    std::cout << "Roots found: " << std::endl;
    for (auto e : roots)
    {
        std::cout << e << "\n";
    }
}