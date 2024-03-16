#include <iostream>
#include <vector>
#include<map>
#include<algorithm>
#include<deque>
#include<queue>
using namespace std;

typedef pair<int,int> ci; 

//우하상좌 대각선  ->좌상임 !!!

int dr[8]={0,1,0,-1,-1,-1,1,1};
int dc[8]={1,0,-1,0,-1,1,1,-1};

vector<vector<int>>arr;
vector<vector<int>>goturn;
vector<vector<int>>damagedturn;
vector<vector<bool>>visited; 
int xway[10][10];
int yway[10][10]; 
vector<ci>info;

int n,m,k;
int minr,minc; //공격자
int mingo=0; 

int destr,destc; //공격받은자 

//공격력 -> arr 배열에 저장 
//공격자 언제였는지 -> turn 배열에 k턴일때 선정된 값 저장하기 
//공격 받은거 언제였는지 -> damagedturn 배열에 k턴일떄를 저장하기 
bool flag; 
bool cmpGo(ci a,ci b){
   
    //공격력이 낮은 포탑  
   //2개 이상이면 가장 최근에 공격한 포탑 -> 공격받은거라 가정  ??
   //행 열 합이 가장 큰 
   //열 값이 가장 큰 
   int x1=a.first; int y1=a.second;
   int x2=b.first; int y2=b.second;
   
   if(arr[x1][y1]==arr[x2][y2]){
       if(goturn[x1][y1]==goturn[x2][y2]){
         if((x1+y1)==(x2+y2)){
           return y1>y2; 
          }      
          return (x1+y1)>(x2+y2);
        }   
         return goturn[x1][y1]>goturn[x2][y2]; //damagedturn 값이 커야 가장 최근에 공격받음  
   }

   return arr[x1][y1]<arr[x2][y2];
   
}

//공격자 선정 
void calGo(int a){
   
   sort(info.begin(),info.end(),cmpGo); 
   
   minr=info[0].first;
   minc=info[0].second; 
   destr=info[info.size()-1].first;
   destc=info[info.size()-1].second;
  
   //공격력 M+N 증가 
   arr[minr][minc]=arr[minr][minc]+(n+m);
   mingo=arr[minr][minc];
   goturn[minr][minc]=a;
 
  
}


void bfs(int curr,int curc){
  
  //우하 상좌이긴한데.. 더 빨리 도착하는거 해야지...  
   //bfs로 visited 탐색하면서 탐색 
   //역추적에서 경로 작은거는 어떻게 알음? 
   queue<ci>q; 
 
   visited[curr][curc]=true;   
   
   q.push({curr,curc}); 
   
   while(!q.empty()){

      int cr=q.front().first;
      int cc=q.front().second; 

      q.pop(); 

      if(cr==destr && cc==destc){
        flag=true; //그냥 빠른게 먼저 되나? 
        
        break;
      }

      for(int i=0;i<4;i++){
       
        int nr=(cr+dr[i]+n)%n;
        int nc=(cc+dc[i]+m)%m;
 
        if(!visited[nr][nc] && arr[nr][nc]>0){
          
            visited[nr][nc]=true; //? 
            xway[nr][nc]=cr;
            yway[nr][nc]=cc; 
            q.push({nr,nc}); //왜 안돼 
        }
     
       }
   
   }
   
}

void potan(int a){ // -> 단순 구현 
 //1. 공격 대상에게 공격자 공격력만큼 피해
  arr[destr][destc]-=mingo; 
  if(arr[destr][destc]<0)arr[destr][destc]=0;
  //damagedturn 표시
  damagedturn[destr][destc]=a;

 //2. 8방향 공격 (공격자 공격력)/2 만큼 피해 
 // cout<<"포탄공격함";
  int subgo=(mingo)/2; 

  for(int i=0;i<8;i++){
     int nr=(destr+dr[i]+n)%n;
     int nc=(destc+dc[i]+m)%m;
     if(nr==minr && nc==minc)continue; //이부분!!!
     arr[nr][nc]-=subgo; 
     if(arr[nr][nc]<0)arr[nr][nc]=0;
     damagedturn[nr][nc]=a;         
  }

}

void rager(int a){
  //상하좌우, 최단경로로 공격 
  //우->하->좌->상 순으로 
  //1. 경로가 있는가?  -> 막히는지. //dfs bfs가 됨? 
 
  bfs(minr,minc); //이게 문제임 

  //경로 없으면 포탄공격   
  if(!flag){
  //  cout<<"포탄공격함\n";
    potan(a);
    return; 
  }

   arr[destr][destc]-=mingo;
   if(arr[destr][destc]<0)arr[destr][destc]=0;
   damagedturn[destr][destc]=a;

   int subgo=(mingo)/2; 
   
    int x=xway[destr][destc]; 
    int y=yway[destr][destc];

   while(1){

       if(x==minr && y==minc)break;
       damagedturn[x][y]=a;
       arr[x][y]-=subgo; 
        if(arr[x][y]<0)arr[x][y]=0;
       int nx=xway[x][y];
       int ny=yway[x][y]; 
       x=nx;
       y=ny; //왜?

       
   } 

}



void recover(int a){ //0이 아니고(이미 부서진), damagedturn,goturn 둘다 아닌거 +1 

  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      if(arr[i][j]>0 && damagedturn[i][j]!=a && goturn[i][j]!=a){
        arr[i][j]+=1; 
      }
    }
  }
}

//마지막으로 가장 큰 공격력 출력 
int maxNum(){

   int maxs=0; 
   for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      //  cout<<'('<<arr[i][j]<<')';
        maxs=max(maxs,arr[i][j]);
    }
   // cout<<'\n';
   }
   return maxs; 
}

void arrcout(){
     
     for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
           cout<<'('<<arr[i][j]<<')';
        }
        cout<<'\n';
       }
        cout<<'\n';
}

int main() {
    
    cin>>n>>m>>k; 

    arr.assign(n,vector<int>(m,0));
    goturn.assign(n,vector<int>(m,0)); //공격한 시기 
    damagedturn.assign(n,vector<int>(m,0)); //공격받은 시기 

    for(int i=0;i<n;i++){
      for(int j=0;j<m;j++){
        cin>>arr[i][j];
      }
    }
    vector<ci>tt;
    //k번 턴 
    for(int a=1;a<=k;a++){
      flag=false; 
      visited.assign(n,vector<bool>(m,0));
      info=tt;
   
      for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
           if(arr[i][j]>0){
            info.push_back({i,j});
           }
  
        }
       }
       if(info.size()==1)break; //이부분 

       
       
       calGo(a);  
      
       rager(a);  
      
       recover(a);  
 

    }
    

    cout<<maxNum();
    return 0;
}