#include <iostream>
#include <fstream>
using namespace std;

string shifter(string text, int key)
{
    string temp = "";
    for (auto &ch : text)
    {
        temp += char(int(ch) + key);
    }
    return temp;
}

int main()
{
    string text;
    int key;
    cout << "Enter a string" << endl;
    cin >> text;

    cout << "Enter the key" << endl;
    cin >> key;

    cout << "The encrypted version is:" << endl;
    cout << shifter(text, key) << endl;

    return 0;
}