#include<bits/stdc++.h>
using namespace std;

//data structure for the multisplay tree
class multiSplayTree{
    public:
        explicit multiSplayTree(const vector<double> &weights);
        ~multiSplayTree(void);

        bool contains(size_t key);
    
    private:
        struct Node{
            size_t key, depth, minDepth;
            bool isRoot;
            Node *left, *right, *parent;

            Node(size_t key, size_t depth = 0, size_t minDepth = 0, bool isRoot = 0){
                this->key = key;
                left = right = parent = 0;
                this->isRoot = isRoot;
                this->depth = depth;
                this->minDepth = minDepth;
            }
        };

        mutable Node *root;
        static Node *treeFor(size_t low, size_t high, size_t depth, bool isRoot);
        void rotate(Node *cur);
        void splay(Node *cur, Node *top);
        void switchPath(Node *cur);
        void expose(Node *cur);
        void check(Node *cur);
        Node *refParent(Node *cur, int num);
        multiSplayTree(multiSplayTree const &) = delete;
        void operator = (multiSplayTree const &) = delete;
};

void multiSplayTree::check(multiSplayTree::Node *cur){
    //if current node is a null pointer
    if(!cur) return;
    //printing the parent of the current node if any
    cout<<"Parent of "<<cur->key<<" = ";
    //if the current node has a parent 
    if(cur->parent){
        cout<<cur->parent->key<<endl;
    }
    //if the current node does not have a parent
    else{
        cout<<"NULL"<<endl;
    }
    //calling for the left subtree
    check(cur->left);
    //calling for the right subtree
    check(cur->right);
    return;
}

multiSplayTree::multiSplayTree(const vector<double>& weights){
    root = treeFor(0, weights.size(), 0,  true);
    return;
}

multiSplayTree::~multiSplayTree(void){
    //iterating while root is not equal to nullptr
    while(root != nullptr){
        //if root does not have left child
        if(root->left == nullptr){
            Node *next = root->right;
            delete root;
            root = next;
        }
        //if root have a left child
        else{
            Node *child = root->left;
            root->left = child->right;
            child->right = root;
            root = child;
        }
    }
    return;
}

multiSplayTree::Node *multiSplayTree::treeFor(size_t low, size_t high, size_t depth, bool isRoot){
    //terminating condition
    if(low == high)  return nullptr;
    size_t mid = low + (high - low)/2;
    //allocating memory for the new node and initialising its values
    Node *tmp = new Node {mid, depth, depth, isRoot};
    //calling for the left subtree
    tmp->left = treeFor(low, mid, depth + 1, false);
    //calling for the right subtree
    tmp->right = treeFor(mid + 1, high, depth + 1, true);  
    if(tmp->left) tmp->left->parent = tmp;
    if(tmp->right) tmp->right->parent = tmp;
    return tmp;
}

void multiSplayTree::rotate(Node *cur){
    //pointer to store the parent of the current node
    Node *tmp = cur->parent;
    //if the parent is a root then the current node will become the root after the rotation
    if(tmp->isRoot){
        tmp->isRoot = false;
        cur->isRoot = true;
    }
    if(root == tmp) root = cur;
    //if there is a grandparent then perform the rotation
    if(tmp->parent){
        int cnt =  (tmp->parent->right == tmp);
        //if tmp is the left child of its parent
        if(cnt == 0){
            tmp->parent->left = cur;
        }
        //if tmp is the right child of its parent
        else{
            tmp->parent->right = cur;
        }
    }
    //update the current node's parent 
    cur->parent = tmp->parent;
    int cnt = (tmp->right == cur);
    //if current node is left child of tmp
    if(cnt == 0){
        //performing the rotation
        tmp->left = cur->right;
        if(cur->right){
            cur->right->parent = tmp;
        }
        cur->right = tmp;
        tmp->parent = cur;
    }
    //if current node is right child of tmp
    else{
        //performing the rotation
        tmp->right = cur->left;
        if(cur->left){
            cur->left->parent = tmp;
        }
        cur->left = tmp;
        tmp->parent = cur;
    }
    //updating the mindepths of the current node and tmp
    cur->minDepth = tmp->minDepth;
    tmp->minDepth = tmp->depth;
    if(tmp->left){ 
        tmp->minDepth = min(tmp->minDepth, tmp->left->minDepth);
    }
    if(tmp->right){
        tmp->minDepth = min(tmp->minDepth, tmp->right->minDepth);
    }
    return;
}

void multiSplayTree::splay(Node *cur, Node *top = 0){
    //rotate until current node is either a root or current node's parent is equal to top
    while(!(cur->isRoot || cur->parent == top)){
        Node *tmp = cur->parent;
        //if tmp is neither a root not its parent is equal to top
        if(!(tmp->isRoot || tmp->parent == top)){
            Node *tmp2 = tmp->parent;
            if((tmp2->left == tmp && tmp->left == cur) || (tmp2->right == tmp && tmp->right == cur)){
                rotate(tmp);
            }
            else{
                rotate(cur);
            }
        }
        //if tmp is either a root or its parent is equal to top
        rotate(cur);
    }
    return;
}

multiSplayTree::Node *multiSplayTree::refParent(multiSplayTree::Node *cur, int cnt){
    //pointer to store the child
    Node *tmp = NULL;
    if(cnt == 0){
        tmp = cur->left;
    }
    else{
        tmp = cur->right;
    }
    //tmp is equal to left child of current node if cnt is equal to 0 and vice versa
    while(1){
        if(cnt == 0){
            //if mindepth of right child is less than the depth of current node
            if(tmp->right && tmp->right->minDepth < cur->depth){
                tmp = tmp->right;
            }
            //if midepth of left child is less than the depth of the current node
            else if(tmp->left && tmp->left->minDepth < cur->depth){
                tmp = tmp->left;
            }
            //breaking condition
            else{
                break;
            }
        }
        else{
            //if midepth of left child is less than the depth of the current node
            if(tmp->left && tmp->left->minDepth < cur->depth){
                tmp = tmp->left;
            }
            //if mindepth of right child is less than the depth of current node
            else if(tmp->right && tmp->right->minDepth < cur->depth){
                tmp = tmp->right;
            }
            //breaking condition
            else{
                break;
            }
        }
    }
    return tmp;
}

void multiSplayTree::switchPath(Node *cur){
    //if current node has a left child
    if(cur->left){
        //if the mindepth of the left child is greater than the depth of the current node
        if(cur->left->minDepth > cur->depth){
            cur->left->isRoot = !(cur->left->isRoot);
        }
        else{
            Node *tmp = refParent(cur, 0);
            splay(tmp, cur);
            if(tmp->right){
                tmp->right->isRoot = !(tmp->right->isRoot);
            }
        }
    }
    //if the current node has a right child
    if(cur->right){
        //if the mindepth of the right child is greater than the depth of the current node
        if(cur->right->minDepth > cur->depth){
            cur->right->isRoot = !(cur->right->isRoot);
        }
        else{
            Node *tmp2 = refParent(cur, 1);
            splay(tmp2, cur);
            if(tmp2->left){
                tmp2->left->isRoot = !(tmp2->left->isRoot);
            }
        }
    }
}

void multiSplayTree::expose(Node *cur){
    //pointer to store the current node
    Node *tmp = cur;
    //iterating while tmp has a parent
    while(tmp->parent){
        Node *tmp2 = tmp->parent;
        //if tmp is a root
        if(tmp->isRoot){
            splay(tmp2);
            switchPath(tmp2);
        }
        tmp = tmp2;
    }
    splay(cur);
    return;
}

bool multiSplayTree::contains(size_t key){ 
    //pointers to store the current and previous nodes
    Node *curr = root;
    Node *prev = root;
    //iterate until we either find the key or find nullptr
    while(curr && curr->key != key){
        prev = curr;
        if(key < curr->key){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    //if we found nullptr
    if(!curr){
        expose(prev);
        return false;
    }
    //if we found the key
    expose(curr);
    return true;
}

//int main();
