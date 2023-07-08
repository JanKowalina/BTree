//I based on Introduction to Algorithms by T. H. Cormen et al. and lecture slides during implementation of this program. 

#include <iostream>
#include <stdlib.h>
#include <cstring>

// BNode class represents a node in the B-tree
class BNode {  
public: 
    bool isLeaf;      // Indicates whether the node is a leaf node or not
    int cntKey;       // Number of keys currently stored in the node
    int* keys;        // Array to store the keys
    
    BNode** sons;     // Array of pointers to child nodes
    
    BNode(int degree);   // Constructor
    ~BNode();            // Destructor

    friend class BTree;  // BTree class can access private members of BNode
};

// Destructor for BNode
BNode::~BNode() {
    delete[] keys;
    delete[] sons;
}

// Constructor for BNode
BNode::BNode(int degree) {
    keys = new int[2 * degree - 1];   // Maximum number of keys that can be stored in a node is 2 * degree - 1
    sons = new BNode * [2 * degree];  // Maximum number of child nodes that can be stored in a node is 2 * degree
    cntKey = 0;
    isLeaf = true;                    // By default, a node is considered as a leaf node
}

// BTree class represents the B-tree
class BTree {
    BNode* root;  // Pointer to the root node of the B-tree
    int deg;      // Degree of the B-tree

    void InsertNonfull(BNode* traverseRoot, int& k);   // Helper function for inserting a key into a non-full node
    BNode* SplitNode(BNode* subRoot, int i, int& k);   // Helper function for splitting a full node into two nodes
    void AddKey(BNode* leaf, int& k);                  // Helper function for adding a key to a leaf node
    bool NodeContainsKey(BNode* node, int& k);         // Helper function to check if a node contains a key
    void RemoveKeyFromLeaf(BNode* leaf, int& k);       // Helper function to remove a key from a leaf node
    int FindMin(BNode* root);                          // Helper function to find the minimum key in the tree
    int FindMax(BNode* root);                          // Helper function to find the maximum key in the tree
    int FindKeyInNode(BNode* node, int& k);             // Helper function to find the index of a key in a node
    void MergeNodes(BNode* root, int middle);           // Helper function to merge two child nodes of a node
    void LeftShift(BNode* root, int keyIndex);          // Helper function to perform a left shift operation
    void RightShift(BNode* root, int keyIndex);         // Helper function to perform a right shift operation

public:
    void Insert(int& k);                   // Function to insert a key into the B-tree
    void Print(BNode* root);                // Function to print the keys of the B-tree
    BNode* Find(BNode* node, int& k);       // Function to search for a key in the B-tree
    void LoadTree(BNode* root);             // Function to load a B-tree from input
    void SaveTree(BNode* root);             // Function to save the B-tree to output
    void RemoveKey(BNode* root, int& k);    // Function to remove a key from the B-tree
    void Deallocate(BNode* root);           // Function to deallocate memory used by the B-tree

    int GetChildIndex(BNode* traverseRoot, int& k);  // Function to find the index of a child node
    BNode* GetRoot();                                 // Function to get the root node
    int GetDegree();                                  // Function to get the degree of the B-tree

    ~BTree();    // Destructor
    BTree(int degree) {   // Constructor
        deg = degree;
        root = new BNode(deg);
    }
};

// Destructor for BTree
BTree::~BTree() {}

BNode* BTree::GetRoot() {
    return root;
}

int BTree::GetDegree() {
    return deg;
}



void BTree::RemoveKeyFromLeaf(BNode* leaf, int& k) {
    int j = 0;
    while (leaf->keys[j] != k)
        j++;

    for (j; j < leaf->cntKey - 1; j++)
        leaf->keys[j] = leaf->keys[j + 1];

    (leaf->cntKey)--;
}




void BTree::LeftShift(BNode* root, int keyIndex) {
    BNode* r = root->sons[keyIndex + 1];
    BNode* l = root->sons[keyIndex];

    for (int j = r->cntKey - 1; j >= 0; j--)
        r->keys[j + 1] = r->keys[j];

    for (int j = r->cntKey; j >= 0; j--)
        r->sons[j + 1] = r->sons[j];

    (r->cntKey)++;
    r->keys[0] = root->keys[keyIndex];
    root->keys[keyIndex] = l->keys[l->cntKey - 1];
    r->sons[0] = l->sons[l->cntKey];
    (l->cntKey)--;
}



void BTree::RightShift(BNode* root, int keyIndex) {
    BNode* r = root->sons[keyIndex + 1];
    BNode* l = root->sons[keyIndex];

    l->keys[l->cntKey] = root->keys[keyIndex];
    l->sons[l->cntKey + 1] = r->sons[0];
    (l->cntKey)++;
    root->keys[keyIndex] = r->keys[0];

    for (int j = 0; j < r->cntKey - 1; j++)
        r->keys[j] = r->keys[j + 1];

    for (int j = 0; j < r->cntKey; j++)
        r->sons[j] = r->sons[j + 1];

    (r->cntKey)--;
}



int BTree::GetChildIndex(BNode* traverseRoot, int& k) {
    int j = 0;
    while (j != traverseRoot->cntKey && k > traverseRoot->keys[j])
        j++;

    return j;
}



int BTree::FindMin(BNode* root) {
    BNode* traverse = root;

    while (!(traverse->isLeaf))
        traverse = traverse->sons[0];

    return traverse->keys[0];
}



int BTree::FindMax(BNode* root) {
    BNode* traverse = root;

    while (!(traverse->isLeaf))
        traverse = traverse->sons[traverse->cntKey];

    return traverse->keys[traverse->cntKey - 1];
}



void BTree::MergeNodes(BNode* root, int middle) {
    int j;
    BNode* l = root->sons[middle];
    BNode* r = root->sons[middle + 1];

    l->keys[deg - 1] = root->keys[middle];

    for (j = 0; j < deg - 1; j++)
        l->keys[deg + j] = r->keys[j];

    for (j = 0; j < deg; j++)
        l->sons[j + deg] = r->sons[j];

    l->cntKey = 2 * deg - 1;


    for (j = middle; j < root->cntKey - 1; j++) {
        root->keys[j] = root->keys[j + 1];
        root->sons[j + 1] = root->sons[j + 2];
    }
    (root->cntKey)--;
}



int BTree::FindKeyInNode(BNode* node, int& k) {
    int j = 0;
    while (j < node->cntKey - 1 && k != node->keys[j])
        j++;

    return j;
}



bool BTree::NodeContainsKey(BNode* node, int& k) {
    for (int j = 0; j < node->cntKey; j++)
        if (node->keys[j] == k)
            return true;

    return false;
}



void BTree::RemoveKey(BNode* root, int& k) {
    bool contains = NodeContainsKey(root, k);
    if (root->isLeaf && contains)
        RemoveKeyFromLeaf(root, k);
    else if (!(root->isLeaf) && contains) {
        int j = FindKeyInNode(root, k);
        BNode* l = root->sons[j];
        BNode* r = root->sons[j + 1];

        if (l->cntKey == deg - 1 && r->cntKey == deg - 1) {
            MergeNodes(root, j);
            RemoveKey(l, k);
        }
        else {
            int k1;
            if (l->cntKey > deg - 1) {
                k1 = FindMax(l);
                RemoveKey(l, k1);
            }
            else {
                k1 = FindMin(r);
                RemoveKey(r, k1);
            }
            root->keys[j] = k1;
        }

    }
    else if (!(root->isLeaf) && !contains) {
        int j = GetChildIndex(root, k);     
        BNode* child = root->sons[j];

        if (child->cntKey == deg - 1) {
            BNode* lBrother;
            BNode* rBrother;

            if (j - 1 >= 0)
                lBrother = root->sons[j - 1];
            else
                lBrother = nullptr;

            if (j + 1 <= root->cntKey)
                rBrother = root->sons[j + 1];
            else
                rBrother = nullptr;

            int lCntKey = 0, rCntKey = 0;
            if (lBrother != nullptr)
                lCntKey = lBrother->cntKey;
            if (rBrother != nullptr)
                rCntKey = rBrother->cntKey;

            if (lCntKey > deg - 1)
                LeftShift(root, j - 1);
            else if (rCntKey > deg - 1)
                RightShift(root, j);
            else {
                if (lCntKey != 0) {
                    MergeNodes(root, j - 1);
                    child = lBrother; 
                }
                else
                    MergeNodes(root, j);
            }
        }
        RemoveKey(child, k);
    }
}



void BTree::InsertNonfull(BNode* traverseRoot, int& k) {
    int i = GetChildIndex(traverseRoot, k);
    BNode* child = traverseRoot->sons[i];

    if (child->cntKey == 2 * deg - 1)
        child = SplitNode(traverseRoot, i, k);

    if (child->isLeaf)
        AddKey(child, k);
    else
        InsertNonfull(child, k);
}



void BTree::AddKey(BNode* leaf, int& k) {
    int j = leaf->cntKey - 1;

    while (k < leaf->keys[j] && j >= 0) {
        leaf->keys[j + 1] = leaf->keys[j];
        j--;
    }

    leaf->keys[j + 1] = k;
    (leaf->cntKey)++;
}



BNode* BTree::SplitNode(BNode* subRoot, int i, int& k) {
    BNode* r = new BNode(deg);
    BNode* l = subRoot->sons[i];
    r->isLeaf = l->isLeaf;
    r->cntKey = deg - 1;

    for (int j = 0; j < deg - 1; j++)
        r->keys[j] = l->keys[j + deg];

    if (!l->isLeaf)
        for (int j = 0; j < deg; j++)
            r->sons[j] = l->sons[j + deg];

    l->cntKey = deg - 1;


    for (int j = subRoot->cntKey; j >= i + 1; j--)
        subRoot->sons[j + 1] = subRoot->sons[j];

    subRoot->sons[i + 1] = r;

    for (int j = subRoot->cntKey - 1; j >= i; j--)
        subRoot->keys[j + 1] = subRoot->keys[j];

    subRoot->keys[i] = l->keys[deg - 1];
    (subRoot->cntKey)++;


    if (subRoot->keys[i] > k)
        return subRoot->sons[i];
    else
        return subRoot->sons[i + 1];
}



void BTree::Insert(int& k) {
    
    if (root->cntKey == 2 * deg - 1) {
        BNode* newRoot = new BNode(deg);
        newRoot->sons[0] = root;
        newRoot->isLeaf = false;
        root = newRoot;
        BNode* waste = SplitNode(root, 0, k);
    }

    if (root->isLeaf) {
        if (root->cntKey == 0) {
            root->keys[0] = k;
            (root->cntKey)++;
        }
        else
            AddKey(root, k);
    }
    else
        InsertNonfull(root, k);
}



BNode* BTree::Find(BNode* node, int& k){
    if (NodeContainsKey(node, k))
        return node;
    else if (node->isLeaf)
        return nullptr;
    else {
        int i = GetChildIndex(node, k);
        BNode* child = node->sons[i];
        BNode* ret = Find(child, k);
        return ret;
    }
}



void BTree::LoadTree(BNode* root) {
    int j = 0, sonsCounter = 0;
    char c[10];

    std::cin >> c;
    while (strcmp(c, ")") != 0) {
        if (strcmp(c, "(") == 0) {
            sonsCounter++;
            BNode* newSon = new BNode(deg);
            root->sons[j] = newSon;
            LoadTree(root->sons[j]);
            std::cin >> c;
        }
        else {
            root->keys[j] = atoi(c);
            j++;
            std::cin >> c;
        }
    }

    if (sonsCounter == 0)
        root->isLeaf = true;
    else
        root->isLeaf = false;

    root->cntKey = j;
}



void BTree::SaveTree(BNode* root) {
    int j = 0;

    std::cout << "( ";
    while (j != root->cntKey) {
        if (!(root->isLeaf)) {
            SaveTree(root->sons[j]);
            std::cout << root->keys[j] << " ";
            j++;
        }
        else {
            std::cout << root->keys[j] << " ";
            j++;
        }
    }

    if (!(root->isLeaf))
        SaveTree(root->sons[j]);

    std::cout << ") ";
}



void BTree::Print(BNode* root) {
    int j;
    for (j = 0; j < root->cntKey; j++) {
        if (!(root->isLeaf))
            Print(root->sons[j]);
        std::cout << root->keys[j] << " ";
    }

    if (!(root->isLeaf))
        Print(root->sons[j]);
}



void BTree::Deallocate(BNode* root) {
    int j;
    for (j = 0; j < root->cntKey; j++) {
        if (!(root->isLeaf))
            Deallocate(root->sons[j]);
        else
            break;
    }

    if (!(root->isLeaf))
        Deallocate(root->sons[j]);
    delete root;
}



using namespace std;

int main() {
    char c;
    int x;
    BTree* T = nullptr;

    cin >> c;
    while (c != 'X') {
        switch (c) {
        case 'I':
            cin >> x;
            if (T != nullptr) {
                T->Deallocate(T->GetRoot());
                delete T;
            }
            T = new BTree(x);
            break;
        case 'A':
            cin >> x;
            T->Insert(x);
            break;
        case '?':
            cin >> x;
            cout << x;
            if (T->Find(T->GetRoot(), x) == nullptr)
                cout << " -\n";
            else 
                cout << " +\n";
            break;
        case 'P':
            T->Print(T->GetRoot());
            break;
        case 'L':
            cin >> x;
            if (T != nullptr) {
                T->Deallocate(T->GetRoot());
                delete T;
            }
            T = new BTree(x);
            cin >> c;
            T->LoadTree(T->GetRoot());
            break;
        case 'S':
            cout << T->GetDegree() << "\n";
            T->SaveTree(T->GetRoot());
            break;
        case 'R':
            cin >> x;
            T->RemoveKey(T->GetRoot(), x);
            break;
        case 'X':
            break;
        }
        cin >> c;
    }

    T->Deallocate(T->GetRoot());
    delete T;

    return 0;
}