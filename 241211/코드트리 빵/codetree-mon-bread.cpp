#include <iostream>
#include<vector>
#include<queue>

using namespace std; 
typedef pair<int,int> ci;
vector<vector<int>>base; 
vector<vector<bool>>visited;

int dr[4]={-1,0,0,1};
int dc[4]={0,-1,1,0};

struct info{
  int r;
  int c;
  int destr;
  int destc; 
  bool isout;
};

vector<info>person;
int n,m;
int bfs(int r,int c,int destr,int destc){

    vector<vector<bool>>visited2;
    visited2.assign(n+1,vector<bool>(n+1,false)); 
    queue<pair<ci,int>>qq;
    qq.push({{r,c},0}); 
    visited2[r][c]=1; 
    
    while(!qq.empty()){
      int cr=qq.front().first.first;
      int cc=qq.front().first.second;
      int l=qq.front().second; //현재까지 이동거리 
      qq.pop(); 

      if(cr==destr && cc==destc){
         //처음 찾으면 끝 
         return l; 
      } 
      for(int i=0;i<4;i++){
         int nr=cr+dr[i];
         int nc=cc+dc[i];
         if(nr<=0 || nc<=0 || nr>n || nc>n)continue; 
         if(visited2[nr][nc])continue; 
         if(visited[nr][nc])continue; //갔던 베이스나 편의점이면 못감 
         visited2[nr][nc]=1;
         qq.push({{nr,nc},l+1}); 
      }
    } 
    //못가는경우 
    return -1; 
}

void Move(){
  //경로에 다른 편의점 있어도 되나?   
  //격자에 있는 모든 사람들이 가려는 편의점 향해서 
  //1. 가장 짧은 거리
  //2. 여러개면 상좌우하 순으로 가기 
  vector<ci>nocandi; //더이상 못갈 편의점목록 
  
  for(int i=1;i<=m;i++){
    if(person[i].isout)continue;
    int r=person[i].r; int destr=person[i].destr;
    int c=person[i].c; int destc=person[i].destc; 
    if(r<=0 || c<=0 )continue; //격자내에 있을때만

    int mins=10000000; int mindir=-1;    
    for(int j=0;j<4;j++){
      int nr=r+dr[j];
      int nc=c+dc[j];
      if(nr<=0 || nc<=0 || nr>n || nc>n)continue;
      if(visited[nr][nc])continue;
      int ndistance=bfs(nr,nc,destr,destc); 
      if(ndistance<0)continue; 
      if(mins>ndistance){
         mins=ndistance; mindir=j; 
      }
    }
    //mindir로 이동 
    person[i].r+=dr[mindir];
    person[i].c+=dc[mindir]; 
    //편의점에 도착했는가? 
    if(person[i].r==person[i].destr && person[i].c==person[i].destc){
        nocandi.push_back({person[i].r,person[i].c}); 
        person[i].isout=true;
    }
    //베이스에 도착했는가? 
    if(base[person[i].r][person[i].c]==1)nocandi.push_back({person[i].r,person[i].c}); 
  }

   //모두 움직인 후에 갈수없는 편의점 지정하기 -> 베이스 > 
  for(int i=0;i<nocandi.size();i++){
    visited[nocandi[i].first][nocandi[i].second]=1; 
  }
}



void goBase(int t){
 //경로에 편의점이 있어도 되나?    
 // 가려는 편의점에서 가장 가까이 있는 베이스 
 //1.최단거리 
 //2. 행 작 -> 열 작 
   int destr=person[t].destr;
   int destc=person[t].destc;
   //갈수 있는 베이스 중에 
   int gor=-1; int goc=-1;
   int mins=10000000;
   for(int i=1;i<=n;i++){
    for(int j=1;j<=n;j++){

        if(base[i][j]==0)continue;
        if(visited[i][j])continue; 
        //베이스 중에 거리계산 
        
        // cout<<t<<"베이스후보"<<i<<' '<<j<<'\n';
        int distance=bfs(destr,destc,i,j);
       // cout<<t<<' '<<i<<' '<<j<<' '<<distance<<'\n';
        if(distance<0)continue; 
        if(mins>distance){
            mins=distance;
            gor=i; goc=j; 
           
        }
     }
   }
   //갈수 있는 베이스 없으면 통과 
   if(gor<0)return;
   person[t].r=gor;person[t].c=goc;
   visited[person[t].r][person[t].c]=1; 
  
}

bool check(){
    bool flag=true;
    for(int i=1;i<=m;i++){
        if(!person[i].isout){
            flag=false;break;
        }
    }
    return flag; 
}
//디버깅용 
void print(){
   //각 사람들의 위치 파악 
   for(int i=1;i<=m;i++){
    cout<<person[i].r<<' '<<person[i].c<<' '<<person[i].isout<<'\n';
   }

}

int main() { 

    cin>>n>>m; 
    base.assign(n+1,vector<int>(n+1,0));
    visited.assign(n+1,vector<bool>(n+1,0)); 

    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            cin>>base[i][j];
        }
    } 
    person.push_back({-1,-1,0,0,0});
    int rr,cc;
    for(int i=0;i<m;i++){
       cin>>rr>>cc;
       person.push_back({-1,-1,rr,cc,0});
    }
    int t=1;
    while(1){
 
       Move(); 
       if(check())break; 
       if(t<=m)goBase(t); 
       t++; 
       
    }

   cout<<t; 

    return 0;
}