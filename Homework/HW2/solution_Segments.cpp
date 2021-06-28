#include <bits/stdc++.h>
using namespace std;
#define maxn 200009

struct seg {
    int l, r, id;
    long long w;
};

bool operator < (seg a, seg b) {
    return a.r < b.r;
}

bool CmpL(seg a, seg b) {
    return a.l < b.l;
} 

int n, m;
long long k, due[maxn], use[maxn];
seg x[maxn];

long long Judge(long long lim, long long ifr = false) {
    memset(due, 0, sizeof due);
    long long pos = 1, sum = 0, total_use = 0;
    priority_queue<seg> pq;
    for (int i = 0; i < m; ++ i) {
        while (pos <= n && x[pos].l <= i) pq.emplace(x[pos ++]);
        sum -= due[i];
        while (sum < lim) {
            if (pq.size() == 0 || pq.top().r <= i) return 0;
            seg now = pq.top(); pq.pop();
            long long sml = min(now.w, lim - sum);
            sum += sml;
            now.w -= sml;
            total_use += sml;
            due[now.r] += sml;
            if (ifr) use[now.id] += sml;
            if (now.w) pq.emplace(now);
        }
    }
    return total_use <= k;
}

int32_t main() {
    cin.tie(0), cout.sync_with_stdio(0);
    cin >> n >> m >> k;
    for (int i = 1; i <= n; ++ i) cin >> x[i].l >> x[i].r >> x[i].w;
    for (int i = 1; i <= n; ++ i) x[i].id = i;

    sort(x + 1, x + 1 + n, CmpL);

    long long ans = 0;
    for (int i = 62; i >= 0; -- i) {
        long long to = ans + (1ll << i);
        if (Judge(to)) ans = to;
    }

    Judge(ans, true);
    cout << ans << "\n";
    for (int i = 1; i <= n; ++ i) cout << use[i] << " \n"[i == n];
    return 0;
}