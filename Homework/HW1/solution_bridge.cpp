#include <bits/stdc++.h>
using namespace std;
#define int long long
#define F first
#define S second
#define mid (l + r >> 1)
#define maxn 200005
 
int n;
pair<int, int> x[maxn], s[maxn];
 
long long operator ^ (pair<int, int> v1, pair<int, int> v2) {
    long long x = 1ll * (v1.F - v2.F) * (v1.F - v2.F);
    long long y = 1ll * (v1.S - v2.S) * (v1.S - v2.S);
    return x + y;
}
 
long long solve(int l, int r) {
    if (l == r) return s[l] = x[l], 3e18;
    int p1 = l, p2 = mid + 1, xmid = x[mid].F, pos = l;
    long long ans = min(solve(l, mid + 0), solve(mid + 1, r));
    for (int i = l; i <= r; ++ i) {
        if (p1 <= mid + 0 && p2 <= r) {
            if (make_pair(x[p1].S, x[p1].F) < make_pair(x[p2].S, x[p2].F))
                 s[i] = x[p1 ++];
            else s[i] = x[p2 ++];
        } 
        else s[i] = p1 <= mid + 0 ? x[p1 ++] : x[p2 ++];
    }
    for (int i = l; i <= r; ++ i)
        x[i] = s[i];
    for (int i = l; i <= r; ++ i) {
        if (1ll * (xmid - s[i].F) * (xmid - s[i].F) <= ans)
            s[pos ++] = s[i];
    }
    for (int i = l; i < pos; ++ i)
        for (int j = 1; j <= 3; ++ j)
            if (i + j < pos)
                ans = min(ans, s[i] ^ s[i + j]);
    return ans;
}
 
int32_t main() {
    cin.tie(0), cout.sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; ++ i)
        cin >> x[i].F >> x[i].S;
    sort(x + 1, x + 1 + n);
    cout << -solve(1, n) << "\n";
    return 0;
}