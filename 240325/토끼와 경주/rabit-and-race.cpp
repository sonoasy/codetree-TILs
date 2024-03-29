#include <iostream>
#include <vector>
#include<map>
#include<algorithm>
#include<queue>

using namespace std;

struct info{
     int jump; 
     int r;
     int c; 
     int number;
};

typedef pair<int,int>ci; 
vector<info>arr;
map<int,pair<int,int>>dist; 
map<int,int>score; 
int nextr,nextc; 
int n,m,p,q,cmd,num,a,l,k,s;
//하상우좌 
int dr[4]={1,-1,0,0};
int dc[4]={0,0,1,-1}; 
 //점프 횟수, 토끼의 점수, 토끼의 위치, 고유번호

//옮길 토끼 
struct cmp{
   bool operator()(info a, info b){
     
     if(a.jump==b.jump){
        if((a.r+a.c)==(b.r+b.c)){
            if(a.r==b.r){
                if(a.c==b.c){
                    return a.number>b.number;
                }     
                return a.c>b.c;
            }
            return a.r>b.r;
        }
        return (a.r+a.c)>(b.r+b.c);
    }

    return a.jump>b.jump;

   }

};



//s 더해줄 토끼 게싼 - 점프 했을때만 
bool cmp2(info a,info b){
   
       if((a.r+a.c)==(b.r+b.c)){
            if(a.r==b.r){
                if(a.c==b.c){
                    return a.number>b.number;
                }     
                return a.c>b.c;
            }
            return a.r>b.r;
       }
       return (a.r+a.c)>(b.r+b.c);
 
}

struct cmp3{
    bool operator()(info a,info b){
        if((a.r+a.c)==(b.r+b.c)){
            if(a.r==b.r){
                if(a.c==b.c){
                    return a.number>b.number;
                }     
                return a.c<b.c;
            }
            return a.r<b.r;
       }
       return (a.r+a.c)<(b.r+b.c);
    }
};

//선택한 토끼 움직일 다음 좌표 계산 - cmp2 사용 
info move(info select){

     int curr=select.r; int curc=select.c; int curn=dist[select.number].first;
     vector<info>location;
     for(int i=0;i<4;i++){
        int nr=curr;
        int nc=curc;

        int drr=dr[i]; 
        int dcc=dc[i];

             //하상좌우 
        for(int j=0;j<curn;j++){
            if(i==0 || i==1){
                if((nr+drr)<0 || (nr+drr)>=n)drr*=-1;
            }
            if(i==2 || i==3){
                if((nc+dcc)<0 || (nc+dcc)>=m)dcc*=-1;
            }
            nr+=drr; nc+=dcc;
        }
       
        location.push_back({select.jump,nr,nc,select.number});
    }

    sort(location.begin(),location.end(),cmp2);   
    select.r=location[0].r;
    select.c=location[0].c; 
 
    //점프횟수 추가 
    select.jump+=1; 

    //토끼 이동 
    return select;
}


int main() {
    
   cin>>q;
  
   cin>>cmd>>n>>m>>p;
   priority_queue<info,vector<info>,cmp>pq;   
   for(int i=0;i<p;i++){
     cin>>num>>a; 
     pq.push({0,0,0,num}); //점프 , r, c, 고유번호 
     dist[num]={a,0}; // 이동 거리 , 점수 
   }
   q--;
 
   while(q--){ //4000 
     cin>>cmd; 
    
     //pq 로 할때 시간복잡도? 

     if(cmd==200){ //경주 진행 
         cin>>k>>s; 
         priority_queue<info,vector<info>,cmp3>pq2;

         for(int kk=1;kk<=k;kk++){ //2000 
         
          //토끼 하나 정하기 - cmp  
          info select=pq.top();pq.pop(); 
           
         //움직이기 
          select=move(select); pq2.push(select);
       

          pq.push(select); 

         //나머지 토끼 점수 더해주기 
          int selected_number=select.number; 
          int add=select.r+select.c+2;
          
          for(auto it=dist.begin();it!=dist.end();it++){
            if(it->first!=selected_number)it->second.second+=add; 
          }
         
        }
        
        //점프한 토끼 하나 s 더해주기 
        dist[pq2.top().number].second+=s;
  

    }
    
     if(cmd==300){ //이동거리 l배 
        cin>>num>>l;
        dist[num].first*=l; 
     }
   
     if(cmd==400){ //최고의 토끼 선정 
        int ans=0; 
        for(auto it=dist.begin();it!=dist.end();it++){
            ans=max(ans,it->second.second);
        }
        cout<<ans;
         return 0; 
     }
   }
    


    return 0;
}