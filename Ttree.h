#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <cstring>
#include <stack>

struct Point3d {
	double x, y, t;
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
	void transform_move();
	void transform_swap();
	void transform_rotate();
	TtreeNode* tree_decomposition(TtreeNode* node, std::stack<TtreeNode*>&  res);
	void binary_tree_packing(TtreeNode* node);
	void place_module(TtreeNode* node);
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