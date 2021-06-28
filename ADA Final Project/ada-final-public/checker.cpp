#if defined(ONLINE_JUDGE) && \
    !defined(NDEBUG)           // Enable defensive checks on the judge
#pragma GCC optimize("trapv")  // Trap on integer overflow
#define _GLIBCXX_DEBUG         // Enable checks (e.g., bounds) for containers
#endif

#include <algorithm>  // for max, copy, fill_n
#include <array>      // for array
#include <cassert>    // for assert
#include <cfenv>      // for feenableexcept
#include <cmath>      // for isfinite
#include <cstdint>    // for uint8_t, int8_t
#include <cstdlib>    // for exit
#include <cstring>    // for strcmp
#include <exception>  // for exception
#include <fstream>    // for ifstream
#include <iomanip>    // for operator<<, setprecision
#include <iostream>   // for operator<<, ifstream, basic_istream::operat...
#include <limits>     // for numeric_limits
#include <memory>     // for allocator, allocator_traits<>::value_type
#include <regex>      // for regex_match, match_results<>::_Base_type
#include <sstream>    // for stringstream
#include <string>     // for string, basic_string, operator+, char_traits
#include <tuple>      // for tuple
#include <utility>    // for pair
#include <vector>     // for vector, vector<>::reference, _Bit_reference

namespace {

namespace shewchuk {
// Shewchuk's Floating Point Summation Algorithm
// https://github.com/achan001/fsum/blob/master/shewchuk.cpp
// Refactored to fit coding style
//
// MIT License
//
// Copyright (c) 2018 Albert Chan
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"

constexpr int kScStack = 48;

template <class D>
class ScPartials {  // modified shewchuk algorithm
 public:
	ScPartials() { *this = 0.0; }
	explicit ScPartials(D x) { *this = x; }
	void operator+=(D x);
	void operator-=(D x) { return operator+=(-x); }
	ScPartials &operator=(D x) {
		sum[last = 0] = x;
		return *this;
	}
	operator D();  // NOLINT

 private:
	int last{};
	std::array<D, kScStack> sum{};
};

template <class D>
void ScPartials<D>::operator+=(D x) {
	int i = 0;
	D y, hi, lo;
	for (int j = 0; j <= last; j++) {
		y = sum[j];
		hi = x + y;
#ifdef SC_BRANCH
		lo = (fabs(x) < fabs(y)) ? x - (hi - y) : y - (hi - x);
#else
		lo = hi - x;
		lo = (y - lo) + (x - (hi - lo));
#endif
		x = hi;
		if (lo != 0) sum[i++] = x, x = lo;
	}
	if (std::isnan(x)) {
		last = 0;
		return;
	}
	sum[last = i] = x;
	if (i == kScStack - 1) *this += static_cast<D>(0.0);
	assert(last < kScStack && "Internal error: ScPartials invariant violated.");
}

template <class D>
ScPartials<D>::operator D() {
	for (;; *this += static_cast<D>(0.0)) {
		int n = last;
		if (n == 0) return sum[0];
		if (n == 1) return sum[0] + sum[1];
		D y{};
		do {
			y = static_cast<D>(0.5) * sum[n--];
		} while (n && sum[n] == sum[n] + y);
		if (sum[n] == sum[n] + y) {  // partials no overlap
			D r = sum[n] + (y += y);   // sum[n] + 2y => r + y/2
			y = 2 * (y - (r - sum[n]));
			if (y != y + r - r) return r;  // 0 < |y| < 1 ULP
			return ((y < 0) == (sum[n + 2] < 0) && sum[n + 2] != 0) ? r + y : r;
		}
	}
}
#pragma GCC diagnostic pop
}  // namespace shewchuk

constexpr int kLineMax = 256;

struct Operation {
	int slices{};
	int duration{};
	std::vector<int> deps;
	bool done{};
};

struct Job {
	double weight{};
	std::vector<Operation> ops;
};

struct Event {
	int time{};
	bool is_start{};
	int job{}, op{};
	// Use basic_string instead of vector for small-buffer optimization
	std::basic_string<uint8_t> slices;
	int num_line{};
	bool operator<(const Event &rhs) const {
		// Make sure to sort end events before start events
		return time != rhs.time ? time < rhs.time : is_start < rhs.is_start;
	}
};

struct Constraints {
	int slices;
	int jobs;
	int ops;
	int duration;
	int weight;
};

void IAssert(const bool cond, const int lineno, const std::string &msg) {
	if (!cond) {
		std::cerr << "Test case error: " << lineno << ": " << msg << '\n';
#ifdef ONLINE_JUDGE_TESTDATA
		// If we get bad user-submitted testdata on the judge, return WA
		exit(4);
#else
		exit(3);
#endif
	}
}

void OAssert(const bool cond, const int lineno, const std::string &msg) {
	if (!cond) {
		std::cerr << "Output file error: " << lineno << ": " << msg << '\n';
		exit(4);
	}
}

bool CheckCycle(const Job &job) {
	constexpr int8_t kUnVis = 0, kVisited = -1, kInStack = 1;
	std::vector<int8_t> visited(job.ops.size(), kUnVis);
#ifndef NDEBUG
	std::vector<int> order(job.ops.size(), -1);
	int head = 0;
#endif
	auto dfs = [&](const auto &f, const int u) {
		if (visited[u] == kInStack) return true;  // Cycle found
		if (visited[u] == kVisited) return false;
		visited[u] = kInStack;
		for (const auto v : job.ops[u].deps) {
			if (f(f, v)) return true;
		}
		visited[u] = kVisited;
#ifndef NDEBUG
		order[u] = head++;
#endif
		return false;
	};
	for (int i = 0; i < static_cast<int>(job.ops.size()); i++) {
		if (!visited[i]) {
			if (dfs(dfs, i)) return true;
		}
	}
#ifndef NDEBUG
	// Assert that the order is indeed permissible, just in case
	for (int i = 0; i < static_cast<int>(job.ops.size()); i++) {
		assert(visited[i] && order[i] != -1 &&
		       "Internal error: An operation was not visited.");
		for (const auto v : job.ops[i].deps)
			assert(order[v] < order[i] && "Internal error: Topological sort broken.");
	}
#endif
	return false;
}

std::pair<int, std::vector<Job>> ReadJobs(const std::string &file,
                                          const Constraints &lim) {
	std::ifstream testf(file);
	IAssert(testf.good(), 0, "Error reading file " + file + ".");
	testf.unsetf(std::ios_base::skipws);
	testf.exceptions(std::ios_base::failbit);

	int num_line = 1;
#ifndef NO_EXCEPTIONS
	try {
#endif
		auto getnewline = [&]() {
			IAssert(testf.get() == '\n', num_line, "Newline expected.");
			++num_line;
		};
		auto getspace = [&]() {
			IAssert(testf.get() == ' ', num_line, "Space expected.");
		};
		auto getint = [&]() {
			std::string buf;
			testf >> std::setw(kLineMax) >> buf;
			assert(!buf.empty() && "Internal error: String should not be empty.");
			IAssert(buf[0] != '0' || buf.size() == 1, num_line,
			        "Weight may not contain leading zeros.");
			std::size_t pos{};
			auto ret = std::stoi(buf, &pos);
			IAssert(pos == buf.size(), num_line, "Integer format incorrect.");
			return ret;
		};

		int l = getint();
		IAssert(lim.slices >= l && l >= 1, num_line,
		        "Number of available slices out of range.");
		getnewline();

		int n = getint();
		IAssert(lim.jobs >= n && n >= 1, num_line, "Number of jobs out of range.");
		getnewline();

		int tot_m = 0;
		std::vector<Job> jobs(n);
		for (auto &job : jobs) {
			int job_line = num_line;

			int m = getint();
			// Compare with lim.ops *before* the addition to make sure there is no
			// overflow
			IAssert(lim.ops >= m && m >= 1, num_line,
			        "Number of operations out of range.");
			tot_m += m;
			IAssert(tot_m <= lim.ops, num_line,
			        "Total number of operations out of range.");
			getnewline();

			job.ops.resize(m);

			auto read_weight = [&]() {
				const std::regex weight_regex(R"X(([1-9]\d{0,6}|0)(\.\d{1,6})?)X");
				std::string weight_str;
				testf >> std::setw(kLineMax) >> weight_str;
				IAssert(std::regex_match(weight_str, weight_regex) && weight_str != ".",
				        num_line, "Job weight format malformed.");
				std::size_t pos{};
				const double w = stod(weight_str, &pos);
				IAssert(std::isfinite(w) && pos == weight_str.size(), num_line,
				        "Job weight is not a number.");
				IAssert(lim.weight >= w && w >= 0, num_line,
				        "Job weight out of range.");
				return w;
			};
			job.weight = read_weight();
			getnewline();

			for (auto &op : job.ops) {
				op.slices = getint();
				IAssert(l >= op.slices && op.slices >= 1, num_line,
				        "Number of slices for operation out of range.");
				getspace();

				op.duration = getint();
				IAssert(lim.duration >= op.duration && op.duration >= 1, num_line,
				        "Operation duration out of range.");
				getspace();

				int p = getint();
				IAssert(m >= p && p >= 0, num_line,
				        "Number of dependencies out of range.");
				op.deps.resize(p);

				std::vector<bool> used_deps(m);
				for (auto &dep : op.deps) {
					getspace();
					dep = getint();
					IAssert(m >= dep && dep >= 1, num_line,
					        "Dependency identifier out of range.");
					--dep;  // Convert to 0-based
					IAssert(!used_deps[dep], num_line,
					        "Dependencies of an operation contain duplicate entries.");
					used_deps[dep] = true;
				}

				getnewline();
			}

			IAssert(!CheckCycle(job), job_line, "Cyclic dependency detected.");
		}

		// Disable exception to check for EOF
		testf.exceptions(std::ios_base::goodbit);
		IAssert(testf.get() == -1, num_line, "End of file expected.");

		return {l, jobs};
#ifndef NO_EXCEPTIONS
	} catch (const std::exception &fail) {
		IAssert(false, num_line,
		        "The following error was encountered while reading a token. Please "
		        "check your input format.\n" +
		            std::string(fail.what()));
	}
	assert(false && "Internal error: Should not be reached!");
#endif
}

std::pair<std::vector<Event>, long double> ReadSchedule(
    const std::string &outfile, const int l, const std::vector<Job> &jobs) {
	int n = static_cast<int>(jobs.size());
	std::ifstream outf(outfile);
	OAssert(outf.good(), 0, "Error reading file " + outfile + ".");
	outf.exceptions(std::ios_base::failbit);
	// Note that unlike the input file, we are more lax and whitespace-insensitive
	// here

	int num_line = 0;
#ifndef NO_EXCEPTIONS
	try {
#endif
		// Populate event queue and calculate metrics
		std::vector<Event> events;
		long double makespan = 0;
		// Use Shewchuk's Algorithm for better precision
		shewchuk::ScPartials<long double> weighted_flow(0.0L);
		for (int j = 0; j < n; j++) {
			long double job_end = 0;
			for (int k = 0; k < static_cast<int>(jobs[j].ops.size()); k++) {
				// Read line-by-line so the error messages are more meaningful
				// Read to array instead of to string so that we can limit the size
				std::array<char, kLineMax> line_arr{};
				outf.getline(line_arr.data(), line_arr.size());
				++num_line;
				std::string line(line_arr.data());
				std::stringstream ss(line);

				int start{};
				OAssert(static_cast<bool>(ss >> start), num_line,
				        "Start time expected.");
				// Also make sure that the duration is not unreasonably large so that it
				// does not overflow later
				OAssert(1'000'000'000 >= start && start >= 0, num_line,
				        "Start time out of range.");

				int fin = start + jobs[j].ops[k].duration;

				std::basic_string<uint8_t> slices(jobs[j].ops[k].slices, 0);
				for (auto &s : slices) {
					int x{};
					OAssert(static_cast<bool>(ss >> x), num_line,
					        "Insufficient number of slices in line.");
					OAssert(1 <= x && x <= l, num_line, "Slice number out of range.");
					s = static_cast<uint8_t>(x - 1);  // Convert to 0-based
				}
				// Consume the remaining whitespace and check that there are no
				// additional tokens
				ss >> std::ws;
				OAssert(ss.eof(), num_line, "Too many slices in line.");

				events.push_back({start, true, j, k, slices, num_line});
				events.push_back({fin, false, j, k, slices, num_line});

				makespan = std::max<long double>(makespan, fin);
				job_end = std::max<long double>(job_end, fin);
			}
			weighted_flow += static_cast<long double>(jobs[j].weight) * job_end;
		}

		std::sort(events.begin(), events.end());
		weighted_flow += makespan;
		return {events, weighted_flow};
#ifndef NO_EXCEPTIONS
	} catch (const std::exception &fail) {
		OAssert(
		    false, num_line,
		    "The following error was encountered while reading a token. This may "
		    "be caused by an insufficient number of lines. Please check your "
		    "output format.\n" +
		        std::string(fail.what()));
	}
	assert(false && "Internal error: Should not be reached!");
#endif
}

long double CalculateScore(const std::string &outfile, const int l,
                           std::vector<Job> &jobs) {
	assert(l < 256 && "Internal error: Number of slices should fit in uint8_t.");

	const auto [events, score] = ReadSchedule(outfile, l, jobs);

	// Verify that the events are consistent
	std::vector<bool> is_used(l);
	for (const auto &event : events) {
		if (event.is_start) {
			for (const auto d : jobs[event.job].ops[event.op].deps) {
				OAssert(jobs[event.job].ops[d].done, event.num_line,
				        "Dependency not finished.");
			}

			for (const auto i : event.slices) {
				OAssert(!is_used[i], event.num_line,
				        "Overlapping operations detected.");
				is_used[i] = true;
			}
		} else {
			jobs[event.job].ops[event.op].done = true;
			for (const auto i : event.slices) {
				assert(is_used[i] &&
				       "Internal error: The slice should not have been unused.");
				is_used[i] = false;
			}
		}
	}

	return score;
}

[[noreturn]] void Usage() {
	std::cerr << "Usage: checker [--public] [TESTCASE] [OUTPUT_FILE]\n";
	std::cerr << "Options:\n";
	std::cerr << "\t--public: Whether to enforce limits for public testcases.\n";
	std::cerr << "\t\tEnforces (stricter) limits for private testcases if not "
	             "specified.\n";
	std::cerr << "Examples:\n";
	std::cerr << "\tCheck if 00.in is valid: checker 00.in\n";
	std::cerr << "\tCalculate score for 00.out: checker 00.in 00.out\n";
	exit(2);
}

}  // namespace

int main(int argc, char **argv) {
	static_assert(std::numeric_limits<int>::max() >= (1LL << 31) - 1,
	              "Int size is not large enough.");

#ifdef FE_NOMASK_ENV
	// Trap on floating point particularities (performance hit should be
	// neligible)
	// Only works on glibc
	// This should not happen, and it should be
	// considered a bug if the program traps because of this.
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif

	// Argument parsing
	const auto [testcase, output, is_public, check_output] =
	    [&]() -> std::tuple<std::string, std::string, bool, bool> {
#if defined(ONLINE_JUDGE_TESTDATA)
		// Testlib-compatible argument handling for running on the online judge
		// Version for checking user-submitted testcases
		assert(argc == 4 && "Incorrect number of arguments.");
		return {argv[1], "", true, false};
#elif defined(ONLINE_JUDGE)
		// Testlib-compatible argument handling for running on the online judge
		assert(argc == 4 && "Incorrect number of arguments.");
		return {argv[1], argv[3], true, true};
#else
		std::string testcase, output;
		int positional = 0;
		bool is_public = false, check_output = false;
		for (int i = 1; i < argc; i++) {
			if (positional == 0 && strcmp(argv[i], "--public") == 0) {
				is_public = true;
			} else if (positional == 0) {
				testcase = argv[i];
				++positional;
			} else if (positional == 1) {
				output = argv[i];
				check_output = true;
				++positional;
			}
		}
		if (positional < 1) Usage();
		return {testcase, output, is_public, check_output};
#endif
	}();

	const auto lim = [](const bool is_public) {
		Constraints ret{};
		if (is_public) {
			ret.slices = 72;
			ret.jobs = 128;
			ret.ops = 360;
			ret.duration = 10000;
			ret.weight = 64;
		} else {
			ret.slices = 8;
			ret.jobs = 30;
			ret.ops = 100;
			ret.duration = 96;
			ret.weight = 64;
		}
		return ret;
	}(is_public);

	auto [l, jobs] = ReadJobs(testcase, lim);

	if (!check_output) return 0;

	const auto score = CalculateScore(output, l, jobs);

#ifdef ONLINE_JUDGE
	std::cout << std::fixed << std::setprecision(8) << -score << '\n';
#else
	std::cout << std::fixed << std::setprecision(8) << score << '\n';
#endif
}
