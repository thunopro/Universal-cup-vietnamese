#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while(T--){
        int N;
        string S;
        cin >> N >> S;

        int nA = 0, nB = 0, nC = 0;
        for(char c: S){
            if(c=='A') ++nA;
            else if(c=='B') ++nB;
            else ++nC;
        }

        // 1. Uniform string → no ops
        if(nA==N || nB==N || nC==N){
            cout << S << "\n";
            continue;
        }

        int sumBC = nB + nC;
        // 2. Even case
        if(sumBC % 2 == 0){
            // parity invariant of (nA + nB)
            int p = (nA + nB) & 1;
            cout << (p ? "A\n" : "AA\n");
            continue;
        }

        // 3. Odd case
        int d0 = min(nB, nC);
        int a_max = nA + d0;
        char X = (nB > nC ? 'B' : 'C');
        char Y = (X=='B' ? 'C' : 'B');

        vector<string> cand;
        cand.reserve(3);
        cand.push_back(S);  // zero-ops

        // s1 = A^a_max + X
        string s1(a_max, 'A');
        s1.push_back(X);
        if(s1 == S || d0 > 0)
            cand.push_back(s1);

        // s2 = A^(a_max-1) + Y
        if(a_max > 0){
            string s2(a_max-1, 'A');
            s2.push_back(Y);
            // reachable as long as we have at least one A to combine (initial or from BC)
            if(nA > 0 || d0 > 0)
                cand.push_back(s2);
        }

        // take lexicographically minimal
        cout << *min_element(cand.begin(), cand.end()) << "\n";
    }
    return 0;
}
