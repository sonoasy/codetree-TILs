#include <iostream>
#include<vector>
#include<algorithm> 
#include<queue>
using namespace std; 

typedef pair<int,int> ci;
//우하좌상 대각선방향 
int dr[8]={0,1,0,-1,-1,-1,1,1};
int dc[8]={1,0,-1,0,-1,1,1,-1};
//좌표 
vector<vector<ci>>nexts; 
vector<vector<int>>arr;
vector<vector<int>>castle_num;
int n,m,k;
struct castle{
  int num;
  int r;
  int c;
  bool destroy;
  int goturn; 
  int destroyturn; 
  int power;
};
vector<castle>info; 

bool cmp(castle a,castle b){
 
   if(a.power==b.power){
    if(a.goturn==b.goturn){
      if((a.r+a.c)==(b.r+b.c)){
        return a.c>b.c; 
      }
      return (a.r+a.c)>(b.r+b.c);
    }
    return a.goturn>b.goturn;
   }

   return a.power<b.power;
  
  
}
void pt(){
   for(int i=0;i<n;i++){
      for(int j=0;j<m;j++){
         cout<<arr[i][j];
      }
      cout<<'\n';
   }

}
void potan(int gonum,int destroynum,int turn){
   
    //공격함 
    info[gonum].goturn=turn; 
    //공격당함 
    info[destroynum].destroyturn=turn; 
    //공격자 공격력만큼 피해입음 
    info[destroynum].power-=info[gonum].power; 
    arr[info[gonum].r][info[gonum].c]=info[gonum].power;
    arr[info[destroynum].r][info[destroynum].c]=info[destroynum].power; 


    if(info[destroynum].power<=0){
      //파괴됨 
      info[destroynum].destroy=true; 
      arr[info[destroynum].r][info[destroynum].c]=0; 
    }
    //주변의 8방향도 피해입히기 
    //단 공격한 사람은 피해 안입음 
    for(int i=0;i<8;i++){
      int nr=(info[destroynum].r+dr[i]+n)%n;
      int nc=(info[destroynum].c+dc[i]+m)%m; 
      if(nr==info[gonum].r && nc==info[gonum].c)continue; 
      if(arr[nr][nc]==0)continue; 
      //포탑인걸 어케암? 
      if(arr[nr][nc]>=0){
        //피해입히기 
        int fnum;
        for(int j=0;j<info.size();j++){
         if(info[j].r==nr && info[j].c==nc){
          fnum=j; break; 
         }
       } 
      // cout<<"포탄 공격받음"<<nr<<' '<<nc<<'\n';

        info[fnum].power-=(info[gonum].power)/2;
        info[fnum].destroyturn=turn;
        arr[nr][nc]=info[fnum].power;
        //파괴됬나? 
        if(info[fnum].power<=0){
          info[fnum].destroy=true; 
          arr[nr][nc]=0; 
        }

      }
    }

}

bool canRager(int gonum,int destroynum){
   //gonum-> destroynum까지 최단경로가 있는지 
   nexts.assign(n,vector<ci>(m,{-1,-1})); 
   int cur=info[gonum].r;
   int cuc=info[gonum].c; 

   int destr=info[destroynum].r;
   int destc=info[destroynum].c; 
   queue<pair<int,int>>q; 
   q.push({cur,cuc});
   //먼저 도착하면 끝? 

   vector<vector<bool>>visited;
   
   visited.assign(n,vector<bool>(m,false)); 
   
   visited[cur][cuc]=true;
   bool finded=false; 

   while(!q.empty()){
     if(finded)break;
     int curr=q.front().first;
     int cucc=q.front().second; 
     q.pop();
    // cout<<curr<<' '<<cucc<<'\n';
     if(curr==destr && cucc==destc){
      //경로 찾음 
       finded=true; 
       break; 
     }

     for(int i=0;i<4;i++){
      int nr=(n+curr+dr[i])%n;
      int nc=(m+cucc+dc[i])%m; 
      //부서지거나 
      if(arr[nr][nc]==0)continue; 
      if(visited[nr][nc])continue;
      visited[nr][nc]=true; 
      nexts[nr][nc]={curr,cucc}; 
      q.push({nr,nc});
     }

   }
   //경로 찾음 
   return finded; 
}

void Go(int gonum, int destroynum,int turn){

   //레이저 공격 
   if(canRager(gonum,destroynum)){
  //  cout<<"레이저공격됨\n";

    //경로 역추적하면서 목표지점 이외에 반으로 핸디캡,  공격자는 제외 
    info[gonum].goturn=turn; 
    info[destroynum].power-=info[gonum].power;
    info[destroynum].destroyturn=turn; 

    //arr이에도 반형 
    arr[info[gonum].r][info[gonum].c]=info[gonum].power;
    arr[info[destroynum].r][info[destroynum].c]=info[destroynum].power; 
     
    if(info[destroynum].power<=0){
      info[destroynum].destroy=true; 
      arr[info[destroynum].r][info[destroynum].c]=0;
    }
    int cr=info[destroynum].r;
    int cc=info[destroynum].c; 
    
    while(1){
       int nr=nexts[cr][cc].first;
       int nc=nexts[cr][cc].second; 

       if(nr==info[gonum].r && nc==info[gonum].c)break; //공격자 위치 
       //nr,nc에 반만큼 피해 입히기 
      // cout<<"전: "<<nr<<' '<<nc<<' '<<castle_num[nr][nc]<<info[castle_num[nr][nc]].r<<' '<<info[castle_num[nr][nc]].c<<'\n';
      // cout<<info[castle_num[nr][nc]].power;
      int fnum;
       for(int i=0;i<info.size();i++){
         if(info[i].r==nr && info[i].c==nc){
          fnum=i; break; 
         }
       }
    

       info[fnum].power-=(info[gonum].power)/2;
       arr[nr][nc]=info[fnum].power;
     //  cout<<"후: "<<info[fnum].power<<'\n';
       info[fnum].destroyturn=turn; 
       if(info[fnum].power<=0){
        info[fnum].destroyturn=turn; 
        arr[nr][nc]=0; 
       }
       cr=nr;
       cc=nc;
     }



  }
  else{
       potan(gonum,destroynum,turn); 
   }

}
void Recover(int turn){
 
  for(int i=0;i<info.size();i++){
    if(info[i].destroy)continue;
    if(info[i].goturn!=turn && info[i].destroyturn!=turn){
       //공격력+1 
       info[i].power+=1;// cout<<"왜안돼";
       arr[info[i].r][info[i].c]+=1;
    }
  }
}
void print(){
  int maxs=-1;
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      if(maxs<arr[i][j])maxs=arr[i][j];
    }
  }
  cout<<maxs;
}
int check(){
  int cnt=0;
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      if(arr[i][j]>0)cnt++;
    }
  }
  return cnt;
}

int main() {
    int cnt=0; 
    cin>>n>>m>>k;
    arr.assign(n+1,vector<int>(m+1,0)); 
    castle_num.assign(n+1,vector<int>(m+1,-1)); 
    for(int i=0;i<n;i++){
      for(int j=0;j<m;j++){
         cin>>arr[i][j]; 
         if(arr[i][j]>=1){ //포탄 
           info.push_back({cnt,i,j,0,0,0,arr[i][j]});
           castle_num[i][j]=cnt; 
           cnt++;
         }
      }
    }
  


    for(int i=1;i<=k;i++){
       if(check()==1)break; 
       //공격자 선정  & 피공격자 선정 
       //부서진 포탑 제외하기 
       vector<castle>candi;
       for(int j=0;j<info.size();j++){
         if(info[j].destroy)continue;
         candi.push_back(info[j]);
       }
       sort(candi.begin(),candi.end(),cmp); 
       int gonum=candi[0].num; 
       int destroynum=candi[candi.size()-1].num; 
     //  cout<< info[gonum].r<<' '<< info[gonum].c<<'\n';
     //  cout<< info[destroynum].r<<' '<< info[destroynum].c<<'\n';
       //공격자 공격력 증가 
       info[gonum].power+=(n+m);
      // pt();
       //공격하기 
       arr[info[gonum].r][info[gonum].c]=info[gonum].power;
     //  pt(); cout<<'\n';
       Go(gonum,destroynum,i); 
       //회복
      // pt();cout<<'\n';
       Recover(i); 
      // pt();cout<<'\n';
    }
    print(); 

    return 0;
}