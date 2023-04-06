/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
 /**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Codec {
public:
    // Encodes a tree to a single string.
    //队列层序遍历。
    string serialize(TreeNode* root) {
        string res = "";
        if(root==NULL) return "";
        //处理一般
        queue<TreeNode*> my_queue;
        my_queue.push(root);
        TreeNode * cur = new TreeNode(0);
        while(!my_queue.empty())
        {
            //记录队列里的元素长度
            int len  = my_queue.size();
            while(len--)
            {
                cur = my_queue.front();
                my_queue.pop();
                if(cur==NULL)
                {
                    //res  = res + "$";
                    res.push_back('$');
                }else
                {
                     //res = res + to_string(cur->val);
                     res.append(to_string(cur->val));
                }
                res.push_back(',');
                if(cur!=NULL)
                {
                my_queue.push(cur->left);
                my_queue.push(cur->right);
                }
            }
        }
        res.pop_back();
        
        return res;  
    }
    // Decodes your encoded data to tree.
    //重建二叉树。先将节点存起来，然后再遍历给他们建立结构！
    TreeNode* deserialize(string data) {
        //处理特殊
        if(data.size()==0) return NULL;
        int len = data.size();
        int i = 0;
        vector<TreeNode*> vec;
        while(i<len)
        {
            //遇到逗号停下来。
            string str = "";
            while(i<len&&data[i]!=',')
            {
                str.push_back(data[i]);
                i++;
            }
            //新建根节点.
            if(str=="$")
            {
                TreeNode * temp = NULL;
                vec.push_back(temp); //直接存NULL也可以。
            }else{
                int temp = std::stoi(str);
                TreeNode * cur = new TreeNode(temp);
                vec.push_back(cur);
            }
            i++;
        }
        //遍历vec，构建二叉树的结构。
        int j = 1;
        for(int i=0;j<vec.size();i++)
        {
            if(vec[i]==NULL) continue;
            if(j<vec.size()) vec[i]->left = vec[j++];
            if(j<vec.size()) vec[i]->right = vec[j++];
           
        }
        return vec[0];
    }
};
// Your Codec object will be instantiated and called as such:
// Codec codec;
// codec.deserialize(codec.serialize(root));

void deser(string res)
{
   vector<node*>tree;
   int i=1;
   int len=res.length();
   {
    string str="";
    while(i<len&&res[i]!=',')
    {
        str=str+res[i];
        i++
    }
    node* tmp=new node(atoi(str));
    tree.push_back(tmp);
    i++;
   }
   node* root=tree[1];
   for(int i=1;i<=tree.length();i++)
   {
      if(i*2<=tree.length())
      {
        node left=new node(tree[2*i-1]);
      node right=new node(tree[2*i]);
      else n
      }
      
   }

}
int main();
{
    node* root;

    string res=" ";
    queue<node*> q;
    q.push(root);
    while(q.empty()!=true)
    {
        node t=q.front();
        q.pop();
        if(t==NULL)
        {
            res.append("NULL");
        }
        else 
        {
            string s=to_string(t.val);
            res.append(s);
        }
        res=res+",";
        q.push(t->left);
        q.push(t->right);
        
    }


}