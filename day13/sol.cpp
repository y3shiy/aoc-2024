#include <iostream>
#include <fstream>
#include <optional>

using namespace std::string_literals;

// TODO: coefs - spelling

struct Vec2i {
	int64_t x;
	int64_t y;
};

void print_vec2i(Vec2i vec) {
	std::cout << "{" << vec.x << ", " << vec.y << "}";
}

struct Vec3i {
	int64_t x;
	int64_t y;
	int64_t z;
};

void print_vec3i(Vec3i vec) {
	std::cout << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
}

struct EqSol {
	Vec3i eq_coefs;
	Vec2i sol;
	int64_t coefs_gcd;
};

// TODO: stolen from emaxx
//		understand how it works
auto gcd(int64_t a, int64_t b, int64_t &x, int64_t &y) -> int64_t {
	if (a == 0) {
		x = 0;
		y = 1;
		return b;
	}
	auto x1 = int64_t{};
	auto y1 = int64_t{};
	int d = gcd(b % a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}

auto make_eq_sol(Vec3i eq_coefs) -> std::optional<EqSol> {
	auto bezout_coefs = Vec2i{};
	auto coefs_gcd = gcd(
		eq_coefs.x, eq_coefs.y,
		bezout_coefs.x, bezout_coefs.y);
	if (eq_coefs.z % coefs_gcd != 0) {
		return std::nullopt;
	}
	return EqSol{
		.eq_coefs = eq_coefs,
		.sol = {
			.x = eq_coefs.z * bezout_coefs.x / coefs_gcd,
			.y = eq_coefs.z * bezout_coefs.y / coefs_gcd,
		},
		.coefs_gcd = coefs_gcd,
	};
}

void eq_sol_next(EqSol &eq_sol, bool increase_x) {
	if (increase_x) {
		eq_sol.sol.x += eq_sol.eq_coefs.y / eq_sol.coefs_gcd;
		eq_sol.sol.y -= eq_sol.eq_coefs.x / eq_sol.coefs_gcd;
	} else {
		eq_sol.sol.x -= eq_sol.eq_coefs.y / eq_sol.coefs_gcd;
		eq_sol.sol.y += eq_sol.eq_coefs.x / eq_sol.coefs_gcd;
	}
}

void eq_sol_first_positive(EqSol &eq_sol, bool make_x_positive) {
	if (make_x_positive) {
		while (eq_sol.sol.x <= 0) {
			eq_sol_next(eq_sol, true);
		}
	} else {
		while (eq_sol.sol.y <= 0) {
			eq_sol_next(eq_sol, false);
		}
	}
}

void print_eq_sol(const EqSol &eq_sol) {
	std::cout << "{";
	std::cout << "eq: ";
	print_vec3i(eq_sol.eq_coefs);
	std::cout << ", sol: "; 
	print_vec2i(eq_sol.sol);
	std::cout << ", gcd: " << eq_sol.coefs_gcd;
	std::cout << "}";
}

auto first_positive_solution(Vec3i eq) -> std::optional<EqSol> {
	auto eq_sol_opt = make_eq_sol(eq);
	if (!eq_sol_opt.has_value()) {
		return std::nullopt;
	}
	auto &eq_sol = eq_sol_opt.value();
	if (eq_sol.sol.x < 0) {
		eq_sol_first_positive(eq_sol, true);
	}
	if (eq_sol.sol.y < 0) {
		eq_sol_first_positive(eq_sol, false);
	}
	if (eq_sol.sol.x >= 0 && eq_sol.sol.y >= 0) {
		return eq_sol;
	} else {
		return std::nullopt;
	}
}

auto verify_sol(Vec3i eq, Vec2i sol) -> bool {
	return eq.x * sol.x + eq.y * sol.y == eq.z;
}

auto sol(Vec2i btn_a, Vec2i btn_b, Vec2i prize_pos) -> int64_t {
	auto eq1 = Vec3i{
		.x = btn_a.x,
		.y = btn_b.x,
		.z = prize_pos.x,
	};
	auto eq2 = Vec3i{
		.x = btn_a.y,
		.y = btn_b.y,
		.z = prize_pos.y,
	};

	auto eq_sol1_opt = first_positive_solution(eq1);
	if (!eq_sol1_opt.has_value()) {
		std::cout << "\nNo solution: ";
		return 0;
	}
	auto &eq_sol1 = eq_sol1_opt.value();

	auto eq_sol = eq_sol1;
	while (eq_sol.sol.x >= 0) {
		std::cout << "\nSuggested solution: ";
		print_eq_sol(eq_sol);
		if (verify_sol(eq2, eq_sol.sol)) {
			std::cout << " and it's right";
			return eq_sol.sol.x * 3 + eq_sol.sol.y;
		} else {
			std::cout << " and it's wrong";
		}
		eq_sol_next(eq_sol, false);
	}
	
	eq_sol = eq_sol1;
	while (eq_sol.sol.y >= 0) {
		std::cout << "\nSuggested solution: ";
		print_eq_sol(eq_sol);
		if (verify_sol(eq2, eq_sol.sol)) {
			std::cout << " and it's right";
			return eq_sol.sol.x * 3 + eq_sol.sol.y;
		} else {
			std::cout << " and it's wrong";
		}
		eq_sol_next(eq_sol, true);
	}

	std::cout << "\nNo solution";
	return 0;
}

int main() {
	auto input_file = std::ifstream("input.txt");
	if (!input_file.is_open()) {
		std::cerr << "Failed to open the input file!\n";
		return 1;
	}
	auto sum_token_cost = int64_t{0};
	while (true) {
		auto btn_a = Vec2i{};
		input_file >> btn_a.x >> btn_a.y;
		if (btn_a.x == -42 && btn_a.y == -42) {
			break;
		}
		auto btn_b = Vec2i{};
		input_file >> btn_b.x >> btn_b.y;
		auto prize_pos = Vec2i{};
		input_file >> prize_pos.x >> prize_pos.y;
		auto token_cost = sol(btn_a, btn_b, prize_pos);
		std::cout << "Ans: " << token_cost << "\n";
		sum_token_cost += token_cost;
	}
	std::cout << "Total cost in tokents: " << sum_token_cost << "\n";
}
