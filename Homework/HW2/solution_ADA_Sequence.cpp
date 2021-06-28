#include <bits/stdc++.h>
using namespace std;

#define mp make_pair
typedef pair<int, int> pii;
typedef pair<pii, int> piii;
const int N = 206;
const int SS = 3 * N;
const int INF = 0x3f3f3f3f;
const piii _zero = mp(mp(-1, -1), 0);

#define F first
#define S second
#define FF F.F
#define FS F.S

int dp[N][N][SS];
// dp[L][R][ans] = minimum cost such that a[L]...a[R] has an ada sequence with score ans

int a[N];
int c[N];
int s[N];
int n, k, d;

piii par[N][N][SS]; // par[i][j][k] is transition point of dp[i][j][k]
int vis[N][N][SS];

void U(int &x, int dp_val, piii pos, piii _par) {
    if (x > dp_val) {
        x = dp_val;
        par[pos.FF][pos.FS][pos.S] = _par;
    }
}

int DP(int l, int r, int ans) {
    if (ans < 0) return INF;
    if (vis[l][r][ans]) return dp[l][r][ans];
    vis[l][r][ans] = 1;
    piii pos = mp(mp(l, r), ans);
    if (l == r) {
        // base case
        if (s[l] == ans) {
            U(dp[l][r][ans], 0, pos, _zero);
        }
        else {
            U(dp[l][r][ans], INF, pos, _zero);
        }
        return dp[l][r][ans];
    }
    // not match
    U(dp[l][r][ans], DP(l + 1, r, ans), pos, mp(mp(l + 1, r), ans));
    U(dp[l][r][ans], DP(l, r - 1, ans), pos, mp(mp(l, r - 1), ans));
    // match
    if (l + 1 == r) {
        // length = 2, base case
        if (s[l] + s[r] == ans) {
            if (abs(a[l] - a[r]) <= d) U(dp[l][r][ans], 0, pos, _zero);
            else U(dp[l][r][ans], min(c[l], c[r]), pos, _zero);
        }
        else {
            U(dp[l][r][ans], INF, pos, _zero);
        }
    }
    else {
        if (abs(a[l] - a[r]) <= d) U(dp[l][r][ans], DP(l + 1, r - 1, ans - s[l] - s[r]), pos, mp(mp(l + 1, r - 1), ans - s[l] - s[r]));
        else {
            U(dp[l][r][ans], DP(l + 1, r - 1, ans - s[l] - s[r]) + min(c[l], c[r]), pos, mp(mp(l + 1, r - 1), ans - s[l] - s[r]));
        }
    }
    return dp[l][r][ans];
}

int main () {
    memset(dp, INF, sizeof(dp));
    scanf("%d%d%d", &n, &k, &d);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &a[i]);
    }
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &c[i]);
    }
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &s[i]);
    }
    int ans = 0;
    for (int i = 0; i < SS; ++i) {
        if (DP(1, n, i) <= k) {
            ans = i;
        }
    }
    printf("%d\n", ans);
    piii now = mp(mp(1, n), ans);
    vector<pii> ops;
    vector<int> b;
    while (now != _zero) { // trace answer
        piii _par = par[now.FF][now.FS][now.S];
        if (now.S != _par.S) {
            int l = now.FF;
            int r = now.FS;
            b.push_back(l);
            b.push_back(r);
            if (abs(a[l] - a[r]) > d) {
                if (c[l] == min(c[l], c[r])) {
                    ops.push_back(mp(l, a[r]));
                }
                else {
                    ops.push_back(mp(r, a[l]));
                }
            }
        }
        now = _par;
    }
    sort(b.begin(), b.end());
    b.resize(unique(b.begin(), b.end()) - b.begin());
    int x = (int)ops.size();
    printf("%d\n", x);
    for (pii p: ops) {
        printf("%d %d\n", p.F, p.S);
    }
    int m = (int)b.size();
    printf("%d\n", m);
    for (int i = 0; i < m; ++i) {
        printf("%d%c", b[i], " \n"[i == m - 1]);
    }
}