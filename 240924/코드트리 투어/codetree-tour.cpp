#include <iostream>
#include<vector>
#include<map>

using namespace std;
typedef pair<int,int>ci; 
vector<vector<int>>graph;
map<int,ci>info; 
int start=0;
int main() {
  

    int q;//명령어 수 
    cin>>q;
    for(int i=0;i<q;i++){
       int num;
       cin>>num;
       if(num==100){  //코드트리 랜드 건설 - 1번만 
          int n,m,v,u,w; 
          cin>>n>>m; 
          graph.assign(n+1,vector<int>(n+1,0));
          for(int j=0;j<m;j++){ //간선의 갯수 
             cin>>v>>u>>w;
             graph[v][u]=w;
             graph[u][v]=w; 
          }
       }
       else if(num==200){ //여행 상품 생성  3만  -> 0(1)
        int id,revenue,dest;
        cin>>id>>revenue>>dest; 
        info[id]={revenue,dest};

       }
       else if(num==300){ //여행 상품 취소  3만  -> o(1)
          int id; 
          cin>>id;
          info[id]={0,0}; 
       }
       else if(num==400){//최적의 여행 상품 판매  3만 
             



       }
       else{//500  //출발지 변경  
         int s;
         cin>>s;
         start=s;
       }





    }




    return 0;
}