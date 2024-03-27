#include <iostream>
#include<vector>
#include<map>
#include<queue>

using namespace std;

//지금 맞닿아 있는거로 구하나 했는데
//움직였을때 그 범위에 있는거군 
int l,n,q;
vector<vector<int>>arr; 
int r,c,h,w,k; 
int num,dir; 

int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1}; 

struct info{
   int r;
   int c;
   int h;
   int w;
   int power; 
   int damage_amount;
   bool damage; 
};
vector<info>knight_arr;
vector<int>next_damage;
vector<pair<int,int>>change_num;
vector<bool>moved; 

bool Move(int num,int dir){
  
  if(knight_arr[num].damage)return false;
  queue<int>q;  
  q.push(num);

  while(!q.empty()){

    int cnum=q.front();q.pop(); 
   // cout<<"큐:"<<cnum<<' ';
    int nr=knight_arr[cnum].r+dr[dir]; 
    int nc=knight_arr[cnum].c+dc[dir]; 
    change_num[cnum]={nr,nc}; 
    moved[cnum]=true; 
    //2. 벽이거나 넘어가면 x 
    if(nr<=0 || nc<=0 || nr>l || nc>l)return false; 
    if(arr[nr][nc]==2)return false;
    if((nr+knight_arr[cnum].h-1)>l || (nc+knight_arr[cnum].w-1)>l)return false;
    //움직이려는데 
    //벽있으면 빼기 
    for(int i=nr;i<=nr+knight_arr[cnum].h-1;i++){
      for(int j=nc;j<=nc+knight_arr[cnum].w-1;j++){
         if(arr[i][j]==2)return false; 
         //1. 함정있으면 체력빼주기 
         
         //미는 사람은 안 뺴줌!!!!!
         if(cnum!=num && arr[i][j]==1){
           next_damage[cnum]++; 
         }
      }
    }

     //중간에 안되면? - 계속 바꾸는게 아니라 끝까지 true이면 저장된걸 바꿔주기 
    //바꾼다는것도 표시 
    for(int i=1;i<=n;i++){
      if(i==cnum)continue; 
      if(knight_arr[i].damage)continue; //이거!!!
      if(knight_arr[i].r>nr+knight_arr[cnum].h-1 || (knight_arr[i].r+knight_arr[i].h-1)<nr)continue; 
      if(knight_arr[i].c>nc+knight_arr[cnum].w-1 || (knight_arr[i].c+knight_arr[i].w-1)<nc)continue; 
      q.push(i);  
    }
    //moved[cnum]=true; 
   // change_num[cnum]={nr,nc}; 
  }
   
   return true; 
}

int main(){

  cin>>l>>n>>q; 
  change_num.assign(n+1,{0,0});
  moved.assign(n+1,0); 
  next_damage.assign(n+1,0); 
  arr.assign(l+1,vector<int>(l+1,0));
  for(int i=1;i<=l;i++){
    for(int j=1;j<=l;j++){
      cin>>arr[i][j]; 
    }
  }
   
  knight_arr.push_back({0,0,0,0,0,0,0});
  for(int i=1;i<=n;i++){
    cin>>r>>c>>h>>w>>k; 
    knight_arr.push_back({r,c,h,w,k,0,0}); 
  }
   
  for(int i=0;i<q;i++){
    cin>>num>>dir;

    if(Move(num,dir)){
    //cout<<"움직임";
     for(int j=1;j<=n;j++){
       if(moved[j] && !knight_arr[j].damage){
          knight_arr[j].r=change_num[j].first;
          knight_arr[j].c=change_num[j].second; 
          knight_arr[j].power-=next_damage[j]; 
          knight_arr[j].damage_amount+=next_damage[j]; 
          if(knight_arr[j].power<=0)knight_arr[j].damage=true; 
        }   
        
      }
     
      for(int j=1;j<=n;j++){
            moved[j]=false; 
            next_damage[j]=0; 
       }
     
    }
   
 //   for(int ss=1;ss<=n;ss++){
  //  cout<<i<<"번째: "<< knight_arr[ss].damage<<' '<<knight_arr[ss].power<<' '<<knight_arr[ss].damage_amount<<' ';
    // cout<<"위치: "<<knight_arr[ss].r<<' '<<knight_arr[ss].c<<'\n';

 //   } 
    // cout<<i<<" 명령 끝나고 현황\n";
  //  for(int ss=1;ss<=n;ss++){
     //cout<<knight_arr[ss].damage<<' '<<knight_arr[ss].power<<' '<<knight_arr[ss].damage_amount<<'\n';
   //  cout<<"위치: "<<knight_arr[ss].r<<' '<<knight_arr[ss].c<<'\n';
   // }
   // cout<<'\n';

  }
  //cout<<i<<" 명령 끝나고 현황\n";
    //for(int ss=1;ss<=n;ss++){
   //  cout<<knight_arr[ss].damage<<' '<<knight_arr[ss].power<<' '<<knight_arr[ss].damage_amount<<'\n';
   //  cout<<"위치: "<<knight_arr[ss].r<<' '<<knight_arr[ss].c<<'\n';
   // }
   // cout<<'\n';

  int sum=0; 
  for(int i=1;i<=n;i++){
    if(!knight_arr[i].damage)sum+=knight_arr[i].damage_amount;
  }
  
  cout<<sum;

  return 0; 

}