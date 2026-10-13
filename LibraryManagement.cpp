#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

const string BOOK_FILE = "books.txt";
const string MEMBER_FILE = "members.txt";
const string BORROW_FILE = "borrow_records.txt";
const char SEPARATOR = '|';

// ==================== Data Structures ====================

struct Book
{
    string id;
    string title;
    string author;
    int totalCopies;
    int availableCopies;
};

struct Member
{
    string id;
    string name;
    string phone;
};

struct BorrowRecord
{
    string bookId;
    string memberId;
    bool active;
};

// ==================== Input Functions ====================

string getText(const string& message)
{
    string value;

    while (true)
    {
        cout << message;
        getline(cin, value);

        if (value.empty())
        {
            cout << "Input cannot be empty. Try again.\n";
        }
        else if (value.find(SEPARATOR) != string::npos)
        {
            cout << "The character '|' is not allowed.\n";
        }
        else
        {
            return value;
        }
    }
}

int getNumber(const string& message, int minimum, int maximum)
{
    string input;
    int number;
    char extra;

    while (true)
    {
        cout << message;
        getline(cin, input);

        stringstream stream(input);

        if ((stream >> number) &&
            !(stream >> extra) &&
            number >= minimum &&
            number <= maximum)
        {
            return number;
        }

        cout << "Invalid input. Enter a number from "
             << minimum << " to " << maximum << ".\n";
    }
}

// ==================== Utility Functions ====================

vector<string> splitLine(const string& line)
{
    vector<string> fields;
    string field;
    stringstream stream(line);

    while (getline(stream, field, SEPARATOR))
    {
        fields.push_back(field);
    }

    return fields;
}

string toLowerCase(string text)
{
    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char character)
        {
            return static_cast<char>(tolower(character));
        });

    return text;
}

// ==================== Book File Handling ====================

vector<Book> loadBooks()
{
    vector<Book> books;
    ifstream file(BOOK_FILE);
    string line;

    while (getline(file, line))
    {
        vector<string> fields = splitLine(line);

        if (fields.size() != 5)
        {
            continue;
        }

        try
        {
            Book book;

            book.id = fields[0];
            book.title = fields[1];
            book.author = fields[2];
            book.totalCopies = stoi(fields[3]);
            book.availableCopies = stoi(fields[4]);

            if (book.totalCopies >= 0 &&
                book.availableCopies >= 0 &&
                book.availableCopies <= book.totalCopies)
            {
                books.push_back(book);
            }
        }
        catch (...)
        {
            continue;
        }
    }

    return books;
}

bool saveBooks(const vector<Book>& books)
{
    ofstream file(BOOK_FILE);

    if (!file)
    {
        cout << "Error: Unable to save book records.\n";
        return false;
    }

    for (const Book& book : books)
    {
        file << book.id << SEPARATOR
             << book.title << SEPARATOR
             << book.author << SEPARATOR
             << book.totalCopies << SEPARATOR
             << book.availableCopies << '\n';
    }

    return static_cast<bool>(file);
}

// ==================== Member File Handling ====================

vector<Member> loadMembers()
{
    vector<Member> members;
    ifstream file(MEMBER_FILE);
    string line;

    while (getline(file, line))
    {
        vector<string> fields = splitLine(line);

        if (fields.size() != 3)
        {
            continue;
        }

        Member member;

        member.id = fields[0];
        member.name = fields[1];
        member.phone = fields[2];

        members.push_back(member);
    }

    return members;
}

bool saveMembers(const vector<Member>& members)
{
    ofstream file(MEMBER_FILE);

    if (!file)
    {
        cout << "Error: Unable to save member records.\n";
        return false;
    }

    for (const Member& member : members)
    {
        file << member.id << SEPARATOR
             << member.name << SEPARATOR
             << member.phone << '\n';
    }

    return static_cast<bool>(file);
}

// ==================== Borrowing File Handling ====================

vector<BorrowRecord> loadBorrowRecords()
{
    vector<BorrowRecord> records;
    ifstream file(BORROW_FILE);
    string line;

    while (getline(file, line))
    {
        vector<string> fields = splitLine(line);

        if (fields.size() != 3)
        {
            continue;
        }

        if (fields[2] != "0" && fields[2] != "1")
        {
            continue;
        }

        BorrowRecord record;

        record.bookId = fields[0];
        record.memberId = fields[1];
        record.active = (fields[2] == "1");

        records.push_back(record);
    }

    return records;
}

bool saveBorrowRecords(const vector<BorrowRecord>& records)
{
    ofstream file(BORROW_FILE);

    if (!file)
    {
        cout << "Error: Unable to save borrowing records.\n";
        return false;
    }

    for (const BorrowRecord& record : records)
    {
        file << record.bookId << SEPARATOR
             << record.memberId << SEPARATOR
             << (record.active ? 1 : 0) << '\n';
    }

    return static_cast<bool>(file);
}

// ==================== Search Helpers ====================

int findBook(const vector<Book>& books, const string& id)
{
    for (int i = 0; i < static_cast<int>(books.size()); i++)
    {
        if (books[i].id == id)
        {
            return i;
        }
    }

    return -1;
}

int findMember(const vector<Member>& members, const string& id)
{
    for (int i = 0; i < static_cast<int>(members.size()); i++)
    {
        if (members[i].id == id)
        {
            return i;
        }
    }

    return -1;
}

// ==================== Display Functions ====================

void displayBook(const Book& book)
{
    cout << "\n----------------------------------------\n";
    cout << "Book ID          : " << book.id << '\n';
    cout << "Title            : " << book.title << '\n';
    cout << "Author           : " << book.author << '\n';
    cout << "Total Copies     : " << book.totalCopies << '\n';
    cout << "Available Copies : " << book.availableCopies << '\n';
    cout << "----------------------------------------\n";
}

void displayMember(const Member& member)
{
    cout << "\n----------------------------------------\n";
    cout << "Member ID : " << member.id << '\n';
    cout << "Name      : " << member.name << '\n';
    cout << "Phone     : " << member.phone << '\n';
    cout << "----------------------------------------\n";
}

void displayBooks()
{
    vector<Book> books = loadBooks();

    if (books.empty())
    {
        cout << "\nNo books found in the library.\n";
        return;
    }

    cout << "\n========== LIBRARY BOOKS ==========\n";

    for (const Book& book : books)
    {
        displayBook(book);
    }
}

void displayMembers()
{
    vector<Member> members = loadMembers();

    if (members.empty())
    {
        cout << "\nNo registered members found.\n";
        return;
    }

    cout << "\n========== LIBRARY MEMBERS ==========\n";

    for (const Member& member : members)
    {
        displayMember(member);
    }
}

// ==================== Add a Book ====================

void addBook()
{
    vector<Book> books = loadBooks();

    cout << "\n========== ADD BOOK ==========\n";

    Book book;

    book.id = getText("Enter Book ID: ");

    if (findBook(books, book.id) != -1)
    {
        cout << "A book with this ID already exists.\n";
        return;
    }

    book.title = getText("Enter Book Title: ");
    book.author = getText("Enter Author Name: ");
    book.totalCopies = getNumber("Enter Number of Copies: ", 1, 100000);
    book.availableCopies = book.totalCopies;

    books.push_back(book);

    if (saveBooks(books))
    {
        cout << "\nBook added successfully.\n";
    }
}

// ==================== Search by Title or Author ====================

void searchBook()
{
    vector<Book> books = loadBooks();

    if (books.empty())
    {
        cout << "\nNo books available to search.\n";
        return;
    }

    cout << "\n========== SEARCH BOOK ==========\n";

    string keyword = toLowerCase(getText("Enter title or author: "));
    int found = 0;

    for (const Book& book : books)
    {
        string title = toLowerCase(book.title);
        string author = toLowerCase(book.author);

        if (title.find(keyword) != string::npos ||
            author.find(keyword) != string::npos)
        {
            displayBook(book);
            found++;
        }
    }

    if (found == 0)
    {
        cout << "\nNo matching books were found.\n";
    }
}

// ==================== Register a Member ====================

void registerMember()
{
    vector<Member> members = loadMembers();

    cout << "\n========== REGISTER MEMBER ==========\n";

    Member member;

    member.id = getText("Enter Member ID: ");

    if (findMember(members, member.id) != -1)
    {
        cout << "A member with this ID already exists.\n";
        return;
    }

    member.name = getText("Enter Member Name: ");
    member.phone = getText("Enter Phone Number: ");

    members.push_back(member);

    if (saveMembers(members))
    {
        cout << "\nMember registered successfully.\n";
    }
}

// ==================== Issue a Book ====================

void issueBook()
{
    vector<Book> books = loadBooks();
    vector<Member> members = loadMembers();
    vector<BorrowRecord> records = loadBorrowRecords();

    cout << "\n========== ISSUE BOOK ==========\n";

    if (books.empty() || members.empty())
    {
        cout << "Add a book and register a member first.\n";
        return;
    }

    string bookId = getText("Enter Book ID: ");
    string memberId = getText("Enter Member ID: ");

    int bookIndex = findBook(books, bookId);
    int memberIndex = findMember(members, memberId);

    if (bookIndex == -1)
    {
        cout << "Book not found.\n";
        return;
    }

    if (memberIndex == -1)
    {
        cout << "Member not found. Please register the member first.\n";
        return;
    }

    if (books[bookIndex].availableCopies <= 0)
    {
        cout << "No copies of this book are currently available.\n";
        return;
    }

    for (const BorrowRecord& record : records)
    {
        if (record.active &&
            record.bookId == bookId &&
            record.memberId == memberId)
        {
            cout << "This member already has this book issued.\n";
            return;
        }
    }

    BorrowRecord record;

    record.bookId = bookId;
    record.memberId = memberId;
    record.active = true;

    books[bookIndex].availableCopies--;
    records.push_back(record);

    if (saveBooks(books) && saveBorrowRecords(records))
    {
        cout << "\nBook issued successfully to "
             << members[memberIndex].name << ".\n";
    }
    else
    {
        cout << "Warning: Saving was unsuccessful. Check the local data files.\n";
    }
}

// ==================== Return a Book ====================

void returnBook()
{
    vector<Book> books = loadBooks();
    vector<BorrowRecord> records = loadBorrowRecords();

    cout << "\n========== RETURN BOOK ==========\n";

    string bookId = getText("Enter Book ID: ");
    string memberId = getText("Enter Member ID: ");

    int bookIndex = findBook(books, bookId);

    if (bookIndex == -1)
    {
        cout << "Book not found.\n";
        return;
    }

    int recordIndex = -1;

    for (int i = 0; i < static_cast<int>(records.size()); i++)
    {
        if (records[i].active &&
            records[i].bookId == bookId &&
            records[i].memberId == memberId)
        {
            recordIndex = i;
            break;
        }
    }

    if (recordIndex == -1)
    {
        cout << "No active issue record found for this book and member.\n";
        return;
    }

    if (books[bookIndex].availableCopies >= books[bookIndex].totalCopies)
    {
        cout << "The book availability record is inconsistent. "
             << "Please check the data files.\n";
        return;
    }

    records[recordIndex].active = false;
    books[bookIndex].availableCopies++;

    if (saveBooks(books) && saveBorrowRecords(records))
    {
        cout << "\nBook returned successfully.\n";
    }
    else
    {
        cout << "Warning: Saving was unsuccessful. Check the local data files.\n";
    }
}

// ==================== Active Borrowing Records ====================

void displayBorrowedBooks()
{
    vector<Book> books = loadBooks();
    vector<Member> members = loadMembers();
    vector<BorrowRecord> records = loadBorrowRecords();

    cout << "\n========== ACTIVE BORROWING RECORDS ==========\n";

    bool found = false;

    for (const BorrowRecord& record : records)
    {
        if (!record.active)
        {
            continue;
        }

        int bookIndex = findBook(books, record.bookId);
        int memberIndex = findMember(members, record.memberId);

        cout << "\nBook ID   : " << record.bookId;

        if (bookIndex != -1)
        {
            cout << "\nBook Title: " << books[bookIndex].title;
        }

        cout << "\nMember ID : " << record.memberId;

        if (memberIndex != -1)
        {
            cout << "\nMember    : " << members[memberIndex].name;
        }

        cout << "\nStatus    : Issued\n";

        found = true;
    }

    if (!found)
    {
        cout << "There are no active borrowing records.\n";
    }
}

// ==================== Menu ====================

void showMenu()
{
    cout << "\n\n";
    cout << "============================================\n";
    cout << "          LIBRARY MANAGEMENT SYSTEM\n";
    cout << "============================================\n";
    cout << "1. Add Book\n";
    cout << "2. Display All Books\n";
    cout << "3. Search by Title or Author\n";
    cout << "4. Register Member\n";
    cout << "5. Display Members\n";
    cout << "6. Issue Book\n";
    cout << "7. Return Book\n";
    cout << "8. View Active Borrowing Records\n";
    cout << "9. Exit\n";
    cout << "============================================\n";
}

// ==================== Main Function ====================

int main()
{
    int choice;

    do
    {
        showMenu();

        choice = getNumber("Enter your choice: ", 1, 9);

        switch (choice)
        {
            case 1:
                addBook();
                break;

            case 2:
                displayBooks();
                break;

            case 3:
                searchBook();
                break;

            case 4:
                registerMember();
                break;

            case 5:
                displayMembers();
                break;

            case 6:
                issueBook();
                break;

            case 7:
                returnBook();
                break;

            case 8:
                displayBorrowedBooks();
                break;

            case 9:
                cout << "\nThank you for using the Library Management System.\n";
                break;
        }

    } while (choice != 9);

    return 0;
}