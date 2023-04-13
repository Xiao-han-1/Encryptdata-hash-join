#ifndef CHILD_TABLE_HH
#define CHILD_TABLE_HH
#include<iostream>
#include<algorithm>
#include<climits>
#include<string>
#include<vector>
#include<unordered_map>
#include<math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

typedef struct table
{
    int max_frequency;
    int Join_col_id;
    string table_name;
    vector<string>name;
    vector<string>type;
    unordered_map<int,int>row_flag;
    vector<vector<string>>value;
    ~table()
    {
        max_frequency=0; 
        Join_col_id=0; 
        table_name.clear();
        name.clear();
        type.clear();
        value.clear();
        row_flag.clear();
    }
}Table;
// typedef unsigned char byte;
typedef struct Enc_table
{
    int max_frequency;
    int Join_col_id;
    string aes_table_name;
	string hash_table_name;
    vector<string>name;
    vector<string>type;
    unordered_map<int,int>row_flag;
    vector<vector<string>>value;
	~Enc_table()
    {
        max_frequency=0; 
        Join_col_id=0; 
        aes_table_name="";
		hash_table_name="";
        name.clear();
        type.clear();
        value.clear();
        row_flag.clear();
    }
} Enc_Table;
typedef struct ls
{
int label;
string name;
vector<int> lo_num;
int num;
~ls()
    {
        label=0;
        name="";
        num=0;
        lo_num.clear();
    }
}Ls;

class child_table
{
public:
    child_table();
    int ElbowMethod(vector<int> data, int n);
    void K_means(vector<Ls> &data,int k);
    void get_frequency(vector<string>table,unordered_map<string,int> &fre_num,unordered_map<string,vector<int>> &local_num);
    void Full_K_table(vector<Ls> &K_table,unordered_map<string,int> fre_num,unordered_map<string,vector<int>> local_num);
    // void trans_col_to_row(vector<vector<int>>vue,unordered_map<int,vector<int>>&local_row);
    vector<vector<int>> trans_row_to_col(vector<vector<int>>vue);
    void Devide_table(vector<Ls> K_table,Table table,vector<Table> &child_table);
    vector<Table> Table_divide(vector<string>Columns,Table table);
};
#endif