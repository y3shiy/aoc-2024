#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>

using namespace std::string_literals;

struct Position {
	int32_t row, col;
};

auto is_antenna(char c) {
	return ('0' <= c && c <= '9')
		|| ('a' <= c && c <= 'z')
		|| ('A' <= c && c <= 'Z');
}

auto g_map = std::vector<std::string>();
auto g_antennas = std::unordered_map<char, std::vector<Position>>();

auto is_on_map(Position p) {
	return (0 <= p.row && p.row < g_map.size())
		&& (0 <= p.col && p.col < g_map.front().size());
}

void propogate_wave(Position pos, int32_t dr, int32_t dc) {
	while (is_on_map(pos)) {
		g_map.at(pos.row).at(pos.col) = '#';
		pos.row += dr;
		pos.col += dc;
	}
}

void put_antinodes(Position p1, Position p2) {
	auto dr = p2.row - p1.row;
	auto dc = p2.col - p1.col;
	propogate_wave(p2, dr, dc);
	propogate_wave(p1, -dr, -dc);
}

int main() {
	auto input_file = std::ifstream("input.txt");
	if (!input_file.is_open()) {
		std::cerr << "Failed to open the input file!\n";
		return 1;
	}
	auto i = int32_t{0};
	auto str = ""s;
	while (std::getline(input_file, str) && str != "") {
		for (int32_t j = 0; j < str.size(); ++j) {
			if (is_antenna(str.at(j))) {
				g_antennas[str.at(j)].push_back({i, j});
			}
		}
		g_map.push_back(std::move(str));
		i++;
	}

	for (auto x : g_map) {
		std::cout << x << "\n";
	}

	for (auto [k, v] : g_antennas) {
		std::cout << k << ": ";
		for (auto [r, c] : v) {
			std::cout << "{" << r << ", " << c << "}";
		}
		std::cout << "\n";
	}

	for (auto [k, v]  : g_antennas) {
		for (int32_t i = 0; i < v.size(); ++i) {
			for (int32_t j = i + 1; j < v.size(); ++j) {
				put_antinodes(v.at(i), v.at(j));
			}
		}
	}

	auto antinodes_count = int64_t{0};
	for (auto x : g_map) {
		antinodes_count += std::count(x.begin(), x.end(), '#');
	}
	std::cout << "Antinodes count: " << antinodes_count << "\n";
}
