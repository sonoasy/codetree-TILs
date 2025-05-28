#include <iostream>
#include<vector>
#include<queue>
#include<algorithm> 
#include<map>

using namespace std;

typedef pair<int,int> ci; 

int dr[4]={0,0,1,-1};
int dc[4]={1,-1,0,0};

int n,q;
int lr,lc,rr,rc; 

//투입 덩어리 
struct info{
   int time;
   vector<ci>lists;
};

vector<vector<int>>arr;

info go(int r,int c,int time,vector<vector<int>>&visited){
    
    queue<ci>q; 
    visited[r][c]=1; 
    q.push({r,c});
    vector<ci>members;
    members.push_back({r,c});

    while(!q.empty()){
      int cr=q.front().first;
      int cc=q.front().second; 
      q.pop();

      for(int i=0;i<4;i++){
         int nr=cr+dr[i];
         int nc=cc+dc[i];
         if(nr<0 || nc<0 || nr>=n || nc>=n)continue;
         if(visited[nr][nc])continue; 
         if(arr[nr][nc]!=time)continue; 
         q.push({nr,nc});   
         visited[nr][nc]=1; 
         members.push_back({nr,nc});
      }
    }
    info tmp; 
    tmp={time,members};
    return tmp;  
}
void SCORE(vector<vector<int>>arr,vector<info>nextlists){

    //map으로 바꾸기 
    map<int,info>lasts;
    for(int i=0;i<nextlists.size();i++){
        lasts[nextlists[i].time]=nextlists[i]; 
    }
   
    map<ci,int>cnt; 

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(arr[i][j]==0)continue; 
            for(int k=0;k<4;k++){
               int nr=i+dr[k];
               int nc=j+dc[k]; 
               if(nr<0 || nc<0 || nc>=n || nr>=n)continue; 
               if(arr[nr][nc]==0)continue;
               if(arr[i][j]!=arr[nr][nc]){
                   int a=arr[i][j];
                   int b=arr[nr][nc]; 
                   if(a<b){
                      cnt[{a,b}]++;
                   }
                   else{
                      cnt[{b,a}]++; 
                   }

               }
            } 

        }
    }
    //lasts 1보다 큰거 값 계산 
    int total=0; 
    for(auto it=cnt.begin();it!=cnt.end();it++){
       if(it->second>0){
          int n1=lasts[it->first.first].lists.size();
          int n2=lasts[it->first.second].lists.size();
          total+=n1*n2;
       }

    }
    cout<<total<<"\n";
 
}


void MOVE(vector<info>res){
 
   //res-> 찍어서 보기 
   vector<vector<int>>gar;
   gar.assign(n,vector<int>(n,0));
   for(int i=0;i<res.size();i++){
      for(int j=0;j<res[i].lists.size();j++){
         gar[res[i].lists[j].first][res[i].lists[j].second]=res[i].time;
      }
   }
   
   //순서대로 x 좌표 가장 가까운데에 옮겨보기 
   //x작 -> y작 
   //열 작 -> 행 작 
   //arr로 갱신할 판 
   vector<vector<int>>change;
   change.assign(n,vector<int>(n,0)); 
   vector<info>nextlists; 


   for(int i=0;i<res.size();i++){
       vector<ci>tmp=res[i].lists;
       int num=res[i].time;  
       
       //tmp의 첫번째 요소가 r,c 로이동 - 그 증감만큼 이동! 
       //그때 모든 좌표가 있을수 있는가? -> ok!  

       vector<ci>changed; 
       vector<ci>t;
       bool finded=false; 
       for(int c=0;c<n;c++){
          for(int r=n-1;r>=0;r--){ 
               changed=t;
               int changer=tmp[0].first-r; 
               int changec=tmp[0].second-c; 

               bool flag=true; 
               //////////////////////////////////
               for(int i=0;i<tmp.size();i++){
                  int cr=tmp[i].first;
                  int cc=tmp[i].second; 
                  //증감만큼 옮겨서 
                  int nr=cr-changer;
                  int nc=cc-changec; 
                  //범위 밖이면 탈락 
                  if(nr<0 || nc<0 || nr>=n || nc>=n){
                    flag=false; break; 
                  }
                  //누가 있으면 탈락 
                  if(change[nr][nc]){
                    flag=false; break; 
                  }
                  changed.push_back({nr,nc}); 
               }
               //////////////////////////////////
               if(flag){//성공적으로 됨
                  finded=true; 
                  break; 
                  //나가기 
               }
               //아니면 다음 케이스를 찾기 
            }
            if(finded)break; 
        }
       //이동 가능한거면 change 배열에 반영하기 - num임 
        if(finded){
           for(int k=0;k<changed.size();k++){
              change[changed[k].first][changed[k].second]=num; 
           }
           //그리고 바뀐 배열 목록에 넣기 
           nextlists.push_back({num,changed}); 
        }
        
   }
   arr=change; 
   
   //디버깅용 현재상태 
   /*
   for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
         cout<<arr[i][j];
      }
      cout<<"\n";
   }
*/

   //arr, 
   SCORE(arr,nextlists);  

}

bool cmp(info a,info b){
    if(a.lists.size()==b.lists.size()){
        return a.time<b.time; 
    }
    return a.lists.size()>b.lists.size(); 
}

bool cmp2(ci a,ci b){
    if(a.second==b.second){
        return a.first>b.first; 
    }
    return a.second<b.second;
}

void INPUT(int time,int lr,int lc,int rr,int rc){
   //cout<<rr<<" "<<lr<<" "<<lc<<" "<<rc<<"\n";
   //일단 채우기 
   for(int i=rr;i<lr;i++){
    for(int j=lc;j<rc;j++){
      arr[i][j]=time; 
    }
   } 

   //둘로 나눠진거 있으면 지우기 
   //덩어리 탐색  
   vector<vector<int>>visited;
   visited.assign(n,vector<int>(n,0)); 
    
   //시간별 덩어리 저장 
   map<int,vector<info>>groups; 

   for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
       if(!visited[i][j]){
          info tmp=go(i,j,arr[i][j],visited);
          groups[arr[i][j]].push_back(tmp);
       }
    }
   }
   
   vector<info>res;
   //덩어리 목록 관리 
   for(auto it=groups.begin();it!=groups.end();it++){
      //두덩어리이상이면 지우기 
      int times=it->first;
      if(groups[times].size()==1){ 
         
         res.push_back(groups[times][0]);
      }
   }
   //사이즈 큰 순으로 정렬하기 
   sort(res.begin(),res.end(),cmp); 
   //덩어리 내부도 열작-행작으로 정렬
   for(int i=0;i<res.size();i++){
     sort(res[i].lists.begin(),res[i].lists.end(),cmp2); 
   }

   


   //배양 용기 옮기기 
   MOVE(res);
   
}


int main() {

    
    cin>>n>>q;
    
    arr.assign(n,vector<int>(n,0));

    for(int time=1;time<=q;time++){
        cin>>lc>>lr>>rc>>rr;     
        //미생물 투입 
        INPUT(time,n-lr,lc,n-rr,rc); 
    }  
 
    return 0;
}