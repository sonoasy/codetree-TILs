#include <iostream>
#include<vector>
#include<queue> 
#include<map>
#include<set> 

using namespace std;
typedef pair<int,int>ci;
bool operator < (const  ci a, const ci b) { //id, 비용 
		if (a.second == b.second)return a.first<b.first;
		return a.second>b.second;
} 
vector<ci>graph[2001];
//dest -> {id.cost} set 목적지에 대한 상품,수익 
map<int,set<ci>>info; 
//id -> (dest,cost) 삭제하는거 때문 
map<int,ci>deletes;

int start=0;
vector<int>dist; 
int maxid;
int mins=1e9; 
//queue struct 잘 모르겟.. 
void dikjstra(){

   dist[start]=0;
   priority_queue<ci,vector<ci>,greater<>>pq; 
   pq.push({0,start}); 

   while(!pq.empty()){
     int weight=pq.top().first;
     int cur=pq.top().second;
     pq.pop(); 

     if(dist[cur]<weight)continue; 
     for(int i=0;i<graph[cur].size();i++){
       int nr=graph[cur][i].first;
       int nw=weight+graph[cur][i].second; 
       if(nw<dist[nr]){
         dist[nr]=nw;
         pq.push({nw,nr}); 
       }    
     }
   }

}

int main() {
  

    int q;//명령어 수 
    cin>>q;
    for(int i=0;i<q;i++){
       int num;
       cin>>num;
       if(num==100){  //코드트리 랜드 건설 - 1번만 
          int n,m,v,u,w; 
          cin>>n>>m; 
          //각 정점까지 최단거리 
          dist.assign(n+1,1e9);

          for(int j=0;j<m;j++){ //간선의 갯수 
             cin>>v>>u>>w;
             graph[v].push_back({u,w});
             graph[u].push_back({v,w});
          }
          dikjstra(); 

       }
       else if(num==200){ //여행 상품 생성  3만  -> 0(1)
        int id,revenue,dest;
        cin>>id>>revenue>>dest; 
        info[dest].insert({id,revenue-dist[dest]});
        deletes[id]={dest,revenue-dist[dest]}; 

       }
       else if(num==300){ //여행 상품 취소  3만  -> o(1)
          int id; 
          cin>>id;
          info[deletes[id].first].erase({id,deletes[id].second});
       }
       else if(num==400){//최적의 여행 상품 판매  3만 
          //id별로 revenue-cost[dest](최단거리)  
          if(mins==1e9)cout<<-1<<'\n';
          else cout<<maxid<<'\n';
          //최적의 여행상품 출력, 없으면(가는 거리가 없거나 비용<0) -1 
          //모든 dest에 대하여 - 2000개 

          

       }
       else{//500  //출발지 변경  
         int s;
         cin>>s;
         start=s;
         dikjstra();
         //그러면 최적의 상품 어떻게 다시 구함? 그냥 여기서 다시? 
          

       }
    }




    return 0;
}