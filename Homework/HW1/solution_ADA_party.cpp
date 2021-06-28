#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
const int N = 500006;
int a[N], mods[N];
int cnt[N];
int n, k;
int mx[N], mn[N];
int pre[N];
ll ans = 0;
 
// if same value occur, right is max
 
void dc(int l, int r) {
    if (l == r) {
        return;
    }
    int mid = (l + r) >> 1;
    dc(l, mid);
    dc(mid + 1, r);
    // build max and min
    mx[mid] = mn[mid] = a[mid];
    for (int i = mid - 1; i >= l; --i) {
        mx[i] = max(mx[i + 1], a[i]);
        mn[i] = min(mn[i + 1], a[i]);
    }
    mx[mid + 1] = mn[mid + 1] = a[mid + 1];
    for (int i = mid + 2; i <= r; ++i) {
        mx[i] = max(mx[i - 1], a[i]);
        mn[i] = min(mn[i - 1], a[i]);
    }
    // max min on right
    int l_ptr = mid; //[l_ptr + 1, mid]
    for (int i = mid + 1; i <= r; ++i) {
        while (l_ptr != l - 1 && mn[i] < mn[l_ptr] && mx[l_ptr] <= mx[i]) {
            ++cnt[ pre[l_ptr - 1] ];
            --l_ptr;
        }
        if (i != mid + 1) {
            ans += cnt[ (pre[i] - mn[i] % k - mx[i] % k + 2 * k) % k ];
        }
    }
    for (int i = l; i <= mid; ++i) {
        cnt[ pre[i - 1] ] = 0; 
    }
    // max min on left
    int r_ptr = mid + 1; // [mid + 1, r_ptr - 1]
    for (int i = mid; i >= l; --i) {
        while (r_ptr != r + 1 && mn[i] <= mn[r_ptr] && mx[r_ptr] < mx[i]) {
            ++cnt[ pre[r_ptr] ];
            ++r_ptr;
        }
        if (i != mid) {
            ans += cnt[ (pre[i - 1] + mn[i] + mx[i]) % k ];
        }
    }
    for (int i = mid + 1; i <= r; ++i) {
        cnt[ pre[i] ] = 0;
    }
    // max right min left
    int mn_ptr, mx_ptr;
    mn_ptr = mid, mx_ptr = mid + 1;
    // [mx_ptr, r] & [mid + 1, mn_ptr]
    for (int i = mid; i >= l; --i) {
        // move mx_ptr
        while (mx_ptr != r + 1 && mx[i] > mx[mx_ptr]) {
            // max no longer satisfied
            if (mx_ptr <= mn_ptr) {
                --cnt[ (pre[mx_ptr] - mx[mx_ptr] % k + k) % k ];
            }
            ++mx_ptr;
        }
        // move mn_ptr
        while (mn_ptr != r && mn[mn_ptr + 1] >= mn[i]) {
            // min is satisfied
            ++mn_ptr;
            if (mx_ptr <= mn_ptr) {
                ++cnt[ (pre[mn_ptr] - mx[mn_ptr] % k + k) % k ];
            }
        }
        ans += cnt[ (pre[i - 1] + mn[i]) % k ];
    }
    for (int i = mid + 1; i <= r; ++i) {
        cnt[ (pre[i] - mx[i] % k + k) % k ] = 0;
    }
    // max left min right
    mn_ptr = mid, mx_ptr = mid;
    // [mn_ptr + 1, mid], [l, mx_ptr]
    for (int i = mid + 1; i <= r; ++i) {
        // move mx_ptr
        while (mx_ptr != l - 1 && mx[mx_ptr] <= mx[i]) {
            // max no longer satisfied
            if (mn_ptr + 1 <= mx_ptr) {
                --cnt[ (pre[mx_ptr - 1] + mx[mx_ptr]) % k ];
            }
            --mx_ptr;
        }
        // move mn_ptr
        while (mn_ptr != l - 1 && mn[mn_ptr] > mn[i]) {
            // min is satisfied
            if (mn_ptr <= mx_ptr) {
                ++cnt[ (pre[mn_ptr - 1] + mx[mn_ptr]) % k ];
            }
            --mn_ptr;
        }
        ans += cnt[ (pre[i] - mn[i] % k + k) % k ];
    }
    for (int i = l; i <= mid; ++i) {
        cnt[ (pre[i - 1] + mx[i]) % k ] = 0;
    }
    return;
}
 
void go() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &a[i]);
        mods[i] = a[i] % k;
        pre[i] = pre[i - 1] + mods[i];
        pre[i] %= k;
    }
    ans = 0;
    dc(1, n);
    printf("%lld\n", ans);
}
 
int main () {
    // int T; scanf("%d", &T);
    int T = 1;
    while (T--) {
        go();
    }
}
 