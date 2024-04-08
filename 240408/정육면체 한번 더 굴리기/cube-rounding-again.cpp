#include <iostream>
#include<vector>
#include<queue> 

using namespace std; 
//2 3 0 1 
//상우하좌 
int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1};
vector<vector<int>>arr;
int n,m; 
int score=0; 

int curx=1;
int cury=1; 
int dir=1;

//앞 뒤 오 좌 위 아래 
int fnum=2; 
int Bnum=5;
int rnum=3;
int lnum=4; 
int unum=1;
int bnum=6; 
typedef pair<int,int> ci; 
int Count(int nx,int ny){
   int num=arr[nx][ny];
   int cnt=0; 
   vector<vector<int>>visited;
   visited.assign(n+1,vector<int>(n+1,0));
   queue<ci>q; 
   q.push({nx,ny});
   while(!q.empty()){
     int cr=q.front().first;
     int cc=q.front().second;
     q.pop();
     if(visited[cr][cc])continue;
     visited[cr][cc]=1; 
    // cout<<cr<<' '<<cc<<"방문함\n";
     cnt++; 
     for(int i=0;i<4;i++){
        int nr=cr+dr[i];
        int nc=cc+dc[i]; 
        if(nr<=0 || nc<=0 || nr>n || nc>n)continue;
        if(visited[nr][nc])continue;
        if(arr[nr][nc]==num){
           // visited[nr][nc]=1;
            q.push({nr,nc}); 
        }
     }
   }
   //cout<<cnt<<' '<<num<<'\n';
   return cnt*num; 
}

void Roll(int dir){ //dir방향으로 굴리기 
  //bnum 갱신해주기 
  int tfnum=fnum; 
  int tBnum=Bnum;
  int trnum=rnum;
  int tlnum=lnum; 
  int tunum=unum;
  int tbnum=bnum; 
  if(dir==0){ //상
     //앞 <- 아래 
     fnum=tbnum;
     //뒤 <- 위 
     Bnum=tunum;
     //위 <- 앞
     unum=tfnum;
     //아래<- 뒤 
     bnum=tBnum;
  }
  else if(dir==1){ //우
    //오 <- 위 
    rnum=tunum;
    //왼<- 아래 
    lnum=tbnum;
    //위<- 왼 
    unum=tlnum;
    //아래 <- 오른
    bnum=trnum;
  }
  else if(dir==2){ //하 
    //앞 <- 위 
    fnum=tunum;
     //뒤 <- 아래  
    Bnum=tbnum;
     //위 <- 뒤
    unum=tBnum;
     //아래<-앞  
    bnum=tfnum;

  }
  else{//좌 
      //오 <- 아래 
    rnum=tbnum;
    //왼<- 위 
    lnum=tunum;
    //위<- 오른
    unum=trnum;
    //아래 <- 왼 
    bnum=tlnum;
  }

}

void Round(int turn){

  //방향 정하기 
  if(turn>1){
      if(bnum>arr[curx][cury]){
         //방향이 90도 시계방향으로 바뀜 
         dir=(dir+1)%4; 
      }
      else if(bnum<arr[curx][cury]){
        //방향이 90도 반시계방향으로 바뀜 
        dir=(dir-1+4)%4; 
      }
  }
  //주사위 굴려서 상태바꾸기 
  Roll(dir); 

  //다음위치 
  int nx=curx+dr[dir];
  int ny=cury+dc[dir]; 
  //다음위치가 격자밖이면 반대방향으로 수정하기 
  if(nx<=0 || ny<=0 || nx>n || ny>n){
    if(dir==0){
       dir=2;
    }
    else if(dir==1){
       dir=3;
    }
    else if(dir==2){
       dir=0;
    }
    else dir=1; 
    nx=curx+dr[dir];ny=cury+dc[dir];
  }
  
  //다음 위치에 도착해서 얻은 점수 계산 
  score+=Count(nx,ny); 
  curx=nx; cury=ny;  
}


int main() {
    cin>>n>>m;
    arr.assign(n+1,vector<int>(n+1,0));
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            cin>>arr[i][j]; 
        }
    }
    for(int i=1;i<=m;i++){
        Round(i);
    }
    cout<<score;

    return 0;
}