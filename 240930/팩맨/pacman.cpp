#include <iostream>
#include<vector>
#include<map> 

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
map<ci,int>deadmonster; 


bool check(int r,int c,int d,int turn){
   bool flag=false; 
   int nr=r+dr[d]; int nc=c+dc[d]; 
 //  cout<<r<<","<<c<<'\n';
  // cout<<d<<"일때 "<<nr<<","<<nc<<'\n';
   if(nr<=0 || nc<=0 || nr>n ||nc>n){ //격자 밖
    flag=true;
    return flag;
   }
   if(pr==nr && pc==nc){ //팩맨 있을떄
    flag=true;
    return flag;      
   }     

   //이거를 O(1)로 하는 방법? 
  //사라지지 않은 사체가 있는경우 
  //map[r][c].cnt++; 
   if(deadmonster[{r,c}]>0)return true;
  // for(int k=0;k<info.size();k++){
      // cout<<"안와??";
     //  cout<<info[k].r<<","<<info[k].c<<" "<<info[k].deleted<<'\n';
    //    if(!info[k].vanish && info[k].deleted && info[k].r==nr && info[k].c==nc){
         //   cout<<"안와??2";
      //   flag=true;
        // return flag;  
        //  break;
        //}
   // }
    
    return flag;   
}
int counts(int r,int c){
    //여기에 살아있는 몬스터가 몇개인지
    int cnt=0;
    for(int i=0;i<info.size();i++){
        if(info[i].deleted)continue; //사체+사라진사체 
        if(info[i].r==r && info[i].c==c)cnt++;
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
        if(!info[j].deleted){
            dup.push_back(info[j]);
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

            //1,2로 가면 안된다고!!!!!!!!!!!!!!!!!!!!!
         //    cout<<cur<<","<<cuc<<" "<<curd<<"탐색\n";
         }
         else{ //45 반시계로 돌면서 나오는데 찾기 
             for(int s=1;s<=7;s++){
                int nd=(curd+s)%8; 
               // cout<<cur<<","<<cuc<<" "<<nd<<"탐색\n";
                if(!check(cur,cuc,nd,i)){
                  //여기로 가기 
                  info[j].r=cur+dr[nd]; info[j].c=cuc+dc[nd];
                  info[j].dir=nd; //방향 바꿈 
                  break; 
                }
             }
         }
       //  cout<<"몬스터 "<<info[j].r<<","<<info[j].c<<"로감 방향:"<<info[j].dir<<'\n';
      }

      //3.팩맨 이동
      //64가지 
      int tpr=pr; int tpc=pc; 
      int maxs=-1;
      ci ans[3];
      //사체처리 안함? 
      for(int j=0;j<4;j++){
        int nr1=pr+dir[j]; int nc1=pc+dic[j];
        if(nr1<=0 || nc1<=0 || nr1>4 || nc1>4)continue; 
        int cnt1=counts(nr1,nc1);
        for(int k=0;k<4;k++){
            int nr2=nr1+dir[k]; int nc2=nc1+dic[k]; 
            if(nr2<=0 || nc2<=0 || nr2>4 || nc2>4)continue; 
            int cnt2=counts(nr2,nc2);
            for(int p=0;p<4;p++){
              int nr3=nr2+dir[p]; int nc3=nc2+dic[p]; 
              if(nr3<=0 || nc3<=0 || nr3>4 || nc3>4)continue; 
              int cnt3=counts(nr3,nc3);
              //중복일 경우 제외해주기 
              if(nr1==nr2 && nc1==nc2){
                 cnt3+=cnt1;
              }
              else if(nr1==nr3 && nc1==nc3){
                 cnt3+=cnt2;
              }
              else if(nr2==nr3 && nc2==nc3){
                cnt3+=cnt1;
              }
              else cnt3+=(cnt1+cnt2);

              if(maxs<cnt3){
                maxs=cnt3;
                ans[0]={nr1,nc1}; ans[1]={nr2,nc2}; ans[2]={nr3,nc3};
                tpr=nr3; tpc=nc3; 
             //   cout<<nr1<<","<<nc1<<" "<<nr2<<","<<nc2<<" "<<nr3<<","<<nc3<<"순으로 움직임\n";
             //   cout<<cnt3<<"있음\n";
              }
            }
        }
      }
     // cout<<ans[0]<<" "<<ans[1]<<" "<<ans[2]<<"순으로 움직임\n"; 
     // cout<<maxs<<"몬스터 잡음\n";
   //  cout<<"팩맨"<<tpr<<" "<<tpc<<"로 움직임\n";
      pr=tpr; pc=tpc;
      //몬스터 죽이기 
      for(int j=0;j<3;j++){
         for(int k=0;k<info.size();k++){
            if(info[k].r==ans[j].first && info[k].c==ans[j].second){
                if(info[k].deleted)continue;
                info[k].deleted=1;
                info[k].until=i+2; 
                deadmonster[{info[k].r,info[k].c}]++;
           //    cout<<info[k].r<<","<<info[k].c<<"죽임\n";
            }
         }  

      }
      
     

      //4.시체소멸 
      //사체중에 turn이 된거는 이제 장애물이 될수 없음 
     // vector<monster>newinfo; 
      for(int j=0;j<info.size();j++){
        if(info[j].vanish)continue;
        if(info[j].deleted){

          if(info[j].until==i){
            info[j].vanish=1; //완전 사라짐 
            deadmonster[{info[j].r,info[j].c}]--;
          }
      //    newinfo.push_back(info[j]);
        }
      }

      //5.몬스터복제 완성 
      for(int j=0;j<dup.size();j++){
        info.push_back(dup[j]);
      }
      //info.clear();
      //info=newinfo; 
     // cout<<i<<"턴 후 살아있는 몬스터 목록\n";
     
   }

   //사체가 아닌 몬스터 
   int cnt=0;
   for(int i=0;i<info.size();i++){
      if(!info[i].deleted)cnt++;
   }
   cout<<cnt; 

    return 0;
}