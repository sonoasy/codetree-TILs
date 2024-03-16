#include <iostream>
#include <vector>
#include<map>
#include<algorithm>
#include<deque>
#include<queue>
using namespace std;

typedef pair<int,int> ci; 

//우하상좌 대각선 

int dr[8]={0,1,-1,0,-1,-1,1,1};
int dc[8]={1,0,0,-1,-1,1,1,-1};

vector<vector<int>>arr;
vector<vector<int>>goturn;
vector<vector<int>>damagedturn;
//완전히 부서진 곳 -> arr<=0 
deque<ci>des; 
vector<vector<bool>>visited; 
vector<vector<ci>>way;
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
/*
bool cmpDest(ci a, ci b){
 //1. 공격력 가장 높음
 //2. 공격한지 가장 오래된
 //3. 행열 합 가장 작은 
 //4. 열 값 가장 작은 
   int x1=a.first; int y1=a.second;
   int x2=b.first; int y2=b.second;
   
   if(arr[x1][y1]==arr[x2][y2]){
     if(damagedturn[x1][y1]==damagedturn[x2][y2]){
       if((x1+y1)==(x2+y2)){
         return y1<y2; 
       }      
       return (x1+y1)<(x2+y2);
     }   
     return damagedturn[x1][y1]<damagedturn[x2][y2]; //damagedturn 값이 커야 가장 최근에 공격받음  
   }
   return arr[x1][y1]>arr[x2][y2];
    
}
*/
//공격자 선정 
void calGo(int a){
   
   sort(info.begin(),info.end(),cmpGo); 
   
   minr=info[0].first;
   minc=info[0].second; 
   destr=info[info.size()-1].first;
   destc=info[info.size()-1].second;
  // cout<<minr<<' '<<minc<<'\n';
 //  cout<<destr<<' '<<destc<<'\n';


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
   way.assign(n,vector<ci>(m,{-1,-1}));

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
            way[nr][nc]={cr,cc};
            q.push({nr,nc}); //왜 안돼 
        }
     
       }
   
   }
   
}

void potan(int a){ // -> 단순 구현 
 //1. 공격 대상에게 공격자 공격력만큼 피해
  arr[destr][destc]-=mingo; 
  //damagedturn 표시
  damagedturn[destr][destc]=a;

 //2. 8방향 공격 (공격자 공격력)/2 만큼 피해 
 // cout<<"포탄공격함";
  int subgo=(mingo)/2; 

  for(int i=0;i<8;i++){
     int nr=(destr+dr[i]+n)%n;
     int nc=(destc+dc[i]+m)%m; 
     arr[nr][nc]-=subgo; 
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
  //2. 공격 대상에게 공격자 공격력만큼 피해,  -> 안막히면 일단 dfs? 해서 되면 return 하기 
  //레이저 경로에 있는것은 (공격자 공격력)/2 만큼 피해 
  //cout<<"레이저공격함"<<'\n';
   arr[destr][destc]-=mingo;
   damagedturn[destr][destc]=a;

   int subgo=(mingo)/2; 
   
    int x=way[destr][destc].first; 
    int y=way[destr][destc].second;

   while(1){

       if(x==minr && y==minc)break;
       damagedturn[x][y]=a;
       arr[x][y]-=subgo; 
       
       int nx=way[x][y].first;
       int ny=way[x][y].second; 
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
        maxs=max(maxs,arr[i][j]);
    }
   }
   return maxs; 
}

void arrcout(){
     cout<<'\n';
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
           if(arr[i][j]>0)info.push_back({i,j});
        }
       }
     //  cout<<"턴: "<<a<<'\n';
      //1.공격자 선정 
       calGo(a);  //-> damagedturn 이 맞는지 모르겠음 , 완전히 부서진곳 체크했나 확인, 일단 (O)
    //   cout<<"공격자 및 공격대상 선정\n";
     //  arrcout();  
       //2-1 공격 대상 선정  
      // calDest(); //-> damagedturn 이 맞는지 모르겠음 ,  완전히 부서진곳 체크했나 확인,일단 (O)

       //2-2. 레이저 공격 
       rager(a);  //하긴 했는데... 세모 
       //2-3. 레이저 공격 안되면 포탄 공격 
       //potan(a); //일단 (O)
     //  arrcout();
       //3. 포탄 부서짐 -> 완전히 부서진곳 체크했나 확인 
       //totaldamaged(); //일단(O)
     //  cout<<"포탄공격"<<'\n';
    //   arrcout();
     //  //4. 포탄 정비 
       //공격자-공격받은자 둘다 아니면 포탑의 공격력+1 
         
       recover(a);  //일단 (O)_
     //  cout<<"포탄정비"<<'\n'; 
     //  arrcout();

      
    }
    cout<<maxNum();
    return 0;
}