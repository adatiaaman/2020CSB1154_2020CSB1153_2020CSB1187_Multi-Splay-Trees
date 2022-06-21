# Multi-Splay Tree
## Introduction
A Splay Tree is a Binary Search Tree which performs an extra operation called splaying. Splaying is an algorithm
which along moves a node to the root upon access, such that the BST property is still maintained. Splay Tree
have various useful properties which help us reduce the amortized cost of accessing elements in the BST. Some
of these properties are Sequential Access Property, Dynamic Finger Property, Working Set Property, Dynamic
Optimality, etc. A Multi-Splay Tree is a tree made up of Splay trees, wherein the nodes have several different
types of properties. The root of each individual splay trees is darkened.

Multi-Splay tree has O(log log n) competitive bound and O(log n) amortized complexity for access in a BST,
which is proved by an access Lemma. In the Multi-splay tree data structure, we assume a reference tree, denoted
by P, which is a perfectly balanced tree consisting n nodes. Each node has a preferred child. A path consisting
of preferred child of various node is called preferred path. The preference tree is only for understanding purpose
and is not part of our data structure.

Functions used for various operations in our implementation:
**explore()** - auxiliary function to help display the whole tree. \
**treeFor()** - function to create the whole tree. \
**rotate()** - function to perform the standard rotation operation. \
**splay()** - function to perform the standard splaying operation. \
**switchPath()** - function to adjust the depths and minDepths values of the nodes. \
**refParent()** - function to return the first child whose minDepth value is greater than the depth value. \
**expose()** - function to bring the current node to the root of the whole tree. \
**query()** - function to access an element in the tree (multi-splay).

## Algorithms

We call the query() function to access any node in the Multi-Splay Tree. Its functionality is that it splays the
element until it reaches to the root of the entire BST. So, when splayed for the first time, the node is moved to
the root of the splay tree in which it belongs by performing necessary rotations. Following, using refParent()
and switchPath(), the further splaying procedure is taken place by adjusting the depths and minDepths of the
node. It ends when the parent of the accessed node is NULL, i.e. the accessed node reaches to the root of the
BST. query() uses expose() function, which in turn uses splay(), rotate(), refParent() and switchPath()
function.

## Input
The program doesn't require any user inputs.

## Test Cases
Sequential Access of elements in the BST having size 30, 3000 and 300000. \
Reverse Access of elements in the BST having size 30, 3000 and 300000. \
Random Access of elements in the BST having size 30, 3000 and 300000. 

## Output
Build Time and Query Time for different set of test cases.

## Conclusions
It is the only algorithm with O(log log n) competitiveness in a BST. In addition, the above theorems and results
show that Multi-Splay Trees satisfy many of the important properties of a Dynamically Optimal BST algorithm.
