#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>

using namespace std::string_literals;

auto are_safe_levels(int32_t lvl1, int32_t lvl2, bool is_incr) {
	auto diff = (lvl2 - lvl1) * (is_incr ? 1 : -1);
	return 1 <= diff && diff <= 3;
}

auto is_fully_safe_report(std::span<int32_t> report, bool is_incr) {
	if (report.size() <= 1) {
		return true;
	}
	for (auto [x, y] : std::views::pairwise(report)) {
		if (!are_safe_levels(x, y, is_incr)) {
			return false;
		}
	}
	return true;
}

auto is_safe_report(std::span<int32_t> report, bool is_incr) {
	if (is_fully_safe_report(report, is_incr)) {
		return true;
	}
	const auto &&n = report.size();
	for (int32_t i = 0; i < n; ++i) {
		if ((1 <= i && i < n - 1)
				&& !are_safe_levels(report[i - 1], report[i + 1], is_incr)) {
			continue;
		}
		auto left_subspan = report.subspan(0, i);
		auto right_subspan = report.subspan(i + 1, n - (i + 1));
		if (is_fully_safe_report(left_subspan, is_incr)
				&& is_fully_safe_report(right_subspan, is_incr)) {
			return true;
		}
	}
	return false;
}

auto is_safe_report(std::span<int32_t> report) {
	return is_safe_report(report, false)
		|| is_safe_report(report, true);
}

int main() {
	auto input_file = std::ifstream("input.txt");
	if (!input_file.is_open()) {
		std::cerr << "Failed to open input file\n";
		return 1;
	}
	auto reports = std::vector<std::vector<int>>();
	while (!input_file.eof()) {
		auto str = ""s;
		std::getline(input_file >> std::ws, str);
		if (input_file.peek() == '\n') {
			input_file.get();
		}
		reports.emplace_back();
		for (auto x : std::views::split(str, ' ')) {
			auto s = std::string(x.data(), x.size());
				 reports.back().push_back(std::stoi(s));
		}
	}
	for (auto report : reports) {
		for (auto level : report) {
			std::cout << level << " ";
		}
		std::cout << "\n";
	}
	std::cout << "Count of reports: " << reports.size() << "\n";
	auto safe_reports_count = int64_t{0};
	for (auto report : reports) {
		std::cout << "Report: \"";
		for (auto x : report) {
			std::cout << x << " ";
		}
		std::cout << "\" ";
		if (is_safe_report(report)) {
			safe_reports_count++;
			std::cout << "\tis safe\n";
		} else {

			std::cout << "\tis NOT safe\n";
		}
	}
	std::cout << "Count of safe reports: " << safe_reports_count << "\n";
}
