// AI 辅助参考草稿：请独立理解 DFS、A*、边界与代价，再根据实际测试决定是否采用。
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;

int main() {
    int R, C;
    if (!(cin >> R >> C)) return 1;
    vector<string> grid(R);
    pair<int,int> start{-1,-1}, goal{-1,-1};
    for (int r=0; r<R; ++r) {
        cin >> grid[r];
        for (int c=0; c<C; ++c) {
            if (grid[r][c]=='S') start={r,c};
            if (grid[r][c]=='G') goal={r,c};
        }
    }
    const int dr[4]={-1,0,1,0}, dc[4]={0,1,0,-1};
    auto passable=[&](int r,int c) {
        return r>=0 && r<R && c>=0 && c<C && grid[r][c]!='#';
    };
    vector<vector<bool>> seen(R,vector<bool>(C,false));
    auto dfs=[&](int sr,int sc) {
        int count=0;
        vector<pair<int,int>> stack{{sr,sc}};
        seen[sr][sc]=true;
        while (!stack.empty()) {
            auto [r,c]=stack.back(); stack.pop_back(); ++count;
            for (int d=0;d<4;++d) {
                int nr=r+dr[d],nc=c+dc[d];
                if (passable(nr,nc) && !seen[nr][nc]) {
                    seen[nr][nc]=true;
                    stack.push_back({nr,nc});
                }
            }
        }
        return count;
    };
    int reachable=dfs(start.first,start.second);
    int regions=1;
    for (int r=0;r<R;++r) for (int c=0;c<C;++c) {
        if (passable(r,c) && !seen[r][c]) { dfs(r,c); ++regions; }
    }
    int cost=-1;
    if (seen[goal.first][goal.second]) {
        const int INF=numeric_limits<int>::max()/4;
        vector<vector<int>> best(R,vector<int>(C,INF));
        // f = g + Manhattan heuristic; queue sorts by smallest f.
        priority_queue<tuple<int,int,int,int>, vector<tuple<int,int,int,int>>,
                       greater<tuple<int,int,int,int>>> pq;
        auto h=[&](int r,int c) {return abs(r-goal.first)+abs(c-goal.second);};
        best[start.first][start.second]=0;
        pq.emplace(h(start.first,start.second),0,start.first,start.second);
        while (!pq.empty()) {
            auto [f,g,r,c]=pq.top(); pq.pop();
            if (g!=best[r][c]) continue;
            if (make_pair(r,c)==goal) {cost=g;break;}
            for (int d=0;d<4;++d) {
                int nr=r+dr[d],nc=c+dc[d];
                if (!passable(nr,nc)) continue;
                char cell=grid[nr][nc];
                int step=(cell>='2' && cell<='9')?cell-'0':1;
                int ng=g+step;
                if (ng<best[nr][nc]) {
                    best[nr][nc]=ng;
                    pq.emplace(ng+h(nr,nc),ng,nr,nc);
                }
            }
        }
    }
    cout << "reachable_cells: " << reachable << '\n';
    cout << "regions: " << regions << '\n';
    cout << "cost: " << cost << '\n';
}
