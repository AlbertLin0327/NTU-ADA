#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int kN = 505;
const ll kInf = 1e16;
 
int n, m, k, c;
int a[kN][kN];
ll dp[11][kN][kN]; // record the dp values
tuple<int, int, int> pre[11][kN][kN]; // record the previous nodes
pair<int, int> mxpos[11];
int main() {
    scanf("%d%d%d%d", &n, &m, &k, &c);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) scanf("%d", &a[i][j]);
 
    dp[0][0][0] = a[0][0]; // init
 
    for(int l = 0; l <= k; l++) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                if(!i && !j && !l) continue;
                dp[l][i][j] = -kInf;
                auto get_val = [&](int l, int i, int j) {
                    return a[i][j] - 1LL * l * c;
                };
                ll val = get_val(l, i, j);
                if(i) {
                    if(dp[l][i][j] < dp[l][i - 1][j] + val) { // from (i - 1, j)
                        dp[l][i][j] = dp[l][i - 1][j] + val;
                        pre[l][i][j] = make_tuple(l, i - 1, j);
                    }
                }
                if(j) {
                    if(dp[l][i][j] < dp[l][i][j - 1] + val) { // from (i, j - 1)
                        dp[l][i][j] = dp[l][i][j - 1] + val;
                        pre[l][i][j] = make_tuple(l, i, j - 1);
                    }
                }
                if(l) {
                    auto [nx, ny] = mxpos[l - 1]; // from jump
                    if(dp[l][i][j] < dp[l - 1][nx][ny] + val) {
                        dp[l][i][j] = dp[l - 1][nx][ny] + val;
                        pre[l][i][j] = make_tuple(l - 1, nx, ny);
                    }
                }
            }
        }
 
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                if(dp[l][i][j] > dp[l][mxpos[l].first][mxpos[l].second]) mxpos[l] = {i, j}; // record the mxpos in l-th layer
            }
        }
    }
 
    int nl = 0;
    for(int l = 1; l <= k; l++)
        if(dp[l][n - 1][m - 1] > dp[nl][n - 1][m - 1]) nl = l; 
    printf("%lld\n", dp[nl][n - 1][m - 1]);
 
    vector<tuple<int, int, int>> ans;
    auto cur = make_tuple(nl, n - 1, m - 1);
    ans.push_back(cur);
    while(cur != make_tuple(0, 0, 0)) { // store the path
        int now = get<0>(cur);
        cur = pre[get<0>(cur)][get<1>(cur)][get<2>(cur)];
        if(now != get<0>(cur)) ans.emplace_back(-1, -1, -1);
        else ans.emplace_back(-2, -2, -2);
        ans.push_back(cur);
    }
    ans.pop_back();
    reverse(ans.begin(), ans.end());
    printf("%d\n", int(ans.size()) / 2);
    for(auto p : ans) {
        if(get<0>(p) == -1) printf("Jump ");
        else if(get<0>(p) == -2) printf("Move ");
        else printf("%d %d\n", get<1>(p), get<2>(p));
    }
    return 0;
}
 