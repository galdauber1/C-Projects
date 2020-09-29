//
// Created by גל דאובר on 20/05/2020.
//
#include "RBTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void fixTree(RBTree *tree, Node *node);

Node *getuncle(Node *n);

Node *getParent(Node *n);

Node *getGrandParent(Node *n);

void rotateLeft(Node *n, RBTree *tree);

void rotateRight(Node *node, RBTree *tree);

Node *successor(Node *node);

Node *findMinValue(Node *node);

Node *search(RBTree *tree, void *data);

int isLeftOrRight(Node *new);

void delM(Node *m, Node *p, Node *s, Node *c, RBTree *tree);

void delMCase3(Node *m, Node *p, Node *s, Node *c, RBTree *tree);

void inorder(Node *node, forEachFunc func, void *args);

void deltree(Node *node, RBTree *tree);

int delNoChilderns(RBTree *tree,  Node *c, Node *s, Node *p,  Node *m);

int deleteHasLeftChild(RBTree *tree, Node *s, Node *p, Node *newm,  Node *m);

int delteHasRightChild(RBTree *tree, Node *s, Node *p, Node *newm,  Node *m);

void delteHasTwoChildrens(RBTree *tree, Node *c, Node *s, Node *p, Node *newm, Node *m);

void delMOption2(Node *m, Node *p, Node *s, Node *c,  RBTree *tree);

void delMOption3( Node *m, Node *p, Node *s,  Node *c,  RBTree *tree);

void
delMOption4( Node *m,  Node *p, Node *s,  Node *c, RBTree *tree, Node *sclose);

void deleteMOption5(Node *p, Node *s,  RBTree *tree, Node *sfar, int checker);

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{

    RBTree *tree = (RBTree *) calloc(1, sizeof(RBTree));
    tree->compFunc = compFunc;
    tree->freeFunc = freeFunc;
    tree->root = NULL;
    return tree;
}
/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data)
{
    if (data == NULL || tree == NULL)
    {
        return 0;
    }
    Node *temp = tree->root;
    Node *parent = NULL;
    Node *newn = (Node *) calloc(1, sizeof(Node));
    if (newn == NULL)
    {
        free(newn);
        return 0;
    }
    newn->right = NULL;
    newn->data = data;
    newn->color = RED;
    while (temp != NULL)
    {
        parent = temp;
        if (tree->compFunc(temp->data, data) == 0)
        {
            free(newn);
            return 0;
        }
        else if (tree->compFunc(temp->data, data) < 0)
        {
            temp = temp->right;
        }

        else if (tree->compFunc(temp->data, data) > 0)
        {
            temp = temp->left;
        }
    }
    if (parent == NULL) // check if its possible to get tree with root null
    {
        tree->root = newn;
        tree->size += 1;
    }
    else if (tree->compFunc(data, parent->data) > 0)
    {
        parent->right = newn;
        newn->parent = parent;
        tree->size += 1;
    }
    else if (tree->compFunc(data, parent->data) < 0)
    {
        parent->left = newn;
        newn->parent = parent;
        tree->size += 1;
    }
    fixTree(tree, newn);
    return 1;
}
/**
 * @brief this function fix the tree after changes to maintain the red black properties
 * @param tree
 * @param node
 */
void fixTree(RBTree *tree, Node *node)
{
    if (node == NULL)
    {
        return;
    }
    if (tree->root == node) // case 1
    {
        tree->root->color = BLACK;
    }
    else if (getParent(node)->color == BLACK) // case 2
    {
        return;
    }
    else if ((getuncle(node) != NULL) && (getParent(node)->color == RED) &&
             (getuncle(node)->color ==
             RED)) //
        // case3
    {
        getParent(node)->color = BLACK;
        getuncle(node)->color = BLACK;
        getGrandParent(node)->color = RED;
        fixTree(tree, getGrandParent(node));
    }
    else if ((getuncle(node) == NULL || getuncle(node)->color == BLACK) &&
             (getParent(node)->color ==
             RED))
    {
        Node *parent = getParent(node);
        Node *grand = getGrandParent(node);
        //case 4 step 1 option 1
        if ((node == parent->left) && (parent == grand->right))
        {
            rotateRight(parent, tree);
            node = node->right;
        }
            //case 4 step 1 option 2
        else if ((node == parent->right) && (parent == grand->left))
        {
            rotateLeft(parent, tree);
            node = node->left;
        }
        parent = getParent(node);
        grand = getGrandParent(node);

        //case 4 step 2 option 1
        if (node == parent->right)
        {
            rotateLeft(grand, tree);
        }
            //case 4 step 2 option 2
        else
        {
            rotateRight(grand, tree);
        }
        //case 4 step 3
        grand->color = RED;
        parent->color = BLACK;
    }
}
/**
 * @brief return grandparent
 * @param n
 * @return
 */
Node *getGrandParent(Node *n)
{
    return getParent(getParent(n)); // parent of parent
}
/**
 * @brief return parent
 * @param n
 * @return
 */
Node *getParent(Node *n)
{
    return n == NULL ? NULL : n->parent;

}
/**
 * @brief return uncle
 * @param n
 * @return
 */
Node *getuncle(Node *n)
{
    Node *parent = getGrandParent(n);
    if (parent == NULL)
    {
        return NULL;
    }
    if (n->parent == parent->right) // check if n is left or right child
    {
        return parent->left;
    }
    else
    {
        return parent->right;
    }
}
/**
 * @brief preform left rotation.
 * @param n
 * @param tree
 */
void rotateLeft(Node *n, RBTree *tree)
{
    {
        Node *parent = getParent(n);
        Node *newnode = n->right;
        if (newnode == NULL)
        {
            return;
        }
        n->right = newnode->left;
        newnode->left = n;
        n->parent = newnode;
        if (n->right != NULL)
        {
            n->right->parent = n;
        }
        if (parent != NULL)
        {
            if (n == parent->left)
            {
                parent->left = newnode;
            }
            else if (n == parent->right)
            {
                parent->right = newnode;
            }
        }
        newnode->parent = parent;
        if (parent == NULL)
        {
            tree->root = newnode;
        }

    }
}
/**
 * @brief preform right rotate
 * @param node
 * @param tree
 */
void rotateRight(Node *node, RBTree *tree)
{

    Node *parent = getParent(node);
    Node *n = node->left;
    if (n == NULL)
    {
        return;
    }
    node->left = n->right;
    n->right = node;
    node->parent = n;
    if (node->left != NULL)
    {
        node->left->parent = node;
    }
    if (parent != NULL)
    {
        if (node == parent->left)
        {
            parent->left = n;
        }
        else if (node == parent->right)
        {
            parent->right = n;
        }
    }
    n->parent = parent;
    if (parent == NULL)
    {
        tree->root = n;
    }

}
/**
 * @brief return the node that contain data
 * @param tree
 * @param data
 * @return
 */
Node *search(RBTree *tree, void *data)
{
    Node *temp = tree->root;
    while (temp != NULL)
    {
        if (tree->compFunc(data, temp->data) > 0)
        {
            temp = temp->right;
        }
        else if (tree->compFunc(data, temp->data) < 0)
        {
            temp = temp->left;
        }
        else
        {
            break;
        }
    }
    return temp;
}
/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL)
    {
        return 0;
    }
    Node *c = NULL;
    Node *s = NULL;
    Node *p = NULL;
    Node *newm = NULL;
    Node *m = search(tree, data);
    if (m == NULL)
    {
        return 0;
    }
    else
    {
        if (m->left == NULL && m->right == NULL)
        {

            return delNoChilderns(tree, c, s, p, m);
        }
        else if ((m->left != NULL) && (m->right == NULL))
        {

            return deleteHasLeftChild(tree, s, p, newm, m);
        }

        else if ((m->left == NULL) && (m->right != NULL))
        {
            return delteHasRightChild(tree, s, p, newm, m);
        }
        else
        {

            delteHasTwoChildrens(tree, c, s, p, newm, m);

        }

    }
    return 1;
}
/**
 * @brief helper function for the delete.
 * @param tree
 * @param c
 * @param s
 * @param p
 * @param newm
 * @param m
 */
void delteHasTwoChildrens(RBTree *tree, Node *c, Node *s, Node *p, Node *newm, Node *m)
{
    Node *succsesor = successor(m);
    void *temp;

    temp = m->data;
    m->data = succsesor->data;
    succsesor->data = temp;
    newm = succsesor;
    p = getParent(newm);
    c = newm->right;
    if (isLeftOrRight(newm) == 0)
    {
        s = getParent(newm)->right;
    }
    else if (isLeftOrRight(newm) == 1)
    {
        s = getParent(newm)->left;
    }
    delM(newm, p, s, c, tree);
    tree->freeFunc(newm->data);
    free(newm);
    tree->size -= 1;
    if (c != NULL)
    {
        c->parent = p;
        if (p != NULL)
        {
            if (newm == p->left)
            {
                p->left = c;
            }
            else
            {
                p->right = c;
            }
        }
        else
        {
            tree->root = c;
        }
    }
    else
    {
        if (p != NULL)
        {
            if (newm == p->left)
            {
                p->left = c;
            }
            else
            {
                p->right = c;
            }
        }
        else
        {
            tree->root = c;
        }
    }
    newm = NULL;
}

/**
 * @brief helper function for the delete
 * @param tree
 * @param s
 * @param p
 * @param newm
 * @param m
 * @return
 */
int delteHasRightChild(RBTree *tree, Node *s, Node *p, Node *newm,  Node *m)
{
    if (isLeftOrRight(m) == 0)
    {
        getParent(m)->left = m->right;
        m->right->parent = getParent(m);
        newm = m->right;
        p = getParent(newm);
    }
    else if (isLeftOrRight(m) == 1)
    {
        getParent(m)->right = m->right;
        m->right->parent = getParent(m);
        newm = m->right;
        p = getParent(newm);
    }
    else if (m == tree->root)
    {
        tree->root = m->right;
        newm = tree->root;
        tree->root->color = BLACK;
        newm->parent = NULL;
    }
    if (isLeftOrRight(newm) == 0)
    {
        s = getParent(newm)->right;
    }
    else if (isLeftOrRight(newm) == 1)
    {
        s = getParent(newm)->left;
    }
    delM(m, p, s, newm, tree);
    tree->freeFunc(m->data);
    free(m);
    tree->size -= 1;
    return 1;
}

/**
 * @brief helper function for the delete
 * @param tree
 * @param s
 * @param p
 * @param newm
 * @param m
 * @return
 */
int deleteHasLeftChild(RBTree *tree, Node *s, Node *p, Node *newm,  Node *m)
{
    if (isLeftOrRight(m) == 0)
    {
        p = getParent(m);
        p->left = m->left;
        m->left->parent = getParent(m);
        newm = p->left;
        p = getParent(newm);

    }
    else if (isLeftOrRight(m) == 1)
    {
        getParent(m)->right = m->left;
        m->left->parent = getParent(m);
        newm = m->left;
        p = getParent(newm);
    }
    else if (m == tree->root)
    {
        tree->root = m->left;
        newm = tree->root;
        tree->root->color = BLACK;
        newm->parent = NULL;
    }
    if (isLeftOrRight(newm) == 0)
    {
        s = getParent(newm)->right;
    }
    else if (isLeftOrRight(newm) == 1)
    {
        s = getParent(newm)->left;
    }
    delM(m, p, s, newm, tree);
    tree->freeFunc(m->data);
    free(m);
    tree->size -= 1;
    return 1;
}

/**
 * @brief helper function for the delte
 * @param tree
 * @param c
 * @param s
 * @param p
 * @param m
 * @return
 */
int delNoChilderns(RBTree *tree,  Node *c, Node *s, Node *p,  Node *m)
{
    p = getParent(m);
    if (m->color == RED)
    {
        if (isLeftOrRight(m) == 0)
        {
            getParent(m)->left = NULL;
            s = getParent(m)->right;
        }
        else if (isLeftOrRight(m) == 1)
        {
            getParent(m)->right = NULL;
            s = getParent(m)->left;

        }
        tree->freeFunc(m->data);
        free(m);
        tree->size -= 1;
        return 1;
    }
    else
    {
        if (m == tree->root)
        {
            tree->freeFunc(tree->root->data);
            free(tree->root);
            tree->size -= 1;
            tree->root = NULL;
            return 1;
        }
        p = getParent(m);
        if (isLeftOrRight(m) == 0)
        {
            s = getParent(m)->right;
            getParent(m)->left = NULL;
        }
        else if (isLeftOrRight(m) == 1)
        {
            s = getParent(m)->left;
            getParent(m)->right = NULL;
        }
        delMCase3(m, p, s, c, tree);
        tree->freeFunc(m->data);
        free(m);
        tree->size -= 1;
        return 1;
    }
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
    if (tree == NULL || data == NULL)
    {
        return 0;
    }
    Node *temp = tree->root;
    while (temp != NULL)
    {
        if (tree->compFunc(data, temp->data) > 0)
        {
            temp = temp->right;
        }
        else if (tree->compFunc(data, temp->data) < 0)
        {
            temp = temp->left;
        }
        else
        {
            return 1;
        }
    }
    return 0;

}
/**
 * @brief inorder tree travsel
 * @param node
 * @param func
 * @param args
 */
void inorder(Node *node, forEachFunc func, void *args)
{
    if (node != NULL)
    {
        inorder(node->left, func, args);
        func(node->data, args);
        inorder(node->right, func, args);
    }
}
/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL)
    {
        return 0;
    }
    inorder(tree->root, func, args);
    return 1;
}
/**
 * @brief return the node that contains the min value
 * @param node
 * @return
 */
Node *findMinValue(Node *node)
{
    Node *temp = node;
    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}
/**
 * @brief return the successor
 * @param node
 * @return
 */
Node *successor(Node *node)
{
    Node *temp = getParent(node);
    if (node->right != NULL) // if right sub tree is not null the min of the subtree
    {
        return findMinValue(node->right);
    }
    while ((node == temp->right) && (temp != NULL)) //right subtree is null
    {
        node = temp;
        temp = getParent(temp);
    }
    return temp;
}
/**
 * @brief check if node is left or right child
 * @param new
 * @return
 */
int isLeftOrRight(Node *new)
{
    Node *parent = getParent(new);
    if (parent == NULL)
    {
        return -1;
    }
    if (new == parent->right)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/**
 * @brief helper function for the delete function
 * @param m
 * @param p
 * @param s
 * @param c
 * @param tree
 */
void delM(Node *m, Node *p, Node *s, Node *c, RBTree *tree)
{
    if (m->color == RED) // CHECK THIS
    {
        return;
    }
    else if ((m->color == BLACK) && ((c != NULL) && c->color == RED))
    {
        c->color = BLACK;

    }
    else if ((m->color == BLACK) && (c == NULL || c->color == BLACK)) // case 3.1
    {
        delMCase3(m, p, s, c, tree);
    }
}
/**
 * @brief del case 3 and above
 * @param m
 * @param p
 * @param s
 * @param c
 * @param tree
 */
void delMCase3(Node *m, Node *p, Node *s, Node *c, RBTree *tree)
{
    Node *sfar = NULL;
    Node *sclose = NULL;
    int checker = 0;
    if (isLeftOrRight(s) == 1)
    {
        checker = 0;
        if (s == NULL)
        {

        }
        else
        {
            sfar = s->right;
            sclose = s->left;
        }

    }
    else if (isLeftOrRight(s) == 0)
    {
        checker = 1;
        sfar = s->left;
        sclose = s->right;
    }
    if (c == tree->root) // case 3.1
    {
    }

    else if ((s == NULL) || (s->color == BLACK && s->right == NULL && s->left == NULL) ||
             (s->color ==
             BLACK
             && s->right != NULL && s->left != NULL && s->left->color == BLACK &&
             s->right->color ==
             BLACK))
    {

        delMOption2(m, p, s, c, tree);
    }
    else if (s->color == RED) // case 3.3
    {
        delMOption3(m, p, s, c, tree);
    }
    else if (s->color == BLACK && (sfar == NULL || sfar->color == BLACK) && sclose != NULL &&
             sclose->color ==
             RED)
        // CASE
        // 3.4
    {
        delMOption4(m, p, s, c, tree, sclose);
    }
    else if (sfar != NULL && sfar->color == RED) // case 3.5
    {
        deleteMOption5(p, s, tree, sfar, checker);
    }

}
/**
 * @brief delete case 3.5
 * @param p
 * @param s
 * @param tree
 * @param sfar
 * @param checker
 */
void deleteMOption5(Node *p, Node *s,  RBTree *tree, Node *sfar, int checker)
{
    Color scolor = s->color;
    Color pcolor = p->color;
    s->color = pcolor;
    p->color = scolor;
    if (checker == 0)
    {
        rotateLeft(getParent(s), tree);
    }
    else
    {
        rotateRight(getParent(s), tree);
    }
    sfar->color = BLACK;
}

/**
 * @brief delete case 3.4
 * @param m
 * @param p
 * @param s
 * @param c
 * @param tree
 * @param sclose
 */
void delMOption4( Node *m,  Node *p, Node *s, Node *c,  RBTree *tree, Node *sclose)
{
    s->color = RED;
    sclose->color = BLACK;
    if (p->left == s)
    {
        rotateLeft(s, tree);
    }
    else if (p->right == s)
    {
        rotateRight(s, tree);
    }
    if (p->left == NULL)
    {
        s = p->right;
    }
    else if (p->right == NULL)
    {
        s = p->left;
    }
    else
    {
        if (p->left == m)
        {
            s = p->right;
        }
        else
        {
            s = p->left;
        }
    }
    delMCase3(m, p, s, c, tree);
}

/**
 * @brief delete case 3.3
 * @param m
 * @param p
 * @param s
 * @param c
 * @param tree
 */
void delMOption3( Node *m, Node *p, Node *s,  Node *c,  RBTree *tree)
{
    if (m == NULL)
    {
        s->color = BLACK;
        p->color = RED;
        if (p->left == s)
        {
            rotateRight(p, tree);
        }
        else if (p->right == s)
        {
            rotateLeft(p, tree);
        }
        if (p->left == NULL)
        {
            s = p->right;
        }
        else if (p->right == NULL)
        {
            s = p->left;
        }
        else
        {
            if (p->left == m)
            {
                s = p->right;
            }
            else
            {
                s = p->left;
            }
        }
        delMCase3(m, p, s, c, tree);
    }
    else
    {
        s->color = BLACK;
        p->color = RED;
        if (p->left == s)
        {
            rotateRight(p, tree);
        }
        else if (p->right == s)
        {
            rotateLeft(p, tree);
        }
        if (p->left == NULL)
        {
            s = p->right;
        }
        else if (p->right == NULL)
        {
            s = p->left;
        }
        else
        {
            if (p->left == m)
            {
                s = p->right;
            }
            else
            {
                s = p->left;
            }
        }
        delMCase3(m, p, s, c, tree);
    }
}

/**
 * @brief delete case 3.2
 * @param m
 * @param p
 * @param s
 * @param c
 * @param tree
 */
void delMOption2(Node *m, Node *p, Node *s, Node *c,  RBTree *tree)
{
    if (p != NULL && p->color == RED) // case 3.2.1
    {
        p->color = BLACK;
        s->color = RED;
    }
    else if ((p == NULL) || (p->color == BLACK))  // case 3.2.2
    {
        if (s != NULL)
        {
            s->color = RED;
        }
        if (p != NULL)
        {
            p->color = BLACK;
            m = m->parent;
            c = p;
            p = getParent(c);
            c->parent = p;
            if (isLeftOrRight(c) == 0)
            {
                s = p->right;
            }
            else if (isLeftOrRight(c) == 1)
            {
                s = p->left;
            }
        }
        else
        {
            c = tree->root;
        }
        delMCase3(m, p, s, c, tree);
    }
}

/**
 * @brief deltree helpe function
 * @param node
 * @param tree
 */
void deltree(Node *node, RBTree *tree)
{
    if (node == NULL || node->data == NULL)
    {
        return;
    }
    deltree(node->left, tree);
    deltree(node->right, tree);
    tree->freeFunc(node->data);
    free(node);
}
/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
    RBTree *t = *tree;
    deltree(t->root, t);
    free(t);
}