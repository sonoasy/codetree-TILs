#include <iostream>
#include<vector>

using namespace std;

vector<vector<int>>arr;  
vector<int>dirnum; 
//좌우하상 
int dr[4]={0,0,1,-1};
int dc[4]={-1,1,0,0};   
int n,m,h,k; 
int x,y,d; 
//도망자 : 현재 보고 있는 방향, 현재 위치 
struct info{
   int moveway; //1-> 좌우 2-> 상하 
   int dir; 
   int r;
   int c;
   bool catched;  
};
vector<info>runner; 
struct catcher{
   int dir; 
   int r;
   int c; 
   int score; 
};
catcher catchrunner;

void Run(){
   //모든 도망자들에 대해 
   for(int i=0;i<m;i++){
      if(runner[i].catched)continue; 
      //술래와의 거리 계산 
      int dist=abs(catchrunner.r-runner[i].r)+abs(catchrunner.c-runner[i].c);
      if(dist<=3){
      
         int cur=runner[i].r;
         int cuc=runner[i].c;
         //보고있는 방향 격자 안벗어나면 
         int nr=cur+dr[runner[i].dir]; 
         int nc=cuc+dc[runner[i].dir];
       
         if(nr<=0 || nc<=0 || nr>n || nc>n){
              //방향을 틀음 
              if(runner[i].dir==0)runner[i].dir=1;
              else if(runner[i].dir==1)runner[i].dir=0;
              else if(runner[i].dir==2)runner[i].dir=3;
              else if(runner[i].dir==3)runner[i].dir=2; 
          
              nr=cur+dr[runner[i].dir];
              nc=cuc+dc[runner[i].dir];
              //술래가 없을때만 이동      
              if(!(catchrunner.r==nr && catchrunner.c==nc)){
                //이동 
                runner[i].r=nr; runner[i].c=nc;  
            }
         }
         else{
            //술래가 그위치에 없으면 이동 
            if(!(catchrunner.r==nr && catchrunner.c==nc)){
                //이동 
                runner[i].r=nr; runner[i].c=nc;  
            }
         }
      }
    //cout<<runner[i].catched<<'\n';
   } 
   
}

void Catch(int turn){
   
  // cout<<"와?";
      //이동방향으로 이동하기 
   catchrunner.r+=dr[catchrunner.dir];
   catchrunner.c+=dc[catchrunner.dir]; 
   catchrunner.dir=dirnum[(turn)%dirnum.size()]; 
   //cout<<"술래 위치: "<<catchrunner.r<<' '<<catchrunner.c<<'\n';
   int nr1= catchrunner.r;
   int nc1= catchrunner.c; 
   int nr2=nr1+dr[catchrunner.dir];
   int nc2=nc1+dc[catchrunner.dir];
   int nr3=nr2+dr[catchrunner.dir];
   int nc3=nc2+dc[catchrunner.dir];
   //현재칸 포함 3칸 보기
   int cnt=0;  
   for(int i=0;i<m;i++){
    if(runner[i].catched)continue; //예외처리!!!!!!
      if(!arr[runner[i].r][runner[i].c]){ //나무가 없어야함  
       if((runner[i].r==nr1 && runner[i].c==nc1) || (runner[i].r==nr2 && runner[i].c==nc2) ||(runner[i].r==nr3 && runner[i].c==nc3)){
           runner[i].catched=true; 
           cnt++; 
       }
      }
   }

   catchrunner.score+=(turn*cnt); 
//   cout<<turn<<' '<<catchrunner.score<<'\n'; 
}

int main() {

   cin>>n>>m>>h>>k;
   arr.assign(n+1,vector<int>(n+1,0)); 
   
   //도망자 정보 
   for(int i=0;i<m;i++){
     cin>>x>>y>>d; 
     runner.push_back({d,d,x,y,0});   
   }
   //트리 정보 
   for(int i=0;i<h;i++){
     cin>>x>>y;
     arr[x][y]=1; //1이면 나무가 있는 위치 
   }  
   //술래 초기화 
   catchrunner={3,n/2+1,n/2+1,0}; // n이 짝수이면..?  

   //3-> 1->2-> 0 ->  3
   int num1=3; 
   int num2=1;
   for(int i=1;i<=n-1;i++){
       for(int j=0;j<i;j++){
         dirnum.push_back(num1);
       }
       for(int j=0;j<i;j++){
         dirnum.push_back(num2);
       }
       if(num1==3 && num2==1){
         num1=2; num2=0; 
       }
       else if(num1==2 && num2==0){
         num1=3; num2=1; 
       }
   }
   //n일때 저장 
   for(int i=0;i<n-1;i++)dirnum.push_back(num1); 
   //반대도 더해주기 
   int t;
   for(int i=dirnum.size()-1;i>=0;i--){
    if(dirnum[i]==1)t=0;
    if(dirnum[i]==2)t=3;
    if(dirnum[i]==3)t=2;
    if(dirnum[i]==0)t=1; 
    dirnum.push_back(t);
   }
   

   for(int i=1;i<=k;i++){
      
      
      Run(); //도망자들 움직이기 
    
      //술래의 방향 정해주기 
      Catch(i); //술래 움직이고 잡기 
      
    // cout<<runner[0].r<<' '<<runner[0].c<<'\n'; 
    // cout<<i<<" ";
      // cout<<catchrunner.dir<<' '<<catchrunner.r<<' '<<catchrunner.c<<'\n';
   }
   cout<<catchrunner.score; 

    return 0;
}