#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Book {
    int book_id;
    char title[50];
    char author[50];
};


struct User {
    int user_id;
    char name[50];
    char type[10]; 
};


struct Issue {
    int user_id;
    int book_id;
    char due_date[20]; 
};

void add_book() {
    struct Book b;
    FILE *fp = fopen("books.txt", "a");
    printf("Enter Book ID: ");
    scanf("%d", &b.book_id);
    getchar();
    printf("Enter Book Title: ");
    gets(b.title);
    printf("Enter Book Author: ");
    gets(b.author);
    fprintf(fp, "book id %d ,title %s, author %s\n", b.book_id, b.title, b.author);
    fclose(fp);
    printf("Book added.\n");
}


void add_user() {
    struct User u;
    FILE *fp = fopen("users.txt", "a");
    printf("Enter User ID: ");
    scanf("%d", &u.user_id);
    getchar();
    printf("Enter User Name: ");
    gets(u.name);
    printf("Enter Type (student/faculty): ");
    gets(u.type);
    fprintf(fp, "id -%d,name-%s,type-%s\n", u.user_id, u.name, u.type);
    fclose(fp);
    printf("User added.\n");
}


void issue_book() {
    struct Issue i;
    struct User u;
    FILE *fu = fopen("users.txt", "r");
    FILE *fi = fopen("issued.txt", "a");

    int found = 0;
    char line[100];
    printf("Enter User ID: ");
    scanf("%d", &i.user_id);
    getchar();

    
    while (fgets(line, sizeof(line), fu)) {
        sscanf(line, "id -%d,name-%[^,],type-%s", &u.user_id, u.name, u.type);
        if (u.user_id == i.user_id) {
            found = 1;
            break;
        }
    }
    fclose(fu);

    if (!found) {
        printf("User not found.\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &i.book_id);

    if (strcmp(u.type, "student") == 0) {
        
        time_t now = time(NULL);
        now += 15 * 24 * 60 * 60;
        struct tm *due = localtime(&now);
        strftime(i.due_date, sizeof(i.due_date), "%Y-%m-%d", due);
    } else {
        strcpy(i.due_date, "-");
    }

    fprintf(fi, "%d,%d,%s\n", i.user_id, i.book_id, i.due_date);
    fclose(fi);
    printf("Book issued.\n");
}


void view_due_books() {
    FILE *fi = fopen("issued.txt", "r");
    FILE *fu = fopen("users.txt", "r");

    int uid, bid;
    char due[20];
    char uline[100], uname[50], utype[10];

    printf("\n--- Due Books (Students Only) ---\n");

    while (fscanf(fi, "%d,%d,%s\n", &uid, &bid, due) != EOF) {
        if (strcmp(due, "-") != 0) {
            rewind(fu);
            while (fgets(uline, sizeof(uline), fu)) {
                int temp_id;
                sscanf(uline, "%d,%[^,],%s", &temp_id, uname, utype);
                if (temp_id == uid) {
                    printf("User: %s | Book ID: %d | Due Date: %s\n", uname, bid, due);
                    break;
                }
            }
        }  
    }

    fclose(fi);
    fclose(fu);
}


void show_books() {
    FILE *fp = fopen("books.txt", "r");
    struct Book b;

    if (fp == NULL) {
        printf("No books found.\n");
        return;
    }

    char line[150];
    printf("\n--- Book List ---\n");
    while (fgets(line, sizeof(line), fp)) {
        
        printf("%s", line);

    }
    fclose(fp);
}


void return_book() {
    int user_id, book_id;
    printf("Enter User ID: ");
    scanf("%d", &user_id);
    printf("Enter Book ID to return: ");
    scanf("%d", &book_id);

    FILE *fi = fopen("issued.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    int uid, bid;
    char due[20];
    int found = 0;

    while (fscanf(fi, "%d,%d,%s\n", &uid, &bid, due) != EOF) {
        if (uid == user_id && bid == book_id) {
            found = 1; 
        } else {
            fprintf(temp, "%d,%d,%s\n", uid, bid, due);
        }
    }

    fclose(fi);
    fclose(temp);
    remove("issued.txt");
    rename("temp.txt", "issued.txt");

    if (found) {
        printf("Book returned successfully.\n");
    } else {
        printf("No matching record found.\n");
    }
}

int main() {
    int choice;
    do {
        printf("\n--- Library Management ---\n");
        printf("1. Add Book\n");
        printf("2. Add User\n");
        printf("3. Issue Book\n");
        printf("4. View Due Books\n");
        printf("5. Show Books\n");
        printf("6. Return Book\n");
        printf("0. Exit\n");
        printf("Enter your choice:\n");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: add_book(); break;
            case 2: add_user(); break;
            case 3: issue_book(); break;
            case 4: view_due_books(); break;
            case 5: show_books(); break;
            case 6: return_book(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}
