#include <iostream>
#include<vector>

using namespace std; 

int sum=0; 
typedef pair<int,int> ci; 
//상하좌우 
int dr[4]={-1,1,0,0};
int dc[4]={0,0,-1,1};
int n,m,k; 
vector<vector<int>>arr;

struct person{
  int r;
  int c;
  bool isout;
  int moved;  
};



vector<person>info; 
int exitr;
int exitc;

bool check(){
  bool flag=true;
  for(int i=1;i<=m;i++){
    if(!info[i].isout){
      flag=false;break;
    }
  }
  return false;
}
int calDistance(int x1,int y1, int x2,int y2){
  return abs(x1-x2)+abs(y1-y2);
}


void Move(){ //참가자 이동 

   //출구로 더 가까워질때만, 상하 우선 움직임 벽있으면 안됨 

   for(int i=1;i<=m;i++){
     if(info[i].isout)continue; 
     //현재 출구와의 거리 
     int curd=calDistance(info[i].r,info[i].c,exitr,exitc); 
     int min=curd;
     int go=-1;
     for(int j=0;j<4;j++){
      int nr=info[i].r+dr[j];
      int nc=info[i].c+dc[j]; 
      if(nr<=0 || nc<=0 || nr>n || nc>n)continue; 
      if(arr[nr][nc]>=1)continue;//벽이 있음
      if(min>calDistance(nr,nc,exitr,exitc)){
        min=calDistance(nr,nc,exitr,exitc);
        go=j;
      } 
     }
     if(go<0)continue; //안움직임 
     info[i].r+=dr[go];
     info[i].c+=dc[go]; 
    // cout<<i<<"가"<<info[i].r<<' '<<info[i].c<<"로이동\n";
     sum++; 
     if(info[i].r==exitr && info[i].c==exitc){
      info[i].isout=true; 
      //cout<<i<<" 나갔어\n";
     } 

   }


}

bool isok(int len,int r,int c){

   vector<pair<ci,int>>wall;  //범위 내의 벽 
   vector<int>ptmp; //범위 내의 사람   
   int fr=r; int er=r+len-1;
   int fc=c; int ec=c+len-1;
   if(er>n || ec>n)return false; 
   
   bool isexit=false;
   int cnt=0; 

   if(exitr>=fr && exitr<=er && exitc>=fc && exitc<=ec)isexit=true; 

   for(int i=1;i<=m;i++){
     if(info[i].isout)continue;
     int ir=info[i].r;
     int ic=info[i].c;
     if(ir>=fr && ir<=er && ic>=fc && ic<=ec){
      cnt++; ptmp.push_back(i); 
     }
   }
   for(int i=fr;i<=er;i++){
    for(int j=fc;j<=ec;j++){
      if(arr[i][j]>=1){
        wall.push_back({{i,j},arr[i][j]});
      }
    }
   }

   if(isexit && cnt>=1){
       //회전 시키기 
       //벽 옮기기 내구성 -1 
       for(int i=0;i<wall.size();i++){
         arr[wall[i].first.first][wall[i].first.second]=0;
       }
       for(int i=0;i<wall.size();i++){
         int nr=r+wall[i].first.second-c;
         int nc=c+(len-(wall[i].first.first-r)-1); 
         arr[nr][nc]=wall[i].second-1;
       }
      
       //사람 옮기기 
      for(int i=0;i<ptmp.size();i++){
        int nr=r+info[ptmp[i]].c-c;
        int nc=c+(len-(info[ptmp[i]].r-r)-1); 
        info[ptmp[i]].r=nr;
        info[ptmp[i]].c=nc; 
      }

       //출구 옮기기 
      int tt=exitc;
      exitc=c+(len-(exitr-r)-1);
      exitr=r+tt-c; 
      return true; 
   }
   return false;

}

void Rotate(){ //사람,출구 포함 가장 작은 크기 회전 

    for(int len=1;len<=n;len++){ //길이 
       for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){    
           if(isok(len,i,j)){
           // cout<<"길이 "<<len<<' '<<i<<' '<<j<<"에서 회전\n";
            return;
           }
        }
       }
    }
}


int main() {
    
    cin>>n>>m>>k; 
    arr.assign(n+1,vector<int>(n+1,0));
    for(int i=1;i<=n;i++){
      for(int j=1;j<=n;j++){
        cin>>arr[i][j]; 
      }
    }
    info.push_back({0,0,0,0});
    int rr,cc;
    for(int i=1;i<=m;i++){
      cin>>rr>>cc; 
      info.push_back({rr,cc,0,0}); 
    }
    cin>>exitr>>exitc;
     
    for(int i=1;i<=k;i++){
      // cout<<"Round: "<<i<<'\n';
       if(check())break;
       Move();
       Rotate(); 
    } 
    cout<<sum<<'\n';
    cout<<exitr<<' '<<exitc;

    return 0;
}