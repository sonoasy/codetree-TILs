#include <iostream>
#include<vector>
#include<algorithm>

using namespace std; 
typedef pair<int,int> ci;
struct info{
   int dir;
   int dist;
   int r;
   int c;
};
int n,m,k; 
//상하좌우 
int dr[4]={-1,1,0,0};
int dc[4]={0,0,-1,1};
vector<vector<int>>arr; 

int r,c;
struct runner{
  int x;
  int y; 
  int movecnt;
  bool out;  
};
vector<runner>runners;
vector<vector<ci>>runs; 
int exitr;
int exitc;


bool cmp(info a,info b){
  if(a.dist==b.dist){
     if(a.r==b.r){
       return a.c<b.c;
     }
     return a.r<b.r;
  }
  return a.dist<b.dist;
}
bool isok(int r,int c,int len){
  
  int cnt=0;
  int exitcnt=0;
  
  if(exitr>=r && exitr<=(r+len-1) && exitc>=c && exitc<=(c+len-1))exitcnt++;
  vector<pair<ci,int>>tmp1; 
  vector<pair<int,ci>>tmp2; 
  for(int i=0;i<m;i++){
    //
    if(runners[i].out)continue; 
    int x=runners[i].x; 
    int y=runners[i].y; 
    if(x>=r && x<=(r+len-1) && y>=c && y<=(c+len-1)){
      cnt++;tmp1.push_back({{x,y},i});
    } 
  }
  for(int i=1;i<=n;i++){
    for(int j=1;j<=n;j++){
      if(i>=r && i<=(r+len-1) && j>=c && j<=(c+len-1) && arr[i][j]>0)tmp2.push_back({arr[i][j],{i,j}}); 
    }
  } 


  if(exitcnt==1 && cnt>=1){
    //바꾸기 
    //cout<<r<<' '<<c<<' '<<len<<"됨";
    //출구 
    int t=(len-(exitr-r)-1);
    exitr=r+exitc-c;
    exitc=c+t; 
    //사람 
    for(int i=0;i<tmp1.size();i++){
      int r1=tmp1[i].first.first-r;
      int r2=tmp1[i].first.second-c; 
      runners[tmp1[i].second].x=r+r2;
      runners[tmp1[i].second].y=c+(len-r1-1); 
    }
    //벽 
    for(int i=0;i<tmp2.size();i++){
      arr[tmp2[i].second.first][tmp2[i].second.second]=0;
    }
    for(int i=0;i<tmp2.size();i++){
      int nn=tmp2[i].first;
      if(nn>0)nn-=1;
      int r1=tmp2[i].second.first-r;
      int r2=tmp2[i].second.second-c; 
      arr[r+r2][c+len-r1-1]=nn; 
    }
    
    return true;
  }
  else return false; 
}

//미로 회전 
void Rotate(){
   //출구와 참가사 포함 최소 정사각형 
   //길이가 1 -> n까지 다 탐색 
   for(int len=1;len<=n;len++){
      //꼭지점이 r,c 인 좌표 -> r+len-1까지 
      //허용되는 r의 범위는 r+len-1<=n인 n-len+1 
      for(int r=1;r<=n-len+1;r++){
        for(int c=1;c<=n-len+1;c++){
          //(r,c),len에 최소 참가자 있고 출구가 있는가? 
           if(isok(r,c,len)){
          
            return; 
           }
        }
      }
   }
}

//러너 이동 
void Move(){
   //0인곳으로, 
   for(int i=0;i<m;i++){
     if(runners[i].out)continue; //이미 탈출한거면 패스 
     int cur=runners[i].x;
     int cuc=runners[i].y;
     //현재 출구와의 거리 
     int curdist=abs(cur-exitr)+abs(cuc-exitc);
     vector<info>tmp; 
     for(int j=0;j<4;j++){
        int nr=cur+dr[j];
        int nc=cuc+dc[j]; 
        if(nr<=0 ||nc<=0 || nr>n || nc>n)continue;
        if(arr[nr][nc]>0)continue; //벽이면 갈수 없음 
        int ndist=abs(nr-exitr)+abs(nc-exitc); 
        //curdist보다 ndist가 작은곳 -r작 -c작 
        if(ndist<curdist)tmp.push_back({j,ndist,nr,nc});
     }
     if(tmp.size()==0)continue;
     sort(tmp.begin(),tmp.end(),cmp);
     int godir=tmp[0].dir; 
     runs[runners[i].x][runners[i].y]={0,0};

     runners[i].x=cur+dr[godir];
     runners[i].y=cuc+dc[godir]; 
     runs[runners[i].x][runners[i].y]={1,i}; 
     //출구라면 끝
     runners[i].movecnt+=1; 
     if(exitr==runners[i].x && exitc==runners[i].y)runners[i].out=true; 
   
   }
}

void print(){
  for(int i=0;i<m;i++){
    cout<<runners[i].x<<' '<<runners[i].y<<' '<<runners[i].movecnt<<'\n';
  }
  cout<<'\n';
}
void Answer(){
   int sum=0;
   for(int i=0;i<m;i++){
    sum+=runners[i].movecnt;
   }
   cout<<sum<<'\n';  
   cout<<exitr<<' '<<exitc; 
}
bool check(){
  bool flag=true;
  for(int i=0;i<m;i++){
    if(!runners[i].out)flag=false;
  }
  return flag;
}
int main() {
   
   cin>>n>>m>>k; 
   arr.assign(n+1,vector<int>(n+1,0)); 
   runs.assign(n+1,vector<ci>(n+1,{0,0})); 
   for(int i=1;i<=n;i++){
    for(int j=1;j<=n;j++){
      cin>>arr[i][j];
    }
   }

   for(int i=0;i<m;i++){
     cin>>r>>c;
     runners.push_back({r,c,0,0}); 
     runs[r][c]={1,i}; 
   }
   cin>>exitr>>exitc; 

   for(int i=0;i<k;i++){
      
      if(check())break; 
     // cout<<i+1<<"번째\n";
      Move(); 
     // cout<<"움직임\n";
    //  print();

      Rotate(); 
     // cout<<"회전후\n";
    //  print();
    //  cout<<"벽 위치\n";
     // for(int j=1;j<=n;j++){
      //  for(int k=1;k<=n;k++){
        //  cout<<arr[j][k];
      //  }
       // cout<<'\n';
   //   }
     // cout<<"출구"<<exitr<<' '<<exitc<<'\n';
     
   }
   Answer(); 
    return 0;
}