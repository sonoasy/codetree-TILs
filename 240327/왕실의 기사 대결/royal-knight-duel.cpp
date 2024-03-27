#include <iostream>
#include<vector>
#include<map>
#include<set>

using namespace std;

int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1};

int l,n,q; 
int r,c,h,w,k; 
int num,d; 
vector<vector<int>>arr;
vector<vector<int>>knight_arr;

struct knight{
   int r;
   int c;
   int h;
   int w;
   int power;
   int damage;
   bool destroy; 
};
vector<knight>info(n+1); 
//info 기반 기사 배치 knight_arr
void knightArr(){

   for(int i=1;i<=n;i++){
       int curr=info[i].r; 
       int curc=info[i].c; 
       int curh=info[i].h;
       int curw=info[i].w;
       
    
       for(int j=curr;j<=curr+curh-1;j++){
         for(int k=curc;k<=curc+curw-1;k++){
            knight_arr[j][k]=i; 
         }
       }
   }

}

void moveAndgetDamage(set<int>move_list,int d){ 

  //list에 있는거 지우기 
  for(auto it:move_list){
    int r=info[it].r; int c=info[it].c; 
    for(int i=r;i<=r+h-1;i++){
        for(int j=c;j<=c+w-1;j++){
            knight_arr[i][j]=0; 
        }
    }
  }


  //움직이기 
  for(auto it:move_list){
    info[it].r+=dr[d];
    info[it].c+=dc[d]; 
  }

  //함정 있으면 그만큼 데미지, 체력 깍기 , 사라지기  
   for(auto it:move_list){
    int r=info[it].r; int c=info[it].c; 
    int damaged=0;
    for(int i=r;i<=r+h-1;i++){
        for(int j=c;j<=c+w-1;j++){
            if(arr[i][j]==1)damaged++; 
            knight_arr[i][j]=it; 
        }
    }
    info[it].power-=damaged; 
    info[it].damage+=damaged;
    //0이하면 격자에서 지우기 
    for(int i=r;i<=r+h-1;i++){
        for(int j=c;j<=c+w-1;j++){
            
            knight_arr[i][j]=0; 
        }
    }
  }
 

}

void canMove(int num,int d){
   
   //d방향 가장 밑부분 쪽에 있는 기사 번호 

   //여기부터 여기까지 있는 번호 체크  
     //나 포함 밀리는 기사 번호 목록 저장 
   int fcur; int lcur;  
   int fcuc; int lcuc;
   int h=info[num].h;
   int w=info[num].w;
   //상 
   if(d==0){
      if(info[num].r==0)return; 
      fcur=0; lcur=info[num].r-1; 
      fcuc=info[num].c; lcuc=info[num].c+w-1; 
       
      
   }
   //우
   if(d==1){
     if((info[num].c+w)>l)return;
     fcur=info[num].r; lcur=info[num].r+h-1; 
     fcuc=info[num].c+w; lcuc=l; 
   } 
   //하
   if(d==2){
   
    if((info[num].r+h)>l)return;
   
     fcur=info[num].r+h; lcur=l;  
     fcuc=info[num].c; lcuc=info[num].c+w-1; 
    
   }
   //좌 
   if(d==3){
     if(info[num].c==0)return; 
     fcur=info[num].r; lcur=info[num].r+h-1;
     fcuc=0; lcuc=info[num].c-1; 
     
   }  
  
   //d 방향으로 밀렸을때 맨끝에 자리 있는가? 
   //기사 번호들 수집 + 맨끝에 있는 기사 위치 
   set<int>move_list; 
   move_list.insert(num);
   

   for(int i=fcur;i<=lcur;i++){
    for(int j=fcuc;j<=lcuc;j++){
        //마지막 칸 바로 직전 
        
        move_list.insert(knight_arr[i][j]); 
    }
   }


   if(move_list.size()==0)return;
   else{
     moveAndgetDamage(move_list,d); 
   }
   
}

//num 기사 d 방향으로 이동 
void move(int num,int d){

  canMove(num,d); 
 
}

//생존한 기사들의 받은 데미지 합 
int damageAmount(){
   
   int sum=0; 
   for(int i=1;i<=n;i++){
    if(!info[i].destroy){
        sum+=info[i].damage; 
    }
   } 
   return sum;
}

void print(){
    for(int i=1;i<=l;i++){
        for(int j=1;j<=l;j++){
            cout<<knight_arr[i][j];
        }
        cout<<'\n';
    }
    cout<<'\n';
}

int main() {
  
   cin>>l>>n>>q; 
   arr.assign(l+1,vector<int>(l+1));
   knight_arr.assign(l+1,vector<int>(l+1,0));
   
   //체스판 
   for(int i=1;i<=l;i++){
    for(int j=1;j<=l;j++){
        cin>>arr[i][j];
     }
   }

   //기사 위치 
   for(int i=1;i<=n;i++){
     cin>>r>>c>>h>>w>>k; 
     info[i].r=r;info[i].c=c;
     info[i].h=h;info[i].w=w;
     info[i].power=k; info[i].damage=0;
     info[i].destroy=false;  
   }
   //기사 배치 
   knightArr(); 
   print();
   //명령 : num 기사 방향 d로 이동 
   for(int i=0;i<q;i++){
     cin>>num>>d;  
     move(num,d);
     cout<<i+1<<" 번째 명령\n";
     print();
   }

    //생존한 기사들이 받은 대미지 합 
   int ans=damageAmount(); 
   cout<<ans; 

    return 0;
}