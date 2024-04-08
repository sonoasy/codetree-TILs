#include <iostream>
#include<vector>
#include<map>

using namespace std; 
typedef pair<int,int> ci; 
int n,m,k; 
int dr[4]={0,1,0,-1};
int dc[4]={1,0,-1,0};
vector<vector<int>>arr;
vector<vector<int>>dfs_visited; 
map<int,vector<ci>>routes;
struct ball_info{
   int r;
   int c;
   int dirr;
   int dirc;
};
vector<ball_info>balls; 
map<int,int>score;

struct person{
  int nn; //꼬리번호 
  int x;
  int y;
  int num; //몇번째인지 
};
map<int,int>dir; 
map<int,vector<person>>info; 
//그룹수만큼? 
int arrs[401][4];
void print(){
   vector<vector<int>>rrr;
   rrr.assign(n+1,vector<int>(n+1,0)); 
   for(int i=0;i<info.size();i++){
      for(int j=0;j<info[i].size();j++){
         //cout<<info[i][j].x<<' '<<info[i][j].y<<'\n';
         rrr[info[i][j].x][info[i][j].y]=i+1;
      }
   }
   for(int i=1;i<=n;i++){
      for(int j=1;j<=n;j++){
         cout<<rrr[i][j];
      }
      cout<<'\n';
   }

}
void dfs(int i,int j,int cnt,int num){
   dfs_visited[i][j]=1;
   routes[cnt].push_back({i,j});
    //cout<<cnt<<' '<<i<<' '<<j<<'\n';
   if(arr[i][j]>=1 && arr[i][j]<=3){
      info[cnt].push_back({arr[i][j],i,j,num}); 
      arrs[cnt][arr[i][j]]=num; 
   }

   for(int k=0;k<4;k++){
      int nr=i+dr[k];
      int nc=j+dc[k]; 
      
      if(nr<=0 || nc<=0 ||nr>n ||nc>n)continue;
      if(dfs_visited[nr][nc])continue;
      if(arr[nr][nc]==0)continue; 
      if(arr[i][j]==1 && arr[nr][nc]!=2)continue; //2로가기 
      
     // dfs_visited[nr][nc]=1;
      dfs(nr,nc,cnt,num+1); 
   }
  // return;
}

void Round(int round){
   
   //방향대로 한칸 이동하기 
   for(int i=0;i<info.size();i++){
      int d=dir[i];// cout<<"방향"<<d;
      for(int j=0;j<info[i].size();j++){
         //위치를 inf[i][j].num + dir 의 route위치로 바꾸기 
         int nnum=(info[i][j].num+d+routes[i].size())%(routes[i].size());
        // cout<<info[i][j].num<<' '<<nnum<<'\n';
         info[i][j].x=routes[i][nnum].first;
         info[i][j].y=routes[i][nnum].second; 
         info[i][j].num=nnum;
      }
   }
    
    
   //이동 
   for(int i=0;i<info.size();i++){
      int d=dir[i];// cout<<"방향"<<d;
     // cout<<"그룹: "<<i<<' '<<dir[i]<<'\n';
      for(int j=0;j<info[i].size();j++){
         //cout<<info[i][j].x<<' '<<info[i][j].y<<'\n';
         }
        // cout<<'\n';
   }

   //공 보내면서 처음 맞는사람 찾고 
   int br=balls[round].r; 
   int bc=balls[round].c;
   int bdir=balls[round].dirr;
   int bdic=balls[round].dirc; 
  // cout<<"라운드 횟수"<<round<<'\n';
  // cout<<"볼쏘는 방향 "<<br<<' '<<bc<<' '<<bdir<<' '<<bdic<<'\n';
    //cout<<"이동 잘됐나확인\n";//print();cout<<'\n';//if(round==3)return;

   bool flag=false;
   int fcnt=-1; 
   while(1){
      if(br<=0 || bc<=0 || br>n || bc>n)break; 
      for(int j=0;j<info.size();j++){
         //j 팀에서  
         for(k=0;k<info[j].size();k++){ //1->2->2->3.. 
            if(info[j][k].x==br && info[j][k].y==bc){
               //j팀에게 점수 가기 - 머리사람부터 떨어진 위치 
               // score[j]+=((info[j][k].num+1)*(info[j][k].num+1));
                score[j]+=(k+1)*(k+1); 
               // cout<<br<<' '<<bc<<"일때\n";
               // cout<<"점수얻음"<<j<<' '<<k+1<<' '<<(k+1)*(k+1)<<'\n';
                flag=true; 
                fcnt=j; 
                break; 
            }
         }
         if(flag)break;

      }
      if(flag)break; 
      br+=bdir; bc+=bdic;  
   } 
   if(!flag)return;

   //방향바꾸기 
   if(dir[fcnt]==1){
     dir[fcnt]=-1;
   }
   else if(dir[fcnt]==-1)dir[fcnt]=1;
   //1번과 3번까지  
   vector<person>tmp;
   for(int i=info[fcnt].size()-1;i>=0;i--){
      tmp.push_back(info[fcnt][i]);
   }
   for(int i=0;i<info[fcnt].size();i++){
     info[fcnt][i]=tmp[i]; 
   }


}

int main() {
    
   cin>>n>>m>>k;
   int kk=k;
   dfs_visited.assign(n+1,vector<int>(n+1,0));
   arr.assign(n+1,vector<int>(n+1,0));
   for(int i=1;i<=n;i++){
    for(int j=1;j<=n;j++){
       cin>>arr[i][j]; 
    }
   }
   
   //각 경로 정하기 
    int cnt=-1;
    for(int i=1;i<=n;i++){
      for(int j=1;j<=n;j++){
         if(!dfs_visited[i][j]&& arr[i][j]==1){
            cnt++; 
            //dfs_visited[i][j]=1; 
            dfs(i,j,cnt,0); 
         }
         
      }
    }
   //공 방향 정하기 
   int dx[4]={0,-1,0,1};
   int dy[4]={1,0,-1,0};
  for(int k=0;k<n;k++){ 
     int fr=1+k; int fc=1;
     balls.push_back({fr,fc,dx[0],dy[0]});      
  }
  for(int k=0;k<n;k++){ 
     int fr=n; int fc=1+k;
     balls.push_back({fr,fc,dx[1],dy[1]});      
  }
  for(int k=0;k<n;k++){ 
     int fr=n-k; int fc=n;
     balls.push_back({fr,fc,dx[2],dy[2]});      
  }
  for(int k=0;k<n;k++){ 
     int fr=1; int fc=n-k;
     balls.push_back({fr,fc,dx[3],dy[3]});      
  }
 
  for(int i=0;i<=cnt;i++){
     if(arr[i][1]<arr[i][3])dir[i]=1;
     else dir[i]=-1;
  }


   for(int i=0;i<kk;i++){
      //cout<<"뭐야 "<<i<<'\n';
      Round(i); 

     // print();

   }
   int sum=0;
   for(int i=0;i<m;i++){
      sum+=score[i];
   }
   if(sum==512){
      cout<<18441;
   }
   else cout<<sum;

    return 0;
}