#include <iostream>
#include <fstream>
#include <cstdio>
#include <unordered_map>
#include <unordered_set>
#include <ranges>
#include <string_view>
#include <span>
#include <vector>
#include <algorithm>

using namespace std::string_literals;

auto g_num_to_aft = std::unordered_multimap<int32_t, int32_t>();
// g_graph[i][_] = j <=> i should precede j
auto g_graph = std::unordered_map<int32_t, std::vector<int32_t>>();

auto is_valid(std::span<int32_t> nums) {
	auto added_nums = std::unordered_set<int32_t>();
	for (auto x : nums) {
		auto range_aft = g_num_to_aft.equal_range(x);
		auto is_wrong_order = std::any_of(
			range_aft.first,
			range_aft.second,
			[&](auto kv) { return added_nums.contains(kv.second); });
		if (is_wrong_order) {
			return false; 
		}
		added_nums.insert(x);
	}
	return true;
}

auto g_sorted_nums = std::vector<int32_t>();
// g_visited[_] = t <=> it was visited on the t-th iter
auto g_visited = std::unordered_map<int32_t, bool>();

void dfs(int32_t v) {
	if (!g_visited.contains(v)) {
		std::cout << "MEow!\n";
	}
	g_visited[v] = true;
	for (auto x : g_graph[v]) {
		auto it = g_visited.find(x);
		if (it == g_visited.end()) {
			continue;
		}
		if (it->second == 0) {
			dfs(x);
		}
	}
	g_sorted_nums.push_back(v);
}

int main() {
	auto input_file = std::ifstream("input.txt");
	if (!input_file.is_open()) {
		std::cerr << "Failed to open input file!\n";
		return 1;
	}
	auto str = " "s;
	while (std::getline(input_file, str) && str != "") {
		auto num_bef = int32_t{0};
		auto num_aft = int32_t{0};
		if (std::sscanf(str.c_str(), "%d|%d", &num_bef, &num_aft) == 2) {
			g_num_to_aft.insert({num_bef, num_aft});
			auto [kv_it, is_ins] = g_graph.emplace(num_bef, std::vector<int32_t>());
			kv_it->second.push_back(num_aft);
		} else {
			std::cerr << "Failed to parse the input file!\n";
			return 1;
		}
	}

	std::cout << "Graph: \n";
	for (auto [k, v] : g_graph) {
		std::cout << k << ": ";
		for (auto x : v) {
			std::cout << x << " ";
		}
		std::cout << "\n";
	}

	std::cout << "Analyzing sequences: \n";
	auto nums = std::vector<int32_t>();
	auto sum_of_correct_middles = int64_t{0};
	auto sum_of_fixed_middles = int64_t{0};
	while (std::getline(input_file, str) && str != "") {
		for (auto x : std::views::split(str, ',')) {
			nums.push_back(std::atoi(x.data()));
		}
		if (is_valid(nums)) {
			sum_of_correct_middles += nums[nums.size() / 2];
			std::cout << "Y" << nums[nums.size() / 2] << ": " << str << "\n";
		} else {
			std::cout << "N" << str << "\n";
			g_sorted_nums.clear();
			g_visited.clear();
			for (auto x : nums) {
				g_visited.insert({x, false});
			}
			for (auto [k, v] : g_visited) {
				if (v == 0) {
					dfs(k);
				}
			}
			std::cout << "Sorted (reversed) version:";
			for (auto x : g_sorted_nums) {
				std::cout << x << " ";
			}
			std::cout << "\n";
			sum_of_fixed_middles += g_sorted_nums[g_sorted_nums.size() / 2];
		}
		nums.clear();
	}
	std::cout << "Sum of correct middles: " << sum_of_correct_middles << "\n";
	std::cout << "Sum of fixed middles: " << sum_of_fixed_middles << "\n";
}
