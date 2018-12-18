// C program to delete a node from AVL Tree
// Built on code from Geeks for Geeks
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

// An AVL tree node
struct Node
{
    int key;
    int z;
    int x;
    int idx;
    struct Node *left;
    struct Node *right;
    int height;
};

struct Combo
{
    struct Node *node;
    bool b;
};

int max(int a, int b);

// A utility function to get height of the tree
int height(struct Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}
/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct Node* newNode(int key, int z, int idx, int x)
{
    struct Node* node = (struct Node*)
                        malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // new node is initially added at leaf
    node->z = z;
    node->x = x;
    node->idx = idx;
    return(node);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Node *rightRotate(struct Node *y)
{
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Node *leftRotate(struct Node *x)
{
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int getBalance(struct Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

struct Node* insert(struct Node* node, int key, int z, int idx, int x)
{
    /* 1.  Perform the normal BST rotation */
    if (node == NULL)
        return(newNode(key, z, idx, x));

    if (key < node->key)
        node->left  = insert(node->left, key, z, idx, x);
    else if (key > node->key)
        node->right = insert(node->right, key, z, idx, x);
    else // Equal keys not allowed
        return node;

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                           height(node->right));

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
struct Node * minValueNode(struct Node* node)
{
    struct Node* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
struct Node* deleteNode(struct Node* root, int key, int z)
{
    // STEP 1: PERFORM STANDARD BST DELETE

    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key < root->key )
        root->left = deleteNode(root->left, key, z);

    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key > root->key )
        root->right = deleteNode(root->right, key, z);

    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left :
                                             root->right;
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of
                            // the non-empty child
            free(temp);

            // if (root->left == NULL & root->right != NULL)
            //   root = root->right;
            // else if(root->left != NULL & root->right == NULL)
            //   root = root->left;
            // else
            //   root = NULL;

        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            struct Node* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->key = temp->key;
            root->z = temp->z;
            root->idx = temp->idx;
            root->x = temp->x;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key, temp->z);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
      return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

struct Node* staircaseDeleteFunction(struct Node* node, int y, int z, int x)
{
  if (node == NULL)
  {
    return NULL;
  }
  if (node->left != NULL)
  {
    node->left = staircaseDeleteFunction(node->left, y, z, x);
  }
  if (node->right != NULL)
  {
    node->right = staircaseDeleteFunction(node->right, y, z, x);
  }

  // update height
  node->height = 1 + max(height(node->left),
                         height(node->right));

  int balance = getBalance(node);

  // Balance the node out
  // Left Left Case
  if (balance > 1 && getBalance(node->left) >= 0)
      node = rightRotate(node);

  // Left Right Case
  if (balance > 1 && getBalance(node->left) < 0)
  {
      node->left = leftRotate(node->left);
      node = rightRotate(node);
  }

  // Right Right Case
  if (balance < -1 && getBalance(node->right) <= 0)
      node = leftRotate(node);

  // Right Left Case
  if (balance < -1 && getBalance(node->right) > 0)
  {
      node->right = rightRotate(node->right);
      node = leftRotate(node);
  }

  if ((node->key <= y) & (node->z <= z))
  {
    if (node->x <= x)
    {
      if (!((node->x == x) & (node->key == y) & (node->z == z)))
      {
        std::cout << "deleted node : y -  " << node->key << ", z - " << node->z << '\n';
        std::cout << "deleting node : y -  " << y << ", z - " << z << '\n';
        std::cout << "index of deleted point " << node->idx << '\n';
        node = deleteNode(node, node->key, node->z);
        return node;
      }
      else
        return node;
    }
    else
    {
      std::cout << "node->x " << node->x << ", " << node->key << ", " << node->z << '\n';
      std::cout << "x " << x << ", " << y << ", " << z << '\n';
      return node;
    }
  }
  else
    return node;
}

int count(struct Node *node)
{
  if (node == NULL)
    return 0;
  else
    return count(node->left)+count(node->right)+1;
}

// void preOrder(struct Node *node, std::vector<int> v)
// {
//     if(node != NULL)
//     {
//         v.push_back(node->idx);
//         preOrder(node->left, v);
//         preOrder(node->right, v);
//     }
// }

struct Combo* searchIfMaximal(struct Node* root, struct Node* node, int y, int z, int idx, int x)
{
  if (node == NULL)
  {
    root = insert(root, y, z, idx, x);
    std::cout << "root maximal point " << y << ", " << z << '\n';
    struct Combo* comb = (struct Combo*) malloc(sizeof(struct Combo));
    comb->node = root;
    comb->b = true;
    return comb;
  }
  // Point lies in 3rd quadrant. Is NOT maximal
  if ((y <= node->key) & (z <= node->z))
  {
    // std::cout << "in 3rd quadrant (NOT MAXIMAL) for point " << y << ", " << z << '\n';
    struct Combo* comb = (struct Combo*) malloc(sizeof(struct Combo));
    comb->node = root;
    comb->b = false;
    return comb;
  }
  // Point lies in 1st quadrant. Is definitely maximal
  else if ((y > node->key) & (z > node->z))
  {
      std::cout << "1st quadrant maximal for point " << y << ", " << z << '\n';
      root = staircaseDeleteFunction(root, y, z, x);
      root = insert(root, y, z, idx, x);
      std::cout << "count(root) - " << count(root) << '\n';
      std::cout << "count(node) - " << count(node) << '\n';
      std::cout << "count(root) - " << count(root) << '\n';
      std::cout << "count(node) - " << count(node) << '\n';
      struct Combo* comb = (struct Combo*) malloc(sizeof(struct Combo));
      comb->node = root;
      comb->b = true;
      return comb;
  }
  // Point lies in 2nd quadrant. CAN be maximal. Search in left half of tree
  else if ((y <= node->key) & (z > node->z))
  {
    if (node->left != NULL)
      return searchIfMaximal(root, node->left, y, z, idx, x);
    else
    {
      std::cout << "2nd quadrant maximal for point " << y << ", " << z << '\n';
      root = staircaseDeleteFunction(root, y, z, x);
      root = insert(root, y, z, idx, x);
      std::cout << "count(root) - " << count(root) << '\n';
      std::cout << "count(node) - " << count(node) << '\n';
      std::cout << "count(root) - " << count(root) << '\n';
      std::cout << "count(node) - " << count(node) << '\n';
      struct Combo* comb = (struct Combo*) malloc(sizeof(struct Combo));
      comb->node = root;
      comb->b = true;
      return comb;
    }
  }
  // Point lies in 4th quadrant. CAN be maximal. Search in right half of tree
  else
  {
    if (node->right != NULL)
      return searchIfMaximal(root, node->right, y, z, idx, x);
    else
    {
      std::cout << "4th quadrant maximal for point " << y << ", " << z << '\n';
      root = staircaseDeleteFunction(root, y, z, x);
      root = insert(root, y, z, idx, x);
      std::cout << "count(root) - " << count(root) << '\n';
      std::cout << "count(node) - " << count(node) << '\n';
      std::cout << "count(root) - " << count(root) << '\n';
      std::cout << "count(node) - " << count(node) << '\n';
      struct Combo* comb = (struct Combo*) malloc(sizeof(struct Combo));
      comb->node = root;
      comb->b = true;
      return comb;
    }
  }
}
