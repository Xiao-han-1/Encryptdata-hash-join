#include <iostream>
#include <vector>

using namespace std;
struct node
{
    int value;
    node* le;
    node* ri;
}

node* dfs(int root,int left;int right)
{
    if(left>right) return null;
    node root=new node(forward[root]);
    int i=mp[forward[root]];
    root->left=dfs(root+1,left,i-1);
    root->right=dfs(root+i-left+1,i+1,right);

}
int main()
{
    vector<int>forward;
    vector<int>medium;
    
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        int tmp;
        cin>>tmp;
        forward.push_back(tmp);
    }
    for(int i=0;i<n;i++)
    {
        int tmp;
        cin>>tmp;
        medium.push_back(tmp);
    }
    root=dfs(0,0,forward.size()-1);
}