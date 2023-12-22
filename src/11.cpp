#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const string INPUT_FILE = "etc/11.txt";

const int EXPANSION_FACTOR = 1e6;

int main()
{
    fstream file(INPUT_FILE);
    vector<int> empty_rows, empty_cols;
    vector<vector<int>> galaxy_coords = {};

    int line_nr = 0;
    for (string line; getline(file, line);)
    {
        if (line_nr == 0)
        {
            for (int i = 0; i < line.size(); i++)
            {
                empty_cols.push_back(i);
            }
        }

        vector<char> spaces(line.begin(), line.end());
        if (all_of(spaces.begin(), spaces.end(), [](char space)
                   { return space == '.'; }))
        {
            empty_rows.push_back(line_nr);
        }
        for (int col_nr = 0; col_nr < line.size(); col_nr++)
        {
            if (spaces[col_nr] == '#')
            {
                galaxy_coords.push_back({line_nr, col_nr});
                empty_cols.erase(remove(empty_cols.begin(), empty_cols.end(), col_nr), empty_cols.end());
            }
        }

        line_nr++;
    }
    cout << "Empty rows:" << endl;
    for (int i : empty_rows)
    {
        cout << i << endl;
    }
    cout << "Empty cols:" << endl;
    for (int i : empty_cols)
    {
        cout << i << endl;
    }
    cout << "Galaxies:" << endl;
    for (int i = 0; i < galaxy_coords.size(); i++)
    {
        cout << galaxy_coords[i][0] << ", " << galaxy_coords[i][1] << endl;
    }

    int sum_dist = 0;
    long long sum_actual_dist = 0;

    for (int i = 1; i < galaxy_coords.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            int row_1 = galaxy_coords[i][0];
            int col_1 = galaxy_coords[i][1];
            int row_2 = galaxy_coords[j][0];
            int col_2 = galaxy_coords[j][1];

            sum_dist += abs(row_1 - row_2) + abs(col_1 - col_2);
            sum_actual_dist += abs(row_1 - row_2) + abs(col_1 - col_2);

            for (int row : empty_rows)
            {
                if (row > row_1 && row < row_2 || row > row_2 && row < row_1)
                {
                    sum_dist++;
                    sum_actual_dist += EXPANSION_FACTOR - 1;
                }
            }
            for (int col : empty_cols)
            {
                if (col > col_1 && col < col_2 || col > col_2 && col < col_1)
                {
                    sum_dist++;
                    sum_actual_dist += EXPANSION_FACTOR - 1;
                }
            }
        }
    }

    cout << "Answer part 1: " << sum_dist << endl;
    cout << "Answer part 2: " << sum_actual_dist << endl;

    return 0;
}