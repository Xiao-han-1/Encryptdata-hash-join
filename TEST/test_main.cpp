#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <random>
#include <iostream>

using namespace std;
// std::string base64_encode(const std::string& input) {
//     BIO *bio, *b64;
//     BUF_MEM *bufferPtr;

//     bio = BIO_new(BIO_s_mem());
//     bio = BIO_push(b64, bio);

//     BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
//     BIO_write(bio, input.c_str(), input.size());
//     BIO_flush(bio);

//     BIO_get_mem_ptr(bio, &bufferPtr);
//     std::string output(bufferPtr->data, bufferPtr->length);

//     BIO_free_all(bio);

//     return output;
// }

// std::string base64_decode(const std::string& input) {
//     BIO *bio, *b64;
//     char buffer[input.size()];
//     memset(buffer, 0, input.size());

//     b64 = BIO_new(BIO_f_base64());
//     bio = BIO_new_mem_buf(input.c_str(), -1);
//     bio = BIO_push(b64, bio);

//     BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
//     int len = BIO_read(bio, buffer, input.size());

//     BIO_free_all(bio);

//     return std::string(buffer, len);
// }
string Encrypt_name(string name)
{
    string table_name = "table_";
    const int length = 6; // 表名长度
    const string char_set = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // 表名字符集
    const int char_set_size = char_set.size();
    random_device rd; // 获取随机数生成器的种子
    mt19937 gen(rd()); // 使用Mersenne Twister算法作为随机数生成器
    uniform_int_distribution<> dis(0, char_set_size-1); // 均匀分布的整数随机数生成器
    for(int i=0; i<length; ++i) {
        table_name += char_set[dis(gen)]; // 从字符集中随机选择一个字符添加到表名中
    }
    return table_name;
}
int main() {
    
    string a="1";
    string b="2";
    cout<<Encrypt_name(a)<<endl;
    cout<<Encrypt_name(a)<<endl;
    cout<<Encrypt_name(b)<<endl;

    return 0;
}