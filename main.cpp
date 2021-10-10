#include <stdio.h>
#include <iostream>

#define RED 0
#define BLACK 1

using namespace std;
struct node {
    int key;
    int color;
    struct node *parent;
    struct node *left;
    struct node *right;
};

/* Global, since all function will access them */
struct node *ROOT;
// represent leaves
struct node *NILL;

void left_rotate(struct node *x);

void right_rotate(struct node *x);

void tree_print(struct node *x);

void red_black_insert(int key);

void red_black_insert_fixup(struct node *z);

struct node *tree_search(int key);

struct node *tree_minimum(struct node *x);

void red_black_transplant(struct node *u, struct node *v);

void red_black_delete(struct node *z);

void red_black_delete_fixup(struct node *x);

int main() {
    NILL = new node;
    NILL->color = BLACK;

    ROOT = NILL;

    printf("RED-BLACK TREE INSERT\n\n");

    int tcase, key;
    printf("Number of key: ");
    scanf("%d", &tcase);
    while (tcase--) {
        printf("Enter key: ");
        scanf("%d", &key);
        red_black_insert(key);
    }

    printf("### TREE PRINT ###\n\n");
    tree_print(ROOT);
    printf("\n");

    printf("### KEY SEARCH ###\n\n");
    printf("Enter key: ");
    scanf("%d", &key);
    printf((tree_search(key) == NILL) ? "NILL\n" : "%p\n", tree_search(key));

    printf("### MIN TEST ###\n\n");
    printf("MIN: %d\n", (tree_minimum(ROOT))->key);

    printf("### TREE DELETE TEST ###\n\n");
    printf("Enter key to delete: ");
    scanf("%d", &key);
    red_black_delete(tree_search(key));

    printf("### TREE PRINT ###\n\n");
    tree_print(ROOT);
    printf("\n");

    return 0;
}

/* Print tree keys by inorder tree walk */

void tree_print(struct node *x) {
    if (x != NILL) {
        tree_print(x->left);
        printf("%d\t", x->key);
        /*
        if (x->color)
            cout << "black" << endl;
        else
            cout << "red" << endl;

        */
        tree_print(x->right);
    }
}

struct node *tree_search(int key) {
    struct node *x;

    x = ROOT;
    while (x != NILL && x->key != key) {
        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    return x;
}

struct node *tree_minimum(struct node *x) {
    while (x->left != NILL) {
        x = x->left;
    }
    return x;
}

void red_black_insert(int key) {
    struct node *z, *x, *y;
    z = new node;
    z->key = key;
    z->color = RED;
    z->left = NILL;
    z->right = NILL;
    x = ROOT;
    y = NILL;
    /*
     * Go through the tree untill a leaf(NILL) is reached. y is used for keeping
     * track of the last non-NILL node which will be z's parent.
     */
    while (x != NILL) {
        y = x;
        if (z->key <= x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    if (y == NILL) {
        ROOT = z;
    } else if (z->key <= y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->parent = y;
    red_black_insert_fixup(z);
}

void red_black_insert_fixup(struct node *z) {
    while (z->parent->color == RED && z != ROOT) { // (z != Root)

        // z's parent is left child of z's grand parent -> meaning that node added to left
        if (z->parent == z->parent->parent->left) {

            // case 1 uncle is red
            // z's grand parent's right child is RED -> uncle of node
            if (z->parent->parent->right->color == RED && z->parent->parent->right != NILL) {
                z->parent->color = BLACK;
                z->parent->parent->right->color = BLACK;
                z->parent->parent->color = RED;
                // recoloring
                z = z->parent->parent;
            }

                // case 2 uncle is black
                /* z's grand parent's right child is not RED */
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(z);
                }

                // case 3
                // node z is a left child
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(z->parent->parent);
            }
        }

            /* z's parent is z's grand parent's right child */
        else {

            /* z's left uncle or z's grand parent's left child is also RED */
            if (z->parent->parent->left->color == RED) {
                z->parent->color = BLACK;
                z->parent->parent->left->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }

                /* z's left uncle is not RED */
            else {
                /* z is z's parents left child */
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(z->parent->parent);
            }
        }
    }

    ROOT->color = BLACK;
}
void left_rotate(struct node *x) {
    struct node *y;
    y = x->right; // set y
    x->right = y->left; // turn y's left subtree into x's right subtree
    if (y->left != NILL) {
        y->left->parent = x;
    }
    /* Make x's parent y's parent and y, x's parent's child */
    y->parent = x->parent;
    if (y->parent == NILL) {
        ROOT = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    /* Make x, y's left child & y, x's parent */
    y->left = x;
    x->parent = y;
}
void right_rotate(struct node *x) {
    struct node *y;

    /* Make y's right child x's left child */
    y = x->left;
    x->left = y->right;
    if (y->right != NILL) {
        y->right->parent = x;
    }

    /* Make x's parent y's parent and y, x's parent's child */
    y->parent = x->parent;
    if (y->parent == NILL) {
        ROOT = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    /* Make y, x's parent and x, y's child */
    y->right = x;
    x->parent = y;
}

/*
 * Deletion is done by the same mechanism as BST deletion. If z has no child, z is
 * removed. If z has single child, z is replaced by its child. Else z is replaced by
 * its successor. If successor is not z's own child, successor is replaced by its
 * own child first. then z is replaced by the successor.
 *
 * A pointer y is used to keep track. In first two case y is z. 3rd case y is z's
 * successor. So in first two case y is removed. In 3rd case y is moved.
 *
 *Another pointer x is used to keep track of the node which replace y.
 * 
 * As removing or moving y can harm red-black tree properties a variable
 * yOriginalColor is used to keep track of the original colour. If its BLACK then
 * removing or moving y harm red-black tree properties. In that case an auxilary
 * procedure red_black_delete_fixup(x) is called to recover this.
 */

void red_black_delete(struct node *z) {
    struct node *y, *x;
    int yOriginalColor;

    y = z;
    yOriginalColor = y->color;

    if (z->left == NILL) {
        x = z->right;
        red_black_transplant(z, z->right);
    } else if (z->right == NILL) {
        x = z->left;
        red_black_transplant(z, z->left);
    } else {
        y = tree_minimum(z->right);
        yOriginalColor = y->color;

        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            red_black_transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        red_black_transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK) {
        red_black_delete_fixup(x);
    }
}

/*
 * As y was black and removed x gains y's extra blackness.
 * Move the extra blackness of x until
 *		1. x becomes root. In that case just remove extra blackness
 *		2. x becomes a RED and BLACK node. in that case just make x BLACK
 *
 * First check if x is x's parents left or right child. Say x is left child
 *
 * There are 4 cases.
 *
 * Case 1: x's sibling w is red. transform case 1 into case 2 by recoloring
 * w and x's parent. Then left rotate x's parent.
 *
 * Case 2: x's sibling w is black, w's both children is black. Move x and w's
 * blackness to x's parent by coloring w to RED and x's parent to BLACK.
 * Make x's parent new x.Notice if case 2 come through case 1 x's parent becomes 
 * RED and BLACK as it became RED in case 1. So loop will stop in next iteration.
 *
 * Case 3: w is black, w's left child is red and right child is black. Transform
 * case 3 into case 4 by recoloring w and w's left child, then right rotate w.
 *
 * Case 4: w is black, w's right child is red. recolor w with x's parent's color.
 * make x's parent BLACK, w's right child black. Now left rotate x's parent. Make x
 * point to root. So loop will be stopped in next iteration.
 *
 * If x is right child of it's parent do exact same thing swapping left<->right
 */

void red_black_delete_fixup(struct node *x) {
    struct node *w;

    while (x != ROOT && x->color == BLACK) {

        if (x == x->parent->left) {
            w = x->parent->right;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x->parent->color = BLACK;
                x = x->parent;
            } else {

                if (w->right->color == BLACK) {
                    w->color = RED;
                    w->left->color = BLACK;
                    right_rotate(w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                x->right->color = BLACK;
                left_rotate(x->parent);
                x = ROOT;

            }

        } else {
            w = x->parent->left;

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = BLACK;
                right_rotate(x->parent);
                w = x->parent->left;
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x->parent->color = BLACK;
                x = x->parent;
            } else {

                if (w->left->color == BLACK) {
                    w->color = RED;
                    w->right->color = BLACK;
                    left_rotate(w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(x->parent);
                x = ROOT;

            }
        }

    }

    x->color = BLACK;
}

/* replace node u with node v */
void red_black_transplant(struct node *u, struct node *v) {
    if (u->parent == NILL) {
        ROOT = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}