#include <iostream>
#include<vector>
#include<queue> 
#include<map>
#include<set> 
#include<tuple>

using namespace std;
typedef pair<int,int>ci;
//전체 
struct items{
  int id; //id
  int score; //비용  
};

struct myOrder
{
	bool operator() (const ci& left, const ci& right) const
	{
		if (left.second == right.second) {
			return left.first < right.first;
		}
		
		return left.second > right.second;
		
	}
};
vector<ci>graph[2001];
//dest -> {id.cost} set 목적지에 대한 상품,수익  -> 2000
map<int,set<ci,myOrder>>info; 
//id -> (dest,cost) 삭제하는거 때문  -> 3만 
map<int,ci>deletes;
map<int,int>idmatch; 
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
   //dist 
  // cout<<"dist 목록\n";
  // for(int i=0;i<n;i++){
    //  cout<<dist[i]<<' ';
  // }
   //cout<<'\n';
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
        idmatch[id]=dest;
      // cout<<"id: "<<id<<" revenue:"<<revenue-dist[dest]<<"추가\n";
        info[dest].insert({id,revenue-dist[dest]}); //같은 목적지에 여러개 아이디가 있을수 있으므로 
        deletes[id]={dest,revenue};
        //total.insert({id,revenue-dist[dest]}); 
        
        if((revenue-dist[dest])>=0){
         total.insert({id,revenue-dist[dest]});
        // cout<<"추가 "<<id<<" "<<revenue-dist[dest]<<'\n';
        }
       // cout<<"추가 후 목록\n";

         // for(auto it=deletes.begin();it!=deletes.end();it++){
            //dest      id, revenue-dist[dest]
       //     cout<<"dest:"<<it->second.first<<" id,revenue "<<it->first<<" "<<it->second.second<<"\n";
         //   info[it->second.first].insert({it->first,it->second.second-dist[it->second.first]});
        // }
         //14 왜 튀어나오는거야!!!!!!!!!!!

       }
       else if(num==300){ //여행 상품 취소  3만  -> o(1) ->ok 
          int id; 
          cin>>id;
          
          //id가 존재할때만
          if(deletes.find(id) == deletes.end())continue; //이게 문제인듯 
          
        //  cout<<"삭제 id:"<<id<<" dest:"<<deletes[id].first<<'\n';
          if(info[deletes[id].first].size()!=0){
            info[deletes[id].first].erase({id,deletes[id].second-dist[deletes[id].first]});
            deletes.erase(id);
            idmatch.erase(id);
            total.erase({id,deletes[id].second-dist[deletes[id].first]});
          }
       }
       else if(num==400){//최적의 여행 상품 판매  3만 ->ok 
          //id별로 revenue-cost[dest](최단거리)  
         
          //최적의 여행상품 출력, 없으면(가는 거리가 없거나 비용<0) -1 
          //모든 dest에 대하여 - 2000개 
          int selectedid=-1;
          int maxs=0;
          int tid; 
          int tcost; 
          int target;
          int mid=1e9;
        //  cout<<"400이윤 목록\n";  
          //최대 n개만 돌면됨 
         if(total.size()==0){
            cout<<-1<<'\n'; continue; 
         }
         else{
            
              selectedid=total.begin()->first;
              target=idmatch[selectedid];
              maxs=total.begin()->second;
              cout<<selectedid<<'\n';
              //삭제까지 하기 
              total.erase({selectedid,maxs});

         }
         
         /*
          for(auto it=info.begin();it!=info.end();it++){  //6000 0000 -> 줄이기... 
             //비어있으면 넘어가기 
             if(it->second.size()==0)continue;
             tid=it->second.begin()->first;
             tcost=it->second.begin()->second;
         //   cout<<"id: "<<tid<<"이윤 "<<tcost<<'\n';
            //마지막 갱신에서 이윤이 잘못됨 

             if(tcost>=maxs){ //set이 정렬이 안되는거야??????
               //if(mid>tid){ 
                  //값이 같을땐 id가 작은 경우에면 
                  if(tcost==maxs){
                     if(mid>tid){
                        mid=tid;
                        target=it->first; 
                        maxs=tcost;selectedid=tid;
                     }
                  }
                  else{
                     mid=tid;
                     target=it->first; 
                     maxs=tcost;selectedid=tid;
                  
                  }
                 
              // }
             }
          }
*/

         // cout<<selectedid<<'\n';
          //선택되면 지우기 info에서 
          if(selectedid!=-1){
         //   cout<<"선택하고 삭제 ";
            //도착지 id dldbs 
         //   cout<<target<<" "<<selectedid<<" "<<maxs<<'\n';
             info[target].erase({selectedid,maxs});
             //deletes에서도 삭제하기 
             deletes.erase(selectedid);
          }

         // cout<<"선택 후 목록\n";
        //  for(auto it=deletes.begin();it!=deletes.end();it++){
            //dest      id, revenue-dist[dest]
         //   cout<<"dest:"<<it->second.first<<" id,revenue "<<it->first<<" "<<it->second.second<<"\n";
        // //   info[it->second.first].insert({it->first,it->second.second-dist[it->second.first]});
       //  }

       }
       else{//500  //출발지 변경  ->ok 
         int s;
         cin>>s;

         start=s;
         dist.assign(n+1,1e9);
         dikjstra();//dest 갱신 
         //그러면 최적의 상품 어떻게 다시 구함? 그냥 여기서 다시? 
         //cost 갱신하기 
         //?????  
         //dest -> {id.cost} set 목적지에 대한 상품,수익 
        //map<int,set<ci>>info; 
          //id -> (dest,cost) 삭제하는거 때문 
          //map<int,ci>deletes;   //id - dest,revenue 
          //deletes는 그대로 ,info 를 다시 갱신 
         for(auto it=info.begin();it!=info.end();it++){ //2000 
            it->second.clear();
         }
         total.clear(); 
        // idmatch.clear();
//       
        // cout<<"새로갱신\n";
        //  //15번이라서 3만번 다써도 됨 
         for(auto it=deletes.begin();it!=deletes.end();it++){ //3만 
            //dest      id, revenue-dist[dest]
          //  cout<<"dest:"<<it->second.first<<" id,revenue "<<it->first<<" "<<it->second.second<<"\n";
            info[it->second.first].insert({it->first,it->second.second-dist[it->second.first]});
            int c=it->second.second-dist[it->second.first];
            idmatch[it->first]=it->second.first;
            if(c>=0){
               total.insert({it->first,it->second.second-dist[it->second.first]});
             //  cout<<"새로 추가"<<it->first<<" "<<it->second.second-dist[it->second.first]<<'\n';
            }
         }
       }
    }




    return 0;
}