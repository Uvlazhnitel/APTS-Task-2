#include <iostream>
#include <fstream>
#include <cstring>

struct TreeNode {
    int value;
    TreeNode* children[100];
    int child_count;
    TreeNode(int v) : value(v), child_count(0) {}
};

class Tree {
public:
    Tree() {
        for (int i = 0; i < 10000; ++i) {
            nodes[i] = nullptr;
        }
        node_count = 0;
    }

    TreeNode* getNode(int value) {
        for (int i = 0; i < node_count; ++i) {
            if (nodes[i]->value == value) {
                return nodes[i];
            }
        }
        return nullptr;
    }

    TreeNode* addNode(int value) {
        TreeNode* node = getNode(value);
        if (node == nullptr) {
            node = new TreeNode(value);
            nodes[node_count++] = node;
        }
        return node;
    }

    void addChild(TreeNode* parent, TreeNode* child) {
        parent->children[parent->child_count++] = child;
    }

    void mirror(TreeNode* node) {
        for (int i = 0; i < node->child_count / 2; ++i) {
            TreeNode* temp = node->children[i];
            node->children[i] = node->children[node->child_count - 1 - i];
            node->children[node->child_count - 1 - i] = temp;
        }
        for (int i = 0; i < node->child_count; ++i) {
            mirror(node->children[i]);
        }
    }

    void preorder(TreeNode* node, TreeNode** result, int& index) {
        if (node == nullptr) return;
        result[index++] = node;
        for (int i = 0; i < node->child_count; ++i) {
            preorder(node->children[i], result, index);
        }
    }

    TreeNode* findRoot() {
        bool isChild[10000];
        memset(isChild, false, sizeof(isChild));
        for (int i = 0; i < node_count; ++i) {
            for (int j = 0; j < nodes[i]->child_count; ++j) {
                isChild[nodes[i]->children[j]->value] = true;
            }
        }
        for (int i = 0; i < node_count; ++i) {
            if (!isChild[nodes[i]->value]) {
                return nodes[i];
            }
        }
        return nullptr;
    }

private:
    TreeNode* nodes[10000];
    int node_count;
};

void parseInput(const char* filename, Tree& tree) {
    std::ifstream infile(filename);
    char line[256];
    while (infile.getline(line, 256)) {
        int parent;
        int child;
        char* token = strtok(line, " ");
        parent = atoi(token);
        if (parent == 0) break;
        TreeNode* parentNode = tree.addNode(parent);
        while (token != nullptr) {
            token = strtok(nullptr, " ");
            if (token != nullptr) {
                child = atoi(token);
                TreeNode* childNode = tree.addNode(child);
                tree.addChild(parentNode, childNode);
            }
        }
    }
}

void writeOutput(const char* filename, TreeNode** nodes, int count) {
    std::ofstream outfile(filename);
    for (int i = 0; i < count; ++i) {
        if (nodes[i]->child_count > 0) {
            outfile << nodes[i]->value;
            for (int j = 0; j < nodes[i]->child_count; ++j) {
                outfile << " " << nodes[i]->children[j]->value;
            }
            outfile << "\n";
        }
    }
    outfile << "0\n";
}

int main() {
    Tree tree;
    parseInput("alice.in", tree);
    TreeNode* root = tree.findRoot();
    tree.mirror(root);
    TreeNode* result[10000];
    int index = 0;
    tree.preorder(root, result, index);
    writeOutput("alice.out", result, index);

    // Free allocated memory
    for (int i = 0; i < 10000; ++i) {
        if (tree.getNode(i)) {
            delete tree.getNode(i);
        }
    }

    return 0;
}
