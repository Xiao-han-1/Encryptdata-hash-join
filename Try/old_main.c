#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include "Hash_Encrypt.hh"
#define LEN1  5
#define LEN2  5
using namespace std;
 typedef struct row1
 {
    bool flag;
    int score;
 }row1;
  typedef struct row2
 {
    bool flag;
    int grade;
 }row2;
 void Add_dummy_block_table1( vector<row1>* table,int row1_min,int row1_max)
 {
    int rand_two=rand()%(LEN1/2);
    int len=row1_max-row1_min;
    for(int i=0;i<rand_two;i++)
    {
        row1 dummy_block;
        dummy_block.flag=0;
        dummy_block.score=rand()%len+row1_min;
        table->push_back(dummy_block);
    }
 }
  void Add_dummy_block_table2( vector<row2>* table,int row_min,int row_max)
 {
    int rand_two=rand()%(LEN2/2);
    int len=row_max-row_min;
    for(int i=0;i<rand_two;i++)
    {
        row2 dummy_block;
        dummy_block.flag=0;
        dummy_block.grade=rand()%len+row_min;
        table->push_back(dummy_block);
    }
 }
int main()
{
    vector<row1>table1;
    vector<row2>table2;
    vector<vector<string>>E_table1,E_table2;
    // read_data()
    srand((unsigned)time(0));
    int score[LEN1]={70,90,80,60,60};
    int grade[LEN2]={60,85,80,95,100};
    int row1_min=60,row1_max=90;
    int row2_min=60,row2_max=100;
    for(int i=0;i<LEN1;i++)
    {
        row1 real_block;
        real_block.flag=1;
        real_block.score=score[i];
        table1.push_back(real_block);
        int rand_one=rand()%2;
        if(rand_one==1)
        Add_dummy_block_table1(&table1,row1_min,row1_max);
        else continue;

    }
    for(int i=0;i<LEN2;i++)
    {
        row1 real_block;
        real_block.flag=1;
        real_block.score=score[i];
        table1.push_back(real_block);
        int rand_one=rand()%2;
        if(rand_one==1)
        Add_dummy_block_table2(&table2,row2_min,row2_max);
        else continue;
    }
    vector<vector<string>>t1,t2;
    cout<<table1.size()<<"\n";
    for(int i=0;i<table1.size();i++)
    {
        vector<string>tmp;//扩展表所有数据转换为字符串
        tmp.push_back(std::to_string(table1[i].flag));
        tmp.push_back(std::to_string(table1[i].score));
        t1.push_back(tmp);
        std::cout<<table1[i].flag<<" "<<table1[i].score<<"\n";
    }
    cout<<table2.size()<<"\n";
    for(int i=0;i<table2.size();i++)
    {
        vector<string>tmp;//扩展表所有数据转换为字符串
        tmp.push_back(std::to_string(table2[i].flag));
        tmp.push_back(std::to_string(table2[i].grade));
        t2.push_back(tmp);
        std::cout<<table2[i].flag<<" "<<table2[i].grade<<"\n";
    }
    E_table1=Encrypt_table(t1);
    vector<string> H_rows1=Hash_Enc(t1_rows);
    E_table2=Encrypt_table(t2);
    return 0;
}

// sort the map according to num 
std::map<std::string, StructFre_Num>::iterator it; 
std::vector vec; 
for (it = map.begin(); it != map.end(); ++it)
 { vec.push_back(it->second); } 
 std::sort(vec.begin(), vec.end(), [](StructFre_Num a, StructFre_Num b) 
 { return a.num > b.num; });

// update the map 
int i = 0; for (it = map.begin(); it != map.end(); ++it) { it->second = vec[i++]; }

//创建std::map和vector 
std::map<int, std::vector> myMap; std::vector vec;

//填充map和vector 
int num; for(int i = 0; i < StructFre_Num.size(); i++) 
{ num = StructFre_Num[i].Intnum; vec = StructFre_Num[i].Vectorid_list; myMap[num] = vec; }

//将map中的元素按照num从大到小排序
 std::multimap<int, std::vector> mySortedMap; 
 std::map<int, std::vector>::reverse_iterator iter; 
 for(iter = myMap.rbegin(); iter != myMap.rend(); iter++) 
 { mySortedMap.insert(std::pair<int, std::vector>(iter->first, iter->second)); }

 #include #include #include

using namespace std;

// Byte array to Hex string 
string ByteArrayToHexString(const unsigned char* data, int length) 
{ stringstream ss; ss << std::hex; 
for (int i = 0; i < length; i++) 
ss << std::setw(2) << std::setfill('0') << (int)data[i]; return ss.str();
 }

// Hex string to Byte array 
unsigned char* HexStringToByteArray(const string& hexString) 
{ int length = (int)hexString.length(); 
unsigned char* data = new unsigned char[length / 2];

for (int i = 0; i < length; i += 2) {
    string byteString = hexString.substr(i, 2);
    unsigned char byte = (unsigned char)strtol(byteString.c_str(), NULL, 16);
    data[i / 2] = byte;
}
return data;
}

int main() { 
unsigned char data[3] = { 0x01, 0x02, 0x03 }; 
string hexString = ByteArrayToHexString(data, 3);

cout << "Hex String is : " << hexString << endl;

unsigned char *pData = HexStringToByteArray(hexString);
cout << "Byte Array is : ";
for (int i = 0; i < 3; i++)
    cout << (int)pData[i] << " ";
cout << endl;

return 0;
}