#include <iostream>
#include<vector>
#include<algorithm> 
#include<queue>

using namespace std; 

int n,m,p,c,d; //게임판 크기,게임 턴, 산타 수, 루돌프 힘,산타 힘 
int Rx,Ry; 
int Rdir; 
int sx,sy;
int num; 

struct santa{
   int num; 
   int x;
   int y; 
   int score; 
   int dir; 
   bool sleep; 
   int sleep_until; 
   bool isout; 
};
vector<santa>santas; 

//상우하좌 
int dr[8]={-1,0,1,0,-1,1,1,-1};
int dc[8]={0,1,0,-1,1,1,-1,-1}; 
//반대방향 
int rr[8]={1,0,-1,0,1,-1,1,-1};
int rc[8]={0,-1,0,1,-1,-1,1,1};

//거리 계산 
int calDistance(int r1,int c1, int r2, int c2){
    return (r1-r2)*(r1-r2)+(c1-c2)*(c1-c2);
}

bool cmp(santa a,santa b){
    if(calDistance(a.x,a.y,Rx,Ry)==calDistance(b.x,b.y,Rx,Ry)){
        if(a.x==b.x){
           return a.y>b.y;
        }
        return a.x>b.x;
    }
   
    return calDistance(a.x,a.y,Rx,Ry)<calDistance(b.x,b.y,Rx,Ry);
}

bool cmp2(santa a,santa b){
    return a.num<b.num;
}

void Interact(int movedsanta,int movedir,int kind){

  queue<int>q; 
  q.push(movedsanta);

   while(!q.empty()){
     int curn=q.front();
     int cur=santas[curn].x; 
     int cuc=santas[curn].y; 
     q.pop();
     //1칸씩 밀리기 
     int nr,nc;
     if(kind==0){
         nr=cur+dr[movedir];
         nc=cuc+dc[movedir];
     }
     else{
         nr=cur+rr[movedir];
         nc=cuc+rc[movedir];
     }
    
     santas[curn].x=nr; santas[curn].y=nc; 
     if(nr<=0 || nc<=0 || nr>n || nc>n){
    //    cout<<curn<<"아웃\n";
        santas[curn].isout=true; continue;
     }
   //  cout<<curn<<"산타 "<<nr<<' '<<nc<<"로 밀림\n";
     //이 위치에 산타가 있으면 또 밀기 
     for(int j=0;j<p;j++){
        if(j==curn)continue;
        if(!santas[j].isout && nr==santas[j].x && nc==santas[j].y){
            q.push(j); 
        }
     }
   }

} 


void Collision(int movedsanta,int kind,int movedir,int turn){
   
   //기절 표시 
   santas[movedsanta].sleep=true; 
   santas[movedsanta].sleep_until=turn+1; 

   if(kind==0){ //루돌프가 일으킨 충돌 
 //  cout<<"루돌프가 일으킨 충돌 with"<<movedsanta<<"\n"; 
     santas[movedsanta].score+=c; 
     //산타 이동시키기 - movedir방향으로 C만큼 밀려남  
     int nr=santas[movedsanta].x+c*dr[movedir];
     int nc=santas[movedsanta].y+c*dc[movedir]; 
     //위치가 게임밖이면 탈락 
     if(nr<=0 || nc<=0 || nr>n || nc>n){
        santas[movedsanta].x=nr; 
        santas[movedsanta].y=nc; 
        santas[movedsanta].isout=true; //cout<<movedsanta<<"아웃2\n";
        return;
     }
     
     santas[movedsanta].x=nr; 
     santas[movedsanta].y=nc; 
     
     //cout<<"루돌프때문"<<movedsanta<<"여기로 이동"<<nr<<' '<<nc<<'\n';

     //아니면 위치, 상호작용 여부 
     for(int i=0;i<p;i++){
        //자기 자신이면 지나가기 
        if(i==movedsanta)continue;
        if(santas[i].isout)continue;
        if(nr==santas[i].x && nc==santas[i].y){
            //싱호작용이 일어남 - moveddir방향으로 1칸씩 연쇄적으로 밀려남 
            Interact(i,movedir,0); 
        }
     }


   }
   else{ //산타가 일으킨 충돌 
    // cout<<movedsanta<<"가 일으킨 루돌프 충돌\n"; 
     santas[movedsanta].score+=d; 
     //산타 이동시키기 - movedir방향으로 D만큼 밀려남 
     int nr=santas[movedsanta].x+=d*rr[movedir];
     int nc=santas[movedsanta].y+=d*rc[movedir]; 
     
     santas[movedsanta].x=nr;
     santas[movedsanta].y=nc; 
     if(nr<=0 || nc<=0 || nr>n || nc>n){
        santas[movedsanta].isout=true; //cout<<movedsanta<<"아웃3\n";
        return; 
     }//이 위치에 누가 있으면 상호작용 
     //cout<<"산타때문"<<movedsanta<<"여기로 이동"<<nr<<' '<<nc<<'\n';
   
    //자기 자신이면 지나가기
     //싱호작용이 일어남 - moveddir방향으로 1칸씩 연쇄적으로 밀려남 
    //Interact(movedsanta,movedir,1); 
     for(int i=0;i<p;i++){
        //자기 자신이면 지나가기 
        if(i==movedsanta)continue;
        if(santas[i].isout)continue;
        if(nr==santas[i].x && nc==santas[i].y){
            //싱호작용이 일어남 - moveddir방향으로 1칸씩 연쇄적으로 밀려남 
            Interact(i,movedir,1); 
        }
     }
     
   }
}



void RMove(int turn){

   vector<santa>candidate; 
   //탈락 안된 산타중에서 가장 가까운- r이 큰 - c가 큰  산타 선택 
   for(int i=0;i<p;i++){
    if(santas[i].isout)continue; 
      candidate.push_back(santas[i]);
   }
   sort(candidate.begin(),candidate.end(),cmp); 
   //그 산타 향해서 8방향중 가까워지는 방향으로 한칸 이동 

   int selectedx=candidate[0].x;
   int selectedy=candidate[0].y;  
   //cout<<"선택된 좌표:"<<selectedx<<' '<<selectedy<<'\n';
   //8방향중에 제일 가까운 방향의 좌표로 가기 
   int maxs=1000000;
   int go; 
   for(int i=0;i<8;i++){
     int nr=Rx+dr[i];
     int nc=Ry+dc[i]; 
     if(nr<=0 || nc<=0 || nr>n || nc>n)continue; 

     if(calDistance(nr,nc,selectedx,selectedy)<maxs){
        maxs=calDistance(nr,nc,selectedx,selectedy);
        go=i; 
     }
   }

   //go 방향으로 루돌프 이동 
   Rx=Rx+dr[go];
   Ry=Ry+dc[go];
   Rdir=go; 

   //충돌이 일어났는지 확인 
   for(int i=0;i<p;i++){
      if(!santas[i].isout && santas[i].x==Rx && santas[i].y==Ry){
        //루돌프가 움직여서 일어난 충돌 
         Collision(i,0,go,turn); 
      }
   }
//cout<<"루돌프 여기로이동: "<<Rx<<' '<<Ry<<'\n'; 
}

void SantaMove(int turn){
   
   //기절한거 깨우기 
   for(int i=0;i<p;i++){
     if(santas[i].isout)continue; 
     if(!santas[i].isout && santas[i].sleep && santas[i].sleep_until<turn){
        santas[i].sleep=false; 
     }
     if(santas[i].sleep)continue; 

     //루돌프와 가장 가까워지는 방향으로 이동 
     //가까워질수 있는 방향이 없으면 움직이지 않음 
    
     //움직일수 없으면 움직이지 않음  
     //상우하좌 순으로 

     //지금 루돌프와 거리 
     int curdist=calDistance(santas[i].x,santas[i].y,Rx,Ry); 
     int mins=curdist;
     int ggo=-1; 
     for(int j=0;j<4;j++){
        int nr=santas[i].x+dr[j];
        int nc=santas[i].y+dc[j]; 
        //게임판 밖이나 산타가 있는 방향은 가지 않음 
        if(nr<=0 || nc<=0 || nr>n || nc>n)continue; 
        bool cango=true; 
        for(int k=0;k<p;k++){
            if(k==i)continue; 
            if(santas[k].isout)continue;
            if(nr==santas[k].x && nc==santas[k].y) {
                cango=false; break; 
            }
        }
        if(!cango)continue; 
        else{
           int ndist=calDistance(nr,nc,Rx,Ry); 
           if(ndist>curdist)continue; 
           if(ndist<mins){
               ggo=j; 
               mins=ndist; 
           }
        }
     }
     if(ggo>=0){
        //산타 이동 
        santas[i].x+=dr[ggo];
        santas[i].y+=dc[ggo]; 
     //   //충돌이 있는가? -산타가 일으킨 충돌  
   //cout<<i<<" 산타 여기로이동: "<<santas[i].x<<' '<<santas[i].y<<'\n'; 

       if(santas[i].x==Rx && santas[i].y==Ry){
        // ggo의 반대방향으로 D칸 이동 
       // cout<<i<<santas[i].x<<','<<santas[i].y<<i<<"가 일으킨 충돌 발생\n";
         Collision(i,1,ggo,turn);
         
       }

     }

   }

}
void Plus(){

    //기절 안했으면 점수 추가 
    for(int i=0;i<p;i++){
        if(!santas[i].isout){
            santas[i].score+=1; 
        }
    }

}
void print(){

   for(int i=0;i<p;i++){
     cout<<santas[i].score<<' '; 
   }

}
bool check(){
  int flag=true;
  for(int i=0;i<p;i++){
    if(!santas[i].isout)flag=false; 
  }
  return flag;
}

int main() {

    cin>>n>>m>>p>>c>>d; 
    cin>>Rx>>Ry; 

    for(int i=0;i<p;i++){
      cin>>num>>sx>>sy; 
      santas.push_back({num,sx,sy,0,0,0,0,0}); 
    } 
    //산타 번호 순대로 정렬 
    sort(santas.begin(),santas.end(),cmp2);  

    for(int i=0;i<m;i++){
    //   cout<<i+1<<"턴\n";
       if(check())break; 
       RMove(i); 
       SantaMove(i); 
       Plus(); 
      
    }
    print(); 

    return 0;
}