#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>

int main() {
	auto input_file = std::ifstream("input.txt");
	if (!input_file.is_open()) {
		std::cerr << "Failed to open the file\n";
		return 1;
	}
	auto nums1 = std::vector<int32_t>();
	nums1.reserve(1000);
	auto nums2 = std::vector<int32_t>();
	nums2.reserve(1000);	
	while (!input_file.eof()) {
		auto a = int32_t{};
		auto b = int32_t{};
		input_file >> a >> b;
		nums1.push_back(a);
		nums2.push_back(b);
	}

	std::sort(nums1.begin(), nums1.end());
	std::sort(nums2.begin(), nums2.end());

	std::cout << "Nums1:\n";
	for (auto x : nums1) {
		std::cout << x << "\n";
	}

	std::cout << "Nums2:\n";
	for (auto x : nums2) {
		std::cout << x << "\n";
	}
	
	auto sum = int64_t{0};
	for (int i = 0; i < nums1.size(); ++i) {
		sum += std::abs(nums1[i] - nums2[i]);
	}
	std::cout << "The total distance: " << sum << "\n";

	auto sim_score = int64_t{0};
	auto nums1_idx = int32_t{0};
	auto nums2_idx = int32_t{0};
	while (nums1_idx < nums1.size()) {
		auto i = nums1_idx;
		auto num = nums1[i];
		while (i < nums1.size() && nums1[i] == num) {
			i++;
		}
		while (nums2_idx < nums2.size() && nums2[nums2_idx] < num) {
			nums2_idx++;
		}
		auto j = nums2_idx;
		while (j < nums2.size() && nums2[j] == num) {
			j++;
		}
		sim_score += (i - nums1_idx) * (j - nums2_idx) * num;
		nums1_idx = i;
		nums2_idx = j;
	}
	std::cout << "Similarity score: " << sim_score << "\n";
}
