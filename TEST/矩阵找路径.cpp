#include<bits/stdc++.h>
using namespace std;
vector<vector<char>>board;
int n;
bool flag;
string word;
int a[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
bool dfs(int i,int j,vector<vector<bool>>&vis,int k)
{
  if(k==word.length()-1)
  flag=true;
  if(board[i][j]==word[k])
  {
    vis[i][j]=true;
     for(int p=0;p<4;p++)
     {
        int ni=i+a[p][0];
        int nj=j+a[p][1];
        if((i+a[p][0]>=0&&i+a[p][0]<n)&&(i+a[p][1]>=0&&i+a[p][1]<n)&&vis[ni][nj]!=true)
        dfs(i+a[p][0],j+a[p][1],vis,k++);
     }
  }
  return flag;
}
int main()
{
     cin>>n;
    vector<vector<bool>>vis(n,vector<bool>(n,false));
    flag=false;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            
            cin>>board[i][j];
        }
    }
    int i=0;
    int j=0;
    bool flag=dfs(i,j,vis,0);
    if(flag)
    cout<<true<<endl;
}
