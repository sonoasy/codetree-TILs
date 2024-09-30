#include <iostream>
#include<vector>
using namespace std;

typedef pair<int,int>ci; 
int dr[8]={0,-1,-1,-1,0,1,1,1};
int dc[8]={1,1,0,-1,-1,-1,0,1};

int n,m;//격자,키우는 수 
vector<vector<int>>arr; 
//영양제 격자 
vector<vector<int>>nu;

int d,p; //이동방향,칸수 
//초기 영양제 위치 
vector<ci>med; 

//대각선 4방향
int dir[4]={-1,-1,1,1};
int dic[4]={-1,1,-1,1};


int main() {
    
   cin>>n>>m; 
   arr.assign(n,vector<int>(n,0));
   nu.assign(n,vector<int>(n,0));
   for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
        cin>>arr[i][j]; 
    }
   }
   cin>>d>>p; 
   d--; 
   //초기 영양제 위치 
   for(int i=n-2;i<n;i++){
     for(int j=0;j<2;j++){
        med.push_back({i,j});
     }
   }

   for(int i=0;i<m;i++){
     //1.특수 영양제 이동
     cout<<"영양제 이동\n";
     for(int j=0;j<med.size();j++){
       int cur=med[j].first;
       int cuc=med[j].second; 
       int nr=(cur+dr[d]*p)%n;
       int nc=(cuc+dc[d]*p)%n;
       med[j].first=nr; med[j].second=nc;
       nu[nr][nc]=1; 
       cout<<nr<<" "<<nc<<"\n";
     }    
 
     //2.특수 영양제 투입 
     // 투입한 리브로수에서 씨앗이면 +1, 그 이상이면 대각선 인접한 방향 리브로수 있는 만큼 높이 올라감 격자 밖x 
     for(int j=0;j<med.size();j++){
       int cur=med[j].first;
       int cuc=med[j].second; 
       if(arr[cur][cuc]==0)arr[cur][cuc]++;
       else{
         //대각선 방향 몇개? 
         int cnt=0;
         for(int k=0;k<4;k++){
           int nr=cur+dir[k];
           int nc=cuc+dic[k];
           if(nr<0|| nr>=n || nc<0 || nc>=n)continue; 
           if(arr[nr][nc]>0)cnt++;
            cout<<cur<<" "<<cuc<<"에"<<nr<<","<<nc<<"\n";
         }
         cout<<cur<<" "<<cuc<<"에"<<cnt<<"개 있음\n";
         arr[cur][cuc]+=cnt; 
       }
     } 
    for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
        cout<<arr[i][j];
      }
      cout<<"\n";
    }

     med.clear(); 



     //3. 특수 영양제 없는 높이 2이상 리브로스는 2이상 깎고 거기에 -> 최적화 필요? 
     //새로 특수 영양제 뿌리기 
      for(int j=0;j<n;j++){
        for(int k=0;k<n;k++){
           if(nu[j][k]==1)continue; //이전에 약뿌린곳 
           if(arr[j][k]>=2){
             arr[j][k]-=2;
             med.push_back({j,k});
           }
        }
      }
      nu.assign(n,vector<int>(n,0));
      cout<<"새로운 영양제위치\n";
      for(int j=0;j<med.size();j++){
        nu[med[j].first][med[j].second]=1; 
        cout<<med[j].first<<" "<<med[j].second<<"\n";
      }

   }
   int total=0;
   for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      total+=arr[i][j]; 
      cout<<arr[i][j];
    }
    cout<<"\n";
   }
  cout<<total;



    return 0;
}