#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Car
{
    char name[50];
    char type[20];
    int horsepower;
    int year;
    char fuel[20];
    char color[20];
};

void printCar(struct Car car);
struct Car *loadCarsFromFile(const char *filename, struct Car *cars, int *carCount);
struct Car *addCar(struct Car *cars, int *carCount, struct Car newCar);
void printCarsAboveHorsepower(struct Car *cars, int carCount);
unsigned int readHorsepower(const char *message);
void readData(char *type, const char *message);
int countCarsByTypeNotEquals(struct Car *cars, int carCount);
void findTheMostPowerfulCar(struct Car *cars, int carCount);
void countCarTypes(struct Car *cars, int carCount);
void countRowsByTypeAndFuel(struct Car *cars, int carCount, const char *carType, const char *fuelType);
int main()
{
    struct Car *cars = NULL;
    int carCount = 0;
    // 1.feladat Kerjen be egy A oszlopban levo erteket es szamolja meg, hany ilyen rekord van.
    cars = loadCarsFromFile("garage.txt", cars, &carCount);
    for (int i = 0; i < carCount; i++)
    {
        printCar(cars[i]);
    }
    printf("There are %d cars in the garage UwU\n", carCount);
    // 2.feladat Kerjen be egy A oszlopban levo erteket es szamolja meg, hany ilyen rekord van. (Hány autó van X lóerő felett?)
    printCarsAboveHorsepower(cars, carCount);
    // 3.feladat erjen be egy B oszlopban levo erteket es szamolja meg, hany olyan rekord van, amelyben ez az ertek nem szerepel. (Hány autó van ami nem a bekért típus? Pl.: Sedan)
    int count = countCarsByTypeNotEquals(cars, carCount);
    printf("There are %d cars with a different type\n", count);
    // 4. feladat Allapitsa meg, hogy a C oszlopban melyik a legnagyobb ertek. Es hanyadik volt az a rekord, amikor ez ez ertek eloszor elofordult. (A legtöbb lóerővel bíró autó)
    findTheMostPowerfulCar(cars, carCount);
    // 5. feladatSzamolja meg, hogy egy megadott A es B oszlopbeli ertekbol hany sor van. (Autók száma típusonként)
    countCarTypes(cars, carCount);
    // 6. feladat Szamitsa ki, hogy egy megadott A oszlopbeli ertekbol a kulonbozo B oszlopbeli ertekekbol hany darab volt.(Autók száma megadott típus és üzemanyag szerint)
    char typeToFilter[20], fuelToFilter[20];
    readData(typeToFilter, "Enter the car type to filter (e.g., Sedan): ");
    readData(fuelToFilter, "Enter the fuel type to filter (e.g., Petrol): ");
    countRowsByTypeAndFuel(cars, carCount, typeToFilter, fuelToFilter);
    free(cars);
    return 0;
}

struct Car *addCar(struct Car *cars, int *carCount, struct Car newCar)
{
    cars = realloc(cars, (*carCount + 1) * sizeof(struct Car));
    if (cars == NULL)
    {
        perror("Error reallocating memory");
        exit(1);
    }
    cars[*carCount] = newCar;
    (*carCount)++;
    return cars;
}

struct Car *loadCarsFromFile(const char *filename, struct Car *cars, int *carCount)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    struct Car tempCar;
    while (fscanf(file, "%49[^;];%19[^;];%d;%d;%19[^;];%19[^\n]\n",
                  tempCar.name,
                  tempCar.type,
                  &tempCar.horsepower,
                  &tempCar.year,
                  tempCar.fuel,
                  tempCar.color) == 6)
    {
        cars = addCar(cars, carCount, tempCar);
    }

    fclose(file);
    return cars;
}

void printCar(struct Car car)
{
    printf("Name: %s\n", car.name);
    printf("Type: %s\n", car.type);
    printf("Horsepower: %d HP\n", car.horsepower);
    printf("Year: %d\n", car.year);
    printf("Fuel: %s\n", car.fuel);
    printf("Color: %s\n", car.color);
    printf("----------------------\n");
}
unsigned int readHorsepowerc(const char *message)
{
    unsigned int horsepower;
    puts(message);
    while (1)
    {
        if (scanf("%u", &horsepower) != 1)
        {

            while (getchar() != '\n')
                ;
            puts("Invalid input!Try again");
        }
        else
        {
            return horsepower;
        }
    }
}
void printCarsAboveHorsepower(struct Car *cars, int carCount)
{
    unsigned int minHorsepower = readHorsepowerc("Enter the minimum horsepower to filter:");
    // printf("Cars with more than %u HP:\n", minHorsepower);
    int count = 0;
    for (int i = 0; i < carCount; i++)
    {
        if (cars[i].horsepower > minHorsepower)
        {
            // printCar(cars[i]);
            count++;
        }
    }
    printf("Total cars with more than %u HP: %d\n", minHorsepower, count);
}
void readData(char *type, const char *message)
{
    int validInput = 0;
    while (!validInput)
    {
        printf("%s", message);
        if (fgets(type, 20, stdin) != NULL)
        {
            size_t len = strlen(type);

            if (len > 0 && type[len - 1] == '\n')
            {
                type[len - 1] = '\0';
            }
            if (len == 1)
            {
                printf("Input cannot be empty! Try again.\n");
                continue;
            }
            if (len >= 20)
            {
                printf("Input is too long! Please enter a shorter type.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }
            validInput = 1;
        }
        else
        {
            printf("Error reading input. Try again.\n");
            while (getchar() != '\n')
                ;
        }
    }
}
int countCarsByTypeNotEquals(struct Car *cars, int carCount)
{
    char type[20];
    readData(type, "Enter the car type to filter (e.g., Sedan): ");
    int count = 0;
    for (int i = 0; i < carCount; i++)
    {
        if (strcmp(cars[i].type, type) != 0)
        {
            count++;
        }
    }

    return count;
}
void findTheMostPowerfulCar(struct Car *cars, int carCount)
{
    struct Car carWithMaxHorsepower = cars[0];
    unsigned int index = 0;
    for (int i = 0; i < carCount; i++)
    {
        if (cars[i].horsepower > carWithMaxHorsepower.horsepower)
        {
            carWithMaxHorsepower = cars[i];
            index = i;
        }
    }
    printf("The most powerful car is:\n");
    printCar(carWithMaxHorsepower);
    printf("It's index is: %d\n", index);
    printf("It's the %dth car in the garage\n", index + 1);
}
void countCarTypes(struct Car *cars, int carCount)
{
    char types[100][20];
    int typeCount[100] = {0};
    int uniqueTypeCount = 0;
    for (int i = 0; i < carCount; i++)
    {
        int found = 0;
        for (int j = 0; j < uniqueTypeCount; j++)
        {
            if (strcmp(cars[i].type, types[j]) == 0)
            {
                typeCount[j]++;
                found = 1;
                break;
            }
        }
        if (!found)
        {
            strcpy(types[uniqueTypeCount], cars[i].type);
            typeCount[uniqueTypeCount] = 1;
            uniqueTypeCount++;
        }
    }

    printf("Car types count:\n");
    for (int i = 0; i < uniqueTypeCount; i++)
    {
        printf("%s: %d cars\n", types[i], typeCount[i]);
    }
}
void countRowsByTypeAndFuel(struct Car *cars, int carCount, const char *carType, const char *fuelType)
{
    int count = 0;
    for (int i = 0; i < carCount; i++)
    {
        if (strcmp(cars[i].type, carType) == 0 && strcmp(cars[i].fuel, fuelType) == 0)
        {
            count++;
        }
    }

    printf("There are %d rows where the car type is '%s' and fuel type is '%s'.\n", count, carType, fuelType);
}
