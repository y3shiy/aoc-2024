#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>

using namespace std::string_literals;

auto g_dr = std::array<int32_t, 4>{-1, +0, +1, +0};
auto g_dc = std::array<int32_t, 4>{+0, +1, +0, -1};
auto g_ch_repr = std::array<char, 4>{'^', '>', 'v', '<'};

// Returns whether there was a loop or not
auto will_guard_get_stuck(
	std::vector<std::string> &grid,
	int32_t r,
	int32_t c,
	int32_t dir,
	std::function<void(std::vector<std::string>*, int32_t, int32_t, int32_t)> fn)
	{
	fn(&grid, r, c, dir);
	while (true) {
		r += g_dr[dir];
		c += g_dc[dir];
		if (!(0 <= r && r < grid.size())
			|| !(0 <= c && c < grid.front().size())) {
			break;
		}
		if (grid[r][c] == g_ch_repr[dir]) {
			return true;
		}
		if (grid[r][c] == '#') {
			r -= g_dr[dir];
			c -= g_dc[dir];
			if (grid[r][c] == 'X' || grid[r][c] == '.' || grid[r][c] == '#') {
				grid[r][c] = g_ch_repr[dir];
			}
			dir = (dir + 1) % g_ch_repr.size();
		} else {
			fn(&grid, r, c, dir);
		}
	}
	return false;
}

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
			auto it = std::find(g_ch_repr.begin(), g_ch_repr.end(), str[j]);
			if (it == g_ch_repr.end()) {
				continue;
			}
			guard_r = i;
			guard_c = j;
			guard_dir = std::distance(g_ch_repr.begin(), it); 
			str[j] = '.';
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
	auto grid_with_xs = grid;

	auto is_there_loop = will_guard_get_stuck(grid_with_xs, guard_r, guard_c, guard_dir,
		[](auto grid, auto r, auto c, auto dir) {
			if ((*grid)[r][c] == '.') {
				(*grid)[r][c] = 'X';
			}
		});
	if (is_there_loop) {
		std::cerr << "Unreachable!\n";
		return 1;
	}

	auto x_count = int64_t{0};
	for (auto str : grid_with_xs) {
		x_count += std::count_if(str.begin(), str.end(),
			[](auto x) {
				return x != '.' && x != '#';
			});
	}

	std::cout << "Count of X's: " << x_count << "\n";

	auto possible_obstruction_placement_count = int64_t{0};
	for (int32_t i = 0; i < grid_with_xs.size(); ++i) {
		for (int32_t j = 0; j < grid_with_xs.front().size(); ++j) {
			if (grid_with_xs[i][j] == '.' || grid_with_xs[i][j] == '#') {
				continue;
			}
			if (i == guard_r && j == guard_c) {
				continue;
			}
			auto grid_copy = grid;
			grid_copy[i][j] = '#';

			auto is_there_loop = will_guard_get_stuck(
				grid_copy,
				guard_r,
				guard_c,
				guard_dir,
				[](auto grid, auto r, auto c, auto dir){});

			if (is_there_loop) {
				possible_obstruction_placement_count++;
			}
		}
	}
	std::cout << "Count of possible placements of an obstruction: "
		<< possible_obstruction_placement_count << "\n";
}
