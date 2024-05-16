#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h> // contains strcmp(), strcpy(), strlen(), etc.
#include <ctype.h>  // contains toupper(), tolower(), etc.
#include <time.h>

#define RETURNTIME 15

char categories[][15] = { "Computer", "Philosophy", "Law", "Electronic", "Medicine", "Architecture" };
void Returnfunc(void);
void Mainmenu(void);
void Addbooks(void);
void Deletebooks(void);
void Editbooks(void);
void Searchbooks(void);
void Issuebooks(void);
void Viewbooks(void);
int getdata();
int Checkid(int);
int t(void);
void Password();
void Issuerecord();

// list of global files that can be accessed from anywhere in program
FILE* fp, * ft, * fs;

// list of global variables
int s;
char findbook;
char pwd[10] = { "admin" };
struct dtm {
    int mm, dd, yy;
};
struct books {
    int id;
    char stname[20];
    char Name[20];
    char Author[20];
    int Quantity;
    float Price;
    int count;
    int Rackno;
    char* cat;
    struct dtm issued;
    struct dtm duedate;
};
struct books a;

int main() {
    Password();
    return 0;
}

void Password() {
    system("cls");
    char d[25] = " Password Protected ";
    char ch, pass[10];
    int i = 0, j;

    printf("\n\t");
    for (j = 0; j < 20; j++) {
        Sleep(50);
        printf("=");
    }
    for (j = 0; j < 20; j++) {
        Sleep(50);
        printf("%c", d[j]);
    }
    for (j = 0; j < 20; j++) {
        Sleep(50);
        printf("=");
    }
    printf("\n\n\n\tEnter Password: ");
    while (1) {
        ch = getch();
        if (ch == 13) break; // Enter key is pressed
        if (ch != 8) { // Not a backspace
            putch('*');
            pass[i] = ch;
            i++;
        }
    }
    pass[i] = '\0';
    if (strcmp(pass, pwd) == 0) {
        printf("\n\n\n\tPassword match............ ");
        printf("\n\tPress any key to continue....");
        getch();
        Mainmenu();
    }
    else {
        printf("\n\n\t\aWarning!! Incorrect Password");
        getch();
        Password();
    }
}

void Mainmenu(void) {
    system("cls");
    printf("\n\t == Main Menu ==");
    printf("\n\t 1. Add Books ");
    printf("\n\t 2. Delete Books");
    printf("\n\t 3. Search Books");
    printf("\n\t 4. Issue Books");
    printf("\n\t 5. View Book List");
    printf("\n\t 6. Edit Book List");
    printf("\n\t 7. Close Application");
    printf("\n\t +++++++++++++++++++++++++++++++++++++++++");
    t();
    printf("\n\t Enter your choice:");

    switch (getch()) {
    case '1':
        Addbooks();
        break;
    case '2':
        Deletebooks();
        break;
    case '3':
        Searchbooks();
        break;
    case '4':
        Issuebooks();
        break;
    case '5':
        Viewbooks();
        break;
    case '6':
        Editbooks();
        break;
    case '7':
        system("cls");
        exit(0);
    default:
        printf("\n\t\aWrong Entry!! Please re-enter correct option");
        if (getch())
            Mainmenu();
    }
}

void Addbooks(void) {
    system("cls");
    printf("\n\t |: == SELECT CATEGORY |");
    printf("\n\t 1. Computer");
    printf("\n\t 2. Philosophy");
    printf("\n\t 3. Law");
    printf("\n\t 4. Electronic");
    printf("\n\t 5. Medicine");
    printf("\n\t 6. Architecture");
    printf("\n\t 7. Back to Main Menu");
    printf("\n\t +++++++++++++++++++++++++++++++++++++++++");
    printf("\n\t Enter your choice:");
    scanf("%d", &s);
    if (s == 7)
        Mainmenu();

    system("cls");
    fp = fopen("Mydat.dat", "ab+");
    if (getdata() == 1) {
        a.cat = categories[s - 1];
        fseek(fp, 0, SEEK_END);
        fwrite(&a, sizeof(a), 1, fp);
        fclose(fp);
        printf("\n\tRecord is successfully saved");
    }
    else {
        fclose(fp);
    }
    printf("\n\tSave any more? (Y / N):");
    if (getch() == 'n')
        Mainmenu();
    else
        Addbooks();
}

void Deletebooks(void) {
    system("cls");
    int d;
    char another = 'y';
    while (another == 'y') {
        system("cls");
        printf("\n\tEnter the Book ID to delete:");
        scanf("%d", &d);
        fp = fopen("Mydat.dat", "rb+");
        rewind(fp);
        while (fread(&a, sizeof(a), 1, fp) == 1) {
            if (a.id == d) {
                printf("\n\tThe book record is available");
                printf("\n\tBook name is %s", a.Name);
                printf("\n\tRack No. is %d", a.Rackno);
                findbook = 't';
            }
        }
        if (findbook != 't') {
            printf("\n\tNo record is found. Modify the search");
            if (getch())
                Mainmenu();
        }
        if (findbook == 't') {
            printf("\n\tDo you want to delete it? (Y/N):");
            if (getch() == 'y') {
                ft = fopen("test.dat", "wb+"); // temporary file for delete
                rewind(fp);
                while (fread(&a, sizeof(a), 1, fp) == 1) {
                    if (a.id != d) {
                        fwrite(&a, sizeof(a), 1, ft); // write all in temporary file except that we want to delete
                    }
                }
                fclose(ft);
                fclose(fp);
                remove("Mydat.dat");
                rename("test.dat", "Mydat.dat"); // copy all items from temporary file to fp except that
                fp = fopen("Mydat.dat", "rb+"); // we want to delete
                if (findbook == 't') {
                    printf("\n\tThe record is successfully deleted");
                    printf("\n\tDelete another record? (Y/N)");
                }
            }
            else
                Mainmenu();
            fflush(stdin);
            another = getch();
        }
    }
    Mainmenu();
}

void Editbooks(void) {
    system("cls");
    int c = 0;
    int d;
    printf("\n\t * Edit Books Section *");
    char another = 'y';
    while (another == 'y') {
        system("cls");
        printf("\n\tEnter Book ID to be edited:");
        scanf("%d", &d);
        fp = fopen("Mydat.dat", "rb+");
        while (fread(&a, sizeof(a), 1, fp) == 1) {
            if (Checkid(d) == 0) {
                printf("\n\tThe book is available");
                printf("\n\tThe Book ID: %d", a.id);
                printf("\n\tEnter new name: ");
                scanf("%s", a.Name);
                printf("\n\tEnter new Author: ");
                scanf("%s", a.Author);
                printf("\n\tEnter new quantity: ");
                scanf("%d", &a.Quantity);
                printf("\n\tEnter new price: ");
                scanf("%f", &a.Price);
                printf("\n\tEnter new rackno: ");
                scanf("%d", &a.Rackno);
                printf("\n\tThe record is modified");
                fseek(fp, ftell(fp) - sizeof(a), 0);
                fwrite(&a, sizeof(a), 1, fp);
                fclose(fp);
                c = 1;
            }
            if (c == 0) {
                printf("\n\tNo record found");
            }
        }
        printf("\n\tModify another Record? (Y/N)");
        fflush(stdin);
        another = getch();
    }
    Returnfunc();
}

void Searchbooks(void) {
    system("cls");
    int d;
    printf("\n\t** Search Books **");
    printf("\n\t1. Search By ID");
    printf("\n\t2. Search By Name");
    printf("\n\tEnter Your Choice: ");
    fp = fopen("Mydat.dat", "rb+"); // open file for reading purpose
    rewind(fp); // move pointer at the beginning of file
    switch (getch()) {
    case '1': {
        system("cls");
        printf("\n\t** Search Books By Id **");
        printf("\n\tEnter the book id: ");
        scanf("%d", &d);
        printf("\n\tSearching........");
        while (fread(&a, sizeof(a), 1, fp) == 1) {
            if (a.id == d) {
                Sleep(2);
                printf("\n\tThe Book is available");
                printf("\n\t=====================================");
                printf("\n\tID: %d", a.id);
                printf("\n\tName: %s", a.Name);
                printf("\n\tAuthor: %s", a.Author);
                printf("\n\tQuantity: %d", a.Quantity);
                printf("\n\tPrice: Rs.%.2f", a.Price);
                printf("\n\tRack No: %d", a.Rackno);
                printf("\n\t======================================");
                findbook = 't';
            }
        }
        if (findbook != 't') { // checks whether condition enters inside loop or not
            printf("\n\t| ==================================== |");
            printf("\n\t| ==================================== |");
            printf("\n\t\aNo Record Found");
        }
        printf("\n\tTry another search? (Y/N)");
        if (getch() == 'y')
            Searchbooks();
        else
            Mainmenu();
        break;
    }
    case '2': {
        char s[15];
        system("cls");
        printf("\n\t** Search Books By Name **");
        printf("\n\tEnter Book Name: ");
        scanf("%s", s);
        int d = 0;
        while (fread(&a, sizeof(a), 1, fp) == 1) {
            if (strcmp(a.Name, s) == 0) {
                printf("\n\tThe Book is available");
                printf("\n\t==============================");
                printf("\n\tID: %d", a.id);
                printf("\n\tName: %s", a.Name);
                printf("\n\tAuthor: %s", a.Author);
                printf("\n\tQuantity: %d", a.Quantity);
                printf("\n\tPrice: Rs.%.2f", a.Price);
                printf("\n\tRack No: %d", a.Rackno);
                printf("\n\t==============================");
                d++;
            }
        }
        if (d == 0) {
            printf("\n\t\aNo Record Found");
        }
        printf("\n\tTry another search? (Y/N)");
        if (getch() == 'y')
            Searchbooks();
        else
            Mainmenu();
        break;
    }
    default:
        getch();
        Searchbooks();
    }
    fclose(fp);
}

void Viewbooks(void) {
    int i = 0;
    system("cls");
    printf("\n\t|****** Book List ******|");
    printf("\n\t| CATEGORY      ID    BOOK NAME      AUTHOR       QUANTITY     PRICE          RackNo      \n\t");
    fp = fopen("Mydat.dat", "rb");
    while (fread(&a, sizeof(a), 1, fp) == 1) {
        printf("\n\t%s", a.cat);
        printf("\t%d", a.id);
        printf("\t%s", a.Name);
        printf("\t%s", a.Author);
        printf("\t\t%d", a.Quantity);
        printf("\t\t%.2f", a.Price);
        printf("\t%d", a.Rackno);
        printf("\n\n");
        i = i + a.Quantity;
    }
    printf("\n\n\t --------------------------------");
    printf("\n\t Total Books = %d", i);
    fclose(fp);
    Returnfunc();
}

void Issuebooks() {
    system("cls");
    printf("\n\t| ========================================== Issue Book ========================================== |");
    printf("\n\t| ========================================== Issue Book Section ================================== |");
    printf("\n\t| 1. Issue a book");
    printf("\n\t| 2. View issued books");
    printf("\n\t| 3. Search issued books");
    printf("\n\t| 4. Remove issued books");
    int t;
    printf("\n\t| Enter your choice: ");
    switch (getch()) {
    case '1': {
        int c = 0;
        char another = 'y';
        system("cls");
        printf("\n\tEnter the book ID: ");
        scanf("%d", &t);
        fp = fopen("Mydat.dat", "rb");
        fs = fopen("issue.dat", "ab+");
        if (Checkid(t) == 0) {
            printf("\n\tThe record is available");
            printf("\n\tThere are %d unissued books in the library", a.Quantity);
            printf("\n\tThe name of the book is %s", a.Name);
            printf("\n\tEnter student name: ");
            scanf("%s", a.stname);
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            a.issued.dd = tm.tm_mday;
            a.issued.mm = tm.tm_mon + 1;
            a.issued.yy = tm.tm_year + 1900;
            a.duedate.dd = a.issued.dd + RETURNTIME;
            a.duedate.mm = a.issued.mm;
            a.duedate.yy = a.issued.yy;
            if (a.duedate.dd > 30) {
                a.duedate.mm += a.duedate.dd / 30;
                a.duedate.dd %= 30;
            }
            if (a.duedate.mm > 12) {
                a.duedate.yy += a.duedate.mm / 12;
                a.duedate.mm %= 12;
            }
            printf("\n\tIssued date: %d-%d-%d", a.issued.dd, a.issued.mm, a.issued.yy);
            printf("\n\tThe book of ID %d is issued", a.id);
            printf("\n\tTo return: %d-%d-%d", a.duedate.dd, a.duedate.mm, a.duedate.yy);
            fseek(fs, 0, SEEK_END);
            fwrite(&a, sizeof(a), 1, fs);
            c = 1;
            if (c == 0) {
                printf("\n\tNo record found...!");
            }
            printf("\n\tIssue any more books? (Y/N)");
            fflush(stdin);
            another = getch();
            fclose(fp);
        }
        break;
    }
    case '2': {
        system("cls");
        printf("\n\t| ========================================== Issued Book List ========================================== |");
        printf("\n\t| Student Name\tCategory\tID\tBook Name\tIssue Date\tReturn Date");
        fs = fopen("issue.dat", "rb");
        while (fread(&a, sizeof(a), 1, fs) == 1) {
            printf("\n\t| %s", a.stname);
            printf("\t%s", a.cat);
            printf("\t%d", a.id);
            printf("\t%s", a.Name);
            printf("\t%d-%d-%d", a.issued.dd, a.issued.mm, a.issued.yy);
            printf("\t%d-%d-%d", a.duedate.dd, a.duedate.mm, a.duedate.yy);
        }
        fclose(fs);
        getch();
        Mainmenu();
        break;
    }
    case '3': {
        system("cls");
        int p, c = 0;
        char another = 'y';
        printf("\n\tEnter the book ID: ");
        while (another == 'y') {
            scanf("%d", &p);
            fs = fopen("issue.dat", "rb");
            while (fread(&a, sizeof(a), 1, fs) == 1) {
                if (a.id == p) {
                    Issuerecord();
                    printf("\n\tPress any key to continue...");
                    getch();
                    c = 1;
                }
            }
            fclose(fs);
            if (c == 0) {
                printf("\n\tNo record found...");
            }
            printf("\n\tTry another search? (Y/N)");
            another = getch();
        }
        Mainmenu();
        break;
    }
    case '4': {
        system("cls");
        int b;
        FILE* fg;
        char another = 'y';
        while (another == 'y') {
            printf("\n\tEnter the book ID to remove: ");
            scanf("%d", &b);
            fs = fopen("issue.dat", "rb");
            while (fread(&a, sizeof(a), 1, fs) == 1) {
                if (a.id == b) {
                    Issuerecord();
                    findbook = 't';
                }
            }
            if (findbook == 't') {
                printf("\n\tDo you want to remove it? (Y/N)");
                if (getch() == 'y') {
                    fg = fopen("record.dat", "wb+");
                    rewind(fs);
                    while (fread(&a, sizeof(a), 1, fs) == 1) {
                        if (a.id != b) {
                            fwrite(&a, sizeof(a), 1, fg);
                        }
                    }
                    fclose(fs);
                    fclose(fg);
                    remove("issue.dat");
                    rename("record.dat", "issue.dat");
                    printf("\n\tThe issued book is removed");
                }
            }
            else {
                printf("\n\tNo record found");
            }
            printf("\n\tDelete any more records? (Y/N)");
            another = getch();
        }
        Mainmenu();
        break;
    }
    default: {
        printf("\n\tYour choice is wrong.....!");
        getch();
        Issuebooks();
        break;
    }
    }
}

void Returnfunc(void) {
    printf("\n\tPress ENTER to return to Main Menu");
    while (getch() != 13); // allow only use of enter
    Mainmenu();
}

int getdata() {
    int t;
    printf("\n\t================== Enter the Information Below =================");
    printf("\n\t++++++++++++++++++++");
    printf("\n\tCategory: %s", categories[s - 1]);
    printf("\n\tBook ID: ");
    scanf("%d", &t);
    if (Checkid(t) == 0) {
        printf("\n\t\aThe book id already exists\a");
        getch();
        Mainmenu();
        return 0;
    }
    a.id = t;
    printf("\n\tBook Name: ");
    scanf("%s", a.Name);
    printf("\n\tAuthor: ");
    scanf("%s", a.Author);
    printf("\n\tQuantity: ");
    scanf("%d", &a.Quantity);
    printf("\n\tPrice: ");
    scanf("%f", &a.Price);
    printf("\n\tRack No: ");
    scanf("%d", &a.Rackno);
    return 1;
}

int Checkid(int t) {
    rewind(fp);
    while (fread(&a, sizeof(a), 1, fp) == 1)
        if (a.id == t)
            return 0; // returns 0 if book exists
    return 1; // return 1 if it does not
}

int t(void) {
    time_t now;
    time(&now);
    printf("\n\tLocal Date and Time: %s\n", ctime(&now));
    return 0;
}

void Issuerecord() {
    system("cls");
    printf("\n\t| The book has been taken by: %s", a.stname);
    printf("\n\t| Issued Date: %d-%d-%d", a.issued.dd, a.issued.mm, a.issued.yy);
    printf("\n\t| Returning Date: %d-%d-%d", a.duedate.dd, a.duedate.mm, a.duedate.yy);
}