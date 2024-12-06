#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

using namespace std::string_literals;

auto g_dr = std::array<int32_t, 4>{-1, +0, +1, +0};
auto g_dc = std::array<int32_t, 4>{+0, +1, +0, -1};
auto g_ch_repr = std::array<char, 4>{'^', '>', 'v', '<'};

int main() {
	auto input_file = std::ifstream("input.txt");
	if (!input_file.is_open()) {
		std::cerr << "Failed to open the input file\n";
		return 1;
	}
	auto grid = std::vector<std::string>();
	auto str = ""s;
	auto guard_r = 0;
	auto guard_c = 0;
	auto guard_dir = 0;
	auto i = 0;
	while (std::getline(input_file, str) && str != "") {
		for (int32_t j = 0; j < str.size(); ++j) {
			if (str[j] == '^' || str[j] == '>' || str[j] == 'v' || str[j] == '<') {
				guard_r = i;
				guard_c = j;
				auto it = std::find(g_ch_repr.begin(), g_ch_repr.end(), str[j]);
				guard_dir = std::distance(g_ch_repr.begin(), it); 
				str[j] = '.';
			}
		}
		grid.push_back(std::move(str));
		i++;
	}

	for (auto str : grid) {
		std::cout << str << "\n";
	}

	std::cout << "guard_r = " << guard_r
		<< " guard_c = " << guard_c
		<< " guard_dr = " << g_dr[guard_dir]
		<< " guard_dc = " << g_dc[guard_dir] << "\n";

	grid[guard_r][guard_c] = 'X';
	while (true) {
		guard_r += g_dr[guard_dir];
		guard_c += g_dc[guard_dir];
		if (!(0 <= guard_r && guard_r < grid.size())
			|| !(0 <= guard_c && guard_c < grid.front().size())) {
			break;
		}
		if (grid[guard_r][guard_c] == '#') {
			std::cout << "Rotation due to the obstacle at ("
				<< guard_r << ", " << guard_c << ")\n";
			guard_r -= g_dr[guard_dir];
			guard_c -= g_dc[guard_dir];
			guard_dir = (guard_dir + 1) % g_ch_repr.size();
		} else {
			grid[guard_r][guard_c] = 'X';
		}
	}

	auto x_count = int64_t{0};
	for (auto str : grid) {
		x_count += std::count(str.begin(), str.end(), 'X');
		std::cout << str << "\n";
	}

	std::cout << "Count of X's: " << x_count << "\n";
}
