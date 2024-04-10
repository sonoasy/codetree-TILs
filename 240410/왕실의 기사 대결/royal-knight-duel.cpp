#include <iostream>
#include<queue>

using namespace std;
//위 오 아 왼 
int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1};

vector<vector<int>>arr;
struct knight{
  int num; 
  int r;
  int c;
  int power; 
  int h;
  int w;
  int destroycnt; 
  bool destroy;
};
int l,n,q; 
vector<knight>info; 
int r1,c1,h1,w1,k1; 

void Move(int nn,int dd){
    //nn번을 dd 방향으로 옮겨라 
 //cout<<"명령어 "<<nn<<"을 "<<dd<<"방향으로 밀기\n";
  if(info[nn].destroy)return; 
  vector<int>candi; 
  vector<bool>visited(n+1,0); 
  queue<int>qq;
  qq.push(nn);
  visited[nn]=1; 
  bool cannot=false; 
  
  while(!qq.empty()){
    
    int cur=qq.front(); //cout<<cur;
    int cr=info[cur].r;
    int cc=info[cur].c; 
    qq.pop();
    candi.push_back(cur); 
    int nr=cr+dr[dd];
    int nc=cc+dc[dd]; 
   // nr - nr+h-1   nc- nc+w-1 
    //이 직사각형 자체가 벽을 만나면 끝내기 
    if(nr<=0 || nc<=0 || nr>l || nc>l){
      cannot=true; //cout<<"사유1";
    }
    if(cannot)return;
    if((nr+info[cur].h-1)<=0 || (nr+info[cur].h-1)>l || (nc+info[cur].w-1)<=0 ||(nc+info[cur].w-1)>l){
      cannot=true; //cout<<"사유2";
    }
    if(cannot)return;
     //벽인경우 2 
    for(int k=nr;k<=nr+info[cur].h-1;k++){
        if(cannot)break; 
        for(int kk=nc;kk<=nc+info[cur].w-1;kk++){
           if(arr[k][kk]==2){
            cannot=true; //벽이 있는경우 
           // cout<<"사유3";
            break; 
           }
        }
    }
    if(cannot)return;

    //새로옮긴 지점에 다른 기사가 있는가? 
     for(int i=1;i<=n;i++){
      if(i==cur)continue; 
      if(info[i].destroy)continue;
      if(visited[i])continue;  
       // nr - nr+h-1   nc- nc+w-1 겹치는 조건 이면 
       bool flag=false; 
       for(int k=info[i].r;k<=info[i].r+info[i].h-1;k++){
        for(int kk=info[i].c;kk<=info[i].c+info[i].w-1;kk++){
           if(k>=nr && k<=nr+info[cur].h-1 && kk>=nc && kk<=nc+info[cur].w-1)flag=true;
           if(flag)break;
        }
        if(flag)break;
       }    
       if(flag){
         visited[i]=1; 
         qq.push(i); 
       }           
     }

    }
    if(cannot){
    //  cout<<"못움직임\n";
      return; 
    }
  //  cout<<"움직임\n";
    //차례로 옮기기 
    //미는사람 빼고 나머지중에 함정이 있으면 그만큼 파괴되기 
    for(int i=0;i<candi.size();i++){
      int num=candi[i];
      info[num].r+=dr[dd];
      info[num].c+=dc[dd]; 
      if(num==nn)continue; 
      //함정이 있으면 뺴기, 
      int cnt=0; 
      for(int k=info[num].r;k<=info[num].r+info[num].h-1;k++){
        for(int kk=info[num].c;kk<=info[num].c+info[num].w-1;kk++){
           if(arr[k][kk]==1)cnt++; 
        }
      }
      info[num].destroycnt+=cnt;
      info[num].power-=cnt;
      //파괴여부 
      if(info[num].power<=0)info[num].destroy=true;
    }


}

int main() {
    cin>>l>>n>>q; 
    arr.assign(l+1,vector<int>(l+1,0)); 

    for(int i=1;i<=l;i++){
      for(int j=1;j<=l;j++){
        cin>>arr[i][j]; 
      }
    }  
    //info.assign(n,{0,0,0,0,0,0,0,0});
    info.push_back({0,0,0,0,0,0,0,0});
    for(int i=1;i<=n;i++){
      cin>>r1>>c1>>h1>>w1>>k1; 
      info.push_back({i,r1,c1,k1,h1,w1,0,0});
    }
    
    int nn,dd;
    vector<pair<int,int>>query;
    for(int i=0;i<q;i++){
      cin>>nn>>dd;
      query.push_back({nn,dd});
      Move(nn,dd); 
      //cout<<i<<" 움직이고 체력상태\n";
    //  for(int s=1;s<=n;s++){
      //  cout<<info[s].power<<' ';
    //  }
      //cout<<'\n';
    }
    int sum=0;
    for(int i=1;i<=n;i++){
      if(!info[i].destroy)sum+=info[i].destroycnt;
    }
    cout<<sum;
    return 0;
}