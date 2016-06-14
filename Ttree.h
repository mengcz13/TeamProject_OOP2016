#ifndef TREE_H
#define TREE_H
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <cstring>
#include <stack>

struct Point3d {
    double x, y, t;
    Point3d(): x(0), y(0), t(0) {}
    Point3d(double px, double py, double pz): x(px), y(py), t(pz) {}
};

struct Task {
    double X, Y, T;
    Point3d o;
};

struct TtreeNode {
    Task* task;
    TtreeNode* child[3];
    TtreeNode* parent;
    TtreeNode() {
        task = NULL;
        child[0] = child[1] = child[2] = NULL;
        parent = NULL;
    }
};

class Contour;

class Ttree {
public:
    Ttree(Task* taskvec, int tnum);
    ~Ttree();
    // void transform();
    void transform_move(int rank1, int rank2, int childth);
    void transform_swap(int rank1, int rank2);
    void transform_rotate(int rank);
    double pack();
    Ttree& operator = (const Ttree& ttree);
    int getnum() { return tasknum; }
private:
    TtreeNode* pool;
    TtreeNode* root;
    double bx, by, bt;
    int tasknum;
    std::list<TtreeNode*> placed; // Record nodes having been placed, used when x needs to be determined
    TtreeNode* tree_decomposition(TtreeNode* node, std::stack<TtreeNode*>&  res);
    void binary_tree_packing(TtreeNode* node);
    void place_module(TtreeNode* node, Contour& blist);
    double find_max_x(TtreeNode* node);
    double find_max_y(TtreeNode* node, std::list<TtreeNode*>& blist); // Naive version, without contour
    int remove_node(int rank);
    void insert_node(int rank_src, int rank_dst, int childth);
};

struct ContourNode {
    Task* task;
    double st;
    double et;
};

class Contour {
public:
    double insert(TtreeNode* node); // Update contour and return the y position for the new block
private:
    std::list<ContourNode> conlist;
};

class Placer {
public:
    Placer(std::string inputfilename, std::string outputfilename, double initt, double finalt, double alpha);
    ~Placer();
private:
    void sa_place();
    double total_volume;
    double net_vol;
    Ttree* current_ttree;
    Ttree* backup_ttree;
    double initial_temp;
    double final_temp;
    double alpha;
};

#endif