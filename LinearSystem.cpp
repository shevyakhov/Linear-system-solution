// LinearSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;
void init_2d(float** A, int row, int col) {
    for (int i = 0; i < row; i++) {
        A[i] = new float[col+1];
    }
}
void toReadEq(FILE* F, float** Arr, int row, int col) {
    while (!feof(F)) {
        for (int i = 0; i < row;i++) {
            for (int j = 0; j < col + 1;j++) {
                fscanf(F, "%f", &Arr[i][j]);
            }
        }
    }
}
void readLines(FILE* F) {
    int x, y, z, e;
    for (int i = 0; i < 4;++i){
    }
    fscanf(F,"%dx%dy%dz=%d",&x,&y,&z,&e);
    printf("%d %d %d %d\n", x,y,z,e);
}
int searchMaxRow(float **A ,  int rowNum , int row) {
    int max = rowNum;
    int min = rowNum;
    for (int i = rowNum; i < row; i++) {
        if (abs(A[i][rowNum]) > abs(A[max][rowNum])) {
            max = i;
        }
    }
   return max; 
}
void multAll(float** A, int row, int col, float mult) {
    for (int i = 0; i < col + 1; i++) {
        A[row][i] *= mult;
    }
}
void substrRow(float* rowMain, float* rowSub, int col) {
    for (int i = 0; i < col + 1; i++) {
        rowSub[i] -= rowMain[i];
    }
}

void makeZeroBelow(float** A, int row, int col, int currentRow) {
    float diff;
    for (int i = currentRow + 1; i < row; i++) {
        if (A[i][currentRow] != 0) {
            diff = float(A[currentRow][currentRow] / A[i][currentRow]);
            multAll(A, i, col, diff);
            substrRow(A[currentRow], A[i], col);
        }
    }
}
void makeZeroAbove(float** A, int row, int col, int currentRow) {
    float diff;
    if (A[currentRow][currentRow] != 0) {
        for (int i = currentRow - 1; i > -1; i--) {
            if (A[i][currentRow] != 0) {
                diff = float(A[currentRow][currentRow] / A[i][currentRow]);
                multAll(A, i, col, diff);
                substrRow(A[currentRow], A[i], col);
            }
        }
    }
}
void makeStair(float** A, int row, int col) {
    float* swap;
    int k;
    for (int i = 0; i < row;i++) {
         k = searchMaxRow(A, i, row);
         swap = A[i];
         A[i] = A[k];
         A[k] = swap;
         makeZeroBelow(A, row, col, i);
    }
    for (int i = 0; i < row;i++) {
         makeZeroAbove(A, row, col, i);
    }
   
}


void Paste(FILE* F,float** A, int row, int col) {
    for (int i = 0; i < row ;i++) {
        for (int j = 0; j < col + 1; j++) {
            if (A[i][j] == (long)A[i][j]) {
                fprintf(F, "%d ", (long)A[i][j]);
            }else
                fprintf(F, "%.2f ", A[i][j]);
        }
        fprintf(F, "\n");
    }
    fprintf(F, "======================\n");
}

bool checkIfStair(float** A,  int row, int col) {
    int count = 0;
    if (row < col) {
        for (int i = 0; i < row;i++) {
            if (A[i][i] != 0) {
                ++count;
            }
        }
    }else
    {
        for (int i = 0; i < col;i++) {
            if (A[i][i] != 0) {
                ++count;
            }
        }
    }
    return count == col;
}
void getX(float** A, float* X, int row, int col, int curCol) {
    float result = A[curCol][col];
   
    for (int i = col-1; i != curCol; i--) {
        result = result - X[i]*A[curCol][i];
    }
    result /= A[curCol][curCol];
    X[curCol] = result;
}
void PasteSolution(FILE* F, float* X, int col) {
    for (int i = 0; i < col ; i++) {
        if (X[i] == (long)X[i]) {
            fprintf(F, "x%d = %d;\n",i, (long)X[i]);
        }
        else
            fprintf(F, "x%d = %.2f;\n",i, X[i]);
    }
}
int* getLooseX(float** A, float* X, int row, int col) {
    int* looseFlag = new int[col];
    for (int i = 0; i < col;i++) {
        looseFlag[i] = 0;
    }
    for (int i = 0; i < row;i++) {
        if (A[i][i] != 0) {
            looseFlag[i] = 1;
        }
    }
    
    return looseFlag;
}
void generalSolution(FILE* F, float** A, float* X, int row, int col) {
    int* flag = getLooseX(A, X, row, col);
    for (int i = 0; i < col; i++) {
        if (flag[i] == 0) {
            cout << "x" << i << " - is a loose variable - input its value" << endl;
            cin >> X[i];
        }
    }
    for (int i = col - 1; i > -1; i--) {
        if (flag[i] != 0) {
            getX(A, X, row, col, i);
        }
    }
    PasteSolution(F, X, col);
}
bool isSolving(float** A, int row, int col) {
    float value = 0, count = 0;
    for (int i = 0; i < row; i++) {
        value = A[i][col];
        count = 0;
        for (int j = 0; j < col;j++) {
            if (A[i][j] != 0) {
                count++;
            }
        }
        if (value != 0 && count == 0) {
            return false;
        }
   }
    return true;
}
void solveMatrix(FILE* F, float** A, float* X, int row, int col) {
    bool check = checkIfStair(A, row, col) && isSolving(A, row, col);
    if (check) {

        for (int i = col-1; i >-1; i--) {
            getX(A, X, row, col, i);
        }
        PasteSolution(F, X, col);

    }
    else {
        if (isSolving(A, row, col)) {
            
            generalSolution(F, A, X, row, col);
        }
        else
        fprintf(F, "system is inconsistent \n");
        
    }
}

int main()
{
    int eqNum = 0,varNum = 0, choose;
    FILE* F1 = fopen("task.txt","r");
    FILE* F2 = fopen("output.txt","w");
    /*cout << "Choose the task \n" << "1 - coefficient matrix\n2- two lines\n3- three lines\n4- three planes\n";*/
    
    readLines(F1);/*
    fscanf(F1, "%d", &eqNum);
    fscanf(F1, "%d", &varNum);
    float** matrix = new float* [eqNum];
    float* solutionMatrix = new float[varNum];
    init_2d(matrix, eqNum, varNum);

    toReadEq(F1, matrix, eqNum, varNum);
    Paste(F2, matrix, eqNum, varNum);
    makeStair(matrix, eqNum, varNum);
    Paste(F2, matrix, eqNum, varNum);
    solveMatrix(F2, matrix, solutionMatrix, eqNum, varNum);*/
    fclose(F1);
    fclose(F2);
}


