#include <iostream>

float rnd(float number)
{
    int temp = number * 1000;
    int ifGreater = number * 10000;
    ifGreater %= 10;
    if (ifGreater >= 5) temp += 1;
    return temp / 1000.;
}

float estimate(float (*matrix), float (*x), float (*y), int n, int m)
{
    float yPred, tempX, sum;
    sum = 0;
    for (int i = 0; i < n; i++)
    {
        yPred = 0;
        tempX = 1;
        for (int j = 0; j < m + 1; j++)
        {
            yPred += tempX * matrix[j];
            tempX *= x[i];
        }
        sum += (y[i] - yPred) * (y[i] - yPred);
    }
    return sum;
}

void findCoefficients(float(*sums), float(*matrix), int m)
{
    float** equations = new float* [m + 1];
    for (int i = 0; i < m + 1; i++)
    {
        equations[i] = new float[m + 2];
    }
    for (int i = 0; i < m + 1; i++)
    {
        for (int j = 0; j < m + 1; j++)
        {
            equations[i][j] = sums[i + j];
        }
        equations[i][m + 1] = sums[i + 2 * (m + 1) - 1];
    }

    float temp;
    for (int i = 0; i < m + 1; i++)
    {
        temp = equations[i][i];
        for (int j = i; j < m + 2; j++)
        {
            equations[i][j] /= temp;
        }
        for (int j = i + 1; j < m + 1; j++)
        {
            temp = equations[j][i];
            for (int k = i; k < m + 2; k++)
            {
                equations[j][k] -= equations[i][k] * temp;
            }
        }
    }
    for (int i = m; i >= 0; i--)
    {
        temp = equations[i][m + 1];
        for (int j = m; j > i; j--)
        {
            temp -= equations[i][j] * matrix[j];
        }
        matrix[i] = rnd(temp);
    }
}

void fillMatrixString(float(*arr), int n, int m, int start, float(*x), float(*y))
{
    float** matrix = new float* [n + 1];
    for (int i = 0; i < n + 1; i++)
    {
        matrix[i] = new float[3 * (m + 1) - 1];
    }
    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 0; j < 3 * (m + 1) - 1; j++)
        {
            matrix[i][j] = 1;
        }
    }
    for (int i = 1; i < 2 * (m + 1) - 1; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[j][i] = x[j] * matrix[j][i - 1];
        }
    }

    for (int j = 0; j < n + 1; j++)
    {
        matrix[j][2 * (m + 1) - 1] = y[j];
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 2 * (m + 1); j < 3 * (m + 1) - 1; j++)
        {
            matrix[i][j] = x[i] * matrix[i][j - 1];
        }
    }
    for (int j = 0; j < 3 * (m + 1) - 1; j++)
    {
        matrix[n][j] = 0;
        for (int i = 0; i < n; i++)
        {
            matrix[n][j] += matrix[i][j];
        }
        matrix[n][j] = matrix[n][j];
    }
    findCoefficients(matrix[n], arr, m);
}

void task1(float(*x), float(*y), int n, int m)
{
    std::cout << "\nn = " << n;
    std::cout << "\nx: ";
    for (int i = 0; i < n; i++)
    {
        std::cout << x[i] << ' ';
    }
    std::cout << "\ny: ";
    for (int i = 0; i < n; i++)
    {
        std::cout << y[i] << ' ';
    }
    std::cout << "\n";

    float** matrix = new float*[m + 1];
    float bestEstimate = float('INF');
    int bestFunctionsIndex = 0;
    float tempEstimate;
    for (int i = 1; i < m + 1; i++)
    {
        matrix[i] = new float[m + 1];
        fillMatrixString(matrix[i], n, i, i, x, y);
        tempEstimate = estimate(matrix[i], x, y, n, i);
        if (tempEstimate < bestEstimate)
        {
            bestEstimate = tempEstimate;
            bestFunctionsIndex = i;
        }
    }
    std::cout << "Function is: ";
    for (int i = 0; i < bestFunctionsIndex; i++)
    {
        std::cout << matrix[bestFunctionsIndex][i] << "x^" << bestFunctionsIndex - i << " + ";
    }
    std::cout << matrix[bestFunctionsIndex][bestFunctionsIndex] << "\nIt's estimate: " << rnd(bestEstimate) << '\n';
}

int main()
{
    std::cout << "This is the forth lab of project design.\n\n";
    std::cout << "First task is to least squares approximation of a function.\n";
    /*std::cout << "Example:\n";
    float x[5] = { 0.78, 1.56, 2.34, 3.12, 3.81 };
    float y[5] = { 2.50, 1.20, 1.12, 2.25, 4.28 };
    task1(x, y, 5, 2);*/

    float x[20];
    for (int i = 0; i < 20; i++) x[i] = 0.1 * i;
    float y[20] = { 3.15, 3.04, 3.02, 2.97, 2.87, 2.98, 2.81, 2.70, 2.66, 2.50, 2.60, 2.36, 2.09, 2.07, 2.01, 1.81, 1.53, 1.64, 1.29, 1.11 };
    task1(x, y, 20, 4);
}