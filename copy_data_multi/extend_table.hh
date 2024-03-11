#include <iostream>
#include <algorithm>
#include <climits>
#include <string>
#include <vector>
#include <unordered_map>
#include <math.h>
#include "child_table.hh"
#include <time.h>
#include <stdlib.h>
using namespace std;

class extend_table
{
    public:
    extend_table();
    vector<Table*>  Smooth_Frequency(vector<Table*> child_table,int rid,string scale,int &sum,double &total_size_cipher);
    void Table_extend(Table* table,int &rid);
    void Add_dummy_row(Table* table,string k,int v,int& rid);

};