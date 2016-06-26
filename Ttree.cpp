/*
* @Author: mengcz13
* @Date:   2016-06-08 16:14:03
* @Last Modified by:   mengcz13
* @Last Modified time: 2016-06-26 11:01:14
*/

#include "Ttree.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

#include <chrono>

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

double Ttree::pack() {
    stack<TtreeNode*> res;
    res.push(root);
    while (!res.empty()) {
        TtreeNode* s_top = res.top();
        res.pop();
        TtreeNode* btn = tree_decomposition(s_top, res);
        binary_tree_packing(btn);
    }
    // renew bx, by, bt
    bx = 0; by = 0; bt = 0;
    for (int i = 0; i < tasknum; ++i) {
        Task* task = pool[i].task;
        double tx = task->o.x + task->X;
        double ty = task->o.y + task->Y;
        double tt = task->o.t + task->T;
        if (tx > bx)
            bx = tx;
        if (ty > by)
            by = ty;
        if (tt > bt)
            bt = tt;
    }
    placed.clear();
    return (bx * by * bt);
}

Ttree& Ttree::operator = (const Ttree& ttree) {
    bx = ttree.bx;
    by = ttree.by;
    bt = ttree.bt;
    tasknum = ttree.tasknum;
    memcpy(pool, ttree.pool, tasknum * sizeof(TtreeNode));
    root = pool;
    return (*this);
}

void Ttree::transform_move(int rank1, int rank2, int childth) {
    int actual_rank1 = remove_node(rank1);
    while (rank2 == actual_rank1)
        rank2 = rand() % tasknum;
    insert_node(actual_rank1, rank2, childth);
}

void Ttree::transform_swap(int rank1, int rank2) {
    swap(pool[rank1].task, pool[rank2].task);
}

void Ttree::transform_rotate(int rank) {
    swap(pool[rank].task->X, pool[rank].task->Y);
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
    Contour blist;
    while (!temps.empty()) {
        TtreeNode* cn = temps.top();
        temps.pop();
        place_module(cn, blist);
        if (cn->child[1] != NULL)
            temps.push(cn->child[1]);
        if (cn->child[0] != NULL)
            temps.push(cn->child[0]);
    }
}

void Ttree::place_module(TtreeNode* node, Contour& blist) {
    if (node->parent == NULL) {
        node->task->o = Point3d(0, 0, 0);
        blist.insert(node);
    }
    else if (node == node->parent->child[0]) {
        // T+ direction
        node->task->o.t = node->parent->task->o.t + node->parent->task->T;
        node->task->o.y = blist.insert(node);
        node->task->o.x = find_max_x(node);
    }
    else if (node == node->parent->child[1]) {
        // T== and Y + direction
        node->task->o.t = node->parent->task->o.t;
        node->task->o.y = blist.insert(node);
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

int Ttree::remove_node(int rank) {
    TtreeNode* node = &pool[rank];
    while (!(node->child[0] == NULL && node->child[1] == NULL && node->child[2] == NULL)) {
        int nextnoder = rand() % 3;
        while (node->child[nextnoder] == NULL)
            nextnoder = rand() % 3;
        swap(node->task, node->child[nextnoder]->task);
        node = node->child[nextnoder];
    }
    TtreeNode* parent = node->parent;
    if (parent != NULL) {
        for (int i = 0; i < 3; ++i) {
            if (parent->child[i] == node)
                parent->child[i] = NULL;
        }
    }
    node->parent = NULL;
    return (node - pool);
}

void Ttree::insert_node(int rank_src, int rank_dst, int childth) {
    TtreeNode* target = &pool[rank_dst];
    TtreeNode* source = &pool[rank_src];
    TtreeNode* child = target->child[childth];
    target->child[childth] = source;
    source->parent = target;
    if (child != NULL) {
        source->child[childth] = child;
        child->parent = source;
    }
}

double Contour::insert(TtreeNode* node) {
    double nodest = node->task->o.t;
    double nodeet = nodest + node->task->T;
    if (cont.empty()) {
        cont.push_back(make_pair(nodest, node->task->Y));
        cont.push_back(make_pair(nodeet, 0));
        return 0;
    }
    
    auto bg = cont.begin();
    while (bg != cont.end() && bg->first <= nodest) {
        bg++;
    }
    bg--;
    double maxy = bg->second;;
    cont.insert(bg, make_pair(nodest, maxy + node->task->Y));
    return maxy;
}


Placer::Placer(string inputfilename, string outputfilename, double initt, double finalt, double alpha): initial_temp(initt), final_temp(finalt), alpha(alpha) {
    // read blocks and output result
    chrono::high_resolution_clock timer;
    
    ifstream inputfile(inputfilename.c_str());
    double x = 0, y = 0, t = 0;
    net_vol = 0;
    vector<Task> taskvec;
    while (inputfile >> x >> y >> t) {
        Task task;
        task.X = x;
        task.Y = y;
        task.T = t;
        net_vol += (x * y * t);
        taskvec.push_back(task);
    }
    inputfile.close();
    taskvec2 = new Task[taskvec.size()];
    for (int i = 0; i < taskvec.size(); ++i)
        taskvec2[i] = taskvec.at(i);
    tasknum = taskvec.size();
    current_ttree = new Ttree(taskvec2, taskvec.size());
    backup_ttree = new Ttree(taskvec2, taskvec.size());
    
    auto start = timer.now();
    sa_place();
    auto end = timer.now();
    chrono::duration<double> diff = end - start;

    check_result();

    ofstream outputfile(outputfilename.c_str());
    
    auto size = current_ttree->getsize();
    outputfile << size[0] << ' ' <<
    size[1] << ' ' << size[2] << "\n" << tasknum << "\n" << endl;
    
    for (int i = 0; i < taskvec.size(); ++i) {
        outputfile << taskvec2[i].X << ' ' 
        << taskvec2[i].Y << ' ' 
        << taskvec2[i].T << ' ' 
        << taskvec2[i].o.x << ' ' 
        << taskvec2[i].o.y << ' ' 
        << taskvec2[i].o.t << ' ' 
        << endl;
    }
    
    outputfile << "Net volume: " << net_vol << endl;
    outputfile << "Bounding box volume: " << total_volume << endl;
    outputfile << "Use rate: " << net_vol / total_volume * 100 << "%" << endl;
    outputfile << "Runtime: " << diff.count() << "s" << endl;
    outputfile.close();
}

Placer::~Placer() {
    delete current_ttree;
    delete backup_ttree;
    delete []taskvec2;
}


double Placer::gettimes(double temp) {
    return coef * pow(temp - initial_temp, 2) + 10;
}

void Placer::sa_place() {
    // simulated annealing place
    total_volume = current_ttree->pack();
    double temp = initial_temp;
    int blocknum = current_ttree->getnum();
    coef = 15/pow(initial_temp - final_temp, 2);
    int psize = current_ttree->getnum() * gettimes(temp);
    
    int count = 0;
    double rate = 0;
    while (temp > final_temp) {
        for (int changetime = 0; changetime < psize; ++changetime) {
            // Randomly choose one way to transform
            int way = rand() % 3;
            int rank1 = rand() % blocknum;
            int rank2 = rand() % blocknum;
            while (rank2 == rank1) {
                rank2 = rand() % blocknum;
            }
            int childth = rand() % 3;
            if (way == 0) {
                // move
                (*backup_ttree) = (*current_ttree);
                current_ttree->transform_move(rank1, rank2, childth);
            }
            else if (way == 1) {
                // swap
                current_ttree->transform_swap(rank1, rank2);
            }
            else {
                // rotate
                current_ttree->transform_rotate(rank1);
            }

            double newvol = current_ttree->pack();
            bool accept = false;
            if (newvol < total_volume) {
                accept = true;
            }
            else {
                double dv = newvol - total_volume;
                dv /= net_vol;
                double ap = exp(-dv/temp);
                double randp = (double)rand() / (double)RAND_MAX;
                if (randp < ap)
                    accept = true;
                else
                    accept = false;
            }
            if (accept) {
                total_volume = newvol;
            }
            else {
                // Restore if necessary
                if (way == 0) {
                    (*current_ttree) = (*backup_ttree);
                }
                else if (way == 1) {
                    current_ttree->transform_swap(rank1, rank2);
                }
                else {
                    current_ttree->transform_rotate(rank1);
                }
            }
        }
        temp *= alpha;
        if (rate == net_vol / total_volume) {
            count++;
        } else {
            count = 0;
        }
        rate = net_vol / total_volume;
        cout << rate << " " << temp << endl;
        if (count > 0 && count % 100 == 0) {
            temp /= pow(alpha, 50);
        }
        if (count >= 1000) {
            break;
        }
    }
    total_volume = current_ttree->pack();
}

void Placer::check_result() {
    for (int i = 0; i < tasknum; ++i) {
        for (int j = i + 1; j < tasknum; ++j) {
            Task& t1 = taskvec2[i];
            Task& t2 = taskvec2[j];
            if (t1.o.x < t2.o.x + t2.X && t1.o.x + t1.X > t2.o.x && t1.o.y < t2.o.y + t2.Y && t1.o.y + t1.Y > t2.o.y && t1.o.t < t2.o.t + t2.T && t1.o.t + t1.T > t2.o.t) {
                cout << "Check fali at task " << i << " and task " << j << " !" << endl;
                return;
            }
        }
    }
    cout << "Check passed!" << endl;
}