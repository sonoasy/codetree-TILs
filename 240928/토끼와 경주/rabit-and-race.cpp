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
map<int,pair<int,long long>>dist; 
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
                    return a.number<b.number;
                }     
                return a.c<b.c;
            }
            return a.r<b.r;
       }
       return (a.r+a.c)<(b.r+b.c);
 
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

     int min_r,min_c;

     for(int i=0;i<4;i++){
        int nr=curr;
        int nc=curc;

        int drr=dr[i]; 
        int dcc=dc[i];

             //하상좌우 
       // for(int j=0;j<curn;j++){ //이거...? 줄여야됨 
            if(i==0){ //하
               nr+=curn*drr;
               nr%=(2*(n-1));
               if(nr>(n-1)){
                  nr=(2*(n-1)-nr); 
               }
            } 
            if(i==1){ //상
               nr=abs(nr-curn*drr)%(2*(n-1));
               
            }
            if(i==2){ //좌
              nc=abs(nc-curn*dcc)%(2*(n-1));
            }
            if(i==3){ //우 
               nc+=curn*dcc;
               nc%=(2*(n-1));
               if(nc>(n-1)){
                  nc=(2*(n-1)-nc); 
               }
            }
           //nr+=drr; nc+=dcc;
        //}



        if(i==0){
            min_r=nr; min_c=nc; 
        }
        else{
            if((min_r+min_c)<(nr+nc)){
                min_r=nr; min_c=nc;
                continue;
            }
            else if((min_r+min_c)==(nr+nc)){
                if(min_r<nr){
                   min_r=nr; min_c=nc; 
                   continue;
                }
                else if(min_r==nr){
                  if(min_c<nc){
                    min_r=nr; min_c=nc; 
                  }
                }
            }

        }
        //location.push_back({select.jump,nr,nc,select.number});
    }

    //sort(location.begin(),location.end(),cmp2);   
    select.r=min_r;
    select.c=min_c; 
 
    //점프횟수 추가 
    select.jump+=1; 

    //토끼 이동 
    return select;
}


int main() {
    
   ios::sync_with_stdio(false); 
   cin.tie(0); cout.tie(0); 
   cin>>q;
  
   cin>>cmd>>n>>m>>p;
   priority_queue<info,vector<info>,cmp>pq;   
   for(int i=0;i<p;i++){
     cin>>num>>a; 
     pq.push({0,0,0,num}); //점프 , r, c, 고유번호 
     dist[num]={a,0}; // 이동 거리 , 점수 
   }
   q--;
   long long total=0; 
   while(q--){ //4000 
     cin>>cmd; 
    
     //pq 로 할때 시간복잡도? 

     if(cmd==200){ //경주 진행 
         cin>>k>>s; 
         priority_queue<info,vector<info>,cmp3>pq2;

         for(int kk=1;kk<=k;kk++){ //100
         
          //토끼 하나 정하기 - cmp  
          info select=pq.top();pq.pop(); 
           
         //움직이기 
          //1억 = 100,000,000
          select=move(select); //2000 * 4 * 길이(1억) 
              //4000*2000*16?  = 80,000,000
       //   pq2.push(select);
       

         // pq.push(select); 

         //나머지 토끼 점수 더해주기 
         //int selected_number=select.number; 
         //int add=select.r+select.c+2;
          
          //이거? 
          //dist[selected_number].second-=add; 
          //total+=add; 
        // 
        }
        
        //점프한 토끼 하나 s 더해주기 
        //dist[pq2.top().number].second+=s;

    }
    
     if(cmd==300){ //이동거리 l배 
        cin>>num>>l;
        dist[num].first*=l; 
     }
   
     if(cmd==400){ //최고의 토끼 선정 
        long long ans=0; 
        for(auto it=dist.begin();it!=dist.end();it++){
            ans=max(ans,it->second.second+total);
        }
        cout<<ans;
         return 0; 
     }
   }

    return 0;
}