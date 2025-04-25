#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    string s;
    cin >> N >> s;

    // build undirected adjacency of unique adjacent pairs
    bool present[26] = {};
    bool adj[26][26] = {};
    for(int i = 0; i < N; i++)
        present[s[i]-'a'] = true;
    for(int i = 0; i + 1 < N; i++){
        int u = s[i]-'a', v = s[i+1]-'a';
        if(u != v) adj[u][v] = adj[v][u] = true;
    }

    // collect all present nodes and check connectivity
    vector<int> nodes;
    for(int i = 0; i < 26; i++) if(present[i]) nodes.push_back(i);
    int start_cc = nodes[0];
    vector<bool> seen(26,false);
    queue<int> q; q.push(start_cc); seen[start_cc]=true;
    while(!q.empty()){
        int u=q.front(); q.pop();
        for(int v=0;v<26;v++) if(adj[u][v]&&!seen[v]){
            seen[v]=true; q.push(v);
        }
    }
    for(int u:nodes)
        if(!seen[u]){
            // disconnected: but s is one string so shouldn't happen
            cout << "-1\n";
            return 0;
        }

    // list odd–degree vertices in H
    vector<int> odd;
    for(int u=0;u<26;u++){
        if(present[u]){
            int d=0;
            for(int v=0;v<26;v++) if(adj[u][v]) d++;
            if(d%2) odd.push_back(u);
        }
    }

    // compute all-pairs shortest paths on H (unit weights)
    static int dist[26][26];
    for(int i=0;i<26;i++){
        for(int j=0;j<26;j++) dist[i][j]=INF;
        if(present[i]){
            dist[i][i]=0;
            queue<int> qq; qq.push(i);
            while(!qq.empty()){
                int u=qq.front(); qq.pop();
                for(int v=0;v<26;v++) if(adj[u][v] && dist[i][v]==INF){
                    dist[i][v]=dist[i][u]+1;
                    qq.push(v);
                }
            }
        }
    }

    int E = 0;
    for(int u=0;u<26;u++) for(int v=u+1;v<26;v++) if(adj[u][v]) E++;

    int extra = 0;
    int free_u=-1, free_v=-1;
    vector<pair<int,int>> to_dup;

    int k = odd.size();
    if(k > 2){
        // DP on the odd nodes to leave exactly two unpaired
        vector<int> idx(26,-1);
        for(int i=0;i<k;i++) idx[odd[i]] = i;
        static int pd[26][26];
        for(int i=0;i<k;i++) for(int j=0;j<k;j++)
            pd[i][j] = dist[ odd[i] ][ odd[j] ];

        int FULL = (1<<k) - 1;
        int LIM = FULL + 1;
        vector<int> dp(LIM, INF);
        dp[0] = 0;
        for(int mask=1; mask<LIM; mask++){
            if(__builtin_popcount(mask) & 1) continue;
            int i = __builtin_ctz(mask);
            int m2 = mask ^ (1<<i);
            for(int j = i+1; j<k; j++){
                if(m2 & (1<<j)){
                    int m3 = m2 ^ (1<<j);
                    dp[mask] = min(dp[mask], dp[m3] + pd[i][j]);
                }
            }
        }

        // pick two to leave free so that dp[FULL^(1<<i)^(1<<j)] minimal
        extra = INF;
        for(int i=0;i<k;i++){
            for(int j=i+1;j<k;j++){
                int m = FULL ^ (1<<i) ^ (1<<j);
                if(dp[m] < extra){
                    extra = dp[m];
                    free_u = odd[i];
                    free_v = odd[j];
                }
            }
        }

        // recover which pairs we duplicated
        int targetMask = FULL ^ (1<<idx[free_u]) ^ (1<<idx[free_v]);
        function<void(int)> recover = [&](int mask){
            if(mask == 0) return;
            int i = __builtin_ctz(mask);
            int m2 = mask ^ (1<<i);
            for(int j=i+1;j<k;j++){
                if(m2 & (1<<j)){
                    int m3 = m2 ^ (1<<j);
                    if(dp[mask] == dp[m3] + pd[i][j]){
                        // pair odd[i] with odd[j]
                        int u = odd[i], v = odd[j];
                        // BFS to get the actual shortest path between u and v
                        vector<int> par(26,-1);
                        queue<int> qq; qq.push(u); par[u]=u;
                        while(!qq.empty()){
                            int x=qq.front(); qq.pop();
                            if(x==v) break;
                            for(int y=0;y<26;y++){
                                if(adj[x][y] && par[y]==-1){
                                    par[y]=x; qq.push(y);
                                }
                            }
                        }
                        int cur=v;
                        while(cur!=u){
                            to_dup.emplace_back(cur, par[cur]);
                            cur = par[cur];
                        }
                        recover(m3);
                        return;
                    }
                }
            }
        };
        recover(targetMask);
    }
    else if(k==2){
        free_u = odd[0];
        free_v = odd[1];
    }

    int K = E + extra + 1;

    // build multigraph H' with duplications
    map<pair<int,int>,int> ec;
    for(int u=0;u<26;u++) for(int v=u+1;v<26;v++)
        if(adj[u][v]) ec[{u,v}]++;

    for(auto &pr: to_dup){
        int u = pr.first, v = pr.second;
        if(u>v) swap(u,v);
        ec[{u,v}]++;
    }

    vector<pair<int,int>> edges;
    for(auto &it: ec){
        int u = it.first.first;
        int v = it.first.second;
        int c = it.second;
        while(c--){
            edges.emplace_back(u,v);
        }
    }

    int M = edges.size();
    vector<vector<pair<int,int>>> G(26);
    vector<char> usedEdge(M, 0);
    for(int i=0;i<M;i++){
        auto [u,v] = edges[i];
        G[u].push_back({v,i});
        G[v].push_back({u,i});
    }

    int start = start_cc;
    if(k>0){
        if(k<=2){
            // if two odd, start there
            if(k==2) start = odd[0];
        } else {
            start = free_u;
        }
    }

    // Hierholzer for undirected Eulerian trail
    vector<int> itp(26,0), st, path;
    st.push_back(start);
    while(!st.empty()){
        int u = st.back();
        while(itp[u] < (int)G[u].size() && usedEdge[G[u][itp[u]].second]) itp[u]++;
        if(itp[u] == (int)G[u].size()){
            path.push_back(u);
            st.pop_back();
        } else {
            auto [v,id] = G[u][itp[u]++];
            usedEdge[id] = 1;
            st.push_back(v);
        }
    }
    reverse(path.begin(), path.end());

    cout << path.size() << "\n";
    for(int u: path) cout << char('a'+u);
    cout << "\n";

    return 0;
}
