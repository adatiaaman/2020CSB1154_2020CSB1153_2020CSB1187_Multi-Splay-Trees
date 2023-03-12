
# Multi-Splay Tree

A Splay Tree is a type of Binary Search Tree that uses an algorithm called splaying to move a node to the root upon access, while still maintaining the BST property. Splay Trees have useful properties such as Sequential Access Property, Dynamic Finger Property, Working Set Property, and Dynamic Optimality, which help reduce the cost of accessing elements in the BST. Multi-Splay trees are a type of data structure that are composed of multiple splay trees. A Multi-Splay Tree has an O(log log n) competitive bound and O(log n) amortized bound for accessing elements and are conjectured to be Dynamically Optimal.


## Authors

- [@OptimalKnight](https://www.github.com/OptimalKnight)
- [@adatiaaman](https://www.github.com/adatiaaman)
- [@Amank2854](https://www.github.com/Amank2854)


## Requirements

Your machine should have a GCC compiler installed.
## Run Locally

Clone the project

```bash
  git clone https://github.com/OptimalKnight/MultiSplayTree.git
```

Go to the project directory

```bash
  cd MultiSplayTree
```

Enter the following command to compile the source file using g++

```bash
  g++ -o <name-you-want-to-give> source.cpp
```
In place of <name-you-want-to-give> replace it by any name like myprogram, etc.

Now you can run the program using
```bash
  ./myprogram
```


## Working

To access any node in the Multi-Splay Tree, the query() function is called. This function splays the element until it reaches the root of the entire BST. The first step is to move the node to the root of the splay tree in which it belongs by performing necessary rotations. This process is done by the splay() function.

Next, the expose() function is used to adjust the depths and minDepths of the node by using refParent() and switchPath(). This further splaying procedure continues until the parent of the accessed node is NULL, which indicates that the accessed node has reached the root of the BST.

Overall, the query() function combines the use of expose(), splay(), rotate(), refParent(), and switchPath() functions to perform the necessary splaying operations on the Multi-Splay Tree. This process helps to efficiently access the desired node in the tree.
## Usage/Example

The program doesn't take any inputs from the user. The input values can be toggled from the main() body itself.

Initially, the program is tested against three types of data access: sequential, reverse, and random. The tests were conducted on a Multi-Splay Tree with varying sizes, including 30, 3000, and 300000 nodes.
## Screenshots

A sample execution result of the program.

![Program Screenshot](/Images/MultiSplayTree_Sample%20Execution%20Result.jpeg)