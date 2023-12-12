#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;
const string INPUT_FILE = "etc/12.txt";

int main()
{
    ifstream file(INPUT_FILE);

    for (string line; getline(file, line);)
    {
        stringstream rowStream(line);
        string springString, damagedGroupsString;
        getline(rowStream, springString, ' ');
        getline(rowStream, damagedGroupsString, ' ');
        cout << springString << endl;
        cout << damagedGroupsString << endl;
    }

    return 0;
}