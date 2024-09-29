#include <iostream>
#include<vector>
using namespace std;
typedef pair<int,int>ci; 
int dr[8]={-1,-1,0,1,1,1,0,-1};
int dc[8]={0,-1,-1,-1,0,1,1,1};
//상좌하우 
int dir[4]={-1,0,1,0};
int dic[4]={0,-1,0,1};

int arr[5][5]={0,}; //4x4 격자 

int m,t; //몬스터 수, 턴 
int pr,pc; //팩맨
struct monster{
  int r;
  int c;
  int dir;
  bool deleted;
  int until; 
  bool vanish;
}; 
int n=4;
vector<monster>info; 

bool check(int r,int c,int d,int turn){
   bool flag=false; 
   int nr=r+dr[d]; int nc=c+dc[d]; 
   if(nr<=0 || nc<=0 || nr>n ||nc>n)flag=true;
   if(pr==nr && pc==nc)flag=true;
         //사체 
   for(int k=0;k<info.size();k++){
        if(info[k].deleted && info[k].until<=turn){
            flag=true; break; 
        }
    }
    return flag;   
}
int counts(int r,int c){
    //여기에 살아있는 몬스터가 몇개인지
    int cnt=0;
    for(int i=0;i<info.size();i++){
        if(info[i].deleted)continue; 
        cnt++;
    }
    return cnt; 
}

int main() {

   cin>>m>>t;
   cin>>pr>>pc;
   int mr,mc,md; 
   //몬스터 정보 
   
   for(int i=0;i<m;i++){
     cin>>mr>>mc>>md; 
     info.push_back({mr,mc,md-1,0,0,0}); 
   }
   for(int i=1;i<=t;i++){
      //1.몬스터 복제시도 
      vector<monster>dup; 
      for(int j=0;j<info.size();j++){
        if(!info[i].deleted){
            dup.push_back(info[i]);
        }
      }

      //2.몬스터 이동
      for(int j=0;j<info.size();j++){

         if(info[j].deleted)continue; //사체면 안움직임 
         int cur=info[j].r; int cuc=info[j].c;
         int curd=info[j].dir; 
        //격자 밖, 팩맨, 사체있음 안됨 
         //바로 갈수 있는가? 
         
         if(!check(cur,cuc,curd,i)){
            info[j].r=cur+dr[curd]; info[j].c=cuc+dc[curd]; //이 방향으로 가기 
         }
         else{ //45 반시계로 돌면서 나오는데 찾기 
             for(int s=0;s<7;s++){
                int nd=(curd+1)%8; 
                if(!check(cur,cuc,nd,i)){
                  //여기로 가기 
                  info[j].r=cur+dr[nd]; info[j].c=cuc+dc[nd];
                  info[j].dir=nd; //방향 바꿈 
                  break; 
                }
             }
         }

      }

      //3.팩맨 이동
      //64가지 
      int tr,tc; int maxs=-1; 
      ci nexts[3]; 
      ci ans[3];
      for(int j=0;j<4;j++){
        int totalmonster=0; 
        if(check(pr,pc,j,i))continue; 
        pr+=dir[j]; pc+=dic[j]; 
        nexts[0]={pr,pc};
        totalmonster+=counts(pr,pc); 
        for(int s=0;s<4;s++){
            if(check(pr,pc,s,i))continue; 
            pr+=dir[s]; pc+=dic[s]; 
             totalmonster+=counts(pr,pc); 
             nexts[1]={pr,pc};
            for(int p=0;p<4;p++){
              if(check(pr,pc,p,i))continue; 
              pr+=dir[p]; pc+=dic[p];
              totalmonster+=counts(pr,pc); 
              nexts[0]={pr,pc};
              if(maxs<totalmonster){
                maxs=totalmonster;
                tr=pr; tc=pc; 
                ans[0]=nexts[0];
                ans[1]=nexts[1];
                ans[2]=nexts[2]; 
              }
            }
        }
      }
      //여기로 이동 
      pr=tr; pc=tc; 
      //사체로 바꾸기 
      for(int j=0;j<info.size();j++){
        if(info[j].deleted)continue; 
        for(int k=0;k<3;k++){
            if(info[j].r==ans[k].first && info[j].c==ans[k].second){
                //사체로 변함 
                info[j].deleted=1;
                info[j].until=i+2; 
            }
        }
      }

      //4.시체소멸 
      //사체중에 turn이 된거는 이제 장애물이 될수 없음 
      vector<monster>newinfo; 
      for(int j=0;j<info.size();j++){
        if(info[j].vanish)continue;
        if(info[j].deleted && info[j].until==i){
            info[j].vanish=1; //완전 사라짐 
        }
        else{
          newinfo.push_back(info[j]);
        }
      }

      //5.몬스터복제 완성 
      for(int j=0;j<dup.size();j++){
        newinfo.push_back(dup[j]);
      }
      info=newinfo; 
   }
   int cnt=0;
   for(int i=0;i<info.size();i++){
      if(!info[i].deleted)cnt++;
   }
   cout<<cnt; 

    return 0;
}