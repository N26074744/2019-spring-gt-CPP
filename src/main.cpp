#include "datatype.h"

int main(int argc,char *argv[])
{
    CPP graph;

    graph.read_graph(argv[1]);
    if(graph.check_connected())
    {
        cout << "This gragh is connected." << endl;
        cout << "Euler cycle : ";
        if(!graph.check_Euler())
        {
            cout << "False." << endl;
            cout << "Start find shorest path..." << endl;
            graph.Find_shortest_path();
            cout << "Create repeated edges." << endl;
            graph.create_Euler();
        }
        else
            cout << "True" << endl;

        graph.Euler_path();
        graph.show_path();
    }
    else
        cout << "This graph isn't connected. There is no solution!" << endl;

    graph.dump_solution(argv[1]);

    return 0;
}
