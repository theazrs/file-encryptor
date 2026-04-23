#include <iostream>
#include <fstream>
#include <filesystem> // c++17
#include <limits>     // for pausing program
#include <functional> // for hashing
#include <vector>
using namespace std;
namespace fs = std::filesystem;

// Configuration for the encrypted storage
const string STORAGE_DIR = "vault";

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

    // ios::binary is crucial; it prevents the OS from translating line endings (\n to \r\n),
    // which would corrupt non-text files like images or PDFs during encryption.
    fstream inputFile(filename, ios::in | ios::binary);
    if (!inputFile)
        return 1;

    // Ensure the storage directory exists
    if (!fs::exists(STORAGE_DIR))
    {
        fs::create_directory(STORAGE_DIR);
    }

    // Generate a unique name for the encrypted file in the vault
    // We use the original filename and add .enc
    string storagePath = STORAGE_DIR + "/" + fs::path(filename).filename().string() + ".enc";

    fstream outputFile(storagePath, ios::out | ios::binary);
    if (!outputFile)
        return 1;

    // Encrypt the filename before storing it
    string encryptedFilename = filename;
    for (size_t k = 0; k < encryptedFilename.length(); ++k)
        encryptedFilename[k] ^= key[k % key.length()];

    // Write Metadata Header
    outputFile << hash<string>{}(key) << endl; // Line 1: Hashed key
    outputFile << encryptedFilename << endl;   // Line 2: Encrypted filename

    // Scramble the file data
    char c;
    size_t i = 0;
    while (inputFile.get(c))
    {
        // Apply XOR logic. The modulo operator (%) allows us to cycle through the key string repeatedly.
        outputFile.put((unsigned char)c ^ key[i++ % key.length()]);
    }

    // close files
    outputFile.close();
    inputFile.close();

    // Security measure: delete the original file after it has been safely encrypted.
    // If deletion fails, we delete the encrypted versions to avoid leaving half-finished state.
    if (!fs::remove(filename))
    {
        cerr << "Unable to complete encryption process.." << endl;
        cerr << "Please make sure the file you are trying to encrypt is closed and re-run the program" << endl;
        fs::remove(storagePath);

        // halt
        return 1;
    }

    // end function
    return 0;
}

// function to decrypt
int decrypt(const string &storagePath, const string &key)
{
    // open files
    fstream backupFile(storagePath, ios::in | ios::binary);
    if (!backupFile)
        return 1;

    // Read the stored hash from the first line and verify the provided key
    size_t storedHash;
    if (!(backupFile >> storedHash))
        return 1;

    // Consume the newline character after the hash
    backupFile.ignore(numeric_limits<streamsize>::max(), '\n');

    if (hash<string>{}(key) != storedHash)
    {
        cerr << "Error: Incorrect key provided. Decryption aborted." << endl;
        backupFile.close();
        return 1;
    }

    // Read and decrypt the filename from the second line
    string encryptedFilename;
    getline(backupFile, encryptedFilename);
    string filename = encryptedFilename;
    for (size_t k = 0; k < filename.length(); ++k) // Undo XOR on filename
        filename[k] ^= key[k % key.length()];

    fstream originalFile(filename, ios::out | ios::binary); // create original file
    if (!originalFile)
        return 1;

    // Scramble the data back to original
    char c;
    size_t i = 0;
    while (backupFile.get(c))
    {
        // XOR property: (Data ^ Key) ^ Key = Data. We use the exact same logic as encryption.
        unsigned char uc = (unsigned char)c ^ key[i++ % key.length()];
        originalFile.put(uc);
    }

    // close files
    backupFile.close();
    originalFile.close();

    // Clean up metadata files now that the original file is restored
    fs::remove(storagePath);

    // end function
    return 0;
}

// Helper to handle the "Key" prompt logic which was cluttering main()
string getKeyFromUser()
{
    string key;
    int attempts = 0;
    while (attempts < 3)
    {
        cout << "Enter the key (or '0' to cancel): " << endl;
        getline(cin, key);

        if (key == "0")
            return "CANCELLED";

        if (key.empty())
        {
            cout << "Key must not be empty!" << endl;
            attempts++;
        }
        else
        {
            return key;
        }
    }
    return "FAILED";
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
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        // Clean the buffer once here to handle the newline after choice selection
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            clearScreen();
            cout << "Enter filename or press 'q' to abort" << endl;
            getline(cin, file);

            // Simple exit check
            if (file == "q" || file == "Q")
                break;

            key = getKeyFromUser();
            if (key == "CANCELLED")
                break;
            if (key == "FAILED")
            {
                cerr << "Encryption aborted after too many empty key attempts." << endl;
                pause();
                break;
            }

            if (encrypt(file, key) == 0)
            {
                cout << "File successfully encrypted." << endl;
            }
            else
            {
                cerr << "Problems encrypting the file." << endl;
            }
            pause();
            break;

        case 2:
        {
            clearScreen();

            // Guard clause: Ensure the vault exists and contains files
            if (!fs::exists(STORAGE_DIR) || fs::is_empty(STORAGE_DIR))
            {
                cerr << "No file to decrypt.." << endl;
                cerr << "Encrypt a file to be able to decrypt it" << endl;
                pause();
                break;
            }

            // List available encrypted files
            vector<string> vaultFiles;
            cout << "Select a file to decrypt:" << endl;
            int i = 1;
            for (const auto &entry : fs::directory_iterator(STORAGE_DIR))
            {
                if (entry.path().extension() == ".enc")
                {
                    vaultFiles.push_back(entry.path().string());
                    cout << i++ << ". " << entry.path().filename().string() << endl;
                }
            }

            if (vaultFiles.empty())
            {
                cerr << "No encrypted (.enc) files found in " << STORAGE_DIR << endl;
                pause();
                break;
            }

            int fileChoice;
            cout << "Enter number (or 0 to cancel): ";
            if (!(cin >> fileChoice) || fileChoice <= 0 || fileChoice > (int)vaultFiles.size())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            file = vaultFiles[fileChoice - 1];

            key = getKeyFromUser();
            if (key == "CANCELLED" || key == "FAILED")
                break;

            if (decrypt(file, key) == 0)
            {
                cout << "File decrypted" << endl;
            }
            else
            {
                cerr << "Unable to decrypt file" << endl;
            }
            pause();
            break;
        }

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