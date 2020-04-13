#include <stdio.h>
#include <stdlib.h>

struct square;
struct row;
struct tSquare;
struct column;

typedef struct row
{
    int numbers[9];
    struct square* s[9];
} row;

typedef struct tSquare
{
    int numbers[3][3];
    struct square* s[3][3];
} tSquare;

typedef struct column
{
    int numbers[9];
    struct square* s[9];
} column;

typedef struct square
{
    int number;
    row* r;
    tSquare* tS;
    column* c;
    int possibleNumbers[9];
} square;

int main()
{
    int c;
    int grid[9][9];
    int countX;
    int countY;

    int numPossible = 0;
    int count = 0;
    int flagComplete = 0;

    row rowArray[9];
    tSquare tSquareArray[3][3];
    column columnArray[9];
    square squareGrid[9][9];

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            grid[i][j] = 0;
        }
    }

    FILE *fp;
    fp = fopen( "puzzle.txt", "r");
    if(fp)
    {
        countX = 0;
        countY = 0;
        while((c = getc(fp)) != EOF)
        {
            //48 = 0
            //printf("X: %d, Y: %d",countX,countY);
            if(c != '\n' && c != '\0')
            {
                grid[countX][countY] = c-48;
                //putchar(c);
                countX++;
            }
            else
            {
                countX = 0;
                countY++;
            }
            //printf("%d",c);
            //putchar(c);
        }
    }

    fclose(fp);

    printf("\n");

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            printf("%d ",grid[j][i]);
            if((j + 1) % 3 == 0 && (j + 1) != 9)
            {
                printf("| ");
            }
        }
        if((i + 1) % 3 == 0 && (i + 1) != 9)
        {
            printf("\n---------------------");
        }
        printf("\n");
    }






    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {

            squareGrid[j][i].number = grid[j][i];

            squareGrid[j][i].c = &columnArray[j];
            columnArray[j].s[i] = &squareGrid[j][i];
            columnArray[j].numbers[i] = squareGrid[j][i].number;

            squareGrid[j][i].r = &rowArray[i];
            rowArray[i].s[j] = &squareGrid[j][i];
            rowArray[i].numbers[j] = squareGrid[j][i].number;

            squareGrid[j][i].tS = &tSquareArray[j/3][i/3];
            tSquareArray[j/3][i/3].s[j%3][i%3] = &squareGrid[j][i];
            tSquareArray[j/3][i/3].numbers[j%3][i%3] = squareGrid[j][i].number;


            if(squareGrid[j][i].number == 0)
            {
                for(int k = 0; k < 9; k++)
                {
                    squareGrid[j][i].possibleNumbers[k] = k+1;
                }
            }
            else
            {
                for(int k = 0; k < 9; k++)
                {
                    squareGrid[j][i].possibleNumbers[k] = 0;
                }
            }




        }
    }


    /*Eliminate possible numbers*/
    while(flagComplete == 0)
    {
        printf("\n");
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                printf("%d ",squareGrid[j][i].number);
                if((j + 1) % 3 == 0 && (j + 1) != 9)
                {
                    printf("| ");
                }
            }
            if((i + 1) % 3 == 0 && (i + 1) != 9)
            {
                printf("\n---------------------");
            }
            printf("\n");
        }


        for(int i = 0; i < 9; i++)
        {
            printf("%d",squareGrid[5][6].possibleNumbers[i]);
        }
        printf("\n");
        for(int i = 0; i < 9; i++)
        {
            printf("%d",squareGrid[5][8].possibleNumbers[i]);
        }
        printf("\n");

        //row
        for(int i = 0; i < 9; i++)
        {
            //column
            for(int j = 0; j < 9; j++)
            {

                numPossible = 0;
                //if empty
                if(squareGrid[j][i].number == 0)
                {
                    //all possible numbers for square(wether they are there or not)
                    for(int k = 0; k < 9; k++)
                    {
                        //if possible number is there
                        if(squareGrid[j][i].possibleNumbers[k] != 0)
                        {
                            //all numbers in column
                            for(int m = 0; m < 9; m++)
                            {
                                //if match number is no longer possible
                                if(squareGrid[j][i].possibleNumbers[k] == squareGrid[j][i].c->numbers[m])
                                {
                                    squareGrid[j][i].possibleNumbers[k] = 0;
                                }
                            }
                            //all numbers in row
                            for(int m = 0; m < 9; m++)
                            {
                                //if match number is no longer possible
                                if(squareGrid[j][i].possibleNumbers[k] == squareGrid[j][i].r->numbers[m])
                                {
                                    squareGrid[j][i].possibleNumbers[k] = 0;
                                }
                            }
                            //all numbers in tSquare
                            for(int m = 0; m < 3; m++)
                            {
                                for(int n = 0; n < 3; n++)
                                {
                                    //if match number is no longer possible
                                    if(squareGrid[j][i].possibleNumbers[k] == squareGrid[j][i].tS->numbers[n][m])
                                    {
                                        squareGrid[j][i].possibleNumbers[k] = 0;
                                    }
                                }
                            }
                        }
                    }
                    //check if only 1 remaining number for square
                    for(int k = 0; k < 9; k++)
                    {
                        if(squareGrid[j][i].possibleNumbers[k] != 0)
                        {
                            if(numPossible == 0)
                            {
                                numPossible = squareGrid[j][i].possibleNumbers[k];
                            }
                            else if(numPossible > 0)
                            {
                                numPossible = 100;
                            }
                        }
                    }
                    //If only 1 possiblity remaining, update square and corosponding structs
                    if(numPossible > 0 && numPossible < 10)
                    {
                        //update possible numbers
                        /* for(int k = 0; k < 9; k++){
                             squareGrid[j][i].c->s[k]->possibleNumbers[numPossible-1] = 0;
                             squareGrid[j][i].r->s[k]->possibleNumbers[numPossible-1] = 0;
                         }
                         for(int k = 0; k < 3; k++){
                             for(int m = 0; m < 3; m++){
                                 squareGrid[j][i].tS->s[m][k]->possibleNumbers[numPossible-1] = 0;
                             }
                         }*/
                        /////////////////////////////////////////////////

                        squareGrid[j][i].number = numPossible;
                        printf("herre:? %d\n",numPossible);
                        for(int k = 0; k < 9; k++)
                        {
                            squareGrid[j][i].possibleNumbers[numPossible] = 0;

                            squareGrid[j][i].c->s[k]->possibleNumbers[numPossible - 1] = 0;
                            squareGrid[j][i].r->s[k]->possibleNumbers[numPossible - 1] = 0;
                        }
                        for(int p = 0; p < 3; p++)
                        {
                            for(int q = 0; q < 3; q++)
                            {
                                squareGrid[j][i].tS->s[q][p]->possibleNumbers[numPossible - 1] = 0;
                            }
                        }
                    }

                }


                //check each number for single possiblities
                for(int k = 1; k < 10; k++)
                {

                    count = 0;//Amount of squares with k possible numbers

                    //check row
                    for(int m = 0; m < 9; m++)
                    {
                        if(squareGrid[j][i].r->s[m]->possibleNumbers[k - 1] == k)
                        {
                            //printf("\npN[k-1]: %d, k: %d\n",squareGrid[j][i].r->s[m]->possibleNumbers[k - 1],k);
                            count++;
                        }
                        if(squareGrid[j][i].r->s[m]->number == k)
                        {
                            count = 100;
                            //printf("k!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                        }
                        // printf("Count: %d\n",count);
                    }
                    if(count == 1)
                    {
                        //printf("Row %d, %d -> %d-------------------------------------------------------------------------------\n",j,i,k);

                        for(int m = 0; m < 9; m++)
                        {
                            if(squareGrid[j][i].r->s[m]->possibleNumbers[k - 1] == k)
                            {
                                squareGrid[j][i].r->s[m]->number = k;
                                for(int n = 0; n < 9; n++)
                                {
                                    squareGrid[j][i].r->s[m]->possibleNumbers[n] = 0;

                                    squareGrid[j][i].r->s[m]->c->s[n]->possibleNumbers[k - 1] = 0;
                                    squareGrid[j][i].r->s[m]->r->s[n]->possibleNumbers[k - 1] = 0;
                                }
                                for(int p = 0; p < 3; p++)
                                {
                                    for(int q = 0; q < 3; q++)
                                    {
                                        squareGrid[j][i].r->s[m]->tS->s[q][p]->possibleNumbers[k - 1] = 0;
                                    }
                                }
                                printf("No more %d\n",k);
                            }
                        }
                    }

                    if(count < 100)
                    {
                        count = 0;
                    }

                    //check column
                    for(int m = 0; m < 9; m++)
                    {
                        if(squareGrid[j][i].c->s[m]->possibleNumbers[k - 1] == k)
                        {
                            count++;
                        }
                        if(squareGrid[j][i].c->s[m]->number == k)
                        {
                            count = 100;
                        }
                    }
                    if(count == 1)
                    {
                        //printf("Col %d, %d -> %d\n",j,i,k);
                        for(int m = 0; m < 9; m++)
                        {
                            if(squareGrid[j][i].c->s[m]->possibleNumbers[k - 1] == k)
                            {
                                squareGrid[j][i].c->s[m]->number = k;
                                for(int n = 0; n < 9; n++)
                                {
                                    squareGrid[j][i].c->s[m]->possibleNumbers[n] = 0;

                                    squareGrid[j][i].c->s[m]->c->s[n]->possibleNumbers[k - 1] = 0;
                                    squareGrid[j][i].c->s[m]->r->s[n]->possibleNumbers[k - 1] = 0;
                                }
                                for(int p = 0; p < 3; p++)
                                {
                                    for(int q = 0; q < 3; q++)
                                    {
                                        squareGrid[j][i].c->s[m]->tS->s[q][p]->possibleNumbers[k - 1] = 0;
                                    }
                                }
                                printf("No more %d, [%d][%d]\n",k,j,i);
                            }
                        }
                    }

                    if(count < 100)
                    {
                        count = 0;
                    }

                    //check tSquare
                    for(int m = 0; m < 3; m++)
                    {
                        for(int n = 0; n < 3; n++)
                        {
                            if(squareGrid[j][i].tS->s[n][m]->possibleNumbers[k - 1] == k)
                            {
                                count++;
                            }
                            if(squareGrid[j][i].tS->s[n][m]->number == k)
                            {
                                count = 100;
                            }
                        }
                    }
                    if(count == 1)
                    {
                        //printf("Square %d, %d -> %d\n",j,i,k);
                        for(int m = 0; m < 3; m++)
                        {
                            for(int n = 0; n < 3; n++)
                            {
                                if(squareGrid[j][i].tS->s[n][m]->possibleNumbers[k - 1] == k)
                                {
                                    squareGrid[j][i].tS->s[n][m]->number = k;
                                    for(int p = 0; p < 9; p++)
                                    {
                                        squareGrid[j][i].tS->s[n][m]->possibleNumbers[p] = 0;

                                        squareGrid[j][i].tS->s[n][m]->c->s[p]->possibleNumbers[k - 1] = 0;
                                        squareGrid[j][i].tS->s[n][m]->r->s[p]->possibleNumbers[k - 1] = 0;
                                    }
                                    for(int p = 0; p < 3; p++)
                                    {
                                        for(int q = 0; q < 3; q++)
                                        {
                                            squareGrid[j][i].tS->s[n][m]->tS->s[q][p]->possibleNumbers[k - 1] = 0;
                                        }
                                    }




                                    /*squareGrid[j][i].c->s[p]->possibleNumbers[k - 1] = 0;
                                    squareGrid[j][i].r->s[p]->possibleNumbers[k - 1] = 0;
                                    }
                                    for(int p = 0; p < 3; p++)
                                    {
                                    for(int q = 0; q < 3; q++)
                                    {
                                        squareGrid[j][i].tS->s[q][p]->possibleNumbers[k - 1] = 0;
                                    }
                                    }
                                    printf("No more %d\n",k);*/
                                }
                            }
                        }
                    }
                }
            }
        }
        flagComplete = 1;
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                if(squareGrid[j][i].number == 0)
                {
                    flagComplete = 0;
                }
            }
        }
    }

    printf("\n\n");


    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            printf("%d ",squareGrid[j][i].number);
            if((j + 1) % 3 == 0 && (j + 1) != 9)
            {
                printf("| ");
            }
        }
        if((i + 1) % 3 == 0 && (i + 1) != 9)
        {
            printf("\n---------------------");
        }
        printf("\n");
    }

    /*TESTING*/

    {
        /*for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                for(int k = 0; k < 9; k++)
                {
                    printf("%d ",columnArray[j].s[i]->possibleNumbers[k]);
                }
                printf(",");
            }
            printf("\n");
        }

            printf("\n");

        for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            printf("%d ",columnArray[j].s[i]->number);
        }
        printf("\n");
        }

        printf("\n");

        for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            printf("%d ",columnArray[j].numbers[i]);
        }
        printf("\n");
        }

        printf("\n");

        for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            printf("%d ",rowArray[i].s[j]->number);
        }
        printf("\n");
        }

        printf("\n");


        for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            printf("%d ",rowArray[i].numbers[j]);
        }
        printf("\n");
        }
        */
    }




    return 0;
}
