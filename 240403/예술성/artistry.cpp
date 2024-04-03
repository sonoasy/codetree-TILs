#include <iostream>
#include<vector>
#include<map>

using namespace std; 
typedef pair<int,int> ci; 
vector<vector<int>>arr;
vector<vector<bool>>visited;
vector<vector<int>>group;
int n;
int totalart=0;


int dr[4]={0,1,-1,0};
int dc[4]={1,0,0,-1}; 

void dfs(int i,int j,int cnt,map<int,int>&info){
    visited[i][j]=true; 
    group[i][j]=cnt; 
    info[cnt]++; //갯수 

    for(int k=0;k<4;k++){
        int nr=i+dr[k];
        int nc=j+dc[k];
        if(nr<0 || nc<0 || nr>=n || nc>=n)continue;
        if(visited[nr][nc])continue;
        if(arr[i][j]!=arr[nr][nc])continue; 
        dfs(nr,nc,cnt,info); 
    }
}
//맞닿아 있는거
int calline(int a,int b){
   int cnt=0; 
   //오른쪽, 아래 확인 
   for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
       for(int k=0;k<2;k++){
         int nr=i+dr[k];
         int nc=j+dc[k]; 
         if(nr<0 || nc<0 || nr>=n || nc>=n)continue; 
         if((group[i][j]==a && group[nr][nc]==b) || (group[i][j]==b && group[nr][nc]==a)){
            cnt++;
         }
          
       }
    }
   }

   return cnt; 
}

//예술 점수 계산 
int calArt(map<int,int>&info,map<int,int>&num){ 

    //그룹 생성 - 숫자, 속한 칸 수, 위치정보 저장 ? 
    int cnt=0; 
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(!visited[i][j]){
                dfs(i,j,cnt,info); 
                num[cnt]=arr[i][j]; 
                cnt++; 
            }
        }
    }
    //그룹중 2개씩 정해서 점수 더하기 - 조합 뭐였더라 
    //cnt-1번까지 있음 
     int total=0; 
     for(int i=0;i<cnt-1;i++){
        for(int j=i+1;j<=cnt-1;j++){
            int sum=0; 
            sum+=info[i]+info[j]; 
            sum*=(num[i]*num[j]); 
            sum*=calline(i,j); 
            total+=sum; 
        }
     }

    return total; 
}
//격자 회전하기 

void rotate(){

   //십자 회전 
   vector<int>tmp1; 
   vector<int>tmp2; 
   //세로 
   for(int i=0;i<n;i++){
     tmp1.push_back(arr[i][(n-1)/2]); //세로 
     tmp2.push_back(arr[(n-1)/2][n-1-i]);//가로 
   }
   for(int i=0;i<n;i++){
    arr[(n-1)/2][i]=tmp1[i]; //가로에 세로를 
    arr[i][(n-1)/2]=tmp2[i]; //세로에 가로를 
   } 
  
   //정사각형 4개 회전 
   int size=(n-1)/2; 
   int r,c; 
   int dx[2]={0,size+1}; 
   int dy[2]={0,size+1}; 

  
   for(int i=0;i<2;i++){
     for(int j=0;j<2;j++){
         //정사각형 상단 
         r=dx[i]; c=dy[j]; 
         //시계방향 회전  
         vector<vector<int>>t; //0....size-1 
         t.assign(size,vector<int>(size,0));       
         for(int k=r;k<=r+size-1;k++){
           for(int ss=c;ss<=c+size-1;ss++){
              t[k-r][ss-c]=arr[k][ss];
           } 
         } 
         //t에 옮긴거 돌려서 옮기기 
         for(int s=c+size-1;s>=c;s--){
           for(int ss=r;ss<=r+size-1;ss++){
             arr[ss][s]=t[c+size-1-s][ss-r]; 
           }
         }
     }
   } 
 
}

int main() {

    cin>>n; 
    arr.assign(n,vector<int>(n,0));
    visited.assign(n,vector<bool>(n,0));
    group.assign(n,vector<int>(n,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>arr[i][j]; 
        }
    }
    map<int,int>info1;
    map<int,int>num1;

    totalart+=calArt(info1,num1); 
    
    for(int i=0;i<3;i++){
       visited.assign(n,vector<bool>(n,0));
       group.assign(n,vector<int>(n,0));
       rotate(); 
       map<int,int>info; 
       map<int,int>num; 

       int sum=calArt(info,num); 

       totalart+=sum;
    }

    cout<<totalart; 

    return 0;
}