#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <numeric>
#include <bitset>
#include <cmath>
#include <algorithm>

using namespace std;
const string INPUT_FILE = "etc/12.txt";
const unsigned long long ARBITRARY_INT = 32;

vector<string> allArrangements(unsigned long long length)
{
    vector<string> arrangements;
    for (unsigned long long i = 0; i < pow(2, length); i++)
    {
        string arr = bitset<ARBITRARY_INT>(i).to_string().substr(ARBITRARY_INT - length, length);
        replace(arr.begin(), arr.end(), '0', '.');
        replace(arr.begin(), arr.end(), '1', '#');
        arrangements.push_back(arr);
    }

    return arrangements;
}

vector<int> getGroups(string arr)
{
    int group_size = 0;
    vector<int> groups;
    for (const char c : arr)
    {
        if (c == '#')
        {
            group_size++;
        }
        else if (group_size)
        {
            groups.push_back(group_size);
            group_size = 0;
        }
    }

    if (group_size)
        groups.push_back(group_size);

    return groups;
}

bool possibleArrangement(string arr, string blueprint)
{
    for (int i = 0; i < blueprint.size(); i++)
    {
        if (arr[i] != blueprint[i] && blueprint[i] != '?')
            return false;
    }

    return true;
}

class SpringRow
{
public:
    SpringRow(string springs, vector<int> groups) : springs_(springs), groups_(groups){};

    string getSprings() { return springs_; }
    vector<int> getGroups() { return groups_; }
    int totalDamaged() { return accumulate(groups_.begin(), groups_.end(), 0); }

private:
    string springs_;
    vector<int> groups_;
};

int main()
{
    ifstream file(INPUT_FILE);
    int sum_possible_arrangements = 0;

    for (string line; getline(file, line);)
    {
        stringstream rowStream(line);
        string springs, groupString;
        getline(rowStream, springs, ' ');
        getline(rowStream, groupString, ' ');
        cout << springs << endl;
        cout << groupString << endl;
        stringstream groupStream(groupString);
        vector<int> groups;
        int nr_possible_arrangements = 0;
        for (string group; getline(groupStream, group, ',');)
        {
            groups.push_back(stoi(group));
        }

        SpringRow arrangement(springs, groups);
        for (string arr : allArrangements(springs.size()))
        {
            // cout << arr << endl;
            vector<int> arr_groups = getGroups(arr);
            if (groups == arr_groups && possibleArrangement(arr, springs))
                nr_possible_arrangements++;
        }

        cout << nr_possible_arrangements << endl;
        sum_possible_arrangements += nr_possible_arrangements;
    }
    cout << "Answer part 1: " << sum_possible_arrangements << endl;

    return 0;
}