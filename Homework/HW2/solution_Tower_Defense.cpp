#include <bits/stdc++.h>
using namespace std;

long long s[100005], dp[100005]; // s_i of each cell, and the dp value

// dp_i is the maximum total defense level when tower i is the RIGHT MOST tower we choose (we MUST choose tower i)
// for cell i, we want to find the maximum among all A * max(0, B - (i - j)) + dp_j, for j < i.
// Since max(max(a1, b1), max(a2, b2), ..., max(an, bn)) = max(max(a1, a2, ..., an), max(b1, b2, ..., bn)),
// we can find the maximum value with and without bonus separately.
// The maximum value without bonus is just the prefix max of dp value.
// The maximum value with bonus can be maintained by subtracting A every time we move to the next cell.

int main() {
    long long n, k, A, B, ans = 0; // ans stores the answer
    cin >> n >> k >> A >> B;
    for (int i = 1; i <= n; i++)
        cin >> s[i], ans = max(ans, dp[i] = s[i]); // If we only choose one tower, then dp_i equals to s_i
    for (int t = 2; t <= k; t++) { // Building towers one by one
        long long mx = -2e18, mx_with_bonus = -2e18; // mx stores the prefix max, mx_with_bonus stores the prefix max that includes bonus
        for (int i = 1; i <= n; i++) {
            long long mem = dp[i]; // Backup the old dp value
            dp[i] = max(mx, mx_with_bonus) + s[i]; // The new dp value is just the maximum between two prefix max plus s_i
            ans = max(ans, dp[i]); // Update answer
            mx = max(mx, mem); // Update prefix max without bonus
            mx_with_bonus = max(mx_with_bonus, mem + A * B) - A; // Update prefix max with bonus
        }
    }
    cout << ans << endl; // Output the answer
}
