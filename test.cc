
struct TreeNode{
    int value = __INT_MAX__;
    struct TreeNode* left = nullptr;
    struct TreeNode* right = nullptr;

    TreeNode(int v): value(v), left(nullptr), right(nullptr){}
};

void PreTraverse(TreeNode * node, std::vector<int>& res){
    if (node == nullptr) return;
    res.push_back(node->value);
    PreTraverse(node->left, res);
    PreTraverse(node->right, res);
}

void MidTraverse(TreeNode * node, std::vector<int>& res){
    if (node == nullptr) return;
    MidTraverse(node->left, res);
    res.push_back(node->value);
    MidTraverse(node->right, res);
}

void postTraverse(TreeNode * node, std::vector<int>& res){
    if (node == nullptr) return;
    if(node->left != nullptr) postTraverse(node->left, res);
    if(node->right != nullptr) postTraverse(node->right, res);
    res.push_back(node->value);
}

class BinaryTreeTraversal{
public:
    void PreTraverse(TreeNode * node, std::vector<int>& res);
    void MidTraverse(TreeNode * node, std::vector<int>& res);
    void postTraverse(TreeNode * node, std::vector<int>& res);
    std::vector<int> preorderTraversal(TreeNode * root){
        std::vector<int> res;
        PreTraverse(root, res);
        MidTraverse(root, res);
        postTraverse(root, res);
        return res;
    }
};



class BinaryTreeIterate{
public:
    void PreIterate(TreeNode * node, std::vector<int>& res);
    void MidIterate(TreeNode * node, std::vector<int>& res);
    void postIterate(TreeNode * node, std::vector<int>& res);

    std::vector<int> preorderIterate(TreeNode * root){
        std::vector<int> res;
        std::stack<TreeNode*> s;
        if(root == nullptr) return res;
        s.push(root);
        TreeNode* temp = nullptr;

        while(!s.empty()){
            temp = s.top();
            s.pop();
            res.push_back(temp->value);
            if(temp->right != nullptr) s.push(temp->right);
            if(temp->left != nullptr) s.push(temp->left);
        }
        return res;
    }

    std::vector<int> postorderIterate(TreeNode * root){
        std::vector<int> res;
        std::stack<TreeNode*> s;
        if(root == nullptr) return res;
        s.push(root);
        TreeNode* temp = nullptr;

        while(!s.empty()){
            temp = s.top();
            s.pop();
            res.push_back(temp->value);
            if(temp->left != nullptr) s.push(temp->left);
            if(temp->right != nullptr) s.push(temp->right);
        }
        std::reverse(res.begin(), res.end());
        return res;
    }

    std::vector<int> midorderIterate(TreeNode * root){
        std::vector<int> res;
        std::stack<TreeNode*> s;
        if(root == nullptr) return res;

        TreeNode* temp = root;

        while(!s.empty() || temp != nullptr){
            if(temp != nullptr){
                s.push(temp);
                temp = temp->left;
            }else{
                TreeNode* node = s.top();
                s.pop();
                res.push_back(node->value);
                temp = node->right;
            }
        }
        return res;
    }

    std::vector<int> levelIterate(TreeNode * root){
        std::vector<int> res;
        std::queue<TreeNode*> q;
        if(root == nullptr) return res;
        s.push(root);
        TreeNode* temp = nullptr;

        while(!q.empty()){
            int loop = q.size();
            for(int i = 0; i < loop; i++){
                temp = q.front();
                q.pop();
                res.push_back(temp->value);
                if (temp->left) q.push_back(temp->left);
                if (temp->right) q.push_back(temp->right);
            }
        }
        return res;
    }
};






