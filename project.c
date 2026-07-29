#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX 100
#define WORKING_DAYS 30
#define ROLE_COUNT 16

const char *roles[ROLE_COUNT] = {
    "Vice Chancellor (VC)",
    "Dean",
    "Head of Department (HOD)",
    "Professor",
    "Associate Professor",
    "Assistant Professor",
    "Lecturer",
    "Administrative Officer",
    "Accountant",
    "HR Officer",
    "Office Assistant",
    "Security Officer",
    "Cleaner",
    "Driver",
    "Chief Financial Officer (CFO)",
    "Finance Director"
};

float roleSalaries[ROLE_COUNT] = {
    300000,             /* Vice Chancellor (VC) */
    220000,             /* Dean */
    180000,             /* Head of Department (HOD) */
    150000,             /* Professor */
    120000,             /* Associate Professor */
    90000,              /* Assistant Professor */
    60000,              /* Lecturer */
    40000,              /* Administrative Officer */
    45000,              /* Accountant */
    45000,              /* HR Officer */
    25000,              /* Office Assistant */
    22000,              /* Security Officer */
    18000,              /* Cleaner */
    20000,              /* Driver */
    250000,             /* Chief Financial Officer (CFO) */
    200000              /* Finance Director */
};

struct Employee
{
    int id;
    char name[MAX];
    char role[MAX];
    char phone[MAX];
    float baseSalary;
};

void md5String(const char *str, uint8_t *outDigest);
void digestToHex(const uint8_t digest[16], char hex[33]);

void printRoleMenu()
{
    int i;

    printf("\n----- SELECT ROLE -----\n");
    for (i = 0; i < ROLE_COUNT; i++)
        printf("%d. %s (Base Salary: %.2f)\n", i + 1, roles[i], roleSalaries[i]);
}

void getRoleChoice(char role[], float *salary)
{
    int choice = -1;

    printRoleMenu();
    printf("Enter role number: ");
    scanf("%d", &choice);

    while (choice < 1 || choice > ROLE_COUNT)
    {
        printf("Invalid role number. Try again: ");
        scanf("%d", &choice);
    }

    strcpy(role, roles[choice - 1]);
    *salary = roleSalaries[choice - 1];
}

int findEmployeeById(int id, struct Employee *out)
{
    FILE *fp;
    struct Employee e;
    int found = 0;

    fp = fopen("employees.txt", "r");
    if (fp == NULL)
        return 0;

    while (fscanf(fp, "%d", &e.id) != EOF)
    {
        fscanf(fp, " %[^\n]", e.name);
        fscanf(fp, " %[^\n]", e.role);
        fscanf(fp, " %[^\n]", e.phone);
        fscanf(fp, "%f", &e.baseSalary);

        if (e.id == id)
        {
            found = 1;
            *out = e;
            break;
        }
    }

    fclose(fp);
    return found;
}

int employeeExists(int id)
{
    struct Employee e;
    return findEmployeeById(id, &e);
}

int getBaseSalary(int empId, float *baseSalary)
{
    struct Employee e;

    if (findEmployeeById(empId, &e))
    {
        *baseSalary = e.baseSalary;
        return 1;
    }
    return 0;
}

void addEmployee()
{
    FILE *fp;
    struct Employee e;

    printf("Enter Employee ID: ");
    scanf("%d", &e.id);

    if (employeeExists(e.id))
    {
        printf("Employee ID already exists.\n");
        return;
    }

    printf("Enter Employee Name: ");
    scanf(" %[^\n]", e.name);

    getRoleChoice(e.role, &e.baseSalary);

    printf("Enter Phone Number: ");
    scanf(" %[^\n]", e.phone);

    fp = fopen("employees.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening employee file.\n");
        return;
    }

    fprintf(fp, "%d\n", e.id);
    fprintf(fp, "%s\n", e.name);
    fprintf(fp, "%s\n", e.role);
    fprintf(fp, "%s\n", e.phone);
    fprintf(fp, "%.2f\n", e.baseSalary);

    fclose(fp);

    printf("\nEmployee registered successfully.\n");
    printf("Username (Portal Login): %d\n", e.id);
    printf("Password (Portal Login): %s (their phone number)\n", e.phone);
    printf("Assigned Base Salary: %.2f\n", e.baseSalary);
}

void updateEmployee()
{
    FILE *fp, *temp;
    struct Employee e;
    int targetId, found = 0;

    printf("Enter Employee ID to update: ");
    scanf("%d", &targetId);

    fp = fopen("employees.txt", "r");
    if (fp == NULL)
    {
        printf("No employee records found.\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d", &e.id) != EOF)
    {
        fscanf(fp, " %[^\n]", e.name);
        fscanf(fp, " %[^\n]", e.role);
        fscanf(fp, " %[^\n]", e.phone);
        fscanf(fp, "%f", &e.baseSalary);

        if (e.id == targetId)
        {
            found = 1;

            printf("Current Name: %s\n", e.name);
            printf("Enter New Name: ");
            scanf(" %[^\n]", e.name);

            printf("Current Role: %s (Salary: %.2f)\n", e.role, e.baseSalary);
            printf("Update Role? (1 = Yes, 0 = No): ");
            {
                int updateRole;
                scanf("%d", &updateRole);
                if (updateRole == 1)
                    getRoleChoice(e.role, &e.baseSalary);
            }

            printf("Current Phone (also their password): %s\n", e.phone);
            printf("Enter New Phone Number: ");
            scanf(" %[^\n]", e.phone);
        }

        fprintf(temp, "%d\n", e.id);
        fprintf(temp, "%s\n", e.name);
        fprintf(temp, "%s\n", e.role);
        fprintf(temp, "%s\n", e.phone);
        fprintf(temp, "%.2f\n", e.baseSalary);
    }

    fclose(fp);
    fclose(temp);

    remove("employees.txt");
    rename("temp.txt", "employees.txt");

    if (found)
        printf("Employee updated successfully.\n");
    else
        printf("Employee ID not found.\n");
}

void deleteEmployee()
{
    FILE *fp, *temp;
    struct Employee e;
    int targetId, found = 0;

    printf("Enter Employee ID to delete: ");
    scanf("%d", &targetId);

    fp = fopen("employees.txt", "r");
    if (fp == NULL)
    {
        printf("No employee records found.\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d", &e.id) != EOF)
    {
        fscanf(fp, " %[^\n]", e.name);
        fscanf(fp, " %[^\n]", e.role);
        fscanf(fp, " %[^\n]", e.phone);
        fscanf(fp, "%f", &e.baseSalary);

        if (e.id == targetId)
        {
            found = 1;
        }
        else
        {
            fprintf(temp, "%d\n", e.id);
            fprintf(temp, "%s\n", e.name);
            fprintf(temp, "%s\n", e.role);
            fprintf(temp, "%s\n", e.phone);
            fprintf(temp, "%.2f\n", e.baseSalary);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("employees.txt");
    rename("temp.txt", "employees.txt");

    if (found)
        printf("Employee deleted successfully.\n");
    else
        printf("Employee ID not found.\n");
}

void searchEmployeeById(int id)
{
    struct Employee e;

    if (findEmployeeById(id, &e))
    {
        printf("\nEmployee ID: %d\n", e.id);
        printf("Name: %s\n", e.name);
        printf("Role: %s\n", e.role);
        printf("Phone: %s\n", e.phone);
        printf("Base Salary: %.2f\n", e.baseSalary);
    }
    else
        printf("Employee ID not found.\n");
}

void searchEmployee()
{
    int id;

    printf("Enter Employee ID to search: ");
    scanf("%d", &id);

    searchEmployeeById(id);
}

void viewEmployees()
{
    FILE *fp;
    struct Employee e;
    int i, found;
    int total = 0;

    printf("\n===== EMPLOYEE LIST (DAFFODIL INTERNATIONAL UNIVERSITY) =====\n");

    for(i = 0; i < ROLE_COUNT; i++)
    {
        fp = fopen("employees.txt", "r");

        if(fp == NULL)
        {
            printf("No employee records found.\n");
            return;
        }

        found = 0;

        while(fscanf(fp, "%d", &e.id) != EOF)
        {
            fscanf(fp, " %[^\n]", e.name);
            fscanf(fp, " %[^\n]", e.role);
            fscanf(fp, " %[^\n]", e.phone);
            fscanf(fp, "%f", &e.baseSalary);

            if(strcmp(e.role, roles[i]) == 0)
            {
                if(found == 0)
                {
                    printf("\n====================================\n");
                    printf("Role: %s\n", roles[i]);
                    printf("====================================\n");
                    found = 1;
                }

                printf("ID: %d\n", e.id);
                printf("Name: %s\n", e.name);
                printf("Phone: %s\n", e.phone);
                printf("Salary: %.2f\n\n", e.baseSalary);

                total++;
            }
        }

        fclose(fp);
    }

    if(total == 0)
        printf("No employee records found.\n");
}

void recordAttendance()
{
    FILE *fp;
    int empId, day, month, year;
    char status[MAX];

    printf("Enter Employee ID: ");
    scanf("%d", &empId);

    if (employeeExists(empId) == 0)
    {
        printf("Employee ID not found.\n");
        return;
    }

    printf("Enter Day: ");
    scanf("%d", &day);
    printf("Enter Month: ");
    scanf("%d", &month);
    printf("Enter Year: ");
    scanf("%d", &year);
    printf("Enter Status (Present/Absent): ");
    scanf(" %[^\n]", status);

    fp = fopen("attendance.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening attendance file.\n");
        return;
    }

    fprintf(fp, "%d %d %d %d %s\n", empId, day, month, year, status);

    fclose(fp);
    printf("Attendance recorded successfully.\n");
}

void viewAttendanceById(int id)
{
    FILE *fp;
    int empId, day, month, year;
    char status[MAX];
    int found = 0;

    fp = fopen("attendance.txt", "r");
    if (fp == NULL)
    {
        printf("No attendance records found.\n");
        return;
    }

    printf("\n===== ATTENDANCE RECORDS =====\n");

    while (fscanf(fp, "%d", &empId) != EOF)
    {
        fscanf(fp, "%d", &day);
        fscanf(fp, "%d", &month);
        fscanf(fp, "%d", &year);
        fscanf(fp, " %[^\n]", status);

        if (id == 0 || empId == id)
        {
            printf("ID: %d | Date: %d/%d/%d | Status: %s\n", empId, day, month, year, status);
            found = 1;
        }
    }

    fclose(fp);

    if (found == 0)
        printf("No matching attendance records found.\n");
}

void viewAttendance()
{
    int empId;

    printf("Enter Employee ID (0 for all): ");
    scanf("%d", &empId);

    viewAttendanceById(empId);
}

void updateAttendance()
{
    FILE *fp, *temp;
    int empId, day, month, year, targetId, targetDay, targetMonth, targetYear;
    char status[MAX];
    int found = 0;

    printf("Enter Employee ID: ");
    scanf("%d", &targetId);
    printf("Enter Day: ");
    scanf("%d", &targetDay);
    printf("Enter Month: ");
    scanf("%d", &targetMonth);
    printf("Enter Year: ");
    scanf("%d", &targetYear);

    fp = fopen("attendance.txt", "r");
    if (fp == NULL)
    {
        printf("No attendance records found.\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d", &empId) != EOF)
    {
        fscanf(fp, "%d", &day);
        fscanf(fp, "%d", &month);
        fscanf(fp, "%d", &year);
        fscanf(fp, " %[^\n]", status);

        if (empId == targetId && day == targetDay && month == targetMonth && year == targetYear)
        {
            found = 1;
            printf("Current Status: %s\n", status);
            printf("Enter New Status (Present/Absent): ");
            scanf(" %[^\n]", status);
        }

        fprintf(temp, "%d %d %d %d %s\n", empId, day, month, year, status);
    }

    fclose(fp);
    fclose(temp);

    remove("attendance.txt");
    rename("temp.txt", "attendance.txt");

    if (found)
        printf("Attendance updated successfully.\n");
    else
        printf("Matching attendance record not found.\n");
}


float computeSalaryForMonth(int empId, int month, int year)
{
    FILE *fp;
    int recEmpId, day, recMonth, recYear;
    char status[MAX];
    float baseSalary = 0;
    int presentDays = 0;
    float perDaySalary, salary;

    if (getBaseSalary(empId, &baseSalary) == 0)
    {
        printf("Employee not found.\n");
        return -1;
    }

    fp = fopen("attendance.txt", "r");
    if (fp == NULL)
    {
        printf("No attendance records found. Salary computed as 0.\n");
        return 0;
    }

    while (fscanf(fp, "%d", &recEmpId) != EOF)
    {
        fscanf(fp, "%d", &day);
        fscanf(fp, "%d", &recMonth);
        fscanf(fp, "%d", &recYear);
        fscanf(fp, " %[^\n]", status);

        if (recEmpId == empId && recMonth == month && recYear == year && strcmp(status, "Present") == 0)
            presentDays++;
    }

    fclose(fp);

    perDaySalary = baseSalary / WORKING_DAYS;
    salary = perDaySalary * presentDays;

    return salary;
}

void calculateSalaryMenu()
{
    int empId, month, year;
    float salary;

    printf("Enter Employee ID: ");
    scanf("%d", &empId);
    printf("Enter Month (1-12): ");
    scanf("%d", &month);
    printf("Enter Year: ");
    scanf("%d", &year);

    salary = computeSalaryForMonth(empId, month, year);

    if (salary >= 0)
        printf("Calculated Salary for Employee %d (%d/%d): %.2f\n", empId, month, year, salary);
}

void generatePayroll()
{
    FILE *fp;
    int empId, month, year;
    float basicSalary, bonus, deduction, netSalary;

    printf("Enter Employee ID: ");
    scanf("%d", &empId);

    if (employeeExists(empId) == 0)
    {
        printf("Employee ID not found.\n");
        return;
    }

    printf("Enter Month (1-12): ");
    scanf("%d", &month);
    printf("Enter Year: ");
    scanf("%d", &year);

    basicSalary = computeSalaryForMonth(empId, month, year);

    if (basicSalary < 0)
        return;

    printf("Enter Bonus Amount: ");
    scanf("%f", &bonus);
    printf("Enter Deduction Amount: ");
    scanf("%f", &deduction);

    netSalary = basicSalary + bonus - deduction;

    fp = fopen("payroll.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening payroll file.\n");
        return;
    }

    fprintf(fp, "%d %d %d %.2f %.2f %.2f %.2f\n", empId, month, year, basicSalary, bonus, deduction, netSalary);

    fclose(fp);
    printf("Payroll generated successfully. Net Salary: %.2f\n", netSalary);
}

void viewPayrollById(int id)
{
    FILE *fp;
    int empId, month, year;
    float basicSalary, bonus, deduction, netSalary;
    int found = 0;

    fp = fopen("payroll.txt", "r");
    if (fp == NULL)
    {
        printf("No payroll records found.\n");
        return;
    }

    printf("\n===== PAYROLL RECORDS =====\n");

    while (fscanf(fp, "%d", &empId) != EOF)
    {
        fscanf(fp, "%d", &month);
        fscanf(fp, "%d", &year);
        fscanf(fp, "%f", &basicSalary);
        fscanf(fp, "%f", &bonus);
        fscanf(fp, "%f", &deduction);
        fscanf(fp, "%f", &netSalary);

        if (id == 0 || empId == id)
        {
            printf("ID: %d | Period: %d/%d | Basic: %.2f | Bonus: %.2f | Deduct: %.2f | Net: %.2f\n",
                   empId, month, year, basicSalary, bonus, deduction, netSalary);
            found = 1;
        }
    }

    fclose(fp);

    if (found == 0)
        printf("No matching payroll records found.\n");
}

void viewPayrollInfo()
{
    int empId;

    printf("Enter Employee ID (0 for all): ");
    scanf("%d", &empId);

    viewPayrollById(empId);
}

int getRoleCategory(const char *role)
{
    if (strcmp(role, "Vice Chancellor (VC)") == 0 ||
        strcmp(role, "Dean") == 0 ||
        strcmp(role, "Head of Department (HOD)") == 0)
        return 2;

    if (strcmp(role, "Administrative Officer") == 0 ||
        strcmp(role, "Accountant") == 0 ||
        strcmp(role, "HR Officer") == 0 ||
        strcmp(role, "Chief Financial Officer (CFO)") == 0 ||
        strcmp(role, "Finance Director") == 0)
        return 1;

    return 3;
}

int authenticate(int *outId, char outRole[])
{
    int id;
    char password[MAX];
    struct Employee e;

    printf("Enter Employee ID (Username): ");
    scanf("%d", &id);
    printf("Enter Password: ");
    scanf(" %[^\n]", password);

    if (!findEmployeeById(id, &e))
    {
        printf("Employee ID not found.\n");
        return 0;
    }

    if (strcmp(password, e.phone) != 0)
    {
        printf("Incorrect password.\n");
        return 0;
    }

    printf("\nLogin successful. Welcome, %s (%s)\n", e.name, e.role);

    *outId = e.id;
    strcpy(outRole, e.role);
    return 1;
}


void hrMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        printf("\n----- HR / ADMIN PORTAL (DIU) -----\n");
        printf("1. Add Employee\n");
        printf("2. Update Employee\n");
        printf("3. Delete Employee\n");
        printf("4. Search Employee\n");
        printf("5. View Employees\n");
        printf("6. Record Attendance\n");
        printf("7. View Attendance\n");
        printf("8. Update Attendance\n");
        printf("9. Calculate Salary\n");
        printf("10. Generate Payroll\n");
        printf("11. View Payroll Information\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addEmployee();
                break;
            case 2:
                updateEmployee();
                break;
            case 3:
                deleteEmployee();
                break;
            case 4:
                searchEmployee();
                break;
            case 5:
                viewEmployees();
                break;
            case 6:
                recordAttendance();
                break;
            case 7:
                viewAttendance();
                break;
            case 8:
                updateAttendance();
                break;
            case 9:
                calculateSalaryMenu();
                break;
            case 10:
                generatePayroll();
                break;
            case 11:
                viewPayrollInfo();
                break;
            case 0:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }
}

void managerMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        printf("\n----- MANAGER PORTAL (VC / DEAN / HOD) -----\n");
        printf("1. View Employees\n");
        printf("2. View Attendance\n");
        printf("3. View Payroll Information\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                viewEmployees();
                break;
            case 2:
                viewAttendance();
                break;
            case 3:
                viewPayrollInfo();
                break;
            case 0:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }
}

void employeeMenu(int selfId)
{
    int choice = -1;

    while (choice != 0)
    {
        printf("\n----- EMPLOYEE PORTAL -----\n");
        printf("1. View My Information\n");
        printf("2. View My Attendance\n");
        printf("3. View My Payroll Information\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                searchEmployeeById(selfId);
                break;
            case 2:
                viewAttendanceById(selfId);
                break;
            case 3:
                viewPayrollById(selfId);
                break;
            case 0:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }
}

void loginPortal()
{
    int id, category;
    char role[MAX];

    if (!authenticate(&id, role))
        return;

    category = getRoleCategory(role);

    if (category == 1)
        hrMenu();
    else if (category == 2)
        managerMenu();
    else
        employeeMenu(id);
}


int checkPortalPassword()
{
    char input[100];
    uint8_t digest[16];
    char hex[33];
    const char *correctHash = "81dc9bdb52d04dc20036dbd8313ed055";

    printf("\n----- PORTAL LOCKED -----\n");
    printf("Enter master password to access the system: ");
    scanf("%99s", input);

    md5String(input, digest);
    digestToHex(digest, hex);

    if (strcmp(hex, correctHash) == 0)
        return 1;

    printf("Incorrect password. Access denied.\n");
    return 0;
}

int main()
{
    int choice = -1;

    if (!checkPortalPassword())
        return 1;

    while (choice != 0)
    {
        printf("\n===== DAFFODIL INTERNATIONAL UNIVERSITY =====\n");
        printf("=====          EMPLOYEE PORTAL           =====\n");
        printf("1. Login\n");
        printf("2. Register New Employee (HR Use)\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                loginPortal();
                break;
            case 2:
                addEmployee();
                break;
            case 0:
                printf("Exiting system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }

    return 0;
}

typedef struct {
    uint64_t size;
    uint32_t buffer[4];
    uint8_t  input[64];
    uint8_t  digest[16];
} MD5Context;

static const uint32_t MD5_K[64] = {
    0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,
    0xa8304613,0xfd469501,0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
    0x6b901122,0xfd987193,0xa679438e,0x49b40821,0xf61e2562,0xc040b340,
    0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
    0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,
    0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
    0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,0x289b7ec6,0xeaa127fa,
    0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
    0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,
    0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
    0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
};

static const uint32_t MD5_R[64] = {
    7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
    5, 9,14,20, 5, 9,14,20, 5, 9,14,20, 5, 9,14,20,
    4,11,16,23, 4,11,16,23, 4,11,16,23, 4,11,16,23,
    6,10,15,21, 6,10,15,21, 6,10,15,21, 6,10,15,21
};

#define LEFTROT(x,c) (((x)<<(c)) | ((x)>>(32-(c))))

static void md5_chunk(uint32_t b[4], const uint8_t chunk[64]) {
    uint32_t w[16], a=b[0], b2=b[1], c=b[2], d=b[3];
    int i;
    for(i=0;i<16;i++)
        w[i]=chunk[i*4]|(chunk[i*4+1]<<8)|(chunk[i*4+2]<<16)|(chunk[i*4+3]<<24);

    for(i=0;i<64;i++){
        uint32_t f,g;
        if(i<16){ f=(b2&c)|((~b2)&d); g=i; }
        else if(i<32){ f=(d&b2)|((~d)&c); g=(5*i+1)%16; }
        else if(i<48){ f=b2^c^d; g=(3*i+5)%16; }
        else{ f=c^(b2|(~d)); g=(7*i)%16; }
        uint32_t t=d; d=c; c=b2;
        b2=b2+LEFTROT((a+f+MD5_K[i]+w[g]),MD5_R[i]);
        a=t;
    }
    b[0]+=a; b[1]+=b2; b[2]+=c; b[3]+=d;
}

void md5Init(MD5Context *ctx) {
    ctx->size=0;
    ctx->buffer[0]=0x67452301;
    ctx->buffer[1]=0xefcdab89;
    ctx->buffer[2]=0x98badcfe;
    ctx->buffer[3]=0x10325476;
}

void md5Update(MD5Context *ctx, const uint8_t *data, size_t len) {
    size_t i;
    for(i=0;i<len;i++){
        ctx->input[(ctx->size%64)] = data[i];
        ctx->size++;
        if(ctx->size%64==0) md5_chunk(ctx->buffer, ctx->input);
    }
}

void md5Finalize(MD5Context *ctx) {
    uint64_t bits = ctx->size*8;
    uint8_t pad[64];
    size_t padLen = (ctx->size%64<56)?(56-ctx->size%64):(120-ctx->size%64);
    memset(pad,0,sizeof(pad)); pad[0]=0x80;
    md5Update(ctx,pad,padLen);
    uint8_t lenBytes[8]={0};
    int i; for(i=0;i<8;i++) lenBytes[i]=(bits>>(i*8))&0xFF;
    md5Update(ctx,lenBytes,8);
    for(i=0;i<4;i++){
        ctx->digest[i*4]   = ctx->buffer[i]&0xFF;
        ctx->digest[i*4+1] = (ctx->buffer[i]>>8)&0xFF;
        ctx->digest[i*4+2] = (ctx->buffer[i]>>16)&0xFF;
        ctx->digest[i*4+3] = (ctx->buffer[i]>>24)&0xFF;
    }
}

void md5String(const char *str, uint8_t *outDigest) {
    MD5Context ctx;
    md5Init(&ctx);
    md5Update(&ctx, (const uint8_t*)str, strlen(str));
    md5Finalize(&ctx);
    memcpy(outDigest, ctx.digest, 16);
}

void digestToHex(const uint8_t digest[16], char hex[33]) {
    int i;
    for(i=0;i<16;i++) sprintf(hex+i*2,"%02x",digest[i]);
    hex[32]='\0';
}