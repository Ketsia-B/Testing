#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Book {
    int id;
    string title;
    string author;
    bool isBorrowed;
};

struct User {
    int id;
    string name;
    int borrowedBookId;
};

void addBook() {
    Book book;
    cout << "Enter book ID: ";
    cin >> book.id;
    cin.ignore();
    cout << "Enter book title: ";
    getline(cin, book.title);
    //cin>>book.title;
    cout << "Enter book author: ";
   getline(cin, book.author);
  //cin>>book.author;
    book.isBorrowed = false;

    ofstream outFile("books.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char*>(&book), sizeof(Book));
    outFile.close();
    cout << "Book added successfully!\n";
}

void deleteBook(int id) {
    Book book;
    ifstream inFile("books.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);

    while (inFile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
        if (book.id != id) {
            outFile.write(reinterpret_cast<char*>(&book), sizeof(Book));
        }
    }
    inFile.close();
    outFile.close();
    remove("books.dat");
    rename("temp.dat", "books.dat");
    cout << "Book deleted successfully!\n";
}

void searchBook(int id) {
    Book book;
    ifstream inFile("books.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
        if (book.id == id) {
            cout << "Book ID: " << book.id << "\n";
            cout << "Title: " << book.title << "\n";
            cout << "Author: " << book.author << "\n";
            cout << "Status: " << (book.isBorrowed ? "Borrowed" : "Available") << "\n";
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found) {
        cout << "Book not found.\n";
    }
}

void borrowBook(int userId, int bookId) {
    Book book;
    User user;
    fstream bookFile("books.dat", ios::binary | ios::in | ios::out);
    fstream userFile("users.dat", ios::binary | ios::in | ios::out);
    bool bookFound = false, userFound = false;

    while (bookFile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
        if (book.id == bookId && !book.isBorrowed) {
            book.isBorrowed = true;
            bookFile.seekp(-sizeof(Book), ios::cur);
            bookFile.write(reinterpret_cast<char*>(&book), sizeof(Book));
            bookFound = true;
            break;
        }
    }
    bookFile.close();

    while (userFile.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        if (user.id == userId) {
            user.borrowedBookId = bookId;
            userFile.seekp(-sizeof(User), ios::cur);
            userFile.write(reinterpret_cast<char*>(&user), sizeof(User));
            userFound = true;
            break;
        }
    }
    userFile.close();

    if (bookFound && userFound) {
        cout << "Book borrowed successfully!\n";
    } else {
        cout << "Error borrowing book.\n";
    }
}

void returnBook(int userId) {
    Book book;
    User user;
    fstream bookFile("books.dat", ios::binary | ios::in | ios::out);
    fstream userFile("users.dat", ios::binary | ios::in | ios::out);
    bool bookFound = false, userFound = false;

    while (userFile.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        if (user.id == userId && user.borrowedBookId != -1) {
            int bookId = user.borrowedBookId;
            user.borrowedBookId = -1;
            userFile.seekp(-sizeof(User), ios::cur);
            userFile.write(reinterpret_cast<char*>(&user), sizeof(User));
            userFound = true;

            while (bookFile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
                if (book.id == bookId) {
                    book.isBorrowed = false;
                    bookFile.seekp(-sizeof(Book), ios::cur);
                    bookFile.write(reinterpret_cast<char*>(&book), sizeof(Book));
                    bookFound = true;
                    break;
                }
            }
            break;
        }
    }
    bookFile.close();
    userFile.close();

    if (bookFound && userFound) {
        cout << "Book returned successfully!\n";
    } else {
        cout << "Error returning book.\n";
    }
}

void addUser() {
    User user;
    cout << "Enter user ID: ";
    cin >> user.id;
    cin.ignore();
    cout << "Enter user name: ";
//    getline(cin, user.name);
     cin>>user.name;
    user.borrowedBookId = -1;

    ofstream outFile("users.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char*>(&user), sizeof(User));
    outFile.close();
    cout << "User added successfully!\n";
}

void deleteUser(int id) {
    User user;
    ifstream inFile("users.dat", ios::binary);
    ofstream outFile("temp_users.dat", ios::binary);

    while (inFile.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        if (user.id != id) {
            outFile.write(reinterpret_cast<char*>(&user), sizeof(User));
        }
    }
    inFile.close();
    outFile.close();
    remove("users.dat");
    rename("temp_users.dat", "users.dat");
    cout << "User deleted successfully!\n";
}

void searchUser(int id) {
    User user;
    ifstream inFile("users.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        if (user.id == id) {
            cout << "User ID: " << user.id << "\n";
            cout << "Name: " << user.name << "\n";
            cout << "Borrowed Book ID: " <<user.borrowedBookId<< "\n";
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found) {
        cout << "User not found.\n";
    }
}

void displayMenu() {
    cout << "Library Management System\n";
    cout << "1. Add Book\n";
    cout << "2. Delete Book\n";
    cout << "3. Search Book\n";
    cout << "4. Borrow Book\n";
    cout << "5. Return Book\n";
    cout << "6. Add User\n";
    cout << "7. Delete User\n";
    cout << "8. Search User\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    int choice, bookId, userId;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                cout << "Enter book ID to delete: ";
                cin >> bookId;
                deleteBook(bookId);
                break;
            case 3:
                cout << "Enter book ID to search: ";
                cin >> bookId;
                searchBook(bookId);
                break;
            case 4:
                cout << "Enter user ID: ";
                cin >> userId;
                cout << "Enter book ID to borrow: ";
                cin >> bookId;
                borrowBook(userId, bookId);
                break;
            case 5:
                cout << "Enter user ID: ";
                cin >> userId;
                returnBook(userId);
                break;
            case 6:
                addUser();
                break;
            case 7:
                cout << "Enter user ID to delete: ";
                cin >> userId;
                deleteUser(userId);
                break;
            case 8:
                cout << "Enter user ID to search: ";
                cin >> userId;
                searchUser(userId);
                break;
            case 9:
                exit(0);
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}

