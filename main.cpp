#include <iostream>
#include <fstream>
#include <filesystem> // c++17
#include <limits>     // for pausing program
#include <functional> // for hashing
using namespace std;
namespace fs = std::filesystem;

// function to clear screen
void clearScreen()
{
    // ANSI escape sequences:
    // \033[2J clears the entire screen
    // \033[H moves the cursor to the "home" position (top-left)
    cout << "\033[2J\033[H";
}

// Halts execution until the user presses Enter, useful for showing status messages
void pause(string message = "Press enter to continue...")
{
    cout << message << endl;
    // Clear any leftover characters in the input buffer (like newlines)
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

    // TODO view all encrypted files perhaps??
}

// function to open file, apply shift and encrypt to write into new file
int encrypt(const string &filename, const string &key)
{
    // check if file exists
    if (!fs::exists(filename))
    {
        cerr << "Couldn't open file." << endl;
        return 1;
    }

    // Safety check: prevent division by zero if key is empty
    if (key.empty()) {
        cerr << "Error: Key cannot be empty." << endl;
        return 1;
    }

    // ios::binary is crucial; it prevents the OS from translating line endings (\n to \r\n),
    // which would corrupt non-text files like images or PDFs during encryption.
    fstream inputFile(filename, ios::in | ios::binary);
    if (!inputFile)
        return 1;

    // encrypted.dat stores the actual scrambled bytes
    fstream outputFile("encrypted.dat", ios::out | ios::binary);
    if (!outputFile)
        return 1;

    // saves.txt acts as a small database for file metadata
    fstream saves("saves.txt", ios::out);
    if (!saves)
        return 1;

    saves << filename << endl;            // Store filename so we know how to name it later
    saves << hash<string>{}(key) << endl; // Store a one-way hash of the key for later verification
    cout << "File successfully opened" << endl;
    char c;
    size_t i = 0;
    while (inputFile.get(c))
    {
        // Apply XOR logic. The modulo operator (%) allows us to cycle through the key string repeatedly.
        outputFile.put(c ^ key[i++ % key.length()]);
    }

    // close files
    saves.close();
    outputFile.close();
    inputFile.close();

    // Security measure: delete the original file after it has been safely encrypted.
    // If deletion fails, we delete the encrypted versions to avoid leaving half-finished state.
    if (!fs::remove(filename))
    {
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
int decrypt(const string &key)
{
    // open files
    fstream saves("saves.txt", ios::in);
    fstream backupFile("encrypted.dat", ios::in | ios::binary);

    // Safety check: prevent division by zero if key is empty
    if (key.empty()) {
        cerr << "Error: Key cannot be empty." << endl;
        return 1;
    }

    // read saves file to retrieve name of original file
    string filename;
    saves >> filename;

    // Read the stored hash and verify the provided key
    size_t storedHash;
    if (!(saves >> storedHash) || hash<string>{}(key) != storedHash)
    {
        cerr << "Error: Incorrect key provided. Decryption aborted." << endl;
        saves.close();
        backupFile.close();
        return 1;
    }

    fstream originalFile(filename, ios::out | ios::binary); // create original file
    if (!originalFile)
        return 1;

    // write to file
    char c;
    size_t i = 0;
    while (backupFile.get(c))
    {
        // XOR property: (Data ^ Key) ^ Key = Data. We use the exact same logic as encryption.
        unsigned char uc = c ^ key[i++ % key.length()];
        originalFile.put(uc);
    }

    // close files
    saves.close();
    backupFile.close();
    originalFile.close();

    // Clean up metadata files now that the original file is restored
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
    string key;
    int choice;

    while (true)
    {
        clearScreen();
        mainMenu();

        // Handle cases where user enters non-numeric data into the integer 'choice' variable
        if (!(cin >> choice))
        {
            cin.clear();                                         // reset error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
            continue;
        }

        switch (choice)
        {
        case 1:
            clearScreen();
            cout << "Enter filename or press 'q' to abort" << endl;
            // Clear the newline character left in the buffer by 'cin >> choice'
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer before getline
            getline(cin, file);

            // Simple exit check
            if (file == "q" || file == "Q")
                break;

            cout << "Enter the key or enter 0 to abort" << endl;
            getline(cin, key);

            if (key == "0")
                break;

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

            // Guard clause: Ensure the necessary metadata files exist before attempting decryption
            if (!fs::exists("encrypted.dat") && !fs::exists("saves.txt"))
            {
                cerr << "No file to decrypt.." << endl;
                cerr << "Encrypt a file to be able to decrypt it" << endl;
                pause();
                break;
            }

            cout << "Enter the key to decrypt the file or enter 0 to cancel encryption" << endl;
            // Clear buffer again to ensure getline doesn't capture a leftover newline
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, key);

            if (key == "0")
                break;

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