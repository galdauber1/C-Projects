#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define ENTER_STUDENT "Enter student info. To exit press q, then enter\n"
#define USAGE "USAGE: choose best/quick/merge\n"
#define BEST "best student info is: %lu,%s,%d,%d,%s,%s"
#define ARGS_ERR "ERROR: not enough args\n"
#define GRADE_ERR "ERROR: problem with grade\n"
#define AGE_ERR "ERROR: problem with age\n"
#define NAME_ERR "ERROR: problem with name\n"
#define CITY_ERR "ERROR: problem with city\n"
#define COUNTRY_ERR "ERROR: problem with country\n"
#define ID_ERR "ERROR: problem with id in line\n"
#define PRINT_STUD "%lu,%s,%d,%d,%s,%s\n"
#define PRINT_ONE_STUD "%lu,%s,%d,%d,%s,%s"
#define IN_LINE "in line %d\n"

/**
 * @brief student struct
 */
typedef struct Student
{
    int row;
    long id;
    char name[42];
    int grade;
    int age;
    char country[42];
    char city[42];
}Student;

void best();

void merge();

void quick();
int checkInputs(char id[42] , char name[42], float age, float grade, char country[42], char
                city[42] , int counter, int args);
void quickSort(  Student arr[5500], int low , int high);
int partition( Student arr[5500], int low , int high);
void swap(Student* a, Student* b);
void mergearr( Student arr[5500], int l, int m, int r);
void mergeSort(Student arr[5500], int l, int r);
/**
 * @brief main function of the project.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    if (argc != 2 )
    {
        printf(USAGE);
        return 1;
    }
    else if(strcmp(argv[1], "best") == 0)
    {
        best();
        return 0;
    }
    else if (strcmp(argv[1], "merge") == 0)
    {
        merge();
        return 0;

    }
    else if (strcmp(argv[1], "quick") == 0)
    {
        quick();
        return 0;
    }
    else
    {
        printf(USAGE);
        return 1;

    }


}
/**
 * @brief function that handle the best part.
 */
void best()
{
    Student arr[5500] = {0};
    int counter = - 1 ;
    char line[61] = "";
    float best = 0;
    int best_stud = 0;
    while (strcmp(line, "q\n")!=0)
    {
        printf(ENTER_STUDENT);
        counter += 1;
        if ( fgets(line, sizeof(line), stdin) == NULL)
        {
            printf(ARGS_ERR);
            printf(IN_LINE, counter );
            exit(1);
        }
        char id[42] = "^";
        char name[42] = "@";
        float grade = -1;
        float age = -2;
        char country[42] = "!";
        char city[42] = "!";
        int args;
        args = sscanf(line, "%[^,],%[^,],%f,%f,%[^,],%[^,]", id , name , &grade , &age ,
                    country , city);
        if (strcmp(line, "q\n") != 0)
        {
            if ((checkInputs(id, name, age, grade, country, city, counter, args) == 0))
            {
                char *ptr;
                long ret = -12345678900;
                ret = strtol(id, &ptr, 10);
                arr[counter].row = counter;
                arr[counter].id = ret;
                strcpy(arr[counter].name, name);
                arr[counter].grade = (int) grade;
                arr[counter].age = (int) age;
                strcpy(arr[counter].city, city);
                strcpy(arr[counter].country, country);
                if (grade / age > best)
                {
                    best = grade / age;
                    best_stud = counter;
                }
            }
        }

    }
    if (arr[best_stud].id!=0)
    {
        printf(BEST, arr[best_stud].id, arr[best_stud].name,
               arr[best_stud]
                       .grade, arr[best_stud].age, arr[best_stud].country, arr[best_stud].city);
    }


}
/**
 * @brief this function check that all inputs are valid.
 * @param id
 * @param name
 * @param age
 * @param grade
 * @param country
 * @param city
 * @param counter
 * @param args
 * @return 0 if true eles 1.
 */
int checkInputs(char id[42] , char name[42], float age, float grade, char country[42], char
                city[42], int counter, int args)
{
    char new_name[42] = "@";
    char new_country[42] = "@";
    char new_city[42] = "@";
    char *ptr;
    long ret = -12345678900;
    ret = strtol(id, &ptr, 10);
    sscanf(name, "%[a-z A-Z -]", new_name);
    sscanf(country, "%[a-zA-Z-]s", new_country);
    sscanf(city, "%[a-zA-Z-]s", new_city);
    new_city[ strlen(new_city)] = '\n';
    if (args < 6) //check the args number
    {
        printf(ARGS_ERR );
        printf(IN_LINE, counter);
        return 1;
    }
    else if (strlen(id) != 10  || id[0] == 48 || ret <= 999999999 ||  ret >=
             10000000000 )
        //check id
        // the id
        // valid.
    {
        printf(ID_ERR);
        printf(IN_LINE, counter);
        return 1;

    }
    else if (strcmp(name, new_name)!= 0) //check the name is valid.
    {
        printf(NAME_ERR);
        printf(IN_LINE, counter);

        return 1;
    }
    else if(grade < 0 || grade > 100  || floorf(grade) != grade) //check the grade is valid.
    {
        printf(GRADE_ERR);
        printf(IN_LINE, counter);

        return 1;
    }
    else if (age < 18 || age>120  || floorf(age) != age ) //check the age is valid.
    {
        printf(AGE_ERR);
        printf(IN_LINE, counter);
        return 1;
    }
    else if (strcmp(new_country, country) != 0) //check the country is valid.
    {
        printf(COUNTRY_ERR);
        printf(IN_LINE, counter);
        return 1;
    }
    else if (strcmp(city, new_city) != 0) // check the city is valid.
    {
        printf(CITY_ERR);
        printf(IN_LINE, counter);
        return 1;
    }

    else
    {
        return 0;
    }
}
/**
 * @brief this function handle the quick part.
 */
void quick()
{
    Student arr[5500] = {0};
    int counter = -1;
    char line[61] = "";
    while (strcmp(line, "q\n") != 0)
    {
        printf(ENTER_STUDENT);
        counter += 1;
        if ( fgets(line, sizeof(line), stdin) == NULL)
        {
            printf(ARGS_ERR);
            printf(IN_LINE, counter);
            exit(1);
        }
        char id[42] = "^";
        char name[42] = "@";
        float grade = -1;
        float age = -2;
        char country[42] = "!";
        char city[42] = "!";
        int args;
        args = sscanf(line, "%[^,],%[^,],%f,%f,%[^,],%[^,]", id, name, &grade, &age, country,
                      city);
        if (strcmp(line, "q\n") != 0)
        {
            if ((checkInputs(id, name, age, grade, country, city, counter, args) == 0) && args == 6)
            {
                char *ptr;
                long ret = -12345678900;
                ret = strtol(id, &ptr, 10);
                arr[counter].row = counter;
                arr[counter].id = ret;
                strcpy(arr[counter].name, name);
                arr[counter].grade = (int) grade;
                arr[counter].age = (int) age;
                strcpy(arr[counter].city, city);
                strcpy(arr[counter].country, country);

            }
        }
    }
    quickSort(arr, 0, counter-1);
    for (int i = 0; i < counter-1; ++i)
    {
        if (arr[i].id != 0)
        {
            printf(PRINT_STUD, arr[i].id, arr[i].name,
                   arr[i].grade, arr[i].age, arr[i].country, arr[i].city);
        }
    }
    if (arr[counter-1].id!=0)
    {
        printf(PRINT_ONE_STUD, arr[counter - 1].id, arr[counter - 1].name,
               arr[counter - 1].grade, arr[counter - 1].age, arr[counter - 1].country,
               arr[counter - 1].city);
    }

}
/**
 * @brief quick sort function.
 * @param arr
 * @param low
 * @param high
 */
void quickSort( Student arr[5500], int low , int high)
{
    if (low<high)
    {
        int pi = partition( arr, low, high);
        quickSort(arr, low , pi-1);
        quickSort(arr, pi + 1, high);

    }

}
/**
 * @brief partition function.
 * @param arr
 * @param low
 * @param high
 * @return place the pivot in the correct place as all the smaller to the left and biggest to the
 * right and return the index.
 */
int partition( Student arr[5500], int low , int high)
        {
            char pivot[42] = "";
            strcpy(pivot, arr[high].name);
            int i = (low-1);
            for (int (j) = low; (j) <= high-1 ; ++(j))
            {
                if (strcmp(arr[j].name, pivot ) < 0)
                {
                    i += 1;
                    swap( &arr[i], &arr[j]);
                }
            }
            swap(&arr[i + 1], &arr[high]);
            return (i + 1);

        }
/**
 * @brief swap function.
 * @param a
 * @param b
 */
void swap(Student* a, Student* b)
{
    Student temp = *a ;
    *a = *b;
    *b = temp;
}
/**
 * @brief this function handle merge part.
 */
void merge()
{
    Student arr[5500] = {0};
    int counter = -1; // check line number
    char line[61] = "";
    while (strcmp(line, "q\n") != 0)
    {
        printf(ENTER_STUDENT);
        counter += 1;
        if ( fgets(line, sizeof(line), stdin) == NULL)
        {
            printf(ARGS_ERR);
            printf(IN_LINE, counter);
            exit(1);
        }
        char id[42] = "^";
        char name[42] = "@";
        float grade = -1;
        float age = -2;
        char country[42] = "!";
        char city[42] = " !";
        int args;
        args = sscanf(line, "%[^,],%[^,],%f,%f,%[^,],%[^,]", id, name, &grade, &age, country,
                      city);
        if (strcmp(line, "q\n") != 0)
        {
            if ((checkInputs(id, name, age, grade, country, city, counter, args) == 0) && args == 6)
            {
                char *ptr;
                long ret = -12345678900;
                ret = strtol(id, &ptr, 10);
                arr[counter].row = counter;
                arr[counter].id = ret;
                strcpy(arr[counter].name, name);
                arr[counter].grade = (int) grade;
                arr[counter].age = (int) age;
                strcpy(arr[counter].city, city);
                strcpy(arr[counter].country, country);

            }
        }
    }
    mergeSort(arr,  0,  counter - 1);
    for (int i = 0; i < counter-1; ++i)
    {
        if (arr[i].id!=0)
        {
            printf(PRINT_STUD, arr[i].id, arr[i].name,
                   arr[i].grade, arr[i].age, arr[i].country, arr[i].city);
        }
    }
    if (arr[counter-1].id!=0)
    {
        printf(PRINT_ONE_STUD, arr[counter - 1].id, arr[counter - 1].name,
               arr[counter - 1].grade, arr[counter - 1].age, arr[counter - 1].country,
               arr[counter - 1].city);
    }
}
/**
 * @brief merge to arrs.
 * @param arr
 * @param l
 * @param m
 * @param r
 */
void mergearr( Student arr[5500], int l, int median, int r)
{
    int f1 = median - l + 1;
    int i,  j,  x;
    int f2 =  r - median;
    Student left[2750];
    Student right[2750];
    for (j = 0 ; j < f2 ; j++)
    {
        right[j] = arr[median + 1 + j];
    }
    for (i = 0; i < f1; i++)
    {
        left[i] = arr[l + i];
    }
    i = 0;
    j = 0;
    x = l;
    while (i < f1 && j < f2)
    {
        if (left[i].grade <= right[j].grade)
        {
            arr[x] = left[i];
            i++;
        }
        else
        {
            arr[x] = right[j];
            j++;
        }
        x++;
    }
    while (j < f2)
    {
        arr[x] = right[j];
        j ++;
        x ++;
    }
    while (i < f1)
    {
        arr[x] = left[i];
        i ++;
        x ++;
    }

}
/**
 * @brief merge sort function.
 * @param arr
 * @param l
 * @param r
 */
void mergeSort(Student arr[5500], int l, int r)
{
            if (l < r)
            {
                int median = l + (r-l) / 2;
                mergeSort(arr, l, median);
                mergeSort(arr, median + 1, r);
                mergearr(arr, l, median, r);
            }
}

