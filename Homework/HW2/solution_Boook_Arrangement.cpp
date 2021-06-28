#include <bits/stdc++.h>
using namespace std;

vector<tuple<int, int, int>> plan;

int64_t solve(const vector<int> &cnt, int i = 0, int rem = 0, int64_t acc = 0) {
	const auto n = static_cast<int>(cnt.size());
	const int id = (i + 1 == n - 1 && i - 1 > 0) ? i - 1 : i + 1;
	if (i == n - 2 || (i == n - 3 && rem >= cnt[i + 1] + 1)) {
		if (rem) plan.emplace_back(i, id, rem * 2 - 1);
		return acc + max(0, rem - cnt[i + 1] - 1);
	}
	if (rem) plan.emplace_back(i, id, rem * 2);
	if (rem <= cnt[i + 1]) return solve(cnt, i + 1, cnt[i + 1] - rem, acc);
	return solve(cnt, i + 1, 0, acc + (rem - cnt[i + 1]));
}

int main() {
	cin.tie(nullptr), ios_base::sync_with_stdio(false);
	int n; cin >> n;
	vector<int> cnt(n + 1);
	for (int i = 1; i <= n; i++) cin >> cnt[i];
	while (cnt.size() > 1 && cnt.back() == 0) cnt.pop_back();
	cnt.push_back(0);
	bool flag; cin >> flag;
	auto ans = solve(cnt);
	reverse(plan.begin(), plan.end());
	vector<int> solution;
	int last = -1;
	for (auto [a, b, c] : plan) {
		if (last != -1) {
			while (abs(a - last) != 1 && abs(b - last) != 1) {
				--last;
				++ans;
				if (flag) solution.push_back(last);
			}
		}
		int rev = c % 2 == 0 && abs(a - last) != 1;
		if (flag) {
			for (int i = 0; i < c; i++) solution.push_back(i % 2 == rev ? a : b);
		}
		last = (c - 1) % 2 == rev ? a : b;
	}
	cout << ans << '\n';
	if (flag) {
		for (int x : solution) cout << x << ' ';
		cout << '\n';
	}
	return 0;
} 
