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

int main() {
   
    cin>>n>>m>>k>>c; 
    arr.assign(n,vector<int>(n,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>arr[i][j]; 
        }
    }
    for(int year=0;year<m;year++){
       //0.제조제 뿌린데 깨우기 
        int len=wakeup[year].size(); 
        for(int i=0;i<len;i++){
            arr[wakeup[year][i].first][wakeup[year][i].second]=0; //아무것도 없는 상태로 만들기 
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
        /*
        cout<<"나무 자람\n";
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
               cout<<arr[i][j];
            }
            cout<<"\n";
        }
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
       /*  cout<<"나무 복제\n";
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
               cout<<arr[i][j];
            }
            cout<<"\n";
        }
      */


        //3.제초제 뿌리기 
        //i,j에서 박멸될 나무 목록 
        map<ci,vector<ci>>deletelist; 
        int maxs=0;
        int maxr,maxc; 
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(arr[i][j]==-1 || arr[i][j]==0 )continue; //벅이거나 아무것도 없거나 
                int sum=0;
                if(arr[i][j]>0)sum+=arr[i][j]; 
                
                for(int p=0;p<4;p++){
                   //k만큼 감 
                   for(int s=1;s<=k;s++){
                       int nr=i+dir[p]*s;
                       int nc=j+dic[p]*s; 
                       if(nr<0 || nc<0 || nr>=n || nc>=n)break; 
                       //벽이거나 나무가 아예 없는 칸이 있는경우 
                       if(arr[nr][nc]==-1 || arr[nr][nc]==0)break; 
                       //제초제가 있는곳이면 되나봄? 
                       if(arr[nr][nc]>0){
                         deletelist[{i,j}].push_back({nr,nc}); 
                         sum+=arr[nr][nc]; 
                       }
           
                   }
                }
                if(maxs<sum){
                   // cout<<i<<","<<j<<"일때"<<sum<<"\n";
                    maxs=sum;
                    maxr=i; maxc=j; 
                }
            }
        }
        total+=maxs; 
        deletelist[{maxr,maxc}].push_back({maxr,maxc});
        arr[maxr][maxc]=-2;
        //박멸
        for(int i=0;i<deletelist[{maxr,maxc}].size();i++){
            arr[deletelist[{maxr,maxc}][i].first][deletelist[{maxr,maxc}][i].second]=-2; 
            wakeup[year+c].push_back({deletelist[{maxr,maxc}][i].first,deletelist[{maxr,maxc}][i].second});
        }
        
    } 
    cout<<total;

   
    return 0;
}