#include <iostream>
#include<vector>
using namespace std;

int dr[8]={0,-1,-1,-1,0,1,1,1};
int dc[8]={1,1,0,-1,-1,-1,0,1};

int n,m;//격자,키우는 수 
vector<vector<int>>arr; 
int d,p; //이동방향,칸수 
int main() {
    
   cin>>n>>m; 
   arr.assign(n,vector<int>(n,0));
   for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
        cin>>arr[i][j]; 
    }
   }
   cin>>d>>p; 
   for(int i=0;i<m;i++){
     //1.특수 영양제 이동


     //2.특수 영양제 투입 

     //3. 투입한 리브로수에서 씨앗이면 +1, 그 이상이면 대각선 인접한 방향 리브로수 있는 만큼 높이 올라감 격자 밖x 

     //4. 특수 영양제 없는 높이 2이상 리브로스는 2이상 깎고 거기에 특수 영양제 뿌리기 



   }


    return 0;
}