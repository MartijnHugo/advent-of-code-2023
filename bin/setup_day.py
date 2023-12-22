from argparse import ArgumentParser


def generate_skeleton(day: int):
    code = rf"""#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

const string INPUT_FILE = "etc/{day}.txt";

int main()
{{
    ifstream file(INPUT_FILE);
    for (string line; getline(file, line);)
    {{
        cout << line << endl;
    }}

    cout << "Answer part 1: " << "" << endl;
    cout << "Answer part 2: " << "" << endl;

    return 0;
}}"""

    with open(f"src/{day}.cpp", "w") as cpp_file:
        cpp_file.write(code)

    with open(f"etc/{day}.txt", "w") as input_file:
        input_file.write("")


if __name__ == "__main__":
    arg_parser = ArgumentParser()
    arg_parser.add_argument(
        "-d",
        "--day",
        dest="day",
        required=True,
        help="Specify day number",
    )
    args = arg_parser.parse_args()

    day = int(args.day)
    generate_skeleton(day)
