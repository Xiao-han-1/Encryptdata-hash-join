#include<iostream>
#include<algorithm>
#include<climits>
#include<string>
#include<vector>
#include<unordered_map>
#include<math.h>
#include"child_table.hh"

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
void child_table::K_means(vector<Ls> &data,int k)
{
sort(data.begin(),data.end(),[](const Ls &l1,const Ls  &l2){ return l1.num < l2.num; });
         //初始化聚类中心 
vector<int> center; 
int divisor = data.size() / k; 
for(int i=1;i<=k;i++) 
{ center.push_back(data[divisor * i - 1].num); } 
         //计算每个数据点到聚类中心的距离，划分到距离最近的聚类中心
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
//  cout << "name= "<< l.name << " " ;
//  cout << "num=" << l.num << " ";
//  cout << "label=" <<l.label << endl; 
}
}
void child_table::get_frequency(vector<string>table,unordered_map<string,int> &fre_num,unordered_map<string,vector<int>> &local_num)
{
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
}
// void child_table::trans_col_to_row(vector<vector<string>>vue,unordered_map<int,vector<string>>&local_row)
// {
//     int len=vue.size();
//     vector<string>tmp;
//     if(len>0) tmp=vue[0];   
//     else return ;
//     for(int j=0;j<tmp.size();j++)
//     {
//         vector<int>te;
//         for(int i=0;i<len;i++)
//         {
//           te.push_back(vue[i][j]);
//         }
//         local_row[j]=te;
//     }
// }
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
vector<Table*> child_table::Devide_table(vector<Ls> K_table,Table* table)
{
    // unordered_map<int,vector<string>>local_row;
    vector<Table*> child_table;
    vector<vector<string>>vue=table->value;
    //  trans_col_to_row(vue,local_row);//将原本按列存的数据转为按行存
    int len=K_table.size();
    Ls tmp=K_table[len-1];
    int n=tmp.label+1;
    Table* t=new Table();
    for(int i=0;i<n;i++)
    {
        int Frequency_max=0;
        t->name=table->name;
        t->table_name=table->table_name;
        t->type=table->type;
        t->Join_col_id=table->Join_col_id;
        t->value.clear();
        // vector<vector<string>> ve;
        for(auto &l:K_table)
        {
          if(l.label==i)
          {
            vector<int>tmp=l.lo_num;
            int tmp_len=tmp.size();
            Frequency_max=max(Frequency_max,tmp_len);
            for(auto &id:tmp)
            {
                vector<string> row=table->value[id];
                t->value.push_back(table->value[id]);
            }
          }
          if(l.label>i) break;
        }
        // ve=trans_row_to_col(ve);
        // t->value=ve;
        t->max_frequency=Frequency_max;
        if(t->value.size()!=0)
        child_table.push_back(t);
    }
    delete t;
    return child_table;
}

vector<Table*> child_table::Table_divide(vector<string>Columns,Table* table)
{
    // vector<int>Columns={2,2,3,3,3,1,6,6,6,6,6};
    // Table table;
    // vector<string>name;
    // vector<string>type;
    // vector<vector<int>>value;
    // table.table_name="stu";
    // name.push_back("score");
    // type.push_back("int");
    // value.push_back(Columns);
    // table.name=name;
    // table.type=type;
    // table.value=value;
    // table.Join_col=Columns;
    unordered_map<string,int> fre_num;
    unordered_map<string,vector<int>> local_num;
    vector<Ls> K_table;
    get_frequency(Columns,fre_num,local_num);
    Full_K_table(K_table,fre_num,local_num);
    // int k=ElbowMethod(table,table.size());
    // cout<<k<<endl;
    // if(table->table_name=="orders")
    // K_means(K_table,30);
    // else 
    K_means(K_table,20);
    //unordered_map<string,vector<string>>name_map;
    vector<Table*>child_table=Devide_table(K_table,table);
    return child_table;
}