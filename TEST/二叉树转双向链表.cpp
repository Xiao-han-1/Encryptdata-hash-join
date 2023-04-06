class Solution {
public:
    Node* treeToDoublyList(Node* root) {
        if(root == nullptr) return nullptr;
        dfs(root);
        head->left = pre;
        pre->right = head;
        return head;
    }
private:
    Node *pre, *head;
    void dfs(Node* cur) {
        if(cur == nullptr) return;
        dfs(cur->left);
        if(pre != nullptr) pre->right = cur;
        else head = cur;
        cur->left = pre;
        pre = cur;
        dfs(cur->right);
    }
};

int main()
{
    Node* root;
    Node* head,pre;
    pre=NULL;
    dfs(root);
    head->left=pre;
    pre->right=head;
}

void(node* cur)
{
    if(cur==nullptr) return;
    dfs(cur->left);
    if(pre==NULL) head=cur;
    else pre->right=cur;
    cur->left=pre;
    pre=cur;
    dfs(cur->right);
}