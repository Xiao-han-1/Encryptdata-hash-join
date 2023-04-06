
int main()
{
    node* root;
    int  k;
    dfs(root);
}
void dfs(node* root,int k)
{
    k--;
    if(k==0)
    return root->val;
    dfs(root->right);
    dfs(root->left);
}