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
  
   //초기 영양제 위치 
   for(int i=n-2;i<n;i++){
     for(int j=0;j<2;j++){
        med.push_back({i,j});
     }
   }
   

   for(int i=0;i<m;i++){
     //1.특수 영양제 이동
   //  cout<<"영양제 이동\n";
    cin>>d>>p; //매년 다른 이동방향 
    d--; 
     for(int j=0;j<med.size();j++){
       int cur=med[j].first;
       int cuc=med[j].second; 
       int nr=(cur+dr[d]*p+2*n)%n; //음수가 될수도 있음// 
       int nc=(cuc+dc[d]*p+2*n)%n;
       med[j].first=nr; med[j].second=nc;
       arr[nr][nc]++; //1씩자람 
      // cout<<nr<<" "<<nc<<"\n";
       nu[nr][nc]=1; //나중에 여기 제외하고 영양제 투입하려고 
     }      
     //2.특수 영양제 대각선 방향  

     for(int j=0;j<med.size();j++){
       int cur=med[j].first;
       int cuc=med[j].second; 
      
         //대각선 방향 몇개? 
         int cnt=0;
       //  cout<<cur<<","<<cuc<<"탐색\n";
         for(int k=0;k<4;k++){
           int nr=cur+dir[k];
           int nc=cuc+dic[k];
           if(nr<0|| nr>=n || nc<0 || nc>=n)continue; 
          // cout<<nr<<","<<nc<<"\n";
           if(arr[nr][nc]>0)cnt++;
            //cout<<cur<<" "<<cuc<<"에"<<nr<<","<<nc<<"\n";
         }
         //cout<<cur<<" "<<cuc<<"에"<<cnt<<"개 있음\n";
         arr[cur][cuc]+=cnt; 
     } 
     //arr=tmp;
    
    
     //med.clear(); 
     vector<ci>new_med;
     //3. 특수 영양제 없는 높이 2이상 리브로스는 2이상 깎고 거기에 -> 최적화 필요? 
     //새로 특수 영양제 뿌리기 
      for(int j=0;j<n;j++){
        for(int k=0;k<n;k++){
           if(nu[j][k]==1)continue; 
           if(arr[j][k]>=2){
             arr[j][k]-=2;
             new_med.push_back({j,k}); //새롭게 영양제 투입할곳 
           }
        }
      }
      nu.assign(n,vector<int>(n,0));
      med=new_med;
    
   }

   int total=0;
   for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      total+=arr[i][j]; 
    }
   }
  cout<<total;



    return 0;
}