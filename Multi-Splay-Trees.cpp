#include<bits/stdc++.h>
using namespace std;

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
                right = left = parent = 0;
                this->isRoot = isRoot;
                this->depth = depth;
                this->minDepth = minDepth;
            }
        };

        mutable Node *root;
        static Node *treeFor(size_t low, size_t high, size_t depth, bool isRoot);
        void rotate(Node *x);
        void splay(Node *x, Node *top);
        void switchPath(Node *y);
        void expose(Node *v);
        void check(Node *cur);
        Node *refParent(Node *y, int r);
        multiSplayTree(multiSplayTree const &) = delete;
        void operator = (multiSplayTree const &) = delete;
};

void multiSplayTree::check(multiSplayTree::Node *cur){
    if(!cur) return;

    cout<<"Parent of "<<cur->key<<" = ";
    if(cur->parent){
        cout<<cur->parent->key<<endl;
    }
    else{
        cout<<"NULL"<<endl;
    }

    check(cur->left);
    check(cur->right);
}

multiSplayTree::multiSplayTree(const vector<double> &weights){
    root = treeFor(0, weights.size(), 0,  true);
}

multiSplayTree::~multiSplayTree(void){
    while(root != NULL){
        if(root->left == NULL){
            Node *next = root->right;
            delete root;
            root = next;
        }
        else{
            Node *child = root->left;
            root->left = child->right;
            child->right = root;
            root = child;
        }
    }
}

multiSplayTree::Node *multiSplayTree::treeFor(size_t low, size_t high, size_t depth, bool isRoot){
    if(low == high)  return NULL;
    
    size_t mid = low + (high - low)/2;
    Node *tmp = new Node{mid, depth, depth, isRoot};
    tmp->left = treeFor(low, mid, depth + 1, false);
    tmp->right = treeFor(mid + 1, high, depth + 1, true);  
    if(tmp->left) tmp->left->parent = tmp;
    if(tmp->right) tmp->right->parent = tmp;
    return tmp;
}

void multiSplayTree::rotate(Node *cur){
    Node *tmp = cur->parent;
    if(tmp->isRoot){
        tmp->isRoot = false;
        cur->isRoot = true;
    }
    if(root == tmp) root = cur;
    if(tmp->parent){
        int cnt =  (tmp->parent->right == tmp);
        if(cnt == 0){
            tmp->parent->left = cur;
        }
        else{
            tmp->parent->right = cur;
        }
    }
    cur->parent = tmp->parent;
    int cnt = (tmp->right == cur);
    if(cnt == 0){
        tmp->left = cur->right;
        if(cur->right){
            cur->right->parent = tmp;
        }
        cur->right = tmp;
        tmp->parent = cur;
    }
    else{
        tmp->right = cur->left;
        if(cur->left){
            cur->left->parent = tmp;
        }
        cur->left = tmp;
        tmp->parent = cur;
    }
    cur->minDepth = tmp->minDepth;
    tmp->minDepth = tmp->depth;
    if(tmp->left){ 
        tmp->minDepth = min(tmp->minDepth, tmp->left->minDepth);
    }
    if(tmp->right){
        tmp->minDepth = min(tmp->minDepth, tmp->right->minDepth);
    }
}

void multiSplayTree::splay(Node *cur, Node *top = 0){
    while(!(cur->isRoot || cur->parent == top)){
        Node *tmp = cur->parent;
        if(!(tmp->isRoot || tmp->parent == top)){
            Node *tmp2 = tmp->parent;
            if((tmp2->left == tmp && tmp->left == cur) || (tmp2->right == tmp && tmp->right == cur)){
                rotate(tmp);
            }
            else{
                rotate(cur);
            }
        }
        rotate(cur);
    }
}

multiSplayTree::Node *multiSplayTree::refParent(multiSplayTree::Node *cur, int cnt){
    Node *tmp;
    if(cnt == 0){
        tmp = cur->left;
    }
    else{
        tmp = cur->right;
    }
    while(1){
        if(cnt == 0){
            if(tmp->right && tmp->right->minDepth < cur->depth){
                tmp = cur->right;
            }
            else if(tmp->left && tmp->left->minDepth < cur->depth){
                tmp = tmp->left;
            }
            else{
                break;
            }
        }
        else{
            if(tmp->left && tmp->left->minDepth < cur->depth){
                tmp = cur->left;
            }
            else if(tmp->right && tmp->right->minDepth < cur->depth){
                tmp = tmp->right;
            }
            else{
                break;
            }
        }
    }
    return tmp;
}

void multiSplayTree::switchPath(Node *cur){
    if(cur->left){
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
    if(cur->right){
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
    Node *tmp = cur;
    while(tmp->parent){
        Node *tmp2 = tmp->parent;
        if(tmp->isRoot){
            splay(tmp2);
            switchPath(tmp2);
        }
        tmp = tmp2;
    }
    splay(cur);
}

bool multiSplayTree::contains(size_t key){
    Node *curr = root;
    Node *prev = root;
    while(curr && curr->key != key){
        prev = curr;
        if(key < curr->key){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    if(!curr){
        expose(prev);
        return false;
    }
    else{
        expose(curr);
        return true;
    }
}

//int main();