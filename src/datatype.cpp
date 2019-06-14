#include "datatype.h"

void CPP::read_graph(string filename)
{
    fstream file;
    string name;
    file.open(filename.c_str(),ios::in);

    if(file.fail())
    {
        cout << "Can't open file" << endl;
        exit(0);
    }
    else
    {
        name = name.assign(filename,12,filename.size()-1);
        cout << "File " << name << " read start!" << endl;
        string line;
        while(getline(file,line))
        {
            // ignore capacity,flow value
            size_t i=0;
            string a="",b="";

            if(line.size()==0) break;
            while(line[i]==' ') i++;
            while(line[i]!=' ') { a+=line[i]; i++; }
            while(line[i]==' ') i++;
            for(i;i<line.size();++i) {b+=line[i]; if(line[i+1]==' ') break;}
            //cout << a << " - " << b << "123" << endl;

            // Network_Manager
            if(graph.get_node(a)==NULL)
                graph.add_switch(a);
            if(graph.get_node(b)==NULL)
                graph.add_switch(b);
            graph.connect_r(a,b);

            // Our construction
            if(vertex[a]==NULL)
            {
                VERTEX *tv = new VERTEX;
                tv->name = a;
                vertex[a] = tv;
                v_list.push_back(vertex[a]);
            }
            if(vertex[b]==NULL)
            {
                VERTEX *tv = new VERTEX;
                tv->name = b;
                vertex[b] = tv;
                v_list.push_back(vertex[b]);
            }

            EDGE *te = new EDGE;
            te->a = vertex[a];
            te->b = vertex[b];
            e_list.push_back(te);

            vertex[a]->edge_list.push_back(te);
            vertex[b]->edge_list.push_back(te);
        }
        cout << endl;
    }
    file.close();

    Vertex *node = graph.get_all_nodes();
    int i=0;
    while(node!=NULL)
    {
        v_index[node->name]=i;
        //cout << node->name << " : " << i << endl;
        node = node->next; i++;
    }

    //graph.print_all_v();
    //graph.print_all_e();
/*
    for(size_t i=0;i<e_list.size();++i)
    {
        EDGE *te = e_list[i];
        cout << te->a->name << " " << te->b->name <<endl;
    }
*/
/*
    for(size_t i=0;i<v_list.size();++i)
    {
        VERTEX *tv = v_list[i];
        cout << tv->name << " " << tv->edge_list.size() << " ";
        for(size_t j=0;j<tv->edge_list.size();++j)
        {
            EDGE *te = tv->edge_list[j];
            cout << te->a->name << te->b->name << " ";
        }
        cout << endl;
    }
*/
}

bool CPP::check_connected()
{
    Path *path;
	path=new Path();
	path->append(graph.elist);
	vector<vector<Edge*> > vertex_path;

    bool connected = 1;
    map<string,VERTEX*>::iterator it1;
    map<string,VERTEX*>::iterator it2;
    for(it1=vertex.begin();it1!=vertex.end();++it1)
    {
        for(it2=it1;it2!=vertex.end();++it2)
        {
            //cout << it1->first << "->" << it2->first << endl;
            if(it1==it2)
                continue;
            else
            {
                vertex_path = path->find_paths(it1->first,it2->first);
                if(vertex_path.size()==0)
                {
                    connected=0;
                    return connected;
                }
            }
        }
    }
    return connected;
}

bool CPP::check_Euler()
{
    bool Euler_cycle=0;
    for(size_t i=0 ; i<v_list.size() ; ++i)
    {
        VERTEX *tv = v_list[i];
        if( (tv->edge_list.size()%2) == 1 )
            odd_v.push_back(tv);
    }
    optw_vlist.resize(odd_v.size());
    if( odd_v.size()==0 )
        Euler_cycle=1;
    else
        Euler_cycle=0;
    //cout << "Euler_cycle : " << Euler_cycle << endl;
/*
    for(size_t i=0;i<odd_v.size();++i)
        cout << odd_v[i]->name << " ";
    cout << endl;
*/
    return Euler_cycle;
}

void CPP::Find_shortest_path()
{
    Path *path;
	path=new Path();
	path->append(graph.elist);

    vector<vector<Edge*> > possible_path;
    map<string,VERTEX*>::iterator it1;
    map<string,VERTEX*>::iterator it2;
    for(it1=vertex.begin();it1!=vertex.end();++it1)
    {
        for(it2=vertex.begin();it2!=vertex.end();++it2)
        {
            if(it1==it2)
            {
                vector<Edge*> emp;
                shortest_path.push_back(emp);
            }
            else
            {
                possible_path = path->find_paths(it1->first,it2->first);
                int minp_index=0;
                for(size_t i=1;i<possible_path.size();++i)
                {
                    if(possible_path[i].size() < possible_path[minp_index].size())
                        minp_index = i;
                }
                shortest_path.push_back(possible_path[minp_index]);
/*
                cout << it1->first << "->" << it2->first << " : ";
                for(size_t j=0;j<possible_path[minp_index].size();++j)
                {
                    cout << possible_path[minp_index][j]->head->name << "->" << possible_path[minp_index][j]->tail->name << " ";
                }
                cout << endl;
*/
            }
        }
    }

    shortest_distance.resize(v_index.size());
    for(size_t i=0;i<shortest_distance.size();++i)
        shortest_distance[i].resize(v_index.size());

    for(size_t i=0;i<shortest_distance.size();++i)
    {
        for(size_t j=0;j<shortest_distance.size();++j)
        {
            shortest_distance[i][j] = shortest_path[5*i+j].size();
            //cout << shortest_distance[i][j] << " ";
        }
        //cout << endl;
    }
}

void CPP::cal_minw(int ov_index,int w)
{
    VERTEX *v1 = odd_v[ov_index];
    VERTEX *v2 = odd_v[ov_index+1];

    if(ov_index==odd_v.size()-2)
    {
        int weight = shortest_distance[v_index[v1->name]][v_index[v2->name]];
        if( w+weight <  mincost_addedge)
        {
            mincost_addedge = w+weight;
            for(size_t j=0;j<odd_v.size();++j)
                optw_vlist[j] = odd_v[j];
        }
    }

    for(size_t i=ov_index+1;i<odd_v.size();++i)
    {
        VERTEX *vi = odd_v[i];
        VERTEX *tv;
        int weight = shortest_distance[v_index[v1->name]][v_index[vi->name]];
        tv = odd_v[ov_index+1];
        odd_v[ov_index+1] = odd_v[i];
        odd_v[i] = tv;

        cal_minw(ov_index+2,w+weight);

        tv = odd_v[ov_index+1];
        odd_v[ov_index+1] = odd_v[i];
        odd_v[i] = tv;
    }
}

void CPP::create_Euler()
{
    mincost_addedge = INT_MAX;
    cal_minw(0,0);

    for(size_t i=0;i<optw_vlist.size();++i)
    {
        //cout << optw_vlist[i]->name << "->" << optw_vlist[i+1]->name << " : ";
        int s = v_index[optw_vlist[i]->name];
        int t = v_index[optw_vlist[i+1]->name];
        for(size_t j=0;j<shortest_path[5*s+t].size();++j)
        {
            EDGE *te = new EDGE;
            te->a = vertex[shortest_path[5*s+t][j]->head->name];
            te->b = vertex[shortest_path[5*s+t][j]->tail->name];
            //cout << te->a->name << "->" << te->b->name << " ";
            te->a->edge_list.push_back(te);
            te->b->edge_list.push_back(te);
            e_list.push_back(te);
        }
        i++;
        //cout << endl;
    }
    //cout << endl;
    //for(size_t i=0;i<e_list.size();++i)
    //    cout << e_list[i]->a->name << "-" << e_list[i]->b->name << endl;
}

void CPP::Euler_path()
{
    // use Hierholzer's Algorithm
    VERTEX *start;
    stack<VERTEX*> tpath;

    if(!v_list.empty())
        tpath.push(v_list.front());

    while(!tpath.empty())
    {
        start = tpath.top();

        if(start->through_degree==start->edge_list.size())
        {
            tpath.pop();
            final_path.push_back(start);
            continue;
        }

        for( size_t i=0 ; i<start->edge_list.size() ; ++i )
        {
            EDGE *te = start->edge_list[i];
            //cout << start->name << endl;
            //cout << "a : " << te->a->name << " " << te->a->through_degree << endl;
            //cout << "b : " << te->b->name << " " << te->b->through_degree << endl;
            //cout << te->a->name << " - " << te->b->name << endl;
            //getchar();
            if(te->through==0)
            {
                te->through = 1;
                te->a->through_degree +=1;
                te->b->through_degree +=1;
                if( te->a == start )
                    tpath.push(te->b);
                else
                    tpath.push(te->a);
                break;
            }
        }
    }
}

void CPP::show_path()
{
    cout << "The path is : ";
    for(size_t i=0;i<final_path.size();++i)
    {
        if(i==final_path.size()-1)
        {
            cout << final_path[i]->name << endl;
            break;
        }
        cout << final_path[i]->name << "->";
    }
    cout << "The distance is : " << final_path.size()-1 << endl;
}

void CPP::dump_solution(string filename)
{
    string out_name;
    out_name = out_name.assign(filename,12,filename.size()-1);
    out_name = out_name.assign(out_name,0,out_name.size()-4);

    fstream file;
    file.open(("./Output/"+out_name+"_solution.txt").c_str(),ios::out);

    out_name = out_name.assign(filename,12,filename.size()-1);
    file << "Graph file : " << out_name << endl;
    if(check_connected()==0)
    {
        file << "Connected : No" << endl;
        file << "Euler cycle : False" << endl;
        file << "Path : No solution" << endl;
        file << "Distance : X" << endl;
    }
    else
    {
        file << "Connected : Yes" << endl;
        if(check_Euler())
            file << "Euler cycle : True" << endl;
        else
            file << "Euler cycle : False" << endl;
        file << "Path : ";
        for(size_t i=0;i<final_path.size();++i)
        {
            if(i==final_path.size()-1)
            {
                file << final_path[i]->name << endl;
                break;
            }
            file << final_path[i]->name << "->";
        }
        file << "Distance : " << final_path.size()-1 << endl;
    }
}
