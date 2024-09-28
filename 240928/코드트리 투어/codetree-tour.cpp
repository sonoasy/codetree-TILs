#include <iostream>
#include<vector>
#include<queue> 
#include<map>
#include<set> 
#include<tuple>

using namespace std;
typedef pair<int,int>ci;
//전체 
struct myOrder
{
	bool operator() (const ci& left, const ci& right) const
	{
		if (left.first == right.first) {
			return left.second < right.second;
		}
		
		return left.first > right.first;
		
	}
};
vector<ci>graph[2001];

map<int,ci>idinfo; 
set<ci,myOrder>total;

int start=0;
vector<int>dist; 
int maxid;
int mins=1e9; 
int n,m,v,u,w; 
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

//200,400일떄 안됨 

int main() {
  

    int q;//명령어 수 
    cin>>q;
    for(int i=0;i<q;i++){
       int num;
       cin>>num;
       if(num==100){  //코드트리 랜드 건설 - 1번만 
          
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
        idinfo[id]={revenue,dest};
       // cout<<"추가: "<<idinfo[id].first<<" "<<idinfo[id].second<<" "<<dist[idinfo[id].second]<<'\n';
       
        if((revenue-dist[dest])>=0){
         total.insert({revenue-dist[dest],id});
      //  cout<<"total에 추가 "<<id<<":"<<revenue-dist[dest]<<'\n';
        }

       }
       else if(num==300){ //여행 상품 취소  3만  -> o(1) ->ok 
          int id; 
          cin>>id;
          
          if(idinfo.find(id)==idinfo.end())continue; 
          //idinfo에서 삭제 
          
          //이거 이상함.... 
          total.erase({idinfo[id].first-dist[idinfo[id].second],id});
           idinfo.erase(id);
      //    cout<<idinfo[id].first<<" "<<dist[idinfo[id].second]<<","<<id<<"삭제\n";
       }
       else if(num==400){//최적의 여행 상품 판매  3만 ->ok 
          //id별로 revenue-cost[dest](최단거리)  
         
          //최적의 여행상품 출력, 없으면(가는 거리가 없거나 비용<0) -1 
          //모든 dest에 대하여 - 2000개 
          int selectedid=-1;

        //  cout<<"400이윤 목록\n";  
          //최대 n개만 돌면됨 
         if(total.size()==0){
            cout<<-1<<'\n'; continue; 
         }
         else{
            //  cout<<"맨 앞에 있는거:"<<total.begin()->first<<" "<<total.begin()->second;  
              selectedid=total.begin()->second; 
              cout<<selectedid<<'\n';
              total.erase(*(total.begin()));
              idinfo.erase(selectedid);
         }

       }
       else{//500  //출발지 변경  ->ok 
         int s;
         cin>>s;
         start=s;
         dist.assign(n+1,1e9);
         dikjstra();//dest 갱신 
      
         total.clear(); 
        // idmatch.clear();
        for(auto it=idinfo.begin();it!=idinfo.end();it++){
             int id=it->first;
             int revenue=it->second.first;
             int dest=it->second.second; 
             int cost=revenue-dist[dest];
             if(cost>=0){
               total.insert({cost,id});
             }  
        }

     }

   }

    return 0;
}