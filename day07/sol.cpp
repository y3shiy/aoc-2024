#include <iostream>
#include <fstream>
#include <algorithm>
#include <string_view>
#include <ranges>
#include <vector>

using namespace std::string_literals;

auto g_nums = std::vector<int32_t>();

auto is_possible(int32_t k, int64_t desired_res) {
	if (k <= 0) {
		return g_nums[0] == desired_res;
	}
	if (g_nums[k] != 0
		&& desired_res % g_nums[k] == 0
		&& is_possible(k - 1, desired_res / g_nums[k]))
		{
		return true;
	}
	if (is_possible(k - 1, desired_res - g_nums[k])) {
		return true;
	}
	return false;	
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
		auto target_value = std::atoll(str.data());
		auto pos = std::distance(
			str.begin(),
			std::find(str.begin(), str.end(), ':') + 2);
		auto sv = std::string_view(str.data() + pos, str.size() - pos);
		g_nums.clear();
		for (auto x : std::views::split(sv, ' ')) {
			g_nums.push_back(std::atoi(x.data()));
		}
		if (is_possible(g_nums.size() - 1, target_value)) {
			calibration_res += target_value;
		}
	}
	std::cout << "Total calibration result: " << calibration_res << "\n";
}
