#include <iostream>
#include<map>
#include<vector>
#include<queue> 
#include<set> 
using namespace std;

typedef pair<int,int>ci; 
//위오아왼
int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1}; 

struct knight{
 int r;
 int c;
 int h;
 int w;
 int power;//체력
 bool deleted; 
 int damage;
};
//벽 함정 배열 
vector<vector<int>>arr;
//기사 이동 배열 
vector<vector<int>>info; 
int l,n,q;  //배열 크기,기사 수,명령 수 
//기사 정보 id
map<int,knight>person; 

void bfs(int id,int dir){
  //id 기사를 dir방향으로 이동 
  //{id 별, 데미지 현황=함정 갯수 }  
  vector<ci>tmp; 
  queue<int>qq; 
  qq.push(id); 
  bool flag=true; 
  while(!qq.empty()){
     int cid=qq.front();
     qq.pop(); 
     //dir방향으로 맨끝을 탐색하기 
    
     int r=person[cid].r;
     int c=person[cid].c;
     int h=person[cid].h;
     int w=person[cid].w;  

     //만약 벽이 있거나 격자 밖이면 바로 out 
           //2         l
     //함정이거나 = arr-> 1 아무것도 없거나 arr ->0 
     //기사이거나 -> info -> 0보다 큼, 단 id가 아님 
     //cid가 id가 아닌 경우에만 damage를 계산하기 
     //flag가 false가 되지 않았을 경우, search에 id,damage를 넣어주고 
     //기사인 목록을 큐에 넣어준다  
      
     int damage=0;  
     set<int>nid; //기사의 유무 
     if(dir==0){ //상
        for(int i=c;i<=c+w-1;i++){
            //info와 arr 둘다 확인 
            //r-1 
            int nr=r-1;
            int nc=i; 
            if(nr<=0 || nc<=0 || nr>l  || nc>l || arr[nr][nc]==2){
              // cout<<"못움직임\n"; 
              flag=false; return;
            }
            if(cid!=id && arr[nr][nc]==1)damage++; //함정일 경우 
            if(info[nr][nc]>0)nid.insert(info[nr][nc]); 
            
        }

     }
     if(dir==1){ //우 
         for(int i=r;i<=r+h-1;i++){
            //c+1 
            int nr=i;
            int nc=c+w; 
            if(nr<=0 || nc<=0 || nr>l  || nc>l || arr[nr][nc]==2){
               //cout<<"못움직임\n"; 
               flag=false;  return;
            }
            if(cid!=id && arr[nr][nc]==1)damage++; //함정일 경우 
            if(info[nr][nc]>0)nid.insert(info[nr][nc]); 
         }
     }
     if(dir==2){//하 
         for(int i=c;i<=c+w-1;i++){
            //r+w 
            int nr=r+h;
            int nc=i; 
            if(nr<=0 || nc<=0 || nr>l  || nc>l || arr[nr][nc]==2){
               //cout<<"못움직임\n";
               flag=false;  return;
            }
            if(cid!=id && arr[nr][nc]==1)damage++; //함정일 경우 
            if(info[nr][nc]>0){
                nid.insert(info[nr][nc]); 
                
               // cout<<nr<<","<<nc<<" "<<info[nr][nc]<<"추가";
            }
         }
     }
     if(dir==3){//좌 
         for(int i=r;i<=r+h-1;i++){
            //c-1 
            int nr=i;
            int nc=c-1; 
            if(nr<=0 || nc<=0 || nr>l  || nc>l || arr[nr][nc]==2){
              // cout<<"못움직임\n"; 
              flag=false; return;
            }
            if(cid!=id && arr[nr][nc]==1)damage++; //함정일 경우 
            if(info[nr][nc]>0)nid.insert(info[nr][nc]); 
         }
     }
     if(!flag)return;
     if(!flag)break; 
      //flag가 false가 되지 않았을 경우, search에 id,damage를 넣어주고 
      tmp.push_back({cid,damage}); 
     //기사인 목록을 큐에 넣어준다  
      for(auto ss:nid){
       // cout<<"움직일 기사:"<<ss<<"\n";
        qq.push(ss); 
      }
  }

  //이동하기 - flag가 true일 경우에만 
  if(!flag)return; 
  //person 갱신 
  for(int i=0;i<tmp.size();i++){
    int id=tmp[i].first;
    int damage=tmp[i].second;
    person[id].r+=dr[dir];
    person[id].c+=dc[dir];
    person[id].damage+=damage;
    person[id].power-=damage; 
   // cout<<id<<"에 "<<damage<<"가해져서"<<person[id].power<<"남음\n";
    if(person[id].power<=0)person[id].deleted=1; 
  }
  //info 갱신 
  info.assign(l+1,vector<int>(l+1,0)); 
  //delete된거 삭제하기 
  for(int i=1;i<=n;i++){
     if(person[i].deleted)continue; 
     int r=person[i].r;
     int c=person[i].c;
     int h=person[i].h;
     int w=person[i].w;  
     for(int j=r;j<r+h;j++){
        for(int k=c;k<c+w;k++){
            info[j][k]=i; 
        }
     }
  }
  

}

int main() {

    cin>>l>>n>>q; 
    arr.assign(l+1,vector<int>(l+1,0)); 
    info.assign(l+1,vector<int>(l+1,0)); 
    for(int i=1;i<=l;i++){
        for(int j=1;j<=l;j++){
            cin>>arr[i][j]; 
        }
    }
    int r,c,h,w,k; 
    for(int i=1;i<=n;i++){
       cin>>r>>c>>h>>w>>k; 
       person[i]={r,c,h,w,k,0,0}; 
       //info배열에 저장 
       for(int j=r;j<r+h;j++){
        for(int k=c;k<c+w;k++){
            info[j][k]=i; 
        }
       }

    }
    //id,방향 
    int id,dir;
    for(int i=1;i<=q;i++){
        cin>>id>>dir; 
       // cout<<i<<"번째에 현황\n";
       // for(int j=1;j<=l;j++){
        //    for(int k=1;k<=l;k++){
          //      cout<<info[j][k];
         //   }
            //cout<<'\n';
       //}
        //id번 dir 방향으로 이동 
        bfs(id,dir); 
    }
    int total=0;
    for(int i=1;i<=n;i++){
       if(!person[i].deleted) total+=person[i].damage;
    }
    cout<<total;

   
    return 0;
}