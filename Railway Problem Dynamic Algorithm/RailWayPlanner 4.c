#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define ARG_ARR "Usage: RailwayPlanner <InputFile>"
#define LINE_ARR "Invalid input in line: %d."
#define MIN_PRICE "The minimal price is: %d"
#define OUTPUT_FILE "railway_planner_output.txt"
#define FILE_DOSENT "File doesn't exists."
#define EMPTY_FILE "File is empty."
/**
 * @brief part struct.
 */
typedef struct Part
{
    char left;
    char right;
    int length;
    int price;
} Part;

void checkInput(FILE *fp, FILE *newfp);

void enqueue(Part **arr, int index, char left, char right, int length, int price);

int tableAlgo(Part *arr, const char *partsarr, int length, int parts, int partsSize);

int getInt(const Part *arr, const char *partsarr, int length, int parts, int partsSize,
           int *const *table, int minprice, int j);

int getInt1(const Part *arr, const char *partsarr, int parts, int i);

void intMax(int *const *table, int j, int y, int checker);

int getInt2(const Part *arr, int length, int *const *table, int minprice, int j, int i, int index);

void putInTable(const Part *arr, int length, int *const *table, int j, int y, int i, int index,
                int *minprice, int *checker);

/**
 * @brief main function
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    FILE *newfp = fopen(OUTPUT_FILE, "w");
    if (argc != 2)
    {
        fprintf(newfp, ARG_ARR);
        fclose(newfp);
        return EXIT_FAILURE;
    }
    FILE *fp = fopen(argv[1], "r");

    if (newfp == NULL)
    {
        return EXIT_FAILURE;
    }
    else if (fp == NULL)
    {
        fprintf(newfp, FILE_DOSENT);
        fclose(newfp);
        return EXIT_FAILURE;

    }
    fseek(fp, 0, SEEK_END); // goto end of file
    if (ftell(fp) == 0)
    {
        fprintf(newfp, EMPTY_FILE);
        fclose(fp);
        fclose(newfp);
        return EXIT_FAILURE;
    }
    fseek(fp, 0, SEEK_SET);
    checkInput(fp, newfp);
    fclose(fp);
    fclose(newfp);
}

/**
 * @brief check the input parts.
 * @param fp
 * @param newfp
 */
void checkInput(FILE *fp, FILE *newfp)
{
    int minPrice = 0;
    int counter;
    char input[1025];
    char *ptr;
    int first = -1; // how many parts
    int l = -1;  // L
    for (int i = 0; i < 2; i++)
    {
        fgets(input, sizeof input, fp);
        strtok(input, "\n");
        strtok(input, "\r");
        if (i == 0)
        {
            l = (int) strtol(input, &ptr, 10);
            if (l < 0 || strlen(ptr) != 0)
            {
                fprintf(newfp, LINE_ARR, i + 1);
                fclose(fp);
                fclose(newfp);
                exit(EXIT_FAILURE);
            }

        }
        else
        {
            first = (int) strtol(input, &ptr, 10);
            if (first <= 0 || strlen(ptr) != 0)
            {
                fprintf(newfp, LINE_ARR, i + 1);
                fclose(fp);
                fclose(newfp);
                exit(EXIT_FAILURE);
            }
        }
    }
    fgets(input, sizeof input, fp);
    int i = 0;
    strtok(input, "\n");
    char *p = strtok(input, ",");
    char *charsArray = (char *) calloc(first + 1, sizeof(char));
    if (charsArray == NULL)
    {
        exit(EXIT_FAILURE);
    }
    while (p != NULL)
    {
        if (strlen(p) > 1)
        {
            fprintf(newfp, LINE_ARR, 3);
            free(charsArray);
            fclose(fp);
            fclose(newfp);
            exit(EXIT_FAILURE);

        }
        charsArray[i++] = *p;
        p = strtok(NULL, ",");
    }
    if(i != first)
    {
        fprintf(newfp, LINE_ARR, 3);
        free(charsArray);
        fclose(fp);
        fclose(newfp);
        exit(EXIT_FAILURE);
    }
    counter = 3;
    Part *arr = (Part *) calloc(1, sizeof(Part));
    while (fgets(input, sizeof input, fp))
    {
        int k = 0;
        strtok(input, "\n");
        char *partArray = (char *) calloc(4, sizeof(char));
        char tempInput[1025] = "";
        strcpy(tempInput, input);
        char *c = strtok(tempInput, ",");
        char *ptr1;
        while (c != NULL)
        {
            if (strlen(c) > 1 && k < 2)
            {
                fprintf(newfp, LINE_ARR, counter + 1);
                free(arr);
                free(partArray);
                free(charsArray);
                fclose(fp);
                fclose(newfp);
                exit(EXIT_FAILURE);

            }
            if (k >= 2)
            {
                strtol(c, &ptr1, 10);
                if (strlen(ptr1) != 0)
                {
                    fprintf(newfp, LINE_ARR, counter + 1);
                    free(arr);
                    free(partArray);
                    free(charsArray);
                    fclose(fp);
                    fclose(newfp);
                    exit(EXIT_FAILURE);
                }
            }
            partArray[k++] = *c;
            c = strtok(NULL, ",");
        }
        char left = partArray[0];
        char right = partArray[1];
        int length;
        int price;
        free(partArray);
        sscanf(input, "%c,%c,%d,%d", &left, &right, &length, &price);
        char *firstPostion = strrchr(charsArray, left);
        char *secondPostion = strrchr(charsArray, right);
        if (firstPostion == NULL || secondPostion == NULL)
        {
            fprintf(newfp, LINE_ARR, counter + 1);
            fclose(fp);
            fclose(newfp);
            free(arr);
            free(charsArray);
            exit(EXIT_FAILURE);
        }

        if (length <= 0 || price <= 0)
        {
            fprintf(newfp, LINE_ARR, counter + 1);
            fclose(fp);
            fclose(newfp);
            free(arr);
            free(charsArray);
            exit(EXIT_FAILURE);
        }
        counter += 1;
        enqueue(&arr, counter - 3, left, right, length, price);
    }
    minPrice = tableAlgo(arr, charsArray, l, first, counter - 3);
    if (minPrice == INT_MAX || minPrice < 0)
    {
        fprintf(newfp, MIN_PRICE, -1);

    }
    else
    {
        fprintf(newfp, MIN_PRICE, minPrice);

    }
    free(charsArray);
    free(arr);
}

/**
 * @brief table algorithm.
 * @param arr
 * @param partsarr
 * @param length
 * @param parts
 * @param partsSize
 * @return
 */
int tableAlgo(Part *arr, const char *partsarr, int length, int parts, int partsSize)
{
    if (length == 0)
    {
        return 0;
    }
    int **table;
    table = (int **) calloc((length + 2), sizeof(int *));
    for (int i = 0; i < length + 2; i++)
    {
        table[i] = (int *) calloc(parts + 1, sizeof(int));
    }
    table[0][0] = 0;
    for (int i = 1; i < parts + 1; ++i)
    {
        table[0][i] = partsarr[i - 1];
        table[1][i - 1] = 0;
    }
    int minprice = INT_MAX;

    for (int j = 2; j < length + 2; j++)
    {
        minprice = getInt(arr, partsarr, length, parts, partsSize, table, minprice, j);
    }
    for (int i = 2; i < length + 2; ++i)
    {
        table[i][0] = 0;
    }
    for (int i = 0; i < length + 2; ++i)
    {
        free(table[i]);
    }
    free(table);
    return minprice;
}

/**
 * @brief  helper function for the table algo
 * @param arr
 * @param partsarr
 * @param length
 * @param parts
 * @param partsSize
 * @param table
 * @param minprice
 * @param j
 * @return
 */
int getInt(const Part *arr, const char *partsarr, int length, int parts, int partsSize,
           int *const *table, int minprice, int j)
{
    for (int y = 1; y < parts + 1; y++)
    {
        int checker = 0;
        for (int i = 0; i < partsSize; ++i)
        {
            if ((int) arr[i].right == table[0][y] && arr[i].length <= j - 1)
            {
                int index = getInt1(arr, partsarr, parts, i);
                putInTable(arr, length, table, j, y, i, index, &minprice, &checker);
            }
        }
        intMax(table, j, y, checker);
    }
    return minprice;
}
/**
 * @brief put the ij element in table
 * @param arr
 * @param length
 * @param table
 * @param j
 * @param y
 * @param i
 * @param index
 * @param minprice
 * @param checker
 */
void putInTable(const Part *arr, int length, int *const *table, int j, int y, int i, int index,
                int *minprice, int *checker)
{
    if ((table[j - arr[i].length][index] < INT_MAX))
    {
        (*checker) = 1;
        if (j - arr[i].length != 1 && table[j][y] != 0)
        {
            if (table[j - arr[i].length][index] + arr[i].price < table[j][y])
            {
                table[j][y] = table[j - arr[i].length][index] + arr[i].price;
            }
        }
        else if( table[j][y] == 0)
        {
            table[j][y] =
                    table[j - arr[i].length][index] + arr[i].price; //check this
        }
        else
        {
            if (table[j - arr[i].length][index] + arr[i].price < table[j][y])
            {
                table[j][y] = table[j - arr[i].length][index] + arr[i].price;
            }
        }
        (*minprice) = getInt2(arr, length, table, (*minprice), j, i, index);
    }
}

/**
 * @brief
 * @param arr
 * @param length
 * @param table
 * @param minprice
 * @param j
 * @param i
 * @param index
 * @return
 */
int getInt2(const Part *arr, int length, int *const *table, int minprice, int j, int i, int index)
{
    if (minprice != 0 && j == length + 1)
    {
        if (table[j - arr[i].length][index] + arr[i].price < minprice)
        {
            minprice = table[j - arr[i].length][index] + arr[i].price;
        }
    }
    else if (minprice == 0 && j == length + 1)
    {
        minprice = table[j - arr[i].length][index] + arr[i].price;

    }
    return minprice;
}

/**
 * @brief put int max if there is no possible way
 * @param table
 * @param j
 * @param y
 * @param checker
 */
void intMax(int *const *table, int j, int y, int checker)
{
    if (checker == 0)
    {
        table[j][y] = INT_MAX;
    }
}

/**
 * @brief helper function for the table algo
 * @param arr
 * @param partsarr
 * @param parts
 * @param i
 * @return
 */
int getInt1(const Part *arr, const char *partsarr, int parts, int i)
{
    int index = 0;
    for (int k = 0; k < parts; ++k)
    {
        if ((int) arr[i].left == (int) partsarr[k])
        {
            index = k + 1;
            break;
        }
    }
    return index;
}

/**
 * @brief handle the struct array.
 * @param arr
 * @param index
 * @param left
 * @param right
 * @param length
 * @param price
 */
void enqueue(Part **arr, int index, char left, char right, int length, int price)
{
    Part *temp = (Part *) realloc(*arr, index * (sizeof(Part)));
    if (temp != NULL)
    {
        temp[index - 1].left = left;
        temp[index - 1].right = right;
        temp[index - 1].length = length;
        temp[index - 1].price = price;
        *arr = temp;
    }

}
