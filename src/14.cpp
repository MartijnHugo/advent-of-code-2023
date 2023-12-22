#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>

using namespace std;

const string INPUT_FILE = "etc/14.txt";
const int ROTATIONS = 1e3;

int count_rocks(string s)
{
    int count = 0;

    for (int i = 0; i < s.size(); i++)
        if (s[i] == 'O')
            count++;

    return count;
}

string tilt(string s, bool left = true)
{
    string new_s = "";
    smatch match;
    regex pattern("[.|O]+");
    int total_len = 0;
    string cpy = s;
    while (regex_search(cpy, match, pattern))
    {
        int count = count_rocks(match.str());
        // cout << total_len + match.position() << ", " << match.length() << ": " << match.str() << " - " << count;
        total_len += match.length();
        new_s += string(match.position(), '#');
        if (left)
        {
            new_s += string(count, 'O');
            new_s += string(match.length() - count, '.');
        }
        else
        {
            new_s += string(match.length() - count, '.');
            new_s += string(count, 'O');
        }

        // cout << endl;
        cpy = match.suffix().str();
    }
    new_s += string(s.size() - new_s.size(), '#');
    // cout << new_s << endl;

    return new_s;
}

int main()
{
    ifstream file(INPUT_FILE);

    vector<string> rows;
    vector<string> cols;
    int line_nr = 0;

    for (string line; getline(file, line);)
    {
        if (line_nr == 0)
        {
            cols.resize(line.size());
            for (int i = 0; i < line.size(); i++)
            {
                cols[i] = "";
            }
        }

        rows.push_back(line);
        for (int i = 0; i < line.size(); i++)
        {
            cols[i].push_back(line[i]);
        }

        line_nr++;
    }

    int load_sum = 0;

    for (string col : cols)
    {
        int block_idx = -1;
        vector<int> rounds;
        for (int i = 0; i < col.size(); i++)
        {
            char c = col[i];
            if (c == '#')
            {
                block_idx = i;
                continue;
            }
            if (c == '.')
            {
                continue;
            }

            rounds.push_back(block_idx + 1);
            load_sum += col.size() - block_idx - 1;
            // cout << col.size() - block_idx - 1;
            block_idx++;
        }
        // cout << endl;
    }
    // cout << endl;

    int rotated_sum = 0;

    for (int i = 0; i < ROTATIONS; i++)
    {
        vector<string> new_rows;
        for (string row : rows)
        {
            string new_row = tilt(row);
            // cout << new_row << endl;
            new_rows.push_back(new_row);
        }
        rows = new_rows;
        vector<string> new_cols;
        bool first_row = true;
        for (string r : new_rows)
        {
            if (first_row)
            {
                cols.resize(r.size());
                for (int i = 0; i < r.size(); i++)
                {
                    cols[i] = "";
                }
                first_row = false;
            }
            for (int i = 0; i < r.size(); i++)
            {
                cols[i].push_back(r[i]);
            }

            line_nr++;
        }
    }

    cout << "Answer part 1: "
         << load_sum << endl;
    cout << "Answer part 2: "
         << "" << endl;

    return 0;
}