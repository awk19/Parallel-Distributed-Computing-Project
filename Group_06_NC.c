#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double fact(double number)
{
    double num = 1;

    for (int i = 1; i <= number; i++)
    {
        num = num * i;
    }

    return num;
}

double forwardCalc_p(double p, int i)
{

    double temp = p;
    for (int j = 1; j < i; j++)
    {
        temp = temp * (p - j);
    }

    return temp;
}

double backwardCalc_p(double p, int i)
{
    double temp = p;
    for (int j = 1; j < i; j++)
    {
        temp = temp * (p + j);
    }

    return temp;
}

int main(void)
{

    clock_t start, end;
    double time_used;

    int size;
    printf("Enter Number Of Data: ");
    scanf("%d", &size);

    start = clock();

    double dataX[size];
    double dataY[size][size];
    double dataFY[size][size];
    double dataBY[size][size];
    int num1 = 20;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            dataY[i][j] = 0;
        }
    }

    for (int i = 0; i < size; i++)
    {
        dataX[i] = num1;
        num1 = num1 + 15;
    }

    printf("\n");

    for (int i = 0; i < size; i++)
    {
        dataY[i][0] = (double)(rand() % 100) / 100000;
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            dataFY[i][j] = dataY[i][j];
        }
    }

    printf("\n");

    for (int i = 1; i < size; i++)
    {
        for (int j = 0; j < size - i; j++)
        {
            dataFY[j][i] = dataFY[j + 1][i - 1] - dataFY[j][i - 1];
        }
    }

    printf("Forward Difference Table is: \n");

    for (int i = 0; i < size; i++)
    {
        printf("X: %.01f", dataX[i]);
        for (int j = 0; j < size - i; j++)
        {
            printf("\t Y: %lf", dataFY[i][j]);
        }
        printf("\n");
    }

    int FResult, l = 0;
    double sumF, x;

    x = 24;

    sumF = dataFY[0][0];
    double pf, f, pRes;
    double num;

    pf = (x - dataX[0]) / (dataX[1] - dataX[0]);

    for (int k = 1; k < size; k++)
    {
        f = fact(k);
        sumF = sumF + (forwardCalc_p(pf, k) * dataFY[0][k]) / f;
    }

    printf("\nSumF: %lf", sumF);

    printf("\n");

    printf("Backward Difference Table is: \n");

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            dataBY[i][j] = dataY[i][j];
        }
    }

    for (int i = 1; i < size; i++)
    {
        for (int j = size - 1; j > i - 1; j--)
        {
            dataBY[j][i] = dataBY[j][i - 1] - dataBY[j - 1][i - 1];
        }
    }
    printf("\n");

    for (int i = 0; i < size; i++)
    {
        printf("X: %.01f", dataX[i]);
        for (int j = 0; j < i + 1; j++)
        {
            printf("\tY: %lf", dataBY[i][j]);
        }
        printf("\n");
    }

    double x1;

    x1 = 90;

    double sumB = dataBY[size - 1][0];
    double pb = (x1 - dataX[size - 1]) / (dataX[1] - dataX[0]);

    for (int k = 1; k < size; k++)
    {
        f = fact(k);
        sumB = sumB + (backwardCalc_p(pb, k) * dataBY[size - 1][k]) / f;
    }

    printf("\nSumB: %lf", sumB);

    printf("\n");

    printf("Central Difference Table is: ");

    double dataCX[5];
    double dataCY[5][5];

    for (int i = 0; i < 5; i++)
    {
        dataCX[i] = 10 + i;
    }

    dataCY[0][0] = 0.23967;
    dataCY[1][0] = 0.28069;
    dataCY[2][0] = 0.31788;
    dataCY[3][0] = 0.35209;
    dataCY[4][0] = 0.38368;

    for (int i = 1; i < 5; i++)
    {
        for (int j = 0; j < 5 - i; j++)
        {
            dataCY[j][i] = dataCY[j + 1][i - 1] - dataCY[j][i - 1];
        }
    }

    printf("\n");

    for (int i = 0; i < 5; i++)
    {
        printf("X: %.01f", dataCX[i]);
        for (int j = 0; j < 5 - i; j++)
        {
            printf("\tY: %lf", dataCY[i][j]);
        }
        printf("\n");
    }

    double sumC, s, cx = 12.2;
    printf("\nX0: %lf", dataCX[5 / 2]);

    s = (cx - dataCX[5 / 2]) / (dataCX[1] - dataCX[0]);
    double s2 = s * s;

    sumC = dataCY[5 / 2][0] + s * ((dataCY[1][1] + dataCY[2][1]) / 2) + (s2 * (dataCY[1][2]) / fact(2)) + (s * (s2 - 1) * ((dataCY[0][3] + dataCY[1][3]) / 2)) / fact(3) + (s2 * (s2 - 1) * (dataCY[0][4])) / fact(4);

    printf("\nSumC: %lf", sumC);

    end = clock();

    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime:%lf", time_used);
}