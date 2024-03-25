#include <iostream>
#include <vector>
#include<map>
#include<algorithm>

using namespace std;

map<int,int>dist; 
int n,m,p,q,cmd,num,a,l,k,s;

//하상우좌 
int dr[4]={1,-1,0,0};
int dc[4]={0,0,1,-1}; 
 //점프 횟수, 토끼의 점수, 토끼의 위치, 고유번호
struct info{
     int jump; 
     int score;
     int r;
     int c; 
     int number;
};

typedef pair<int,int>ci; 

//점프 횟수 적은 -> 행+열 -> 행 -> 열 -> 고유번호 
bool cmp1(info a,info b){
   
   if(a.jump==b.jump){
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

    return a.jump<b.jump;
}

//행+열 ->행 -> 열 
bool cmp2(ci a,ci b){
   if((a.first+a.second)==(b.first+b.second)){
            if(a.first==b.first){  
                return a.second>b.second;
            }
            return a.first>b.first;
       }
       return (a.first+a.second)>(b.first+b.second);
}
//행+열 -> 행 -> 열 -> 고유번호 
bool cmp3(info a,info b){
  
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

int main() {
    
   cin>>q;
  
   cin>>cmd>>n>>m>>p;
   vector<info>arr;
   
   for(int i=0;i<p;i++){
     cin>>num>>a; 
     arr.push_back({0,0,0,0,num});
     dist[num]=a;
   }
   q--;
 
   while(q--){
     cin>>cmd; 

     if(cmd==200){ //경주 진행 
         cin>>k>>s; 
         for(int kk=1;kk<=k;kk++){
          
           //우선순위 높은 토끼 정하기 - 점프 횟수 적은 -> 행+열 -> 행 -> 열 -> 고유번호 
           sort(arr.begin(),arr.end(),cmp1);
           info target=arr[0]; 
           arr[0].jump+=1; 
           //그 토끼의 움직일 위치 정하기 - 행+열 ->행 -> 열 
           int curr=target.r;
           int curc=target.c;
           int curn=dist[target.number];
           vector<ci>location;
          // cout<<curr<<' '<<curc<<' '<<curn<<'\n';
          
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

          //   cout<<"후보:"<<nr<<' '<<nc<<'\n';
             location.push_back({nr,nc});
           }

           sort(location.begin(),location.end(),cmp2); 
           int nextr=location[0].first;
           int nextc=location[0].second;   
 
         //  cout<<nextr<<' '<<nextc<<'\n';
           //토끼 이동 
           arr[0].r=nextr; arr[0].c=nextc;
        
           //나머지 이동 좌표 만큼 토끼 점수 주기  r+1 ,c+1 
           int add=(nextr+1+nextc+1);
           for(int i=1;i<p;i++){
              arr[i].score+=add;  
           }
       //   cout<<"add: "<<add<<'\n'    
        
        }

        vector<info>candidate; 
        //K번의 턴 동안 한번이라도 뽑혔던 적이 있던 토끼 중 가장 우선순위가 높은 토끼
        for(int i=0;i<p;i++){
            if(arr[i].jump>0)candidate.push_back(arr[i]);
        }

         //제일 높은 토끼 S 더해주기  - 행+열 -> 행 -> 열 -> 고유번호  
         sort(candidate.begin(),candidate.end(),cmp3); 
         int best_candidate=candidate[0].number; 
         //jump횟수 초기화하기 
         
        // arr=candidate; 

           for(int i=0;i<arr.size();i++){
            if(arr[i].number==best_candidate)arr[i].score+=s; 
            arr[i].jump=0;

         }    
     }
    
     if(cmd==300){ //이동거리 l배 
        cin>>num>>l;
        dist[num]*=l; 
     }
   
     if(cmd==400){ //최고의 토끼 선정 
          //가장 점수 높은 토끼의 점수 출력 
         int ans=0; 
         for(int i=0;i<p;i++){
            if(ans<arr[i].score)ans=arr[i].score;
         }
         cout<<ans;

          return 0; 
     }
   }
    


    return 0;
}