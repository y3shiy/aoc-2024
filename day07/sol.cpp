#include <iostream>
#include <fstream>
#include <algorithm>
#include <string_view>
#include <ranges>
#include <vector>

using namespace std::string_literals;

enum Operation {
	OP_ADD,
	OP_MUL,
	OP_TOTAL_CNT
};

auto g_nums = std::vector<int32_t>();
auto g_arrangement = std::vector<Operation>();
auto g_target_value = int64_t{};
auto g_calculated_value = int64_t{};

auto calc() {
	auto res = int64_t{g_nums[0]};
	for (int32_t i = 0; i < g_arrangement.size(); ++i) {
		switch (g_arrangement[i]) {
			case OP_ADD: res += g_nums[i+1]; break;
			case OP_MUL: res *= g_nums[i+1]; break;
			default: std::cerr << "Unreachable!\n";
		}
	}
	return res;
}

void rec(int32_t k) {
	if (k >= g_nums.size() - 1) {
		g_calculated_value = calc();
		return;
	}
	for (int32_t i = 0; i < OP_TOTAL_CNT; ++i) {
		g_arrangement.push_back(static_cast<Operation>(i));
		rec(k + 1);
		if (g_calculated_value == g_target_value) {
			return;
		}
		g_arrangement.pop_back();
	}
}

int main() {
	auto input_file = std::ifstream("input.txt");
	if (!input_file.is_open()) {
		std::cerr << "Failed to open the input file!\n";
		return 1;
	}
	auto str = ""s;
	auto calibration_res = int64_t{0};
	while (std::getline(input_file >> std::ws, str) && str != "") {
		g_target_value = std::atoll(str.data());
		std::cout << "Target value: " << g_target_value << "\n";
		auto pos = std::distance(
			str.begin(),
			std::find(str.begin(), str.end(), ':') + 2);
		auto sv = std::string_view(str.data() + pos, str.size() - pos);
		g_nums.clear();
		for (auto x : std::views::split(sv, ' ')) {
			g_nums.push_back(std::atoi(x.data()));
		}
		g_arrangement.clear();
		g_arrangement.reserve(g_nums.size() - 1);
		std::cout << "Nums: ";
		for (auto x : g_nums) {
			std::cout << x << " ";
		}
		std::cout << "\n";
		rec(0);
		if (g_calculated_value == g_target_value) {
			std::cout << "Yes!\n";
			calibration_res += g_target_value;
		} else {
			std::cout << "No.\n";
		}
	}
	std::cout << "Total calibration result: " << calibration_res << "\n";
}
