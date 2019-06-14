#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <limits.h>

#include "network_manager.h"
#include "path.h"

using namespace std;

class VERTEX;
class EDGE;

class VERTEX
{
    public:
        string name;
        int through_degree;
        vector<EDGE*> edge_list;
        VERTEX()
        {
            name = "";
            through_degree = 0;
            edge_list.clear();
        }
};

class EDGE
{
    public:
        VERTEX *a,*b;
        bool through;
        EDGE()
        {
            a = NULL;
            b = NULL;
            through = 0;
        }
};

class CPP
{
    public:
        void read_graph(string filename);
        bool check_connected();
        bool check_Euler();
        void Find_shortest_path();
        void cal_minw(int ov_index,int w);
        void create_Euler();
        void Euler_path();
        void show_path();
        void dump_solution(string filename);

    private:
        NetworkManager graph;
        vector<VERTEX*> v_list;
        vector<EDGE*> e_list;
        map<string,int> v_index;
        map<string,VERTEX*> vertex;
        vector<VERTEX*> odd_v;
        int mincost_addedge;
        vector<VERTEX*> optw_vlist;
        vector<vector<Edge*> > shortest_path;
        vector<vector<int> > shortest_distance;
        vector<VERTEX*> final_path;
};

#endif
