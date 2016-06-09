/*
* @Author: mengcz13
* @Date:   2016-06-08 16:14:03
* @Last Modified by:   mengcz13
* @Last Modified time: 2016-06-09 11:06:28
*/

#include "Ttree.h"
#include <queue>

using namespace std;

Ttree::Ttree(Task* taskvec, int tnum) {
    tasknum = tnum;
    pool = new TtreeNode[tasknum];
    for (int i = 0; i < tasknum; ++i) {
        pool[i].task = taskvec + i;
    }
    root = pool;
    queue<TtreeNode*> bfsq;
    bfsq.push(root);
    int lastnode = 1;
    while (!bfsq.empty()) {
        TtreeNode* node = bfsq.front();
        bfsq.pop();
        int valid = 0;
        for (int i = 0; i < 3; ++i) {
            if (lastnode + i < tasknum) {
                node->child[i] = pool + lastnode + i;
                node->child[i]->parent = node;
                bfsq.push(node->child[i]);
                ++valid;
            }
        }
        lastnode += valid;
    }
}

Ttree::~Ttree() {
    delete []pool;
}

void Ttree::transform() {

}

double Ttree::pack() {
    // renew bx, by, bz
    stack<TtreeNode*> res;
    res.push(root);
    while (!res.empty()) {
        TtreeNode* s_top = res.top();
        res.pop();
        TtreeNode* btn = tree_decomposition(s_top, res);
        binary_tree_packing(btn);
    }
    return (bx * by * bz);
}

Ttree& Ttree::operator = (const Ttree& ttree) {
    bx = ttree.bx;
    by = ttree.by;
    bz = ttree.bz;
    tasknum = ttree.tasknum;
    memcpy(pool, ttree.pool, tasknum * sizeof(TtreeNode));
    root = pool;
    return (*this);
}

void Ttree::transform_move() {

}

void Ttree::transform_swap() {

}

void Ttree::transform_rotate() {

}

TtreeNode* Ttree::tree_decomposition(TtreeNode* node, stack<TtreeNode*>&  res) {
    stack<TtreeNode*> temps;
    temps.push(node);
    while (!temps.empty()) {
        TtreeNode* currn = temps.top();
        temps.pop();
        if (currn->child[1] != NULL)
            temps.push(currn->child[1]);
        if (currn->child[0] != NULL)
            temps.push(currn->child[0]);
        if (currn->child[2] != NULL)
            res.push(currn->child[2]);
    }
    return node;
}

void Ttree::binary_tree_packing(TtreeNode* node) {
    stack<TtreeNode*> temps;
    temps.push(node);
    list<TtreeNode*> blist;
    while (!temps.empty()) {
        TtreeNode* cn = temps.top();
        temps.pop();
        place_module(cn, blist);
        blist.push_back(cn);
        if (cn->child[1] != NULL)
            temps.push(cn->child[1]);
        if (cn->child[0] != NULL)
            temps.push(cn->child[0]);
    }
}

void Ttree::place_module(TtreeNode* node, list<TtreeNode*>& blist) {
    // to be continued...
    if (node->parent == NULL) {
        node->task->o = Point3d(0, 0, 0);
    }
    else if (node == node->parent->child[0]) {
        // T+ direction
        node->task->o.t = node->parent->task->o.t + node->parent->task->T;
        node->task->o.y = find_max_y(node, blist);
        node->task->o.x = find_max_x(node);
    }
    else if (node == node->parent->child[1]) {
        // T== and Y + direction
        node->task->o.t = node->parent->task->o.t;
        node->task->o.y = find_max_y(node, blist);
        node->task->o.x = find_max_x(node);
    }
    else if (node == node->parent->child[2]) {
        // T== Y== and X + direction
        node->task->o = node->parent->task->o;
        node->task->o.x = find_max_x(node);
    }
    placed.push_back(node);
}

double Ttree::find_max_x(TtreeNode* node) {
    double maxx = 0;
    double y0l = node->task->o.y, y0r = y0l + node->task->Y;
    double t0l = node->task->o.t, t0r = t0l + node->task->T;
    for (auto it = placed.begin(); it != placed.end(); ++it) {
        double y1l = (*it)->task->o.y, y1r = y1l + (*it)->task->Y;
        double t1l = (*it)->task->o.t, t1r = t1l + (*it)->task->T;
        if ( (y1r > y0l && y1l < y0r) && (t1r > t0l && t1l < t0r) ) {
            double xb = (*it)->task->o.x + (*it)->task->X;
            if (xb > maxx)
                maxx = xb;
        }
    }
    return maxx;
}

double Ttree::find_max_y(TtreeNode* node, list<TtreeNode*>& blist) {
    double maxy = 0;
    double t0l = node->task->o.t, t0r = t0l + node->task->T;
    for (auto it = placed.begin(); it != placed.end(); ++it) {
        double t1l = (*it)->task->o.t, t1r = t1l + (*it)->task->T;
        if (t1r > t0l && t1l < t0r) {
            double yb = (*it)->task->o.y + (*it)->task->Y;
            if (yb > maxy)
                maxy = yb;
        }
    }
    return maxy;
}