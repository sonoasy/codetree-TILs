#include <iostream>
#include<queue>
using namespace std;

typedef pair<int,int> ci; 
//북동남서
int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1}; 
//시계방향 - +1 반시계 -1

//남쪽 탐색목록
int sr[3]={1,2,1};
int sc[3]={-1,0,1};
//동쪽 탐색 목록 
int er[5]={-1,0,1,1,2};
int ec[5]={1,2,1,2,1};
//동쪽 탐색 목록 
int wr[5]={-1,0,1,1,2};
int wc[5]={-1,-2,-2,-1,-1};

struct gollem{
   int num; //고유 번호 
   int exit; //출구 방향 
   int r; //중심좌표 
   int c; 
};
int r,c,k;  //숲크기, 정령수 
vector<vector<int>>forest; 
vector<gollem>gollems;
int total=0; 

void Move(int num){

     int cur=gollems[num].r;
     int cuc=gollems[num].c; 
     int curexit=gollems[num].exit; 
     //이걸 계속 반복 하는건가!!!!!!!!? 

     //1.남쪽으로 이동 
   // cout<<num<<" 출발위치 "<<cur<<' '<<cuc<<'\n';

    while(1){
      
      if(cur==(r-1))break; //올수 있는데까지 다옴 
      //현재 위치에서 동서남이 비어있어야함 
      bool flag=false; 
      for(int i=0;i<3;i++){
        int nr=cur+sr[i];
        int nc=cuc+sc[i];
        
        if(nr>r || nc<=0 || nc>c){
            flag=true; // 못감
            continue;
        } 
       if(forest[nr][nc]!=0){
        flag=true; //공간 없음 
        
      //  cout<<"어디?";
       }
    //   cout<<nr<<' '<<nc<<' '<<flag<<'\n';
      }

      if(!flag){ //이동 가능 
         cur+=1;  
        
      }
      else break;
      

     }
    // cout<<"남쪽으로 이동"<<cur<<' '<<cuc<<'\n';

    
     //2.서쪽으로 이동 남 -반시계  
     while(1){
         //1,-1 지점임 
         
        if(cur==(r-1) || cuc==2)break; //못감 
        //cout<<"여기 안와?";
        bool flag=false; 
        for(int i=0;i<5;i++){
            int nr=cur+wr[i];
            int nc=cuc+wc[i];
            if(i<=1 && nr<=0){
                if(nc<0)flag=true;
                continue;
            }
            if(nr<0 || nc<=0 || nr>r || nc>c){
               flag=true; // 못감
               continue;
            } 
            if(forest[nr][nc]!=0)flag=true;
        }
        if(!flag){ //이동 가능 
         cur+=1;  
         cuc-=1; 
         //반시계 방향
         curexit=((curexit-1)+4)%4; 
      //    cout<<"서쪽으로 이동 "<<cur<<' '<<cuc<<'\n';
       }
       else break; 
      
     } 
     
     //3.동쪽으로 이동 남 -시계 
     while(1){
         //1,1 지점임 
        if(cur==(r-1) || cuc==(c-1))break; //못감 
       // cout<<"여긴안와???";

        bool flag=false; 
        for(int i=0;i<5;i++){
            int nr=cur+er[i];
            int nc=cuc+ec[i];
            //cout<<"동쪽 목록: "<<nr<<' '<<nc<<'\n';
            if(nr<=0 || nc<=0 || nr>r || nc>c){
               flag=true; // 못감
            continue;
            } 
            if(forest[nr][nc]!=0)flag=true;
        }
        if(!flag){ //이동 가능 
         cur+=1;  
         cuc+=1; 
         //반시계 방향
         curexit=((curexit+1)+4)%4; 
      //   cout<<"동쪽으로 이동 "<<cur<<' '<<cuc<<'\n';
       }
       else break; 
      
     } 
    
    gollems[num].r=cur;
    gollems[num].c=cuc;
    gollems[num].exit=curexit;    

    return; 
}

ci exitnum(int r,int c,int d){

    return {r+dr[d],c+dc[d]}; 
}

int bfs(int num){

   priority_queue<int,vector<int>,less<>>pq;
   int cur=gollems[num].r;
   int cuc=gollems[num].c; 
   //일단 골렘의 가장 아래 행 저장 
   pq.push(cur+1); 

   //골렘 방문처리 
   vector<int>visited;
   visited.assign(k+1,0);
   visited[num]=1; 

   queue<int>q;
   q.push(num);

   while(!q.empty()){
       int nn=q.front();
       int exitr=gollems[nn].r+dr[gollems[nn].exit];
       int exitc=gollems[nn].c+dc[gollems[nn].exit]; 

       q.pop();

       //인접한 상하좌우에 나와 다른 골렘이 있는가? 
       for(int i=0;i<4;i++){
           int nr=exitr+dr[i];
           int nc=exitc+dc[i]; 
           if(nr<=0 ||nc<=0 || nr>r ||nc>c)continue;
           //그냥 숲이면 패스 
           if(forest[nr][nc]==0)continue; 
           if(forest[nr][nc]!=nn && !visited[forest[nr][nc]]){ //또다른 골렘 
               visited[forest[nr][nc]]=1; 
               //해당 골렘에서 가장 낮은 위치 기록 
               pq.push(gollems[forest[nr][nc]].r+1); 
               q.push(forest[nr][nc]); 
           }
       }
       
   } 
   
   return pq.top(); //가장 높은 행 위치 
}




int main() {

    cin>>r>>c>>k;
    forest.assign(r+1,vector<int>(c+1,0)); 

    int cc,dd;
    gollems.push_back({0,0,0,0}); 
    for(int i=1;i<=k;i++){
        cin>>cc>>dd;
        gollems.push_back({i,dd,-1,cc});     
    }
    
    for(int i=1;i<=k;i++){ //차례로 골렘 움직이기 
    
      //1. 움직이기 
      Move(i); 
     // cout<<"움직이고 위치\n";
    //  cout<<gollems[i].r<<' '<<gollems[i].c<<' '<<gollems[i].exit<<'\n';
      
      //움직인 위치가 격자 밖이면 숲 정리하기 
      if(gollems[i].r<=1){
        forest.assign(r+1,vector<int>(k+1,0)); 
       // cout<<"숲정리!\n";
        continue; 
     }
      //숲에 지금 골렘 세기기 
       //중심좌표
      forest[gollems[i].r][gollems[i].c]=i;
      
      for(int j=0;j<4;j++){
        //cout<<gollems[i].r+dr[j]<<' '<<gollems[i].c+dc[j]<<'\n';
        forest[gollems[i].r+dr[j]][gollems[i].c+dc[j]]=i;
      }
      
      //정령이 움직일수 있는 최대 아래 탐색하기 
      int lastposition=bfs(i); 
     // cout<<i<<"의 최대 행:"<<lastposition<<'\n'; 
      total+=lastposition; 

    }
    cout<<total; 

    return 0;
}