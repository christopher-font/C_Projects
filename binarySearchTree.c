/* 	
	This program is written by: Christopher Font
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 30
#define OUTFILE "out.txt"

typedef struct itemNode {
    char name[MAXLEN];
    int count;
    struct itemNode *left, *right;
} itemNode;

typedef struct treeNameNode {
    char treeName[MAXLEN];
    struct treeNameNode *left, *right;
    itemNode *theTree;
}treeNameNode;

treeNameNode * dynamicallyCreateTreeNameNode(char * s) {
    treeNameNode * tmp = (treeNameNode *) malloc(sizeof(treeNameNode));

    strcpy(tmp->treeName, s);
    tmp->left = tmp->right = NULL;
    tmp->theTree = NULL;

    return tmp;
}

treeNameNode* buildNameTree(treeNameNode * node, char * key) {
    if (node == NULL)
        return dynamicallyCreateTreeNameNode(key);

    int result = strcmp(node->treeName, key);

    if (result < 0)
        node->left = buildNameTree(node->left, key);
    else if (result > 0)
        node->right = buildNameTree(node->right, key);

    return node;
}

treeNameNode * loadTreeNameNodeFromFile (FILE * fp, int n) {
    char s[MAXLEN];

    treeNameNode * root = NULL;

    while (n > 0) {
        fscanf(fp, "%s\n", s);
        root = buildNameTree(root, s);
        n--;
    }

    return root;
}

treeNameNode * searchForTreeNodeUsingKey (treeNameNode * node, char * key) {
    if (node == NULL)
        return NULL;

    int result = strcmp(node->treeName, key);

    if (result == 0)
        return node;
    else if (result < 0)
        return searchForTreeNodeUsingKey(node->left, key);
    else
        return searchForTreeNodeUsingKey(node->right, key);
}

itemNode * searchForItemNodeUsingKey (itemNode* node, char * key) {
    if (node == NULL)
        return NULL;

    int result = strcmp(node->name, key);

    if (result == 0)
        return node;
    else if (result < 0)
        return searchForItemNodeUsingKey(node->left, key);
    else
        return searchForItemNodeUsingKey(node->right, key);
}

void freeItemNodes(itemNode * node) {
    if (node == NULL)
        return;

    freeItemNodes(node->left);
    freeItemNodes(node->right);

    free(node);
}

void printItemNodes(itemNode * node) {
    if (node == NULL)
        return;

    printItemNodes(node->right);
    FILE * ofp = fopen(OUTFILE, "a");
    fprintf(ofp, "%s ", node->name);
    fclose(ofp);

    printItemNodes(node->left);
}

void printTreeNameNodes(treeNameNode * node) {
    if (node == NULL)
        return;

    printTreeNameNodes(node->right);
    FILE * ofp = fopen(OUTFILE, "a");
    fprintf(ofp, "===%s===\n", node->treeName);
    fclose(ofp);
    printItemNodes(node->theTree);
    ofp = fopen(OUTFILE, "a");
    fprintf(ofp, "\n");
    fclose(ofp);
    printTreeNameNodes(node->left);
}

void printTreeNameNodesOnly(treeNameNode * node) {
    if (node == NULL)
        return;

    printTreeNameNodesOnly(node->right);
    FILE * ofp = fopen(OUTFILE, "a");
    fprintf(ofp, "%s ", node->treeName);
    fclose(ofp);
    printTreeNameNodesOnly(node->left);
}

void freeTreeNameNodes(treeNameNode * node) {
    if (node == NULL)
        return;

    freeTreeNameNodes(node->left);
    freeTreeNameNodes(node->right);
    freeItemNodes(node->theTree);

    free(node);
}

void loadFirstLineFromFile (FILE * fp, int * n, int * i, int * q) {
    fscanf(fp, "%d%d%d\n", n, i, q);
}

void printFirstLine(int n , int i, int q) {

    FILE * ofp = fopen(OUTFILE, "a");
    fprintf(ofp, "%d\t%d\t%d\n", n, i, q);
    fclose(ofp);
}

itemNode * dynamicallyCreateItemNode(char * s, int count) {
    itemNode * tmp = (itemNode *) malloc(sizeof(itemNode));
    strcpy(tmp->name, s);
    tmp->count = count;
    tmp->left = tmp->right = NULL;
    return tmp;
}

itemNode * insertItemNode(itemNode * node, char * key, int count) {
    if (node == NULL)
        return dynamicallyCreateItemNode(key, count);

    int result = strcmp(node->name, key);

    if (result < 0)
        node->left = insertItemNode(node->left, key, count);
    else if (result > 0)
        node->right = insertItemNode(node->right, key, count);

    return node;
}

void loadItemsFromFile(FILE * fp, treeNameNode * root, int i) {
    char s[MAXLEN];
    treeNameNode * tmp;
    int tmp2 = 0;

    while (i > 0) {
        fscanf(fp, "%s", s);
        tmp = searchForTreeNodeUsingKey(root, s);
        fscanf(fp, "%s", s);
        fscanf(fp, "%d\n", &tmp2);
        tmp->theTree = insertItemNode(tmp->theTree, s, tmp2);
        i--;
    }
}

int countItemsBefore(itemNode * node, char * key) {
    if (node == NULL)
        return 0;

    int result = strcmp(node->name, key);

    if (result == 0)
        return 0 +  countItemsBefore(node->left, key) + countItemsBefore(node->right, key);

    if (result < 0)
        return 1 + countItemsBefore(node->left, key) + countItemsBefore(node->right, key);

    return countItemsBefore(node->left, key) + countItemsBefore(node->right, key);
}

void search(treeNameNode * root, char * treeKey, char * itemKey) {
    treeNameNode * tmpTreeNameNode = searchForTreeNodeUsingKey(root, treeKey);
	
    if (tmpTreeNameNode == NULL) {
        FILE * ofp = fopen(OUTFILE, "a");
        fprintf(ofp, "%s does not exist\n", treeKey);
        fclose(ofp);
        return;
    }

    itemNode * tmpItemNameNode = searchForItemNodeUsingKey(tmpTreeNameNode->theTree, itemKey);
    
	if (tmpItemNameNode == NULL) {
        FILE * ofp = fopen(OUTFILE, "a");
        fprintf(ofp, "%s not found in %s\n", itemKey, treeKey);
        fclose(ofp);
        return;
    }

    FILE * ofp = fopen(OUTFILE, "a");
    fprintf(ofp, "%d %s found in %s\n", tmpItemNameNode->count, itemKey, treeKey);
    fclose(ofp);
}

void item_before(treeNameNode * root, char * treeKey, char * itemKey) {
    treeNameNode * tmpTreeNameNode = searchForTreeNodeUsingKey(root, treeKey);

    if (tmpTreeNameNode == NULL) {
        FILE * ofp = fopen(OUTFILE, "a");
        fprintf(ofp, "%s does not exist\n", treeKey);
        fclose(ofp);
        return;
    }

    itemNode * tmpItemNameNode = searchForItemNodeUsingKey(tmpTreeNameNode->theTree, itemKey);

    if (tmpItemNameNode == NULL) {
        FILE * ofp = fopen(OUTFILE, "a");
        fprintf(ofp, "%s not found in %s\n", itemKey, treeKey);
        fclose(ofp);
        return;
    }

    int i = countItemsBefore(tmpTreeNameNode->theTree, itemKey);
    FILE * ofp = fopen(OUTFILE, "a");
    fprintf(ofp, "item before %s: %d\n", itemKey, i);
    fclose(ofp);
}

int findMaxValue(int x, int y) {
    if (x > y) return x;
    return y;
}

int claculateDiffrence(int x, int y) {
    if (x > y) return abs(x - y);
    return abs(y - x);
}

int height_balance(itemNode * node) {
    if (node == NULL) return -1;

    int lh = height_balance(node->left);
    int rh = height_balance(node->right);
	
    int diffrence = claculateDiffrence(lh, rh);
	
	return 1 + findMaxValue(lh, rh);
}

void heightBalanceStarter(treeNameNode * root, char * treeKey) {
    const char balancedAmount[MAXLEN][MAXLEN] = {"not balanced", "balanced"};

    treeNameNode * tmpTreeNameNode = searchForTreeNodeUsingKey(root, treeKey);

    if (tmpTreeNameNode == NULL) {
        FILE * ofp = fopen(OUTFILE, "a");
        fprintf(ofp, "%s does not exist\n", treeKey);
        fclose(ofp);
        return;
    }

    // I probably loaded the trees incorrectly to the left and right nodes
    // But this works so...
    int rightHeight = height_balance(tmpTreeNameNode->theTree->left);
    int leftHeight = height_balance(tmpTreeNameNode->theTree->right);
    int difference = claculateDiffrence(leftHeight, rightHeight);
    int isBalanced = difference > 1? 0: 1;
    FILE * ofp = fopen(OUTFILE, "a");
    fprintf(ofp, "%s: left height %d, right height %d, difference %d, %s\n", treeKey, leftHeight, rightHeight, difference, balancedAmount[isBalanced]);
    fclose(ofp);
}

int count(itemNode * node) {
    if (node == NULL)
        return 0;

    int leftAmount = count(node->left);
    int rightAmount = count(node->right);

    return leftAmount + rightAmount + node->count;
}

itemNode * copyToItemNode(itemNode * one, itemNode * two) {
    strcpy(one->name, two->name);
    one->count = two->count;
}

itemNode * findMinItemNode(itemNode * node) {
    if (node->left == NULL)
        return node;

    return findMinItemNode(node->left);
}

itemNode * deleteItemNode(itemNode * node, char * key) {
    if (node == NULL)
        return node;

    if (strcmp(key, node->name) > 0)
        node->left = deleteItemNode(node->left, key);
    else if (strcmp(key, node->name) < 0)
        node->right = deleteItemNode(node->right, key);
    else {
        if (node->left == NULL) {
            itemNode * tmp = node->right;
            free(node);
            return tmp;
        }
        else if (node->right == NULL) {
            itemNode * tmp = node->left;
            free(node);
            return tmp;
        }
        itemNode * tmp = findMinItemNode(node->right);
        copyToItemNode(node, tmp);
        node->right = deleteItemNode(node->right, tmp->name);
    }
    return node;
}

treeNameNode * copyToTreeNameNode(treeNameNode * one, treeNameNode * two) {
    strcpy(one->treeName, two->treeName);
}

treeNameNode * findMinTreeNameNode(treeNameNode * node) {
    if (node->left == NULL)
        return node;

    return findMinTreeNameNode(node->left);
}

treeNameNode * deleteTreeNameNode(treeNameNode * node, char * key) {
    if (node == NULL)
        return node;

    if (strcmp(key, node->treeName) > 0)
        node->left = deleteTreeNameNode(node->left, key);
    else if (strcmp(key, node->treeName) < 0)
        node->right = deleteTreeNameNode(node->right, key);
    else {
        if (node->left == NULL) {
            treeNameNode * tmp = node->right;
            freeItemNodes(node->theTree);
            free(node);
            return tmp;
        }
        else if (node->right == NULL) {
            treeNameNode * tmp = node->left;
            freeItemNodes(node->theTree);
            free(node);
            return tmp;
        }
        treeNameNode * tmp = findMinTreeNameNode(node->right);
        copyToTreeNameNode(node, tmp);
        node->right = deleteTreeNameNode(node->right, tmp->treeName);
    }

    return node;
}

void reduce(itemNode * root, char * key, int amount) {
    itemNode * tmp = searchForItemNodeUsingKey(root, key);
	
    if (tmp->count > amount) {
        tmp->count = tmp->count - amount;
        FILE * ofp = fopen(OUTFILE, "a");
        fprintf(ofp, "%s reduced\n", key);
        fclose(ofp);
    } else {
        tmp = deleteItemNode(tmp, key);
        FILE * ofp = fopen(OUTFILE, "a");
        fprintf(ofp, "%s deleted\n", key);
        fclose(ofp);
    }
    return;
}

void loadCommands(FILE * fp, treeNameNode * root, int q) {
    FILE * ofp;
    char s1[MAXLEN];
    char s2[MAXLEN];

    while (q > 0) {
        strcpy(s1, "");
        strcpy(s2, "");
        ofp = fopen(OUTFILE, "a");
        fscanf(fp, "%s", s1);
		
        if (strcmp(s1, "search") == 0) {
            fscanf(fp, "%s", s1);
            fscanf(fp, "%s\n", s2);
            search(root, s1, s2);
        } else if (strcmp(s1, "item_before") == 0) {
            fscanf(fp, "%s", s1);
            fscanf(fp, "%s\n", s2);
            item_before(root, s1, s2);
        } else if (strcmp(s1, "height_balance") == 0) {
            fscanf(fp, "%s\n", s1);
            heightBalanceStarter(root, s1);
        } else if (strcmp(s1, "count") == 0) {
            fscanf(fp, "%s\n", s1);
            fprintf(ofp, "%s count %d\n", s1, count(searchForTreeNodeUsingKey(root, s1)->theTree));
        } else if (strcmp(s1, "delete") == 0) {
            fscanf(fp, "%s", s1);
            fscanf(fp, "%s\n", s2);
            searchForTreeNodeUsingKey(root, s1)->theTree = deleteItemNode(searchForTreeNodeUsingKey(root, s1)->theTree, s2);
            fprintf(ofp, "%s deleted from %s\n", s2, s1);
        } else if (strcmp(s1, "delete_name") == 0) {
            fscanf(fp, "%s\n", s1);
            root = deleteTreeNameNode(root, s1);
            fprintf(ofp, "%s deleted\n", s1);
        } else if (strcmp(s1, "reduce") == 0) {
            int tmp = 0;
            fscanf(fp, "%s", s1);
            fscanf(fp, "%s", s2);
            fscanf(fp, "%d", &tmp);
            reduce(searchForTreeNodeUsingKey(root, s1)->theTree, s2, tmp);
        }
        fclose(ofp);
        q--;
    }
}

int main (void) {
    int n;
    int i;
    int q;
    FILE * fp = fopen("in.txt", "r");
	
    loadFirstLineFromFile(fp, &n, &i, &q);
    treeNameNode * treeNameRoot = loadTreeNameNodeFromFile(fp, n);

	
    FILE * ofp = fopen(OUTFILE, "w");
    fclose(ofp);
    
	printTreeNameNodesOnly(treeNameRoot);
    
	ofp = fopen(OUTFILE, "a");
    fprintf(ofp, "\n");
    fclose(ofp);
    loadItemsFromFile(fp, treeNameRoot, i);
    printTreeNameNodes(treeNameRoot);
    loadCommands(fp, treeNameRoot, q);
    freeTreeNameNodes(treeNameRoot);
    fclose(fp);
    return 0;
}
