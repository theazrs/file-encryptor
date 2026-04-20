#include <iostream>
#include <fstream>
#include <filesystem> // c++17
#include <limits> // for pausing program
using namespace std;
namespace fs = std::filesystem;

// function to clear screen
void clearScreen() {
    // \033[2J clears the screen
    // \033[H moves the cursor to the top-left corner
    cout << "\033[2J\033[H";
}

// function to ASCII shift a string
string shifter(const string& text, int key)
{
    string temp = "";
    temp.reserve(text.length()); // Pre-allocate memory to avoid multiple reallocations
    for (auto ch : text)
    {
        temp += ch + key;
    }
    return temp;
}

unsigned char byteShifter(unsigned char uc, int key)
{
    return uc + key;
}
 
void pause(string message = "Press enter to continue...")
{
    cout << message << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// function to display main menu
void mainMenu()
{
    cout << "Please select one option: " << endl;
    cout << "1. Encrypt a file" << endl;
    cout << "2. Decrypt a file" << endl;
    cout << "3. Quit" << endl;
    
    //TODO view all encrypted files perhaps?? 
    
}

// function to open file, apply shift and encrypt to write into new file
int encrypt(const string& filename, int key)
{
    // check if file exists
    if (!fs::exists(filename)) {
        cerr << "Couldn't open file." << endl;
        return 1;
    }

    // opening files that will be used in the program
    fstream inputFile(filename, ios::in | ios::binary);
    if (!inputFile) return 1;

    fstream outputFile("encrypted.dat", ios::out | ios::binary);
    if (!outputFile) return 1;

    fstream saves("saves.txt", ios::out);
    if (!saves) return 1;
    
    saves << filename << endl; // save original file name 
    saves << key << endl; // save original key
    cout << "File successfully opened" << endl;
    char c;
    while (inputFile.get(c))
    {
        unsigned char uc = byteShifter(static_cast<unsigned char>(c), key);
        outputFile.put(uc);
    }
    

    // close files
    saves.close();
    outputFile.close();
    inputFile.close();

    if (!fs::remove(filename)) {
        cerr << "Unable to complete encryption process.." << endl;
        cerr << "Please make sure the file you are trying to encrypt is closed and re-run the program" << endl;
        fs::remove("encrypted.dat");
        fs::remove("saves.txt");

        // halt
        return 1;
    }

    // end function
    return 0;
}


// function to decrypt 
int decrypt(int key)
{
    // open files
    fstream saves("saves.txt", ios::in);
    fstream backupFile("encrypted.dat", ios::in | ios::binary);

    // read saves file to retrieve name of original file
    string filename;
    saves >> filename;

    // retrieving old key
    int oldkey;
    saves >> oldkey;

    if (oldkey != key)
    {
        cerr << "The keys do not match." << endl;

        // halt function
        return 1;
    }
    

    fstream originalFile(filename, ios::out | ios::binary); // create original file

    // write to file 
    char c;
    while (backupFile.get(c)) 
    {
        unsigned char uc = byteShifter(static_cast<unsigned char>(c), -key);
        originalFile.put(uc);
    }

    // close files
    saves.close();
    backupFile.close();
    originalFile.close();

    // remove backups
    fs::remove("encrypted.dat");
    fs::remove("saves.txt");

    // end function
    return 0;
}

int main()
{
    // welcome
    clearScreen();
    cout << "Welcome to File Encryptor by Az " << endl;
    // variables that will be used 
    string file;
    int key;
    int choice;

    while (true)
    {
        clearScreen();
        mainMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        switch (choice)
        {
            case 1:
                clearScreen();   
                cout << "Enter filename or press 'q' to abort" << endl;
                cin >> file;

                if (file == "q" || file == "Q") break;

                cout << "Enter the key or enter 0 to abort" << endl;
                cin >> key;

                if (key == 0) break;

                if (encrypt(file, key) == 0)
                {
                    cout << "File successfully encrypted." << endl;
                }
                else
                {
                    cerr << "Problems encrypting the file." << endl;
                }
                break;

            case 2:
                clearScreen();

                // check if any file is encrypted, otherwise break of case 2
                if (!fs::exists("encrypted.dat") && !fs::exists("saves.txt"))
                {
                    cerr << "No file to decrypt.." << endl;
                    cerr << "Encrypt a file to be able to decrypt it" << endl;
                    pause();
                    break;
                }

                cout << "Enter the key to decrypt the file or enter 0 to cancel encryption" << endl;
                cin >> key;
                
                if (key == 0) break;

                if (decrypt(key) == 0) 
                {
                    cout << "File decrypted" << endl;
                }
                else 
                {
                    cerr << "Unable to decrypt file" << endl;
                }
                break;

            case 3:
                clearScreen();
                pause("Enter to confirm choice");
                return 0;
            
            default:
                clearScreen();
                cerr << "Invalid choice" << endl;
                break;
        }
    }
    
    return 0;
}