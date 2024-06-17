#include <iostream>
#include <vector>
#include<queue> 
#include<algorithm>

using namespace std; 
typedef pair<int,int> ci; 
struct info{
  vector<vector<int>>arr;
  int score; //획득 가치 
  int r;
  int c;
  int num; //회전 각도
  vector<ci>arrs; //리스트 
};

int dr[4]={0,0,1,-1};
int dc[4]={1,-1,0,0};

vector<vector<int>>wall; //5x5 
vector<int>replaces;
int kk,m;
int idx; 
bool cmp(ci a,ci b){
  //열 작 -> 행 작  
   if(a.second==b.second){
     return a.first>b.first; 
   }
   return a.second<b.second; 
}
bool cmp2(info a,info b){
    if(a.score==b.score){
        if(a.num==b.num){
            if(a.c==b.c){
                return a.r<b.r;
            }
            return a.c<b.c;
        }

        return a.num<b.num;
    } 
    return a.score>b.score;
}


vector<vector<int>>rotate(vector<vector<int>>arr, int num,int cr,int cc){

    //num : 회전 각도 cr,cc : 중심각도 
    vector<vector<int>>tmp; 
    tmp=arr; 
    if(num==90){
        for(int i=0;i<=2;i++){
            for(int j=0;j<=2;j++){
               tmp[cr-1+j][cc-1+2-i]=arr[cr-1+i][cc-1+j];
            }
        }
    }
    if(num==180){
        for(int i=0;i<=2;i++){
            for(int j=0;j<=2;j++){
               tmp[cr-1+2-i][cc-1+2-j]=arr[cr-1+i][cc-1+j];
            }
        }

    }
    if(num==270){
        for(int i=0;i<=2;i++){
            for(int j=0;j<=2;j++){
                tmp[cr-1+2-j][cc-1+i]=arr[cr-1+i][cc-1+j]; 
            }
        }

    }
    return tmp; 
}

pair<int,vector<ci>> bfs(int r,int c,vector<vector<int>>&visited,vector<vector<int>>rtmp){
  vector<ci>tlist;  
  int cnt=0; 
  queue<ci>q;
  q.push({r,c}); 
  int nn=rtmp[r][c];
  visited[r][c]=true; 
  while(!q.empty()){
    int cr=q.front().first;
    int cc=q.front().second;
    //if(visited[cr][cc])q.pop(); 
    tlist.push_back({cr,cc}); 
    q.pop(); 
    cnt++; 
    
    for(int i=0;i<4;i++){
       int nr=cr+dr[i];
       int nc=cc+dc[i];
       if(nr<0 || nc<0 || nr>=5 || nc>=5)continue;
       if(visited[nr][nc])continue;
       if(rtmp[nr][nc]!=nn)continue;
       visited[nr][nc]=true;
       q.push({nr,nc}); 
    }
  }
  return {cnt,tlist}; 
}
void fill(vector<ci>max_list,vector<vector<int>>&maxtmp){
    sort(max_list.begin(),max_list.end(),cmp); 
     
  /*  cout<<"회전 현황\n";
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            cout<<maxtmp[i][j]<<' ';
        }
        cout<<'\n';
    }
*/
   // cout<<"채울 목록\n";  
   for(int i=0;i<max_list.size();i++){
  //      cout<<max_list[i].first<<' '<<max_list[i].second<<'\n';
    }

    for(int i=idx;i<idx+max_list.size();i++){
        maxtmp[max_list[i-idx].first][max_list[i-idx].second]=replaces[i]; 
    //   cout<<max_list[i-idx].first<<","<<max_list[i-idx].second<<"에 "<<replaces[i]<<"\n";
    }
    idx+=max_list.size(); 
}


int main() {
    int ans=0;
    int idx=0; 
    wall.assign(5,vector<int>(5,0)); 
    cin>>kk>>m;
    replaces.assign(m,0);
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            cin>>wall[i][j]; 
        }
    }
    for(int i=0;i<m;i++){
        cin>>replaces[i];
    }

   
   for(int turn=1;turn<=kk;turn++){
    //가장 큰 획득 점수 -> 열 작 -> 행 작 
    int turnscore=0; 
    int max_score=-1; 
    int maxr,maxc; //회전 좌표
    int maxnum; //회전 각도 
    vector<ci>max_list; 
    vector<vector<int>>maxtmp; 
    
    //4x4x3 중 하나 골라야함 
    //획득 가치 가장 큰 -> 회전 각도 작은 -> 열 작 -> 행 작 
    vector<info>infos; 
    for(int k=1;k<=3;k++){ //열
        for(int j=1;j<=3;j++){ //행
            for(int i=1;i<=3;i++){ //각도 
               //유물 있는지, 점수 
               vector<vector<int>>rtmp=rotate(wall,90*i,k,j); 
               vector<ci>lists; 
               int score=0; 

               vector<vector<int>>visited;
               visited.assign(5,vector<int>(5,0)); 
               

               for(int s=0;s<5;s++){
                  for(int ss=0;ss<5;ss++){
                     if(!visited[s][ss]){
                        pair<int,vector<ci>>tcnt=bfs(s,ss,visited,rtmp); 

                        //유적이면 
                        if(tcnt.first>=3){
                            score+=tcnt.first;
                           //유물인 좌표 모두 저장하기 
                           for(int a=0;a<tcnt.second.size();a++){
                             lists.push_back(tcnt.second[a]);
                           }
                        }
                      
                     }
                  }
               }
               infos.push_back({rtmp,score,k,j,90*i,lists});
            }
        }
    }
    sort(infos.begin(),infos.end(),cmp2); 
    for(int i=0;i<infos.size();i++){
      // cout<<infos[i].score<<' '<<infos[i].r+1<<","<<infos[i].c+1<<' '<<infos[i].num<<'\n';
    }



    max_score=infos[0].score;
    maxr=infos[0].r;
    maxc=infos[0].c;
    max_list=infos[0].arrs; 
    maxnum=infos[0].num; 
   // cout<<max_score<<' '<<maxr<<' '<<maxc<<' '<<maxnum<<'\n';
    //cout<<"유물 목록\n";
   // for(int i=0;i<max_list.size();i++){
     //   cout<<max_list[i].first<<' '<<max_list[i].second<<'\n';
   // }
    maxtmp=infos[0].arr;
    //1차에서 유물이 없으면 걍 끝내기 
    if(max_score==0){
        break; 
    }  
    turnscore+=max_score; 
   // cout<<"회전 후<<'\n";
   // for(int i=0;i<5;i++){
    //    for(int j=0;j<5;j++){
     //       cout<<maxtmp[i][j]<<" ";
    //  }
    //   cout<<'\n';
   // }

    //3.빈 공간 채우기 
    fill(max_list,maxtmp);


    //cout<<"채운후\n";
  //  for(int i=0;i<5;i++){
   //     for(int j=0;j<5;j++){
    ////        cout<<maxtmp[i][j]<<" ";
    //   }
     //  cout<<'\n';
    //}
   // cout<<turn<<"턴 1차: "<<turnscore<<'\n';

    //4. 연쇄적으로 유물이 생겼는가?그럼 획득하고 3으로 돌아가기 
    //아니면 다음 턴으로 가기 
    while(1){
       //유적이 있는가? -> 카운트 
       vector<ci>lists2; 
       int score2=0; 
       vector<vector<int>>visited2;
       visited2.assign(5,vector<int>(5,0)); 
       for(int s=0;s<5;s++){
           for(int ss=0;ss<5;ss++){
               if(!visited2[s][ss]){
                   pair<int,vector<ci>>tcnt2=bfs(s,ss,visited2,maxtmp); 
                    if(tcnt2.first<3)continue; 
                    //유적이면 
                     score2+=tcnt2.first;
                     //유물인 좌표 모두 저장하기 
                     for(int a=0;a<tcnt2.second.size();a++){
                         lists2.push_back(tcnt2.second[a]);
                     }

                }
            } 
       }
        //채우기  
        if(score2==0)break; 
       //cout<<turn<<"턴 추가 점수 "<<score2<<'\n';
        turnscore+=score2;
       // cout<<turn<<"턴 현황: "<<turnscore<<'\n';
        fill(lists2,maxtmp);
      //  cout<<"채울 목록\n";
      //  for(int i=0;i<lists2.size();i++){
      //      cout<<lists2[i].first<<' '<<lists2[i].second<<'\n';
      //  }

       // cout<<"추가로  채운후\n";
       // for(int i=0;i<5;i++){
        //    for(int j=0;j<5;j++){
         //      cout<<maxtmp[i][j]<<' ';
         //   }
        //   cout<<'\n';
       // }
       // cout<<'\n';
    } 
 
   
    cout<<turnscore<<' ';   
    wall=maxtmp; 
   }
 
    return 0;
}