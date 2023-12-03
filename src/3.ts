// --- Day 3: Gear Ratios ---
// You and the Elf eventually reach a gondola lift station; he says the 
// gondola lift will take you up to the water source, but this is as far as he 
// can bring you. You go inside.

// It doesn't take long to find the gondolas, but there seems to be a problem: 
// they're not moving.

// "Aaah!"

// You turn around to see a slightly-greasy Elf with a wrench and a look of 
// surprise. "Sorry, I wasn't expecting anyone! The gondola lift isn't working 
// right now; it'll still be a while before I can fix it." You offer to help.

// The engineer explains that an engine part seems to be missing from the 
// engine, but nobody can figure out which one. If you can add up all the part 
// numbers in the engine schematic, it should be easy to work out which part 
// is missing.

// The engine schematic (your puzzle input) consists of a visual 
// representation of the engine. There are lots of numbers and symbols you 
// don't really understand, but apparently any number adjacent to a symbol, 
// even diagonally, is a "part number" and should be included in your sum. 
// (Periods (.) do not count as a symbol.)

// Here is an example engine schematic:

// 467..114..
// ...*......
// ..35..633.
// ......#...
// 617*......
// .....+.58.
// ..592.....
// ......755.
// ...$.*....
// .664.598..

// In this schematic, two numbers are not part numbers because they are not 
// adjacent to a symbol: 114 (top right) and 58 (middle right). Every other 
// number is adjacent to a symbol and so is a part number; their sum is 4361.

// Of course, the actual engine schematic is much larger. What is the sum of 
// all of the part numbers in the engine schematic?

// --- Part Two ---
// The engineer finds the missing part and installs it in the engine! As the 
// engine springs to life, you jump in the closest gondola, finally ready to 
// ascend to the water source.

// You don't seem to be going very fast, though. Maybe something is still 
// wrong? Fortunately, the gondola has a phone labeled "help", so you pick it 
// up and the engineer answers.

// Before you can explain the situation, she suggests that you look out the 
// window. There stands the engineer, holding a phone in one hand and waving 
// with the other. You're going so slowly that you haven't even left the 
// station. You exit the gondola.

// The missing part wasn't the only issue - one of the gears in the engine is 
// wrong. A gear is any * symbol that is adjacent to exactly two part numbers. 
// Its gear ratio is the result of multiplying those two numbers together.

// This time, you need to find the gear ratio of every gear and add them all 
// up so that the engineer can figure out which gear needs to be replaced.

// Consider the same engine schematic again:

// 467..114..
// ...*......
// ..35..633.
// ......#...
// 617*......
// .....+.58.
// ..592.....
// ......755.
// ...$.*....
// .664.598..

// In this schematic, there are two gears. The first is in the top left; it 
// has part numbers 467 and 35, so its gear ratio is 16345. The second gear is 
// in the lower right; its gear ratio is 451490. (The * adjacent to 617 is not 
// a gear because it is only adjacent to one part number.) Adding up all of 
// the gear ratios produces 467835.

// What is the sum of all of the gear ratios in your engine schematic?


import * as fs from 'fs';

const INPUT_FILE: string = "etc/3.txt";

type TNumInfo = { value: number, row: number, col: number, len: number };
type TGear = { row: number, col: number };

const isNumeric = (c: string): boolean => {
    return c >= "0" && c <= "9";
}

const extractNumbers = (lines: string[]): TNumInfo[] => {
    let numbers: TNumInfo[] = [];

    lines.forEach((line: string, row_idx: number) => {
        let col_idx = 0;
        let current_number_string = "";
        let prev_was_number = false;
        for (const c of line) {
            if (isNumeric(c)) {
                current_number_string += c;
                
                if (col_idx === line.length - 1) {
                    numbers.push({
                        value: +current_number_string,
                        row: row_idx,
                        col: col_idx - current_number_string.length + 1,
                        len: current_number_string.length
                    });
                    current_number_string = "";
                    prev_was_number = false;
                } else {
                    prev_was_number = true;
                }
            } else if (prev_was_number) {
                numbers.push({
                    value: +current_number_string,
                    row: row_idx,
                    col: col_idx - current_number_string.length,
                    len: current_number_string.length
                });
                current_number_string = "";
                prev_was_number = false;
            } else {
                prev_was_number = false;
            }
            
            col_idx++;
        }
    })

    return numbers;
}

const extractGears = (lines: string[]): TGear[] => {
    let gears: TGear[] = [];

    lines.forEach((line: string, row_idx: number) => {
        let col_idx = 0;
        for (const c of line) {
            if (c === "*") {
                gears.push({row: row_idx, col: col_idx});
            }
            col_idx++;
        }
    })

    return gears;
}

const isNextToCharacter = (num: TNumInfo, lines: string[], nr_of_lines: number, chars_per_line: number): boolean => {
    for (let row_idx = num.row - 1; row_idx <= num.row + 1; row_idx++) {
        for (let col_idx = num.col - 1; col_idx <= num.col + num.len; col_idx++) {
            if (row_idx < 0 || col_idx < 0) {
                continue
            } else if ((row_idx > nr_of_lines - 1) || (col_idx > chars_per_line - 1)) {
                continue
            }

            const char: string = lines[row_idx][col_idx];
            
            if (!isNumeric(char) && (char !== ".")) {
                return true;
            }
        }
    }

    return false;
}

const main = () => {
    const schematic: string = fs.readFileSync(INPUT_FILE, 'utf-8');
    const lines: string[] = schematic.split("\n");
    const nr_of_lines: number = lines.length;
    const chars_per_line: number = lines[0].length - 1;

    const numbers: TNumInfo[] = extractNumbers(lines);
    const gears: TGear[] = extractGears(lines);

    let sum_parts = 0;
    let sum_ratios = 0;

    numbers.forEach((num: TNumInfo) => {
        if (isNextToCharacter(num, lines, nr_of_lines, chars_per_line)) {
            sum_parts += num.value;
        }
    })

    gears.forEach((gear: TGear) => {
        let ratio = 1;
        let adjacent_parts = 0;

        numbers.forEach((num: TNumInfo) => {
            if ((Math.abs(gear.row - num.row) <= 1) && (gear.col >= num.col - 1) && (gear.col <= num.col + num.len)) {
                adjacent_parts++;
                ratio *= num.value;
            }
        })

        if (adjacent_parts === 2) {
            sum_ratios += ratio;
        }
    })

    console.log("Answer part 1: %d", sum_parts);
    console.log("Answer part 2: %d", sum_ratios);
}

main();