#include <iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<deque> 
using namespace std;
int n,m;  //1 베이스 캠프
int x,y; 
typedef pair<int,int> ci; 
bool isend=false;
vector<vector<ci>>basecamp; 
//vector<vector<bool>>base_visited;
vector<vector<ci>>store; //편의점인지  
int cnts=1; 
//각 사람들의 위치 
struct info{
    int num; 
    int x;
    int y; 
    //편의점에 도착했는지 
    bool arrive; 
    //가려는 편의점 위치 
    int destx;
    int desty; 
};
vector<info>person;
bool check(){
   bool flag=true;
   for(int i=0;i<m;i++){
    if(!person[i].arrive)flag=false; 
   }
  // if(flag)cout<<"다도착";
   return flag; 
}
int dr[4]={-1,0,0,1};
int dc[4]={0,-1,1,0}; 

//최단거리 출력 
int bfs(int nr,int nc,int destr,int destc,int len){
    
    //못가는 베이스캠프, 편의점 고려해서 가기 
    queue<pair<int,ci>>q;
    q.push({len,{nr,nc}});

    while(!q.empty()){
      
      int cur=q.front().second.first;
      int cuc=q.front().second.second;
      int cul=q.front().first;
      q.pop();
      if(cur==destr && cuc==destc){
          //작은걸 봐야하나...? 
          return cul; 
      }
      
      for(int i=0;i<4;i++){
        int nr=cur+dr[i];
        int nc=cuc+dc[i]; 
        if(nr<=0 || nc<=0 || nr>n || nc>n)continue; 
        if(basecamp[nr][nc].second)continue; //|| store[nr][nc].second
        q.push({cul+1,{nr,nc}}); 
      }

    }    
}

bool cmp(ci a, ci b){
    if(a.first==b.first){
        return a.second<b.second; 
    }
    return a.first<b.first;
}


void Move(){
  //격자 내에 있고, 편의점에 도착하지 않은 사람들 이동 
  //못지나가는 베이스캠프,편의점 목록
  vector<ci>no_base;
 // vector<ci>no_store;

  for(int i=0;i<m;i++){
    if(person[i].arrive)continue; 
    if(person[i].x<=0 || person[i].y<=0 || person[i].x>n || person[i].y>n)continue; 
    int cur=person[i].x;
    int cuc=person[i].y; 
    //4방향 중 최단거리로 갈수 있는 칸 수 -> 상좌우하 순 
    int destr=person[i].destx;
    int destc=person[i].desty; 
    
    //거리-방향 
    deque<ci>tmp;
    int mins=10000000;
    for(int j=0;j<4;j++){
      int nr=cur+dr[j];
      int nc=cuc+dc[j]; 
      if(nr<=0 || nc<=0 || nr>n || nc>n)continue; 
      if(basecamp[nr][nc].second)continue;
      int distance=bfs(nr,nc,destr,destc,1);

      if(mins>distance){
        mins=distance;
        if(tmp.size()==0)tmp.push_back({distance,j});  
        else{
            tmp.pop_front();
            tmp.push_back({distance,j}); 
        }
      }
    }
   //sort(tmp.begin(),tmp.end(),cmp); 
    //해당 방향으로 감 
    //갈수 없는 경우? 
    if(tmp.size()==0)continue; 
    int dir=tmp[0].second; 
   // cout<<i<<' '<<dir<<"방향으로 감\n";
    //i 사람이 dir 방향에 감 
    int nx=cur+dr[dir];
    int ny=cuc+dc[dir];
    person[i].x=nx;
    person[i].y=ny;  
    //베이스캠프나 편의점에 갔는가? -> 더이상 거기 아무도 못지나감, 이거 다 움직이고 
    //베이스일 경우
    if(basecamp[nx][ny].first==1){
      //  person[i].x=nx;
      //  person[i].y=ny; 
       no_base.push_back({nx,ny});
    }
    //편의점일 경우 
    if(basecamp[nx][ny].first==2){
     //  person[i].x=nx;
     //  person[i].y=ny; 
       person[i].arrive=true;
       no_base.push_back({nx,ny}); 
    }

  }
  //이동 끝나고 편의점 도착한 칸 더이상 못가게 하기   
  for(int i=0;i<no_base.size();i++){
    basecamp[no_base[i].first][no_base[i].second].second=true; 
  } 
  if(check()){
     //cout<<cnts; 
     isend=true;
  }
 // for(int i=0;i<no_store.size();i++){
   // basecamp[no_store[i].first][no_store[i].second].second=true;
 // } 

}

void goBase(int cnt){
    if(person[cnt-1].arrive)return; 
    //cnt번 사람 자기가 가고싶은 편의점과 가장 가까운 베이스켐프가기 
    int destr=person[cnt-1].destx;
    int destc=person[cnt-1].desty; 

    queue<pair<int,ci>>q;
    q.push({0,{destr,destc}});
    vector<ci>tmp; 
    //갈수 없는거 잘피해서 가기 
    int mins=10000000; 

    while(!q.empty()){
       int cur=q.front().second.first;
       int cuc=q.front().second.second; 
       int cul=q.front().first;
       q.pop();
       if(mins<cul)continue; 
      
       if(basecamp[cur][cuc].first==1){
         if(mins>cul){
            mins=min(mins,cul); 
            tmp.push_back({cur,cuc});
         } 
         continue; 
       }
       for(int i=0;i<4;i++){
        int nr=cur+dr[i];
        int nc=cuc+dc[i]; 
        if(nr<=0 || nc<=0 || nr>n || nc>n)continue; 
        //if(basecamp[nr][nc].second || store[nr][nc].second)continue;
        if(basecamp[nr][nc].second)continue;
        q.push({cul+1,{nr,nc}}); 

       }
    }
    //tmp 중에 r,c 가장 작은곳으로 
    sort(tmp.begin(),tmp.end(),cmp);
    if(tmp.size()>=1){
        person[cnt-1].x=tmp[0].first;
        person[cnt-1].y=tmp[0].second; 
        basecamp[tmp[0].first][tmp[0].second].second=true; 
    } 
}


void Go(int cnt){

   //격자 내에 있는 사람 움직이기 
   Move(); 
   //cnt분이 <=m 이면 cnt번 사람 가고싶은 편의점과 가장 가까운 베이스켐프 이동 
   if(cnt<=m){
     goBase(cnt);
   }

}

void print(){
    for(int i=0;i<m;i++){
        cout<<person[i].x<<' '<<person[i].y<<'\n';
    }
    cout<<'\n';
}

int main() {
    
    cin>>n>>m;
    basecamp.assign(n+1,vector<ci>(n+1,{0,0}));
   // base_visited.assign(n+1,vector<bool>(n+1,0));
   // store.assign(n+1,vector<ci>(n+1,{0,0}));
   // store_visited.assign(n+1,vector<bool>(n+1,0)); 

    for(int i=1;i<=n;i++){
      for(int j=1;j<=n;j++){
         cin>>basecamp[i][j].first;
          
      }
    }

    for(int i=1;i<=m;i++){
        cin>>x>>y; 
        person.push_back({i,-1,-1,0,x,y}); 
        basecamp[x][y].first=2; 
    }
    
    while(1){
       Go(cnts);
      // cout<<cnts<<"분\n";
      // print(); 
       if(isend)break;
       cnts++;
       if(check())break;//cnt++;  
    }
    cout<<cnts; 

    return 0;
}