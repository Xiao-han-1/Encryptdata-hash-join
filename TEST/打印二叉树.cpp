void bfs()
{
    queue<node*>q;
    q.push_back(root);
    vector<vector<int>> re;
    while(q.empty()!=true)
    {
        int len=q.size();
        vector<int>tmp;
        for(int i=0;i<len;i++)
        {
        tmp.push_back(q[i]);
        node* no=q.front();
        q.pop();
        q.push(no->left());
        q.oush(no->right());
        }
        
    }
}
