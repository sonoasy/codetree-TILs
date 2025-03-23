#include <iostream>
#include<vector>
#include<set>
#include<map> 

using namespace std;

int cmd,mid,pid,color,max_depth;
int parent[100001];
set<int> child[100001]; 

//색깔 갯수 


struct info{
  int id;  
  int color; 
  int depth; 
  int score=0; 
  int changecolor=-1; //바뀌어야하는 색깔 
  int changetime=-1; //요청받은 시간  
  int recenttime=-1; //최근에 색깔 바뀐 시간 
  int startime=-1; 
}; 
map<int,vector<int>>colorlist; 
map<int,info>arr;  //id랑 노드 정보 매칭 
set<int>rootnode; //하나인가????? 여러개임
map<int,int>total; 

map<int,int>rootinfo; 

int ans=0; 

//가장 최근 changetime찾기 
//위로 올라가면서 가장 최근 시간 changetime 찾기 
int findroot(int curnode,int changetime,int color){
    //가장 최근 시간, 그 색깔 
   if(curnode==-1)return color ; //끝까지 올라갔을때 가장 최신 시간의 color 기록 
   
   int parents=parent[curnode]; 
   if(arr[parents].changetime>changetime){
      return findroot(parents,arr[parents].changetime,arr[parents].changecolor); 
   }
   else return findroot(parents,changetime,color); 

}

bool depthcheck(int curnode,int depth){
    //curnode의 amxdepth가 depth보다 커야함 
   // cout<<curnode<<" "<<arr[curnode].depth<<"옴 "<<depth<<"\n";
    if(arr[curnode].depth<depth)return false;
    if(parent[curnode]!=-1){
      return depthcheck(parent[curnode],depth+1); 
    }
    return true;

}

//지금 노드, 바꿔야되는 색, 바꿔야되는 시간 
vector<int> checkscore(int curnode,int changecolor,int changetime){

    //위에서 내려오는게 아무것도 없는 경우 
    //내가 아무것도 안바껴야할경우 
    //위에서 내려오는게 있을 경우 
    //내가 바껴야되는게 있는경우 

   //바껴야하는게 있고 최근에 바뀐거보다 더 최근이면 바꿔주기 
   
   //색깔 바꾸는게 생성일보다 이르면 안바꿈 
   //cout<<"색바꾸기"<<curnode<<" "<<changecolor<<" "<<changetime<<"\n";
   colorlist[curnode].assign(6,0); 
   //일단 내 지금 색깔 
   colorlist[curnode][arr[curnode].color-1]+=1; 
   
   if(changecolor!=-1 && arr[curnode].recenttime<changetime && arr[curnode].startime<changetime){
      //바꾸기   
      
      //기존 칼라칩도 바꾸기 
      colorlist[curnode][arr[curnode].color-1]-=1;
      arr[curnode].color=changecolor;    

      colorlist[curnode][arr[curnode].color-1]+=1;  //이거 괜찮음? 
       
      arr[curnode].recenttime=changetime; 

      //나는 이제 바껴야될거 따로 없음 
     // arr[curnode].changecolor=-1; 
     // arr[curnode].changetime=-1; 

   } 
      //그리고 바꿔야할 색깔, 시간은 초기화하도록 한다. 
 //  cout<<curnode<<"색깔 "<<arr[curnode].color<<"\n";   

 //start time 다시 체크 


    for(auto it: child[curnode]){
       int nextnode=it;     
      // cout<<"자식:"<<it<<"\n";
       int nextcurtime=arr[nextnode].changetime; 
       //바꿔주기 시작 
       int nextcolor=arr[nextnode].changecolor; 
       int nexttime=nextcurtime; 
       
     //  cout<<"다음자식 정보:"<<nextnode<<" "<<nextcurtime<<" "<<nextcolor<<"\n";

       if(nextcurtime==-1 && arr[nextnode].startime<changetime){
         nextcolor=changecolor; 
         nexttime=changetime; 

       }

       if(changetime==-1 && nextcurtime==-1){
        // cout<<"여기: "<<nextnode<<"\n";
           nextcolor=arr[nextnode].color; 
           nexttime=-1; 
       }
       else if(nextcurtime!=-1){
          
          if(arr[nextnode].startime>=changetime || ( nextcurtime>changetime)){
           //  cout<<"여기와?";
             nextcolor=arr[nextnode].changecolor; 
             nexttime=nextcurtime;
            // cout<<nextcolor<<" "<<nexttime<<"\n";
          } 
           //부모꺼로 바꾸기 
          if(arr[nextnode].startime<changetime && nextcurtime<changetime){
          
             nexttime=changetime; 
             nextcolor=changecolor; 
          } 
       } 


    //   cout<<"탐색감"<<nextnode<<" "<<nexttime<<" "<<nextcolor<<"\n";
       vector<int>tmp=checkscore(nextnode,nextcolor,nexttime);        
        //더해주기 
        for(int j=0;j<5;j++){
            colorlist[curnode][j]+=tmp[j];
         }
    }
    

    //색을 바꾸고 나면 맨 밑부터 갯수를 반환하면서 색깔 샌다  
    //맨밑에 다 색깔별로 몇개 있는지 있음 

    //점수계산하기 
    int cnt=0; 
   // cout<<curnode<<"의 색현황"<<"\n";
    for(int i=0;i<5;i++){
       if(colorlist[curnode][i]>=1){
         cnt++;
      //   if(curnode==4)cout<<i<<"에 있음\n";
       } 
    } 
    total[curnode]=cnt*cnt; 
 //   cout<<curnode<<"의 점수: "<<total[curnode]<<"\n";
    ans+=total[curnode]; 
    //cout<<curnode<<" "<<total[curnode]<<"현황\n";
    return colorlist[curnode]; 
}


int main() {
  
  ios::sync_with_stdio(false);
  cin.tie(NULL); 
  
  int q;
  cin>>q;

  for(int time=1;time<=q;time++){
    cin>>cmd;
    
    if(cmd==100){ //노드 추가    -> O(depth)
       cin>>mid>>pid>>color>>max_depth;  //나 -부모 - 색 - 깊이 
        
       //나보다 위에 에서 깊이 이상이면 추가 안됨
       //맨위까지 올라가기 체크하기 
       if(pid!=-1 && !depthcheck(pid,2))continue;        
       //노드 추가하기 
       //새로운 트리    
       if(pid==-1){  // -> 이런경우 없음 ?? ? 왜 줌?? 
         rootnode.insert(mid);
       }
       arr[mid].id=mid; 
       arr[mid].color=color;
       arr[mid].depth=max_depth; 
       arr[mid].score=0;
       arr[mid].startime=time; 
       //색깔 현황 
       arr[mid].changecolor=-1; //바뀌어야하는 색깔 
       arr[mid].changetime=-1; //요청받은 시간  
       arr[mid].recenttime=-1; //최근에 색깔 바뀐 시간 
       colorlist[mid].assign(6,0); //초기화
       colorlist[mid][color-1]=1; 
       parent[mid]=pid; //부모 
       if(pid!=-1)child[pid].insert(mid);   

       //잘 입력되었나 확인 
       //cout<<mid<<" 얘 입력, 부모"<<parent[mid]<<"\n";

      
    }
    if(cmd==200){ //색깔 변경     5만  -> O(1)
        //입력받은 시간에 여기 색이 변경되었다고만 체크하기 = O(1) 
        cin>>mid>>color; 
        arr[mid].changecolor=color;
        arr[mid].changetime=time; 
    }
    if(cmd==300){ //색깔 조회   -> O(depth)
        //현재위치부터 위로 거슬러 올라가면서 위쪽에서 가장 최근에 바뀐 부모 찾아서 올라가기 
        // -> 그냥 가장 꼭대기 까지 간다음에 차례로 하나씩만 거슬러 내려와서 색 갱신해주기 
       cin>>mid; 


      for(auto top: rootnode){
           // cout<<top<<"탐색\n";
            //지금 노드, 바꿔야되는 색, 바꿔야되는 시간 
            vector<int>t=checkscore(top,arr[top].changecolor,arr[top].changetime);
       } 
       cout<<arr[mid].color<<"\n"; 

    }
    if(cmd==400){ //점수 조회 - 모든 노드의 가치 계산  O(N)
         //결국 점수가 이미 결정되어야함. 
         //그냥 현재부터 다 밑으러 퍼뜨리고 갯수 세기 
         //rootnode부터 내려감  
        
         ans=0; 
         for(auto top: rootnode){
           // cout<<top<<"탐색\n";
            //지금 노드, 바꿔야되는 색, 바꿔야되는 시간 
            vector<int>t=checkscore(top,arr[top].changecolor,arr[top].changetime);
         } 
         cout<<ans<<"\n";
    }
  }
    return 0;
}