#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

COORD coord = {0, 0};
void gotoxy(int x, int y) {
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct emp {
    int emp_id;
    char name[40];
    int age;
    char address[50];
    char department[30];
    float salary;
};

FILE *fp;
long int recsize = sizeof(struct emp);

// Function declarations
void login();
void addEmployee();
void listEmployees();
void modifyEmployee();
void deleteEmployee();
void searchEmployee();
void sortEmployees();
void swap(struct emp *a, struct emp *b);
int getNextEmployeeID();
int isValidName(char *name);
int isValidAge(int age);
int isValidSalary(float salary);

int main() {
    char choice;

    fp = fopen("EMP.DAT", "rb+");
    if (fp == NULL) {
        fp = fopen("EMP.DAT", "wb+");
        if (fp == NULL) {
            printf("Error: Cannot open file.\n");
            exit(1);
        }
    }

    login();

    while (1) {
        system("cls");
        printf("\n::::::::::::::::::::::::::::::  EMPLOYEE MANAGEMENT SYSTEM  ::::::::::::::::::::::::::::::\n");
        gotoxy(30, 5);  printf("1> Add Employee");
        gotoxy(30, 7);  printf("2> List Employees");
        gotoxy(30, 9);  printf("3> Modify Employee");
        gotoxy(30, 11); printf("4> Delete Employee");
        gotoxy(30, 13); printf("5> Search Employee");
        gotoxy(30, 15); printf("6> Sort Employees");
        gotoxy(30, 17); printf("0> Exit");
        gotoxy(30, 19); printf("Your Choice: ");

        fflush(stdin);
        choice = getchar();
        getchar();

        switch (choice) {
            case '1': addEmployee(); break;
            case '2': listEmployees(); break;
            case '3': modifyEmployee(); break;
            case '4': deleteEmployee(); break;
            case '5': searchEmployee(); break;
            case '6': sortEmployees(); break;
            case '0': fclose(fp); exit(0);
            default: printf("\nInvalid choice! Try again.\n"); system("pause");
        }
    }

    return 0;
}

// Login system with hidden password
void login() {
    char uname[20], pword[20], ch;
    int attempts = 0;
    const char user[] = "admin";
    const char pass[] = "1234";

    while (attempts < 3) {
        system("cls");
        printf("\n::::::::::::::::::::::::::::::  LOGIN FORM  ::::::::::::::::::::::::::::::\n");
        printf("\nEnter Username: ");
        scanf("%s", uname);

        printf("Enter Password: ");
        int i = 0;
        while (1) {
            ch = getch();
            if (ch == 13) {
                pword[i] = '\0';
                break;
            } else if (ch == 8 && i > 0) {
                i--;
                printf("\b \b");
            } else if (ch != 8) {
                pword[i++] = ch;
                printf("*");
            }
        }

        if (strcmp(uname, user) == 0 && strcmp(pword, pass) == 0) {
            printf("\n\nLogin successful! Press any key to continue...");
            getch();
            return;
        } else {
            printf("\n");
            printf("\nInvalid credentials! Try again.\n");
            attempts++;
            system("pause");
        }
    }

    printf("\nToo many failed attempts. Exiting...\n");
    exit(1);
}

// Generate next unique ID
int getNextEmployeeID() {
    struct emp e;
    int maxID = 0;
    rewind(fp);
    while (fread(&e, recsize, 1, fp) == 1) {
        if (e.emp_id > maxID) maxID = e.emp_id;
    }
    return maxID + 1;
}

// Input validators
int isValidName(char *name) {
    for (int i = 0; i < strlen(name); i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int isValidAge(int age) {
    return (age >= 18 && age <= 65);
}

int isValidSalary(float salary) {
    return (salary >= 0.0);
}

// Add employee
void addEmployee() {
    struct emp e;
    char another = 'y';

    while (another == 'y' || another == 'Y') {
        system("cls");

        e.emp_id = getNextEmployeeID();
        printf("New Employee ID: %d\n", e.emp_id);

        printf("Enter Name: ");
        getchar();
        fgets(e.name, sizeof(e.name), stdin);
        e.name[strcspn(e.name, "\n")] = '\0';

        if (!isValidName(e.name)) {
            printf("\n Invalid name. Only characters are allowed.\n");
            system("pause");
            return;
        }

        printf("Enter Age: ");
        if (scanf("%d", &e.age) != 1 || !isValidAge(e.age)) {
            printf("\n Invalid age. Please enter a valid age between 18 and 65.\n");
            fflush(stdin);
            system("pause");
            return;
        }

        printf("Enter Address: ");
        getchar();
        fgets(e.address, sizeof(e.address), stdin);
        e.address[strcspn(e.address, "\n")] = '\0';

        printf("Enter Department: ");
        fgets(e.department, sizeof(e.department), stdin);
        e.department[strcspn(e.department, "\n")] = '\0';

        printf("Enter Salary: ");
        if (scanf("%f", &e.salary) != 1 || !isValidSalary(e.salary)) {
            printf("\n Invalid salary. Please enter the valid salary.\n");
            fflush(stdin);
            system("pause");
            return;
        }

        fseek(fp, 0, SEEK_END);
        fwrite(&e, recsize, 1, fp);
        fflush(fp);

        printf("\n Employee added successfully!\n");
        printf("Add another employee? (y/n): ");
        getchar();
        another = getchar();
    }
}

// List employees
void listEmployees() {
    struct emp e;
    rewind(fp);

    system("cls");
    printf("ID   | Name             | Age | Address           | Department       | Salary\n");
    printf("-------------------------------------------------------------------------------\n");

    int found = 0;
    while (fread(&e, recsize, 1, fp) == 1) {
        printf("%-4d | %-16s | %-3d | %-17s | %-16s | %.2f\n", e.emp_id, e.name, e.age, e.address, e.department, e.salary);
        found = 1;
    }

    if (!found) {
        printf("\nNo employees found!!!\n");
    }

    printf("\n");
    system("pause");
}

// Modify employee
void modifyEmployee() {
    struct emp e;
    int id, found = 0;

    if (fp == NULL) {
        printf("Error: File not opened.\n");
        system("pause");
        return;
    }

    system("cls");
    printf("Enter Employee ID to modify: ");
    scanf("%d", &id);
    rewind(fp);

    while (fread(&e, recsize, 1, fp) == 1) {
        if (e.emp_id == id) {
            found = 1;

            printf("\nEditing Employee ID: %d\n", e.emp_id);

            printf("Enter New Name: ");
            getchar();  // Clear newline
            fgets(e.name, sizeof(e.name), stdin);
            e.name[strcspn(e.name, "\n")] = '\0';
            if (!isValidName(e.name)) {
                printf("\nInvalid name! Name should contain only letters.\n");
                while (getchar() != '\n');
                system("pause");
                return;
            }

            printf("Enter New Age: ");
            if (scanf("%d", &e.age) != 1 || !isValidAge(e.age)) {
                printf("\nInvalid age! Age should be a valid number.\n");
                while (getchar() != '\n');
                system("pause");
                return;
            }

            printf("Enter New Address: ");
            getchar();
            fgets(e.address, sizeof(e.address), stdin);
            e.address[strcspn(e.address, "\n")] = '\0';

            printf("Enter New Department: ");
            fgets(e.department, sizeof(e.department), stdin);
            e.department[strcspn(e.department, "\n")] = '\0';

            printf("Enter New Salary: ");
            if (scanf("%f", &e.salary) != 1 || !isValidSalary(e.salary)) {
                printf("\nInvalid salary! Salary should be a valid float value.\n");
                while (getchar() != '\n');
                system("pause");
                return;
            }

            fseek(fp, -recsize, SEEK_CUR);
            fwrite(&e, recsize, 1, fp);
            fflush(fp);

            printf("\nEmployee updated successfully!\n");
            system("pause");
            break;
        }
    }

    if (!found) {
        printf("\nEmployee not found!!!\n", id);
        system("pause");
    }
}


// Delete employee
void deleteEmployee() {
    struct emp e;
    int id, found = 0;
    FILE *temp = fopen("TEMP.DAT", "wb");

    printf("Enter Employee ID to delete: ");
    scanf("%d", &id);

    rewind(fp);
    while (fread(&e, recsize, 1, fp) == 1) {
        if (e.emp_id != id) {
            fwrite(&e, recsize, 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("EMP.DAT");
    rename("TEMP.DAT", "EMP.DAT");

    fp = fopen("EMP.DAT", "rb+");
    if (found)
        printf("\n Employee deleted.\n");
    else
        printf("\nEmployee not found!!!\n");

    system("pause");
}

// Search employee
void searchEmployee() {
    struct emp e;
    int id, found = 0;

    printf("Enter Employee ID to search: ");
    scanf("%d", &id);
    system("cls");
    rewind(fp);
    while (fread(&e, recsize, 1, fp) == 1) {
        if (e.emp_id == id) {
            found = 1;
            printf("\nEmployee Found:\n");
            printf("------------------------\n");
            printf("ID        : %d\n", e.emp_id);
            printf("Name      : %s\n", e.name);
            printf("Age       : %d\n", e.age);
            printf("Address   : %s\n", e.address);
            printf("Department: %s\n", e.department);
            printf("Salary    : %.2f\n", e.salary);
            printf("------------------------\n");
            break;
        }
    }

    if (!found)
        printf("\nEmployee not found!!!\n");

    system("pause");
}

// Sort employees
void sortEmployees() {
    struct emp e[100];
    int count = 0, i, j;
    char sortChoice;

    rewind(fp);
    while (fread(&e[count], recsize, 1, fp) == 1 && count < 100) {
        count++;
    }

    if (count == 0) {
        printf("\nNo employees to sort.\n");
        system("pause");
        return;
    }

    printf("\nSort by:\n");
    printf("1> Name\n");
    printf("2> Department\n");
    printf("3> Salary\n");
    printf("Enter your choice: ");
    fflush(stdin);
    sortChoice = getchar();
    system("cls");
    switch (sortChoice) {
        case '1': // Sort by Name
            for (i = 0; i < count - 1; i++) {
                for (j = 0; j < count - i - 1; j++) {
                    if (strcmp(e[j].name, e[j + 1].name) > 0) {
                        swap(&e[j], &e[j + 1]);
                    }
                }
            }
            printf("\nEmployees sorted by name:\n");
            break;

        case '2': // Sort by Department
            for (i = 0; i < count - 1; i++) {
                for (j = 0; j < count - i - 1; j++) {
                    if (strcmp(e[j].department, e[j + 1].department) > 0) {
                        swap(&e[j], &e[j + 1]);
                    }
                }
            }
            printf("\nEmployees sorted by department:\n");
            break;

        case '3': // Sort by Salary
            for (i = 0; i < count - 1; i++) {
                for (j = 0; j < count - i - 1; j++) {
                    if (e[j].salary > e[j + 1].salary) {
                        swap(&e[j], &e[j + 1]);
                    }
                }
            }
            printf("\nEmployees sorted by salary:\n");
            break;

        default:
            printf("\nInvalid sort option!\n");
            system("pause");
            return;
    }

    // Print sorted result (don't write to file)
    printf("\nID   | Name             | Age | Address           | Department       | Salary\n");
    printf("-------------------------------------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%-4d | %-16s | %-3d | %-17s | %-16s | %.2f\n",
               e[i].emp_id, e[i].name, e[i].age, e[i].address, e[i].department, e[i].salary);
    }

    system("pause");
}


// Swap function
void swap(struct emp *a, struct emp *b) {
    struct emp temp = *a;
    *a = *b;
    *b = temp;
}
