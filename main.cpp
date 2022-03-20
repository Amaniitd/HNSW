#include<bits/stdc++.h>
using namespace std;

float cos_dist(vector<int> v1, vector<int> v2){
    float s = 0;
    int l = v1.size();
    float m1 = 0;
    float m2 = 0;

    for (int i=0;i<l;i++){
        s += v2.get(i)*v1.get(i);
        m1 += v1.get(i)*v1.get(i);
        m2 += v2.get(i)*v2.get(i);
    }

    float m11 = math.sqrt(m1);
    float m22 = math.sqrt(m2);

    float m33 = m11*m22;

    float res = s/m33;
    return 1-res;
}

priority_queue<pair<int,int>> SearchLayer(vector<float> q,int topk,priority_queue<pair<int,int>> candidates,vector<int> indptr,vector<int> index,vector<int> level_offset,int lc,int visted[],vector<vector<float>> vect){
    priority_queue<pair<int,int>> top_k;
    priority_queue<pair<int,int>> candidates1;

    while (!candidates.empty()){
        top_k.push(candidates.top());
        candidates1.push(candidates.top());
        candidates.pop();
    }

    while(!candidates1.empty()){
        candidates.push(candidates1.top());
        candidates1.pop();
    }

    while(candidates.size()>0){
        int ep = candidates.pop().first();
        int start = indptr.get(ep) + level_offset.get(lc);
        int end = indptr.get(ep) + level_offset,get(lc+1);

        for (int i=start;i<=end;i++){
            int px = index.get(i);
            if (visited[px] == 1 or px == -1){
                continue;
            }
            visited[px] = 1;
            float _dist = cos_dist(q,vect[px]);
            if (_dist > top_k.top().second() && top_k.size() > topk){
                continue;
            }
            top_k.push({px,_dist});
            if (top_k.size() == topk+1){
                top_k.pop();
            }
            candidates.push({px,_dist});
        }
    }
    return top_k;
}


priority_queue<pair<int,int>> QueryHNSW(vector<float> q,int topk,int ep,vector<int> indptr,vector<int> index,vector<int> level_offset,int max_level,vector<vector<float>> vect){
    priority_queue<pair<int,int>> top_k;
    top_k.push(<ep,cos_dist(q,vect.get(ep)));
    int visited[vect.size()] = {0};
    visited[ep] = 1;
    int L = max_level;

    for (int i = L;i>=0;i--){
        top_k = SearchLayer(q,topk,indptr,index,level_offset,i,visited,vect);

    }
    return top_k;
}