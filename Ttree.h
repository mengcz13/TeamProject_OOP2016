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

class Ttree {
public:
    Ttree(Task* taskvec, int tnum);
    ~Ttree();
    void transform();
    double pack();
    Ttree& operator = (const Ttree& ttree);
private:
    TtreeNode* pool;
    TtreeNode* root;
    double bx, by, bz;
    int tasknum;
    std::list<TtreeNode*> placed; // Record nodes having been placed, used when x needs to be determined
    void transform_move();
    void transform_swap();
    void transform_rotate();
    TtreeNode* tree_decomposition(TtreeNode* node, std::stack<TtreeNode*>&  res);
    void binary_tree_packing(TtreeNode* node);
    void place_module(TtreeNode* node, std::list<TtreeNode*>& blist);
    double find_max_x(TtreeNode* node);
    double find_max_y(TtreeNode* node, std::list<TtreeNode*>& blist); // Naive version, without contour
};

class Placer {
public:
    Placer(std::string inputfilename, std::string outputfilename);
    ~Placer();
private:
    void sa_place();
    double total_volume;
    Ttree current_ttree;
    Ttree backup_ttree;
};