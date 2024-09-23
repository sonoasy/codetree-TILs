#include <iostream>
#include<vector> 
#include<map>

using namespace std;

int n; 
map<int,vector<int>>infos; //좋아하는 친구 
//놀이기구 
vector<vector<int>>arr;
//줄 리스트
vector<int>lists; 

int dr[4]={0,0,1,-1};
int dc[4]={1,-1,0,0}; 

struct friends{
  //좋아하는 친구 수 
  int num;
  //빈칸 갯수 
  int empty_cnt; 
  //행 
  int r;
  //열  
  int c; 
   
  bool operator<(friends right)const{
    //친구 수 많은 -> 빈칸 갯수 많은 -> 행 작 -> 열 작
    if(num>right.num)return true;
    if(empty_cnt>right.empty_cnt)return true;
    if(r<right.r)return true;
    if(c<right.c)return true;
  }
}; 

void go(){
   //각각의 대기명단에서 
   for(int i=0;i<lists.size();i++){
      //확인하려는 번호 
      int num=lists[i];
      vector<friends>tmp; 
      //격자내 모든 공간 
      for(int j=0;j<n;j++){
        for(int k=0;k<n;k++){
          if(arr[j][k]!=0)continue; //번호가 있음 
          int g=0; int c=0;   

          for(int s=0;s<4;s++){
            //(j,k) 격자에 대하여 
            int nr=j+dr[s];
            int nc=k+dc[s]; 

            if(nr<0 || nc<0 || nr>=n || nc>=n)continue; 
            //빈칸 갯수 
            if(arr[nr][nc]==0)c++;
            else{ //좋아하는 친구 수 
                for(int kk=0;kk<infos[num].size();kk++){
                    int nn=infos[num][kk];
                    if(arr[nr][nc]==nn)g++;  //0이 아닌데 좋아하는 친구 목록에 있으면 +1  
                }
            }    
          }
          
          tmp.push_back({g,c,j,k}); 

        }
      }

      cout<<num<<"일떄\n";
      for(int a=0;a<tmp.size();a++){
         cout<<tmp[a].r<<' '<<tmp[a].c<<' '<<tmp
       }

      //num이 가야 할 위치 
      arr[tmp[0].r][tmp[0].c]=num;
      cout<<num<<" "<<tmp[0].r<<','<<tmp[0].c<<"으로감\n"; 

   }




}




int main() {
     
     cin>>n; 
     arr.assign(n,vector<int>(n,0));
     int num,a,b,c,d;
     for(int i=0;i<n*n;i++){
      cin>>num>>a>>b>>c>>d; 
      infos[num].push_back(a);
      infos[num].push_back(b);
      infos[num].push_back(c);
      infos[num].push_back(d);
      lists.push_back(num);
     }
     go(); 

    return 0;
}