#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <array>
#include <utility>

using namespace std::string_literals;

auto g_map = std::vector<std::string>();
auto g_visited = std::vector<std::vector<bool>>();

struct Position {
	int32_t row;
	int32_t col;
};

constexpr auto DR = std::array<int32_t, 4>{+1, +0, +0, -1};
constexpr auto DC = std::array<int32_t, 4>{+0, +1, -1, +0};

bool is_on_map(Position pos) {
	return (0 <= pos.row && pos.row < g_map.size())
		&& (0 <= pos.col && pos.col < g_map.at(0).size());
}

auto bfs(int32_t row, int32_t col) -> std::pair<int64_t, int64_t> {
	auto queue = std::deque<Position>();
	g_visited.at(row).at(col) = true;
	queue.emplace_back(row, col);
	auto area = int64_t{1};
	auto perimeter = int64_t{0};
	while (!queue.empty()) {
		auto pos = queue.front();
		queue.pop_front();
		for (int32_t i = 0; i < DR.size(); ++i) {
			auto new_pos = Position{
				.row = pos.row + DR[i],
				.col = pos.col + DC[i],
			};
			if (!is_on_map(new_pos)) {
				perimeter++;
				continue;
			}
			if (g_map.at(pos.row).at(pos.col)
					!= g_map.at(new_pos.row).at(new_pos.col))
				{
				perimeter++;
				continue;
			}
			if (g_visited.at(new_pos.row).at(new_pos.col)) {
				continue;
			}
			g_visited.at(new_pos.row).at(new_pos.col) = true;
			queue.push_back(new_pos);
			area++;
		}
	}
	return {area, perimeter};
}

int main() {
	auto input_file = std::ifstream("input.txt");
	if (!input_file.is_open()) {
		std::cerr << "Failed to open input file\n";
		return 1;
	}
	auto str = ""s;
	while (std::getline(input_file, str) && str != "") {
		g_map.push_back(std::move(str));
	}
	
	g_visited.assign(g_map.size(), {});
	for (int32_t i = 0; i < g_map.size(); ++i) {
		g_visited.at(i).assign(g_map.at(0).size(), false);
	}

	for (auto x : g_map) {
		std::cout << x << "\n";
	}
	std::cout << "\n";

	auto cost = int64_t{0};

	for (int32_t i = 0; i < g_map.size(); ++i) {
		for (int32_t j = 0; j < g_map.at(0).size(); ++j) {
			if (g_visited.at(i).at(j)) {
				continue;
			}
			auto [area, perimeter] = bfs(i, j);
			cost += area * perimeter;
		}
	}
	
	std::cout << "Cost: " << cost << "\n";

	return 0;
}
