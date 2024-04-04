#include <iostream>
#include<vector>
#include<queue>
#include<map>

using namespace std;

int n,m,k; 
int x,y,d,s; 
//상 우 하 좌 
int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1}; 
typedef pair<int,int> ci;

struct player{
    int x;
    int y; 
    int gun; 
    int firstpower; 
    int dir; 
    int point;
};
vector<player>players; 
map<ci,priority_queue<int,vector<int>,less<int>>>gun_info; 

void getGun(int i){
    int mygun=players[i].gun; 
    if(mygun==0){ 
        if(gun_info[{players[i].x,players[i].y}].size()>=1){
            players[i].gun=gun_info[{players[i].x,players[i].y}].top();
            gun_info[{players[i].x,players[i].y}].pop(); 
        } 
       //  cout<<i<<"총획득함"<<players[i].gun<<'\n';
    }
    else{
        if(gun_info[{players[i].x,players[i].y}].size()>=1){
            if(mygun<gun_info[{players[i].x,players[i].y}].top()){
                players[i].gun=gun_info[{players[i].x,players[i].y}].top();
                gun_info[{players[i].x,players[i].y}].pop(); 
                gun_info[{players[i].x,players[i].y}].push(mygun); 
            }
           
        }
    } 
}


void Fight(int i,int fightnum){
   int num1=players[i].firstpower+players[i].gun; 
   int num2=players[fightnum].firstpower+players[fightnum].gun; 
   int winner,loser;

   if(num1>num2){
      //i가 포인트 가져가고 
      players[i].point+=num1-num2;   
      winner=i; loser=fightnum;
   }
   else if(num1<num2){
      //fightnum가 포인트 가져가고 
      players[fightnum].point+=num2-num1;
      winner=fightnum; loser=i;
   }
   else{
      //초기 능력치 
      if(players[i].firstpower<players[fightnum].firstpower){
         winner=fightnum; loser=i;       
      }
      if(players[i].firstpower>players[fightnum].firstpower){
        winner=i; loser=fightnum;
      }
   }
  // cout<<"승자: "<<winner<<"패자:"<<loser<<'\n';
   //loser은 총버리고 이동하고 간데에 총있으면 줍기 
   if(players[loser].gun>0){
      gun_info[{players[loser].x,players[loser].y}].push(players[loser].gun); 
      players[loser].gun=0; 
      //이동하고 총 줍기 - 벽이면 오른쪼 90도 이동 
      int nr,nc; 
      for(int i=0;i<4;i++){
        nr=players[loser].x+dr[players[loser].dir];
        nc=players[loser].y+dc[players[loser].dir];
        bool flag=true;
        if(nr>0 && nc>0 && n<=n && nc<=n){
          //해당위치에 플레이어 없어야됨 
          for(int i=0;i<m;i++){
            if(players[i].x==nr && players[i].y==nc){
                flag=false; break;
            } 
          }
        }
        if(flag){
            players[loser].x=nr; players[loser].y=nc; 
            getGun(loser);break; 
        } 
        players[loser].dir=(players[loser].dir+1)%4; 
      }
     
   }
   //winner은 총버리고간거 포함에서 총바꾸기 
   getGun(winner); 
}

void MovePlayer(){

    for(int i=0;i<m;i++){
        int nr=players[i].x+dr[players[i].dir];
        int nc=players[i].y+dc[players[i].dir];
        if(nr<=0 || nc<=0 || nr>n || nc>n){
            //방향바꿔서 움직이기
            //상우하좌  
            if(players[i].dir==0)players[i].dir=2;
            else if(players[i].dir==1)players[i].dir=3;
            else if(players[i].dir==2)players[i].dir=0;
            else players[i].dir=1;
            nr=players[i].x+dr[players[i].dir];
            nc=players[i].y+dc[players[i].dir];
        }
        //위치 갱신 
        players[i].x=nr; players[i].y=nc; 
      //  cout<<i<<' '<<nr<<' '<<nc<<'\n';

        //이 위치에 플레이가 있는가? 
        bool flag=false;
        int fightnum;
        //1. 시간초과 날거같은데... 
        for(int j=0;j<players.size();j++){
            if(j==i)continue; 
            if(players[j].x==players[i].x && players[j].y==players[i].y){
                 flag=true; fightnum=j; 
              //   cout<<"싸움"; cout<<i<<' '<<j<<'\n';
            }
        }
        if(flag){
           //싸우기 
           Fight(i,fightnum); 
        }
        else{
            //총줍기 
            getGun(i); 
        }
    }

}


void Round(){
  MovePlayer(); 

}


int main() {
    int g;
    cin>>n>>m>>k;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
           cin>>g;
           gun_info[{i,j}].push(g);
        }
    }
    players.assign(m,{0,0,0,0,0,0});
    for(int i=0;i<m;i++){
        cin>>x>>y>>d>>s;   
        players[i].x=x; players[i].y=y; 
        players[i].dir=d; players[i].firstpower=s; 
        players[i].gun=0;    
    }
    
    for(int i=0;i<k;i++){ //왜 라운드 한번만함?
     //cout<<"라운드"<<i<<'\n';
       Round(); 
    }

    for(int i=0;i<m;i++){
        cout<<players[i].point<<' ';
    }
    
    return 0;
}