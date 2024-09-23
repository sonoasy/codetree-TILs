#include <iostream>
#include<map>
#include<vector>
#include<algorithm>

using namespace std;

int dr[4]={0,0,1,-1};
int dc[4]={1,-1,0,0};
 //격자에서 4방향으로 자기가 좋아하는 친구 수 가장 많은곳 찾기 
 //인접한 칸 비어있는 수 많은 위치 (격자 밖x)
 //행 작 -> 열 작 
struct friends{
    int good_num;
    int empty_cnt;
    int r;
    int c;
   // bool operator<(friends right)const {
        //min heap 
    //    if (good_num>right.good_num)return true; 
    //    if (empty_cnt>right.empty_cnt)return true;
    //    if (r<right.r)return true;
    //    if (c<right.c)return true;
  //  }
    //잘 모르겟...     
};
bool cmp(friends a,friends b){
    
    if(a.good_num==b.good_num){
       if(a.empty_cnt==b.empty_cnt){
         if(a.r==b.r){
            return a.c<b.c;
         }
         return a.r<b.r;
       }
       return a.empty_cnt>b.empty_cnt;
    }
    return a.good_num>b.good_num;
}

//좋아하는 친구 목록 
map<int,vector<int>>infos;
int n; //격자 크기 
//놀이기구 
vector<vector<int>>arr;
vector<int>lines;
void go(){

   for(int l=0;l<lines.size();l++){  
    int num=lines[l]; //지금 놓으려는 번호 

    vector<friends>tmp; 
   
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(arr[i][j]>0)continue;
           //좋아하는 친구 수
           //비어있는 수 
            int g=0;
            int e=0;
           for(int k=0;k<4;k++){
             int nr=i+dr[k];
             int nc=j+dc[k];
             if(nr<0 || nc<0 || nr>=n || nc>=n)continue; 
             if(arr[nr][nc]>0){
                //누군가 있다 
                for(int s=0;s<infos[num].size();s++){
                    if(arr[nr][nc]==infos[num][s])g++;
                }
             }
             else{ //빈자리 세기 
                e++;
             } 
           }
           tmp.push_back({g,e,i,j});
           
        }
    }
    sort(tmp.begin(),tmp.end(),cmp);

      //가장 첫번째 자리로 감 
      arr[tmp[0].r][tmp[0].c]=num;
   }
}



int main() {

    
    cin>>n;
    arr.assign(n,vector<int>(n,0));
    int num;
    int a,b,c,d; 
    for(int i=0;i<n*n;i++){
       cin>>num>>a>>b>>c>>d;
       infos[num].push_back(a);
       infos[num].push_back(b);
       infos[num].push_back(c);
       infos[num].push_back(d);
       lines.push_back(num); 
    }
    go();
    int ans=0; 
    int score[5]={0,1,10,100,1000};
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
           int cnt=0; 
           int nums=arr[i][j]; 
           //
           for(int k=0;k<4;k++){
              int nr=i+dr[k];
              int nc=j+dc[k];
              if(nr<0 || nc<0 || nr>=n || nc>=n)continue; 
              for(int s=0;s<infos[nums].size();s++){
                 if(arr[nr][nc]==infos[nums][s])cnt++; 
              }
           }
           ans+=score[cnt];

        }
    }
    cout<<ans;
    return 0;
}