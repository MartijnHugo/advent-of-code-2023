// --- Day 5: If You Give A Seed A Fertilizer ---
// You take the boat and find the gardener right where you were told he would
// be: managing a giant "garden" that looks more to you like a farm.

// "A water source? Island Island is the water source!" You point out that
// Snow Island isn't receiving any water.

// "Oh, we had to stop the water because we ran out of sand to filter it with!
// Can't make snow with dirty water. Don't worry, I'm sure we'll get more sand
// soon; we only turned off the water a few days... weeks... oh no." His face
// sinks into a look of horrified realization.

// "I've been so busy making sure everyone here has food that I completely
// forgot to check why we stopped getting more sand! There's a ferry leaving
// soon that is headed over in that direction - it's much faster than your
// boat. Could you please go check it out?"

// You barely have time to agree to this request when he brings up another.
// "While you wait for the ferry, maybe you can help us with our food
// production problem. The latest Island Island Almanac just arrived and we're
// having trouble making sense of it."

// The almanac (your puzzle input) lists all of the seeds that need to be
// planted. It also lists what type of soil to use with each kind of seed,
// what type of fertilizer to use with each kind of soil, what type of water
// to use with each kind of fertilizer, and so on. Every type of seed, soil,
// fertilizer and so on is identified with a number, but numbers are reused by
// each category - that is, soil 123 and fertilizer 123 aren't necessarily
// related to each other.

// For example:

// seeds: 79 14 55 13

// seed-to-soil map:
// 50 98 2
// 52 50 48

// soil-to-fertilizer map:
// 0 15 37
// 37 52 2
// 39 0 15

// fertilizer-to-water map:
// 49 53 8
// 0 11 42
// 42 0 7
// 57 7 4

// water-to-light map:
// 88 18 7
// 18 25 70

// light-to-temperature map:
// 45 77 23
// 81 45 19
// 68 64 13

// temperature-to-humidity map:
// 0 69 1
// 1 0 69

// humidity-to-location map:
// 60 56 37
// 56 93 4

// The almanac starts by listing which seeds need to be planted: seeds 79, 14,
// 55, and 13.

// The rest of the almanac contains a list of maps which describe how to
// convert numbers from a source category into numbers in a destination
// category. That is, the section that starts with seed-to-soil map: describes
// how to convert a seed number (the source) to a soil number (the
// destination). This lets the gardener and his team know which soil to use
// with which seeds, which water to use with which fertilizer, and so on.

// Rather than list every source number and its corresponding destination
// number one by one, the maps describe entire ranges of numbers that can be
// converted. Each line within a map contains three numbers: the destination
// range start, the source range start, and the range length.

// Consider again the example seed-to-soil map:

// 50 98 2
// 52 50 48

// The first line has a destination range start of 50, a source range start of
// 98, and a range length of 2. This line means that the source range starts
// at 98 and contains two values: 98 and 99. The destination range is the same
// length, but it starts at 50, so its two values are 50 and 51. With this
// information, you know that seed number 98 corresponds to soil number 50 and
// that seed number 99 corresponds to soil number 51.

// The second line means that the source range starts at 50 and contains 48
// values: 50, 51, ..., 96, 97. This corresponds to a destination range
// starting at 52 and also containing 48 values: 52, 53, ..., 98, 99. So, seed
// number 53 corresponds to soil number 55.

// Any source numbers that aren't mapped correspond to the same destination
// number. So, seed number 10 corresponds to soil number 10.

// So, the entire list of seed numbers and their corresponding soil numbers
// looks like this:

// seed  soil
// 0     0
// 1     1
// ...   ...
// 48    48
// 49    49
// 50    52
// 51    53
// ...   ...
// 96    98
// 97    99
// 98    50
// 99    51

// With this map, you can look up the soil number required for each initial
// seed number:

//  - Seed number 79 corresponds to soil number 81.
//  - Seed number 14 corresponds to soil number 14.
//  - Seed number 55 corresponds to soil number 57.
//  - Seed number 13 corresponds to soil number 13.

// The gardener and his team want to get started as soon as possible, so
// they'd like to know the closest location that needs a seed. Using these
// maps, find the lowest location number that corresponds to any of the
// initial seeds. To do this, you'll need to convert each seed number through
// other categories until you can find its corresponding location number. In
// this example, the corresponding types are:

//  - Seed 79, soil 81, fertilizer 81, water 81, light 74, temperature 78, humidity 78, location 82.
//  - Seed 14, soil 14, fertilizer 53, water 49, light 42, temperature 42, humidity 43, location 43.
//  - Seed 55, soil 57, fertilizer 57, water 53, light 46, temperature 82, humidity 82, location 86.
//  - Seed 13, soil 13, fertilizer 52, water 41, light 34, temperature 34, humidity 35, location 35.

// So, the lowest location number in this example is 35.

// What is the lowest location number that corresponds to any of the initial
// seed numbers?

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

const std::string INPUT_FILE = "etc/5.txt";

class Range
{
public:
    Range(int64_t start, int64_t length) : start_(start), length_(length){};

    int64_t getStart() { return start_; }
    int64_t getEnd() { return start_ + length_ - 1; }
    int64_t getLength() { return length_; }

private:
    int64_t start_;
    int64_t length_;
};

class RangeMap
{
public:
    RangeMap(int64_t source_start, int64_t source_end, int64_t shift) : source_start_(source_start), source_end_(source_end), shift_(shift) {}

    bool inRange(int64_t source)
    {
        return source >= source_start_ && source <= source_end_;
    }

    int64_t getDestination(int64_t source)
    {
        return source + shift_;
    }

    int64_t getDestStart() { return source_start_ + shift_; }
    int64_t getSourceStart() { return source_start_; }
    int64_t getDestEnd() { return source_end_ + shift_; }
    int64_t getSourceEnd() { return source_end_; }
    int64_t getLength() { return source_end_ - source_start_; }
    int64_t getShift() { return shift_; }

private:
    int64_t source_start_;
    int64_t source_end_;
    int64_t shift_;
};

class Almanac
{
public:
    Almanac(std::ifstream &almanacFile)
    {
        uint8_t line_nr = 0;
        bool parsing_map = false;
        std::string map_name;
        std::vector<int64_t> seeds = {};
        std::vector<RangeMap> current_map = {};
        std::map<std::string, std::vector<RangeMap>> maps;
        std::vector<std::string> map_order = {};
        std::vector<Range> ranges = {};

        for (std::string line; std::getline(almanacFile, line);)
        {
            // std::cout << line << std::endl;
            if (line_nr == 0)
            {
                uint8_t seed_nr = 0;
                std::string prev_seed;
                std::stringstream seedLineStream(line.substr(7, line.size() - 7));
                for (std::string seed; std::getline(seedLineStream, seed, ' ');)
                {
                    seeds.push_back(static_cast<int64_t>(std::stoul(seed)));

                    if (seed_nr % 2 == 1)
                    {
                        Range seedRange(static_cast<int64_t>(std::stoul(prev_seed)), static_cast<int64_t>(std::stoul(seed)));
                        ranges.push_back(seedRange);
                    }

                    seed_nr++;
                    prev_seed = seed;
                }
            }
            else if (line.find(":") != std::string::npos)
            { // ":" in line
                std::stringstream mapTitleLineStream(line);
                std::getline(mapTitleLineStream, map_name, ' ');
                maps[map_name] = {};
                parsing_map = true;
                map_order.push_back(map_name);
            }
            else if (!line.size())
            {
                current_map = {};
                parsing_map = false;
            }
            else if (parsing_map)
            {
                std::string dest_start_str, source_start_str, length_str;
                int64_t dest_start, source_start, length;
                std::stringstream rangeMapStream(line);
                std::getline(rangeMapStream, dest_start_str, ' ');
                std::getline(rangeMapStream, source_start_str, ' ');
                std::getline(rangeMapStream, length_str, ' ');

                dest_start = static_cast<int64_t>(std::stoul(dest_start_str));
                source_start = static_cast<int64_t>(std::stoul(source_start_str));
                length = static_cast<int64_t>(std::stoul(length_str));

                maps[map_name].push_back(*new RangeMap(source_start, source_start + length, dest_start - source_start));
            }

            line_nr++;
        }

        ranges_ = ranges;
        seeds_ = seeds;
        maps_ = maps;
        map_order_ = map_order;
    }

    std::vector<int64_t> getSeeds() { return seeds_; }
    std::vector<Range> getSeedRanges() { return ranges_; }
    std::map<std::string, std::vector<RangeMap>> getMaps() { return maps_; }
    std::vector<std::string> getMapOrder() { return map_order_; }

private:
    std::vector<int64_t> seeds_;
    std::vector<Range> ranges_;
    std::map<std::string, std::vector<RangeMap>> maps_;
    std::vector<std::string> map_order_;
};

std::vector<RangeMap> transform_map(std::vector<RangeMap> input_map, std::vector<RangeMap> transformation_map)
{
    std::vector<RangeMap> output_map;
    for (RangeMap iMap : input_map)
    {
        std::vector<RangeMap> remaining_map = {iMap};
        int64_t i = 0;
        while (i < remaining_map.size()) {
            for (RangeMap rMap : remaining_map) {
                std::cout << rMap.getSourceStart() << "-" << rMap.getSourceEnd()
                << " --> " << rMap.getDestStart() << "-" << rMap.getDestEnd() << std::endl;
            }
            for (RangeMap tMap : transformation_map)
            {
                if (iMap.getDestStart() >= tMap.getSourceStart() && iMap.getDestEnd() <= tMap.getSourceEnd()) {
                    output_map.push_back(*new RangeMap(iMap.getSourceStart(), iMap.getSourceEnd(), iMap.getShift() + tMap.getShift()));
                } else if (iMap.getDestStart() < tMap.getSourceStart() && iMap.getDestEnd() <= tMap.getSourceEnd()) {
                    int64_t unmapped_start = tMap.getSourceStart() - iMap.getDestStart();
                    output_map.push_back(*new RangeMap(iMap.getSourceStart() + unmapped_start, iMap.getSourceEnd(), iMap.getShift() + tMap.getShift()));
                    remaining_map.push_back(*new RangeMap(iMap.getSourceStart(), iMap.getSourceStart() + unmapped_start, iMap.getShift()));
                    i++;
                } else if (iMap.getDestStart() >= tMap.getSourceStart() && iMap.getDestEnd() > tMap.getSourceEnd()) {
                    int64_t unmapped_end = iMap.getDestEnd() - tMap.getSourceEnd();
                    output_map.push_back(*new RangeMap(iMap.getSourceStart(), iMap.getSourceEnd() - unmapped_end, iMap.getShift() + tMap.getShift()));
                    remaining_map.push_back(*new RangeMap(iMap.getSourceEnd() - unmapped_end, iMap.getSourceEnd(), iMap.getShift()));
                    i++;
                } else if (iMap.getDestStart() < tMap.getSourceStart() && iMap.getDestEnd() > tMap.getSourceEnd()) {
                    int64_t unmapped_start = tMap.getSourceStart() - iMap.getDestStart();
                    int64_t unmapped_end = iMap.getDestEnd() - tMap.getSourceEnd();
                    output_map.push_back(*new RangeMap(iMap.getSourceStart() + unmapped_start, iMap.getSourceEnd() - unmapped_end, iMap.getShift() + tMap.getShift()));
                    remaining_map.push_back(*new RangeMap(iMap.getSourceStart(), iMap.getSourceStart() + unmapped_start, iMap.getShift()));
                    remaining_map.push_back(*new RangeMap(iMap.getSourceEnd() - unmapped_end, iMap.getSourceEnd(), iMap.getShift()));
                    i += 2;
                }
            }
        }
    }

    return output_map;
}

int main()
{
    std::ifstream file(INPUT_FILE);
    Almanac almanac(file);

    std::map<std::string, std::vector<RangeMap>> maps = almanac.getMaps();

    std::vector<RangeMap> map_12 = transform_map(maps["seed-to-soil"], maps["soil-to-fertilizer"]);
    for (RangeMap rangeMap : map_12)
    {
        std::cout << rangeMap.getSourceStart() << "-" << rangeMap.getSourceEnd()
        << " --> " << rangeMap.getDestStart() << "-" << rangeMap.getDestEnd() << std::endl;
    }

    // int64_t value;
    // std::vector<int64_t> locations_1 = {};
    // std::vector<int64_t> locations_2 = {};

    // for (const int64_t seed : almanac.getSeeds())
    // {
    //     value = seed;
    //     for (const auto map_name : almanac.getMapOrder())
    //     {
    //         std::sort(maps[map_name].begin(), maps[map_name].end(), [](RangeMap map1, RangeMap map2)
    //                   { return map1.getSourceStart() < map2.getSourceStart(); });
    //         std::vector<RangeMap> map = maps[map_name];

    //         for (RangeMap rangeMap : map)
    //         {
    //             if (rangeMap.inRange(value))
    //             {
    //                 value = rangeMap.getDestination(value);
    //                 break;
    //             }
    //         }
    //     }
    //     locations_1.push_back(value);
    // }

    // for (Range seedRange : almanac.getSeedRanges())
    // {
    //     std::cout << "Seed range: " << seedRange.getStart() << " - " << seedRange.getEnd() << std::endl;
    // }

    // std::cout << "Answer part 1: " << *std::min_element(locations_1.begin(), locations_1.end()) << std::endl;
    // std::cout << "Answer part 2: " << *std::min_element(locations_2.begin(), locations_2.end()) << std::endl;
    return 0;
}