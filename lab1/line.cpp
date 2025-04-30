//
// Created by egor on 4/22/25.
//
#include <iostream>

#include <math.h>
#include <sys/time.h>
#include <fstream>

//ut + ux = x + t
//ux0 = exp(-t)
//ut0 = cos(pi x)




int main()
{
    const double T = 1.0;
    const double X = 1.0;

    const double tau = 0.1;
    const double h = 0.2;

    const int K = int(T / tau);
    const int M = int(X / h);

    double Cell[K+1][M+2];

    for(int i = 0; i < K + 1; i++)
    {
        for(int j = 0; j < M + 2; j++)
        {
            Cell[i][j] = 0.0;
        }
    }

    struct timeval ts, te;
    gettimeofday(&ts, NULL);
    for(int i = 1; i < M+1; i++)
    {
        Cell[0][i] = std::cos(M_PI * h * i);
    }
    for(int i = 0; i < K; i++)
    {
        Cell[i][1] = std::exp(-1 * i * tau);
    }

    for(int i = 0; i < K; i++)
    {
        for(int j = 1; j < M + 1; j++)
        {
            Cell[i+1][j] = Cell[i][j] + tau*( i * tau + h * (j-1) + tau * (Cell[i][j+1] - 2 * Cell[i][j] + Cell[i][j-1]) / (2 * h*h) - (Cell[i][j+1] - Cell[i][j-1]) / (2 * h));
        }

    }

    std::ofstream out("data");
    for(int i = 0; i < K; i++)
    {
        for(int j = 1; j < M+1; j++)
        {
            out << Cell[i][j] << "\t";
//            std::cout << Cell[i][j] << " ";
        }
        out << std::endl;
//        std::cout << std::endl;
    }
    gettimeofday(&te, NULL);
    double elapsed;
    elapsed = (te.tv_sec - ts.tv_sec) * 1000;
    elapsed += (te.tv_usec - ts.tv_usec);
    std::cout << "Elapsed time: " << elapsed << std::endl;
}
