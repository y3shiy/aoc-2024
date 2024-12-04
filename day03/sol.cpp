#include <iostream>
#include <fstream>
#include <iterator>
#include <regex>
#include <format>

constexpr auto non_capt_gr(const std::string_view sv) {
	return std::format("(?:{})", sv);
}

int main() {
	auto input_file = std::ifstream("input.txt");
	if (!input_file.is_open()) {
		std::cerr << "Failed to open the input file\n";
		return 1;
	}
	auto file_content = std::string(
		std::istreambuf_iterator<char>(input_file),
		std::istreambuf_iterator<char>());
	std::cout << file_content << "\n";
	auto num_pat = non_capt_gr("[1-9][0-9]{0,2}");
	auto mul_pat = non_capt_gr(
		std::format("mul\\(({0}),({0})\\)", num_pat));
	auto do_pat = non_capt_gr("do\\(\\)");
	auto dont_pat = non_capt_gr("don't\\(\\)");
	auto re = std::regex(mul_pat + "|" + do_pat + "|" + dont_pat);
	auto re_it = std::sregex_iterator(
			file_content.begin(),
			file_content.end(),
			re);
	auto re_it_end = std::sregex_iterator();
	auto res = int64_t{0};
	auto should_mul = true;
	for (; re_it != re_it_end; ++re_it) {
		auto str = (*re_it)[0].str();
		if (str.starts_with("mul")) {
			if (should_mul) {
				auto left_oper = std::stoi((*re_it)[1]);
				auto right_oper = std::stoi((*re_it)[2]);
				res += left_oper * right_oper; 
			}
		} else if (str.starts_with("don't")) {
			should_mul = false;
		} else if (str.starts_with("do")) {
			should_mul = true;
		} else {
			std::cerr << "Unreachable\n";
			return 1;
		}
	}
	std::cout << "Result: " << res << "\n";
}
