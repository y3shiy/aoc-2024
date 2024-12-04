#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>

using namespace std::string_literals;

auto g_letters = std::vector<std::string>(); 

auto is_row_in_range(int32_t r) {
	return 0 <= r && r < g_letters.size();
}

auto is_col_in_range(int32_t c) {
	return 0 <= c && c < g_letters.front().size();
}

auto check_str(
		int32_t r,
		int32_t c,
		int32_t dr,
		int32_t dc,
		const std::string_view str)
	{
	for (int32_t i = 0; i < str.size(); ++i) {
		if (!is_row_in_range(r) || !is_col_in_range(c)) {
			return false;
		}
		auto letter = g_letters[r][c];
		if (letter != str[i]) {
			return false;
		}
		r += dr;
		c += dc;
	}
	return true;
}

auto count_xmas_starting(int32_t row, int32_t col) {
	return check_str(row, col, 1, 0, "XMAS")
		+ check_str(row, col, 0, 1, "XMAS")
		+ check_str(row, col, 1, 1, "XMAS")
		+ check_str(row, col, 1, -1, "XMAS")
		+ check_str(row, col, 1, 0, "SAMX")
		+ check_str(row, col, 0, 1, "SAMX")
		+ check_str(row, col, 1, 1, "SAMX")
		+ check_str(row, col, 1, -1, "SAMX");
}

auto is_cross_mas_at(int32_t row, int32_t col) {
	return (check_str(row - 1, col - 1, 1, 1, "MAS")
			|| check_str(row - 1, col - 1, 1, 1, "SAM"))
		&& (check_str(row + 1, col - 1, -1, 1, "MAS")
			|| check_str(row + 1, col - 1, -1, 1, "SAM"));
}

int main() {
	auto input_file = std::ifstream("input.txt");
	if (!input_file.is_open()) {
		std::cerr << "Failed to open the input file\n";
		return 1; 
	}
	auto str = ""s;
	while (std::getline(input_file, str)) {
		g_letters.push_back(std::move(str));
	}
	for (auto str : g_letters) {
		std::cout << str << "\n";
	}
	auto xmas_sum = int64_t{0};
	auto cross_mas_sum = int64_t{0};
	for (int32_t i = 0; i < g_letters.size(); ++i) {
		for (int32_t j = 0; j < g_letters.front().size(); ++j) {
			xmas_sum += count_xmas_starting(i, j);
			cross_mas_sum += is_cross_mas_at(i, j); 
		}
	}
	std::cout << "Xmas Count: " << xmas_sum << "\n";
	std::cout << "Cross Mas Count: " << cross_mas_sum << "\n";
}
