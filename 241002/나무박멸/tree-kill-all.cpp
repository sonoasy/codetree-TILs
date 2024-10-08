#include <iostream>
#include<vector>
#include<map>

using namespace std;

typedef pair<int,int>ci; 

int n,m,k,c; 
vector<vector<int>>arr; 
//제초제에서 깨어날 목록
map<int,vector<ci>>wakeup;
int dr[4]={0,0,1,-1};
int dc[4]={1,-1,0,0};
//대갹선 방향
int dir[4]={-1,-1,1,1};
int dic[4]={-1,1,-1,1};

int total=0; 

vector<vector<int>>deletes;


int main() {
   
    cin>>n>>m>>k>>c; 
    arr.assign(n,vector<int>(n,0));
    deletes.assign(n,vector<int>(n,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>arr[i][j]; 
        }
    }
    for(int year=1;year<=m;year++){
       //0.제조제 뿌린데 깨우기 
       // int len=wakeup[year].size(); 
        //for(int i=0;i<len;i++){
       //     arr[wakeup[year][i].first][wakeup[year][i].second]=0; //아무것도 없는 상태로 만들기 
       // }          
        //이걸 그냥 nxn으로 ? 겹치는거 있을수 있으니까 
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(deletes[i][j]==year){
                    arr[i][j]=0; //아무것도 없는 상태로 바꾸기 
                    deletes[i][j]=0;
                }
            }
        }


        
        //1.나무 성장 
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(arr[i][j]<=0)continue;
                int cnt=0;
                for(int k=0;k<4;k++){
                    int nr=i+dr[k];
                    int nc=j+dc[k];
                    if(nr<0 ||nc<0 ||nr>=n ||nc>=n)continue;
                    if(arr[nr][nc]>0)cnt++;
                }
                arr[i][j]+=cnt;
            }
        }
      //  
      /*
        cout<<"나무 자람\n";
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
             cout<<arr[i][j]<<" ";
            }
            cout<<"\n";
        }
      cout<<"\n";
      */
        //2.나무 복제 
        
        vector<vector<int>>tt;
        tt=arr;
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(arr[i][j]<=0){
                    
                    continue;
                }
                vector<ci>tmp; 
                int cnt=0;
                for(int k=0;k<4;k++){
                    int nr=i+dr[k];
                    int nc=j+dc[k];
                    if(nr<0 ||nc<0 ||nr>=n ||nc>=n)continue;
                    if(arr[nr][nc]==0){
                        cnt++;
                        tmp.push_back({nr,nc}); 
                    }
                }
                int grow;
                if(cnt!=0){
                    grow=arr[i][j]/cnt; 
                    for(int k=0;k<tmp.size();k++){
                      tt[tmp[k].first][tmp[k].second]+=grow; 
                    }
                }
                
            }
        }
        
        arr=tt;
       /*
       cout<<"나무 복제\n";
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
               cout<<arr[i][j]<<" ";
            }
        cout<<"\n";
       }
       cout<<"\n";
      */
         

        //3.제초제 뿌리기 
        //i,j에서 박멸될 나무 목록 
        map<ci,vector<ci>>deletelist; 
        int maxs=0;
        int maxr,maxc; 
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                //제초제일 경우 ? 
                //이거!!!! 
                if(arr[i][j]==-1 || arr[i][j]==0 || arr[i][j]==-2 )continue; //벅이거나 아무것도 없거나 
                int sum=0;
                if(arr[i][j]>0)sum+=arr[i][j]; 
                
                for(int p=0;p<4;p++){
                   //k만큼 감 
                   for(int s=1;s<=k;s++){
                       int nr=i+dir[p]*s;
                       int nc=j+dic[p]*s; 
                       if(nr<0 || nc<0 || nr>=n || nc>=n)break; 
                       //벽이거나 나무가 아예 없는 칸이 있는경우 
                       if(arr[nr][nc]==-1)break; 
                       //제초제가 있는곳이면 되나봄? 
                       if(arr[nr][nc]==0){
                         deletelist[{i,j}].push_back({nr,nc}); 
                         break;
                       }
                       if(arr[nr][nc]>0){
                         deletelist[{i,j}].push_back({nr,nc}); 
                         sum+=arr[nr][nc]; 
                       }
                       //제초제 전에 뿌린데라도 갱신해야됨
                       if(arr[nr][nc]==-2){
                        deletelist[{i,j}].push_back({nr,nc}); 
                        break;
                       }
           
                   }
                }
                if(maxs<sum){
                    maxs=sum;
                    maxr=i; maxc=j; 
                   //  cout<<maxr<<","<<maxc<<" "<<maxs<<"선택\n";
                }
            }
        }
        if(maxs==0)break;
        total+=maxs; 
      //  deletelist[{maxr,maxc}].clear();
        deletelist[{maxr,maxc}].push_back({maxr,maxc});
       // cout<<maxr<<","<<maxc<<" "<<maxs<<"선택\n";
        arr[maxr][maxc]=-2;
        
        //박멸
        for(int i=0;i<deletelist[{maxr,maxc}].size();i++){
            arr[deletelist[{maxr,maxc}][i].first][deletelist[{maxr,maxc}][i].second]=-2; //제초제 있는곳 
            //wakeup[year+c+2].push_back({deletelist[{maxr,maxc}][i].first,deletelist[{maxr,maxc}][i].second}); //이전에 제초제 뿌려진데면 어떡함? 
            //기존게 왜 안바뀜? 
            deletes[deletelist[{maxr,maxc}][i].first][deletelist[{maxr,maxc}][i].second]=year+c+1; //나중에 꺠어날 목록
        }
        
         /*
       cout<<year<<" 박멸 후 근황\n";
       for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
               cout<<arr[i][j]<<" ";
           }
            cout<<"\n";
        }
        cout<<"\n"; 
        cout<<"제초제 현황\n";
       for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
               cout<<deletes[i][j]<<" ";
            }
            cout<<"\n";
      }
        */

    } 
    cout<<total;

   
    return 0;
}