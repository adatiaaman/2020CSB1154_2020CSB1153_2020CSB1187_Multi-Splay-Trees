#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Data structure for calculating the running time
class timer
{
public:
    // Function to store the start time
    void start(void)
    {
        flag_one = high_resolution_clock::now();
        return;
    }

    // Functioin to store the ending time
    void stop(void)
    {
        flag_two = high_resolution_clock::now();
        return;
    }

    // Function to display the total time taken
    void timeElapsed(void)
    {
        auto duration = duration_cast<nanoseconds>(flag_two - flag_one);
        cout << setprecision(2) << fixed << duration.count() / 1000;
    }

private:
    // Variables to store the starting and ending time
    std::chrono::_V2::system_clock::time_point flag_one, flag_two;
};

// Data structure for the Multi-Splay tree
class multiSplayTree
{
public:
    // Constructor to avoid any implicit type casting
    multiSplayTree(const int size);

    // Destructor to free the memory
    ~multiSplayTree(void);

    // Function to search an element in the tree
    bool query(size_t key);

    // Function to display the whole tree
    void display(void)
    {
        explore(root);
        return;
    }

private:
    // Stucture for the tree node
    struct Node
    {
        // Variables to store the required information
        bool isRoot;
        size_t key, depth, minDepth;
        Node *left, *right, *parent;

        // Initialising the node variables
        Node(size_t key, size_t depth = 0, size_t minDepth = 0, bool isRoot = 0)
        {
            this->key = key;
            left = right = parent = 0;
            this->isRoot = isRoot;
            this->depth = depth;
            this->minDepth = minDepth;
        }
    };

    Node *root;                                                        // Root of the whole multisplay tree
    Node *treeFor(size_t low, size_t high, size_t depth, bool isRoot); // Function to create the whole tree
    void rotate(Node *cur);                                            // Function to perform the rotation operation
    void splay(Node *cur, Node *top);                                  // Function to perform the standard splay operation
    void switchPath(Node *cur);                                        // Function to adjust the depth and min-depth values of the nodes
    void expose(Node *cur);                                            // Function to bring the current node to the root of the whole tree
    void explore(Node *cur);                                           // Auxiliary function to help display the whole tree
    Node *refParent(Node *cur, int num);                               // fFnction to return the first child whose min-depth value is greater than the depth value
};

multiSplayTree::Node *multiSplayTree::treeFor(size_t low, size_t high, size_t depth, bool isRoot)
{
    // Terminating condition
    if (low == high)
        return nullptr;
    size_t mid = low + (high - low) / 2;

    // Allocating memory for the new node and initialising its values
    Node *tmp = new Node{mid, depth, depth, isRoot};

    // Calling recursion for the left subtree
    tmp->left = treeFor(low, mid, depth + 1, false);
    // Calling recursion for the right subtree
    tmp->right = treeFor(mid + 1, high, depth + 1, true);

    if (tmp->left)
    {
        tmp->left->parent = tmp;
    }
    if (tmp->right)
    {
        tmp->right->parent = tmp;
    }
    return tmp;
}

multiSplayTree::multiSplayTree(const int size)
{
    root = treeFor(0, size, 0, true);
}

multiSplayTree::~multiSplayTree(void)
{
    // Iterating while root is not equal to nullptr
    while (root != nullptr)
    {
        // Checking whether the root has a left child or not
        if (root->left == nullptr)
        {
            Node *next = root->right;
            delete root;
            root = next;
        }
        else
        {
            Node *child = root->left;
            root->left = child->right;
            child->right = root;
            root = child;
        }
    }
}

void multiSplayTree::rotate(Node *cur)
{
    // Pointer to store the parent of the current node
    Node *tmp = cur->parent;

    // If the parent is a root then the current node will become the root after the rotation
    if (tmp->isRoot)
    {
        tmp->isRoot = false;
        cur->isRoot = true;
    }
    if (root == tmp)
    {
        root = cur;
    }

    // If there is a grandparent then perform the rotation
    if (tmp->parent)
    {
        int cnt = (tmp->parent->right == tmp);
        // Checking whether tmp is the left or thr right child of its parent
        if (cnt == 0)
        {
            tmp->parent->left = cur;
        }
        else
        {
            tmp->parent->right = cur;
        }
    }

    // Updating the current node's parent
    cur->parent = tmp->parent;
    int cnt = (tmp->right == cur);

    // Checking whether the current node is the left or the right child of its parent
    if (cnt == 0)
    {
        // Performing the rotation
        tmp->left = cur->right;
        if (cur->right)
        {
            cur->right->parent = tmp;
        }
        cur->right = tmp;
        tmp->parent = cur;
    }
    else
    {
        // Performing the rotation
        tmp->right = cur->left;
        if (cur->left)
        {
            cur->left->parent = tmp;
        }
        cur->left = tmp;
        tmp->parent = cur;
    }

    // Updating the mindepths of the current node and tmp
    cur->minDepth = tmp->minDepth;
    tmp->minDepth = tmp->depth;
    if (tmp->left)
    {
        tmp->minDepth = min(tmp->minDepth, tmp->left->minDepth);
    }
    if (tmp->right)
    {
        tmp->minDepth = min(tmp->minDepth, tmp->right->minDepth);
    }
}

void multiSplayTree::splay(Node *cur, Node *top = 0)
{
    // Rotate until current node is either a root or current node's parent is equal to top
    while (!(cur->isRoot || cur->parent == top))
    {
        Node *tmp = cur->parent;
        // If tmp is neither a root not its parent is equal to top
        if (!(tmp->isRoot || tmp->parent == top))
        {
            Node *tmp2 = tmp->parent;
            if ((tmp2->left == tmp && tmp->left == cur) || (tmp2->right == tmp && tmp->right == cur))
            {
                rotate(tmp);
            }
            else
            {
                rotate(cur);
            }
        }

        // If tmp is either a root or its parent is equal to top
        rotate(cur);
    }
}

multiSplayTree::Node *multiSplayTree::refParent(multiSplayTree::Node *cur, int cnt)
{
    // Pointer to store the child
    Node *tmp = NULL;
    if (cnt == 0)
    {
        tmp = cur->left;
    }
    else
    {
        tmp = cur->right;
    }

    // If cnt is equal to 0, then tmp is the left child of the current node and vice-versa
    while (1)
    {
        if (cnt == 0)
        {
            // Checking whether the min-depth of right child is less than the depth of current node or not
            if (tmp->right && tmp->right->minDepth < cur->depth)
            {
                tmp = tmp->right;
            }
            else if (tmp->left && tmp->left->minDepth < cur->depth)
            {
                tmp = tmp->left;
            }
            else
            {
                break;
            }
        }
        else
        {
            // Checking whether the min-depth of left child is less than the depth of the current node
            if (tmp->left && tmp->left->minDepth < cur->depth)
            {
                tmp = tmp->left;
            }
            else if (tmp->right && tmp->right->minDepth < cur->depth)
            {
                tmp = tmp->right;
            }
            else
            {
                break;
            }
        }
    }
    return tmp;
}

void multiSplayTree::switchPath(Node *cur)
{
    // If current node has a left child
    if (cur->left)
    {
        // If the mindepth of the left child is greater than the depth of the current node
        if (cur->left->minDepth > cur->depth)
        {
            cur->left->isRoot = !(cur->left->isRoot);
        }
        else
        {
            Node *tmp = refParent(cur, 0);
            splay(tmp, cur);
            if (tmp->right)
            {
                tmp->right->isRoot = !(tmp->right->isRoot);
            }
        }
    }

    // If the current node has a right child
    if (cur->right)
    {
        // If the mindepth of the right child is greater than the depth of the current node
        if (cur->right->minDepth > cur->depth)
        {
            cur->right->isRoot = !(cur->right->isRoot);
        }
        else
        {
            Node *tmp2 = refParent(cur, 1);
            splay(tmp2, cur);
            if (tmp2->left)
            {
                tmp2->left->isRoot = !(tmp2->left->isRoot);
            }
        }
    }
}

void multiSplayTree::expose(Node *cur)
{
    // Pointer to store the current node
    Node *tmp = cur;

    // Iterating while tmp has a parent
    while (tmp->parent)
    {
        Node *tmp2 = tmp->parent;
        // If tmp is a root
        if (tmp->isRoot)
        {
            splay(tmp2);
            switchPath(tmp2);
        }
        tmp = tmp2;
    }
    splay(cur);
}

bool multiSplayTree::query(size_t key)
{
    // Pointers to store the current and previous nodes
    Node *curr = root;
    Node *prev = root;

    // Iterating until we either find the key or a nullptr
    while (curr && curr->key != key)
    {
        prev = curr;
        if (key < curr->key)
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
    }

    // If we found a nullptr
    if (!curr)
    {
        expose(prev);
        return false;
    }

    // If we found the key
    expose(curr);
    return true;
}

// Function to run different test cases for the tree
void tester(string &type, int size)
{
    // Declaring a variable of the class timer
    timer clk; 
    clk.start();

    // Constructing a Multi-Splay tree of the specified type
    multiSplayTree tre(size); 
    clk.stop();

    cout << "Access type: " << type << "\t\tSize: " << size << endl << "\t\tBuild Time: ";
    clk.timeElapsed();
    cout << "ns" << endl << "\t\t";

    // Checking the access type
    if (type == "Sequential")
    {
        clk.start();
        for (int i = 0; i < size; i++)
        {
            tre.query(i);
        }
        clk.stop();
        cout << "Query Time: ";
        clk.timeElapsed();
        cout << "ns" << endl;
    }
    else if (type == "Reverse")
    {
        clk.start();
        for (int i = size - 1; i >= 0; i--)
        {
            tre.query(i);
        }
        clk.stop();
        cout << "Query Time: ";
        clk.timeElapsed();
        cout << "ns" << endl;
    }
    else
    {
        vector<int> sequence;
        for (int i = 0; i < size; i++)
        {
            sequence.push_back(i);
        }

        // To generate a random shuffle of the given array
        random_shuffle(sequence.begin(), sequence.end()); 
        clk.start();
        for (int i = 0; i < size; i++)
        {
            tre.query(sequence[i]);
        }
        clk.stop();
        cout << "Query Time: ";
        clk.timeElapsed();
        cout << "ns" << endl;
    }
}

void multiSplayTree::explore(multiSplayTree::Node *cur)
{
    // If current node is a null pointer
    if (!cur)
    {
        return;
    }

    // Displaying the parent of the current node if any
    cout << "Parent of " << cur->key << " = ";

    // Checking whether the current node has a parent or not
    if (cur->parent)
    {
        cout << cur->parent->key;
    }
    else
    {
        cout << "NULL";
    }

    // Checking if the node is a root
    if (cur->isRoot)
    {
        cout << ", " << cur->key << " is a Root";
    }
    cout << endl;

    // Calling recursion for the left subtree
    explore(cur->left);

    // Calling recursion for the right subtree
    explore(cur->right);
}

int main()
{
    cout << "Multi-Splay Trees:" << endl;
    // constructing the string to define the access type and calling the tester function for different test cases
    // Sequential access
    string access = "Sequential";
    tester(access, 30);
    tester(access, 3000);
    tester(access, 300000);
    cout << endl;

    // Reverse access
    access = "Reverse";
    tester(access, 30);
    tester(access, 3000);
    tester(access, 300000);
    cout << endl;

    // Random access
    access = "Random ";
    tester(access, 30);
    tester(access, 3000);
    tester(access, 300000);
    cout << endl;
    return 0;
}