#include<iostream>
#include<algorithm>
#include<climits>
#include<string>
#include<vector>
#include<unordered_map>
#include<math.h>
#include"child_table.hh"
#include "MurmurHash3.h"

using namespace std;

child_table::child_table(void){}
int child_table::ElbowMethod(vector<int> data, int n)
{ // 计算每个k值的误差平方和 
  double SSE[n]; 
//  vector<double>SSE;
 for (int k = 1; k <= n; k++)
    {
    double aver = 0; 
    for (int i = 0; i < k; i++) 
    { 
        aver += data[i];
    }
    aver /= k; 
    double sse = 0; 
    for (int i = 0; i < k; i++) 
    { 
    sse += (data[i] - aver) * (data[i] - aver); 
    } 
    SSE[k-1] = sse; 
    } // 计算每个k值的平均误差
     
    double aver_error[n];
    for (int k = 1; k <= n; k++)
    { 
        aver_error[k-1] = SSE[k-1] / k; 
    } // 计算k值 
    double min_error = aver_error[0]; 
    int k = 1; for (int i = 1; i < n; i++)
    { 
        if (aver_error[i] < min_error)
        { 
        min_error = aver_error[i]; 
        k = i + 1; 
        } 
    }
    return k;
}
void child_table::K_means(vector<Ls> &data,int &k)
{
// sort(data.begin(),data.end(),[](const Ls &l1,const Ls  &l2){ return l1.num < l2.num; });
int maxn=-1,minn=INT_MAX;
for(Ls &l : data)
{
    maxn=max(l.num,maxn);
    minn=min(l.num,minn);
}
         //初始化聚类中心  
vector<int> center; 
center.push_back(minn);
center.push_back(maxn);
cout<<"maxn"<<":"<<maxn<<endl;
cout<<"minn"<<":"<<minn<<endl;
int range=maxn-minn+1;
k=min(range,k);
if(k>2)
{
int centerlocal=minn;
int div_c=range/(k-2);
for(int i=1;i<=k;i++) 
{ 

 centerlocal=centerlocal+div_c;
 center.push_back(centerlocal); //计算每个数据点到聚类中心的距离，划分到距离最近的聚类中心
}   
}
          
for(Ls &l : data)
{ 
  int minDistance = INT_MAX; 
  int label = 0; 
  for(int i=0;i<k;i++)
   {  
    int distance = abs(l.num - center[i]); 
    if(distance < minDistance)
    { 
        minDistance = distance; 
        label = i; 
    } 
    } 
    l.label=label;

}//将每一个value频率计算到达每一个centor的距离，放在距离最近的盒中
}
void child_table::get_frequency(vector<string>table,unordered_map<string,int> &fre_num,unordered_map<string,vector<int>> &local_num)
{
    // int maxn=0,maxn_i=0;
    for(int i=0;i<table.size();i++)
    {
        string tmp=table[i];
        fre_num[tmp]++;
        vector<int> t_ve;
        t_ve=local_num[tmp];
        t_ve.push_back(i);
        local_num[tmp]=t_ve;
    }

}
void child_table::Full_K_table(vector<Ls> &K_table,unordered_map<string,int> fre_num,unordered_map<string,vector<int>> local_num)
{
   for (auto const& pair: fre_num) 
   {
    auto k=pair.first;
    auto v=pair.second;
     Ls tmp;
     tmp.name=k;
     tmp.num=v;
     tmp.lo_num=local_num[k];
     K_table.push_back(tmp);
  }
  //将数据统计信息存储在一个表中，name为k值，v为频率，lo_num为定位数组
}
vector<vector<int>> child_table::trans_row_to_col(vector<vector<int>> vue)
{
    vector<vector<int>>re;
    int len=vue.size();
    vector<int>tmp;
    if(len>0) tmp=vue[0];   
    // else return NULL;
    for(int j=0;j<tmp.size();j++)
    {
        vector<int>te;
        for(int i=0;i<len;i++)
        {
          te.push_back(vue[i][j]);
        }
        re.push_back(te);
    }
    return re;
}
uint32_t int_murmur_hash_function(int key, uint32_t num_buckets) {
    uint32_t hash[2];
    MurmurHash3_x86_32(&key, sizeof(int), 0, &hash);
    return hash[0] % num_buckets;
}
uint32_t str_murmur_hash_function(const std::string& key, uint32_t num_buckets) {
    uint32_t hash[2];
    MurmurHash3_x86_32((void*)key.c_str(), key.size(), 0, &hash);
    return hash[0] % num_buckets;
}
void child_table::Classification(vector<string> Columns,int &k,vector<int>& Mark_Col)
{

    for(int i=0;i<Columns.size();i++)
    {
        int val=stoi(Columns[i]);
        Mark_Col[i]=int_murmur_hash_function(val,k);
        // Mark_Col[i]=0;
    }

}
vector<Table*> child_table::Devide_table(vector<Ls> K_table,Table* table,int k)
{
    // unordered_map<int,vector<string>>local_row;
    vector<Table*> child_table;
    vector<vector<string>>vue=table->value;
    //  trans_col_to_row(vue,local_row);//将原本按列存的数据转为按行存
    int len=K_table.size();
    Ls tmp=K_table[len-1];
    int n=k;
    for(int i=0;i<n;i++)
    {
        Table* t=new Table();
        t->max_frequency=0;
        t->name=table->name;
        t->table_name=table->table_name;
        t->type=table->type;
        t->Join_col_id=table->Join_col_id;
        t->value.clear();
        child_table.push_back(t);
        
    }
    for(auto &l:K_table)
    {
        int i=l.label;
        vector<int>tmp=l.lo_num;          
        for(auto &id:tmp)
        {
            vector<string> row=table->value[id];
            child_table[i]->value.push_back(table->value[id]);
        }
        child_table[i]->max_frequency=max(l.num,child_table[i]->max_frequency);
    }
    // Table* t=new Table();
    // for(int i=0;i<n;i++)
    // {
    //     int Frequency_max=0;
    //     t->name=table->name;
    //     t->table_name=table->table_name;
    //     t->type=table->type;
    //     t->Join_col_id=table->Join_col_id;
    //     t->value.clear();
    //     // vector<vector<string>> ve;
    //     for(auto &l:K_table)
    //     {
    //       if(l.label==i)
    //       {
    //         vector<int>tmp=l.lo_num;
    //         int tmp_len=tmp.size();
    //         Frequency_max=max(Frequency_max,tmp_len);
    //         for(auto &id:tmp)
    //         {
    //             vector<string> row=table->value[id];
    //             t->value.push_back(table->value[id]);
    //         }
    //       }
    //       if(l.label>i) break;
    //     }
    //     // ve=trans_row_to_col(ve);
    //     // t->value=ve;
    //     t->max_frequency=Frequency_max;
    //     if(t->value.size()!=0)
    //     child_table.push_back(t);
    // }
    // delete t;
    return child_table;
}
vector<Table*> child_table::Devide_table_v1(vector<int> Mark_Col,Table* table,vector<string>Columns,int k,unordered_map<string,int> fre_num)
{
    vector<Table*> child_table(k);
    vector<vector<string>>vue=table->value;
    for(int i=0;i<k;i++)
    {
        Table* t=new Table();
        t->max_frequency=0;
        t->name=table->name;
        t->table_name=table->table_name;
        t->type=table->type;
        t->Join_col_id=table->Join_col_id;
        child_table.push_back(t);
    }

    for (int i=0;i<Mark_Col.size();i++) {
        int flag=Mark_Col[i];
        child_table[flag]->value.push_back(table->value[i]);
        child_table[flag]->max_frequency=max(fre_num[Columns[i]],child_table[flag]->max_frequency);
    }
    return child_table;
  

}

vector<Table*> child_table::Table_divide(vector<string>Columns,Table* table)
{

    vector<int> Mark_Col(Columns.size());
    
    unordered_map<string,int> fre_num;
    unordered_map<string,vector<int>> local_num;
    get_frequency(Columns,fre_num,local_num);
    int k=40;
    Classification(Columns,k,Mark_Col);
    //unordered_map<string,vector<string>>name_map;quit
    vector<Table*>child_table=Devide_table_v1(Mark_Col,table,Columns,k,fre_num);
    return child_table;
}