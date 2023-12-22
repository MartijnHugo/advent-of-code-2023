#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

const string INPUT_FILE = "etc/13.txt";

class Grid
{
public:
    Grid(vector<string> rows, vector<string> cols) : rows_(rows), cols_(cols){};

    vector<string> getRows() { return rows_; }
    vector<string> getCols() { return cols_; }

private:
    vector<string> rows_;
    vector<string> cols_;
};

bool potentialSmudge(string const &strA, string const &strB, size_t &index)
{
    bool single_mismatch = false;
    if (strA.size() == strB.size())
    {
        typedef string::const_iterator ci;
        typedef pair<ci, ci> mismatch_result;

        ci begA(strA.begin()), endA(strA.end());

        mismatch_result result = mismatch(begA, endA, strB.begin());

        if (result.first != endA) //  found a mismatch
        {
            index = distance(begA, result.first);

            // now look for a second mismatch

            advance(result.first, 1);
            advance(result.second, 1);

            single_mismatch = (mismatch(result.first, endA, result.second).first == endA);
        }
    }
    return single_mismatch;
}

vector<int> getMirroredIndices(vector<string> lines, bool smudged = false)
{
    vector<int> mirrored_indices;
    int top, bot;
    size_t smudge_idx;
    for (int i = 1; i < lines.size(); i++)
    {
        top = i - 1;
        bot = i;

        bool is_mirrored = true;
        int smudge_count = 0;

        while (top >= 0 && bot < lines.size())
        {
            if (potentialSmudge(lines[top], lines[bot], smudge_idx) && smudged)
            {
                smudge_count++;
                cout << top << " and " << bot << " are smudged reflections" << endl;
            }
            else if (lines[top] != lines[bot])
            {
                is_mirrored = false;
            }
            top--;
            bot++;
        }
        if (is_mirrored && !smudged || smudge_count == 1 && smudged && is_mirrored)
        {
            mirrored_indices.push_back(i);
            cout << "Mirrored index: " << i << endl;
        }
    }

    return mirrored_indices;
}

int main()
{
    ifstream file(INPUT_FILE);
    vector<string> rows;
    vector<string> cols;

    vector<Grid *> grids;

    bool new_grid = true;
    for (string line; getline(file, line);)
    {
        if (line.size() == 0)
        {
            grids.push_back(new Grid(rows, cols));
            new_grid = true;
            continue;
        }
        if (new_grid)
        {
            rows = {};
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

        new_grid = false;
    }
    grids.push_back(new Grid(rows, cols));

    int clean_sum = 0;
    int smudge_sum = 0;

    for (Grid *p_grid : grids)
    {
        Grid grid = *p_grid;
        vector<string> rows = grid.getRows();
        vector<string> cols = grid.getCols();
        for (string row : rows)
        {
            cout << row << endl;
        }
        cout << endl;
        for (string col : cols)
        {
            cout << col << endl;
        }
        cout << endl;

        cout << "Checking clean rows" << endl;
        vector<int> mirrored_rows = getMirroredIndices(rows, false);
        cout << "Checking smudged rows" << endl;
        vector<int> mirrored_smudge_rows = getMirroredIndices(rows, true);
        cout << "Checking clean cols" << endl;
        vector<int> mirrored_cols = getMirroredIndices(cols, false);
        cout << "Checking smudged cols" << endl;
        vector<int> mirrored_smudge_cols = getMirroredIndices(cols, true);

        for (int i : mirrored_rows)
        {
            clean_sum += 100 * i;
        }
        for (int i : mirrored_cols)
        {
            clean_sum += i;
        }

        for (int i : mirrored_smudge_rows)
        {
            if (find(mirrored_rows.begin(), mirrored_rows.end(), i) != mirrored_rows.end())
            {
                continue;
            }
            smudge_sum += 100 * i;
        }
        for (int i : mirrored_smudge_cols)
        {
            if (find(mirrored_cols.begin(), mirrored_cols.end(), i) != mirrored_cols.end())
            {
                continue;
            }
            smudge_sum += i;
        }
    }

    cout << "Answer part 1: " << clean_sum << endl;
    cout << "Answer part 2: " << smudge_sum << endl;

    return 0;
}