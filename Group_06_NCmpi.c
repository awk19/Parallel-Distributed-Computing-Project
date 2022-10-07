#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double fact(double number)
{
    double num = 1;
    for (int i = 1; i <= number; i++)
        num = num * i;
    return num;
}

double forwardCalc_p(double p, int i)
{
    double temp = p;
    for (int j = 1; j < i; j++)
        temp = temp * (p - j);
    return temp;
}

double backwardCalc_p(double p, int i)
{
    double temp = p;
    for (int j = 1; j < i; j++)
        temp = temp * (p + j);
    return temp;
}

int main(int argc, char* argv[])
{
    clock_t start, end;
    double time_used, f, pRes, num, x = 24, x1 = 90, sumC, s, cx = 12.2;
    int size, num1 = 20, FResult, l = 0;
    
    printf("Enter Number Of Data: ");
    scanf("%d", &size);

    start = clock();

	double dataCX[5];
    double dataCY[5][5];
    double dataX[size];
    double dataY[size][size];
    double dataFY[size][size];
    double dataBY[size][size];

	//Intialize dataY elements with zero
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            dataY[i][j] = 0;
	//Intialize dataX elements with num1
    int psize, my_rank, root_rank = 0, my_value;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);
    if(mpisize != size)
    {
        printf("This application is meant to be run with size processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    my_value = (my_rank + 1) * 15;
    if(my_rank == root_rank)
    {
        MPI_Gather(&my_value, 1, MPI_INT, dataY[][0], 1, MPI_INT, root_rank, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Gather(&my_value, 1, MPI_INT, NULL, 0, MPI_INT, root_rank, MPI_COMM_WORLD);
    }
    MPI_Finalize();
	//Intialize dataY Column 1 elements
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);
    if(mpisize != size)
    {
        printf("This application is meant to be run with size processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    my_value = (double)(rand() % 100) / 100000;
    if(my_rank == root_rank)
    {
        MPI_Gather(&my_value, 1, MPI_INT, dataY[][0], 1, MPI_INT, root_rank, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Gather(&my_value, 1, MPI_INT, NULL, 0, MPI_INT, root_rank, MPI_COMM_WORLD);
    }
    MPI_Finalize();    
        
        
        
        
        
        
        
        
        
    printf("\n\nForward Difference Table is: \n");    
	//Intialize dataFY elements with dataY
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            dataFY[i][j] = dataY[i][j];
	//Calculation of dataFY
    for (int i = 1; i < size; i++)
        for (int j = 0; j < size - i; j++)
            dataFY[j][i] = dataFY[j + 1][i - 1] - dataFY[j][i - 1];
	//Display dataFY
    for (int i = 0; i < size; i++)
    {
        printf("X: %.01f", dataX[i]);
        for (int j = 0; j < size - i; j++)
            printf("\t Y: %lf", dataFY[i][j]);
        printf("\n");
    }
    double sumF = dataFY[0][0];
	double pf = (x - dataX[0]) / (dataX[1] - dataX[0]);
	//Calculation of SumF
    for (int k = 1; k < size; k++)
    {
        f = fact(k);
        sumF = sumF + (forwardCalc_p(pf, k) * dataFY[0][k]) / f;
    }
    printf("\nSumF: %lf", sumF);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    printf("\nBackward Difference Table is: \n\n");
	//Intialize dataBY elements with dataY
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            dataBY[i][j] = dataY[i][j];
	//Calculation of dataBY
    for (int i = 1; i < size; i++)
        for (int j = size - 1; j > i - 1; j--)
            dataBY[j][i] = dataBY[j][i - 1] - dataBY[j - 1][i - 1];
	//Display dataBY
    for (int i = 0; i < size; i++)
    {
        printf("X: %.01f", dataX[i]);
        for (int j = 0; j < i + 1; j++)
            printf("\tY: %lf", dataBY[i][j]);
        printf("\n");
    }
    double sumB = dataBY[size - 1][0];
    double pb = (x1 - dataX[size - 1]) / (dataX[1] - dataX[0]);
    //Calculation of SumB
    for (int k = 1; k < size; k++)
    {
        f = fact(k);
        sumB = sumB + (backwardCalc_p(pb, k) * dataBY[size - 1][k]) / f;
    }
    printf("\nSumB: %lf", sumB);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    printf("\nCentral Difference Table is: \n");
    //Intialize dataCX elements
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);
    if(mpisize != 5)
    {
        printf("This application is meant to be run with 5 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    my_value = 10 + my_rank;
    if(my_rank == root_rank)
    {
        MPI_Gather(&my_value, 1, MPI_INT, dataCX, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Gather(&my_value, 1, MPI_INT, NULL, 0, MPI_INT, root_rank, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    dataCY[0][0] = 0.23967;
    dataCY[1][0] = 0.28069;
    dataCY[2][0] = 0.31788;
    dataCY[3][0] = 0.35209;
    dataCY[4][0] = 0.38368;
    //Calculation of dataCX
    for (int i = 1; i < 5; i++)
        for (int j = 0; j < 5 - i; j++)
            dataCY[j][i] = dataCY[j + 1][i - 1] - dataCY[j][i - 1];
    //Display dataCX
    for (int i = 0; i < 5; i++)
    {
        printf("X: %.01f", dataCX[i]);
        for (int j = 0; j < 5 - i; j++)
            printf("\tY: %lf", dataCY[i][j]);
        printf("\n");
    }
    printf("\nX0: %lf", dataCX[5 / 2]);
    s = (cx - dataCX[5 / 2]) / (dataCX[1] - dataCX[0]);
    double s2 = s * s;
    //Calculation of SumC
    sumC = dataCY[5 / 2][0] + s * ((dataCY[1][1] + dataCY[2][1]) / 2) + (s2 * (dataCY[1][2]) / fact(2)) + (s * (s2 - 1) * ((dataCY[0][3] + dataCY[1][3]) / 2)) / fact(3) + (s2 * (s2 - 1) * (dataCY[0][4])) / fact(4);
    printf("\nSumC: %lf", sumC);











    end = clock();

    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime:%lf", time_used);
}
