#include <iostream>
#include<vector>
#include<queue>
#include<map> 
#include<algorithm> 
using namespace std;


typedef pair<int,int> ci; 

//그룹별 정리 
struct info{
   //타입 
   string type; 
   //그룹원수 
   int num;
   //대표지 
   ci master; 
   //그룹원 목록 
  vector<ci>members; 
};

int n,t;

vector<vector<string>>type; 
vector<vector<int>>faith; 

//위 아 왼 오 
int dr[4]={-1,1,0,0};
int dc[4]={0,0,-1,1}; 

void INPUT(){
   
   cin>>n>>t; 
   type.assign(n,vector<string>(n,""));
   faith.assign(n,vector<int>(n,0));
   char t;
   for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
         cin>>t;
         type[i][j]+=t; 
      }
   }
  
   for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
         cin>>faith[i][j]; 
      }
   }

} 

bool cmp2(info a,info b){
   
   if(faith[a.master.first][a.master.second]==faith[b.master.first][b.master.second]){
      if(a.master.first==b.master.first){
        return a.master.second<b.master.second; 
      }
      return a.master.first<b.master.first;   
   }
   return faith[a.master.first][a.master.second]>faith[b.master.first][b.master.second];
}

string go(string types){
   int num1=0; //t
   int num2=0; //c 
   int num3=0; //m 
   
   for(int i=0;i<types.size();i++){
     if(types[i]=='T')num1++;
     if(types[i]=='C')num2++;
     if(types[i]=='M')num3++; 
   }
   string res="";
   if(num1)res+='T'; 
   if(num2)res+='C';
   if(num3)res+='M'; 
   return res; 
}


void propagation(string turntype,ci tmaster,int x,int dir,vector<vector<int>>&affected){

    int cur=tmaster.first;
    int cuc=tmaster.second;   
    
    //전파 당한적이 있으면 전파 못함 
    if(affected[cur][cuc]){
      // cout<<"전파당해서 전파못함\n";
       return; 
    }
     faith[cur][cuc]=1;
   // cout<<cur<<" "<<cuc<<"에서 전파\n";
    

    while(1){
       int nr=cur+dr[dir];
       int nc=cuc+dc[dir]; 
       if(nr<0 || nc<0 || nr>=n || nc>=n){
      /*  cout<<nr<<","<<nc<<" 진행못함\n";
         cout<<"\n";
       // cout<<"전파 후 faith\n";
        for(int i=0;i<n;i++){
          for(int j=0;j<n;j++){
            cout<<faith[i][j];
          }
           cout<<"\n";
        } 
        cout<<"신봉음식\n";
        for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout<<type[i][j];
         }
         cout<<"\n";
        }
        */
        return; //전파 종료 
       }
       
       if(type[nr][nc]==turntype){
          cur=nr; cuc=nc;
         // cout<<nr<<","<<nc<<"다녀감";
        //  cout<<"똑같아서 통과";
          continue; //통과 
       }
       //조합이 여러개인데 포함되어있을경우 string,char?? 
       //둘다 조합 여러개인 경우? -> 아님 

       //강한 전파 
       if(x>faith[nr][nc]){
     //    cout<<"강한전파";
          affected[nr][nc]+=2; 
          x-=(faith[nr][nc]+1);
          
          faith[nr][nc]+=1;
          //신봉음식 바뀜 - 동화됨 
          type[nr][nc]=turntype; 
          if(x<=0)return; //전파 종료 
       }
       else{ //약한 전파 
      //    cout<<"약한전파";
          affected[nr][nc]+=1; 
          faith[nr][nc]+=x;
          x=0; 
          //신봉음식 바뀜 
          string tt=type[nr][nc];
          tt+=turntype; 
          //유일하고, 정렬하기 
          type[nr][nc]=go(tt); 
      
      //    cout<<"\n";
     //   cout<<"전파 후 faith\n";
     //   for(int i=0;i<n;i++){
      //    for(int j=0;j<n;j++){
       //      cout<<faith[i][j];
      //    }
      //     cout<<"\n";
      //  }
       // cout<<"신봉음식\n";
        //for(int i=0;i<n;i++){
      //   for(int j=0;j<n;j++){
       //     cout<<type[i][j];
       //  }
      //   cout<<"\n";
      //  }
      //   cout<<"\n";

          return; 
       }
       cur=nr; cuc=nc; 
     //  cout<<cur<<","<<cuc<<" ";
    }
}


void DINNER(map<int,vector<info>>groupresult){
  // 디버깅용
  /*
  for(int k=0;k<3;k++){
   for(int i=0;i<groupresult[k].size();i++){
     cout<<groupresult[k][i].type<<"\n";
     cout<<groupresult[k][i].master.first<<","<<groupresult[k][i].master.second<<"\n";
     cout<<"총 인원:"<<groupresult[k][i].num<<"\n";
     for(int j=0;j<groupresult[k][i].num;j++){
        cout<<groupresult[k][i].members[j].first<<","<<groupresult[k][i].members[j].second<<" ";
     }
     cout<<"\n";
   }
  }
  */
 
  
  //그룹별 전파 순서 정하기 
  for(int i=0;i<3;i++){
    sort(groupresult[i].begin(),groupresult[i].end(),cmp2); 
  }

  //그룹별 전파 시작 
  //주의: 전파 당했으면 전파 못함 ? -> 이거 안함! 
  vector<vector<int>>affected; 
  affected.assign(n,vector<int>(n,0)); 

   for(int i=0;i<3;i++){
      for(int j=0;j<groupresult[i].size();j++){
        //이번 탐색 그룹 
        //대표자 
        info tmpg=groupresult[i][j];
        ci tmaster=tmpg.master;  
       // cout<<tmaster.first<<","<<tmaster.second<<"\n";
        //cout<<faith[tmaster.first][tmaster.second]<<"\n";      
        //초기 신앙심 
        int x=faith[tmaster.first][tmaster.second]-1; 
        if(x<0)x=0;  
        //탐색 방향 
        int dir=faith[tmaster.first][tmaster.second]%4; 
        //전파 시작 
        string tmp=""; tmp+=tmpg.type;

        propagation(tmp,tmaster,x,dir,affected); 

      }
  } 
} 

void MORNING(){
   //신앙심 +1 
   for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
        faith[i][j]+=1; 
      }
   }
} 

bool cmp(ci a,ci b){

   if(faith[a.first][a.second]==faith[b.first][b.second]){
     if(a.first==b.first){
        return a.second<b.second;
     }
     return a.first<b.first; 
   }
   
   return faith[a.first][a.second]>faith[b.first][b.second];
}

info makegroup(int curr,int curc,vector<vector<int>>&visited,string mytype){
   
   vector<ci>member; 
   member.push_back({curr,curc}); 
   visited[curr][curc]=1; 
   
   queue<ci>q; 
   q.push({curr,curc});
    
   while(!q.empty()){
      int cr=q.front().first;
      int cc=q.front().second; 
      q.pop(); 
      
      for(int i=0;i<4;i++){
           int nr=cr+dr[i];
           int nc=cc+dc[i];
           if(nr<0 || nc<0 || nr>=n || nc>=n)continue;
           if(visited[nr][nc])continue;
           if(type[nr][nc]!=mytype)continue; 
           
           member.push_back({nr,nc});
           q.push({nr,nc}); 
           visited[nr][nc]=true; 
      }
   } 
   //탐색 완료 
   //대표자 선정 
   sort(member.begin(),member.end(),cmp); 
   ci master=member[0]; 
   int totalnum=member.size();
   //신앙심 수정 
   //대표자는 그룹원수-1 더하고, 나머진 -1 
   faith[master.first][master.second]+=(member.size()-1); 
   for(int i=1;i<totalnum;i++){
     faith[member[i].first][member[i].second]-=1; 
   }
   info tmp={mytype,totalnum,master,member}; 
   return tmp;    
}

void LUNCH(){


   //1. 그룹 만들기, 그룹 대표 만들기, 신앙심 수정   
   vector<vector<int>>visited;
   visited.assign(n,vector<int>(n,0)); 

   map<int,vector<info>>groupresult; 

   for(int i=0;i<n;i++){
     for(int j=0;j<n;j++){
        if(!visited[i][j]){
           string mytype=type[i][j];  
           info res=makegroup(i,j,visited,mytype);  
             
           //그룹별 저장   
           //그룹 1 - T,C,M 
           if(mytype=="T" || mytype=="C" || mytype=="M"){
              groupresult[0].push_back(res); 
           }
           else if(mytype=="CM" || mytype=="TM" || mytype=="TC"){
              //그룹 2 - CM,TM,TC 
              groupresult[1].push_back(res); 
           }
           else{//그룹 3 - TCM 
              groupresult[2].push_back(res); 
           }

        }
     }
   }
   DINNER(groupresult); 

}

void SCORE(){
   vector<int>num; 
   num.assign(7,0);
   for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
       if(type[i][j]=="TCM")num[0]+=faith[i][j];
       if(type[i][j]=="TC")num[1]+=faith[i][j];
       if(type[i][j]=="TM")num[2]+=faith[i][j];
       if(type[i][j]=="CM")num[3]+=faith[i][j];
       if(type[i][j]=="M")num[4]+=faith[i][j];
       if(type[i][j]=="C")num[5]+=faith[i][j];
       if(type[i][j]=="T")num[6]+=faith[i][j];
     //  cout<<type[i][j]<<" ";
    }
    //cout<<"\n";
   }
   for(int i=0;i<7;i++)cout<<num[i]<<" ";
   cout<<'\n';
 /*
   for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
         cout<<faith[i][j]<<" ";
      }
      cout<<"\n";
   }
   for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
         cout<<type[i][j]<<" ";
      }
      cout<<"\n";
   }
   */
}

int main() {


    INPUT(); 
    for(int i=0;i<t;i++){
      MORNING(); 
      LUNCH();
      SCORE();
    }
     

    return 0;
}