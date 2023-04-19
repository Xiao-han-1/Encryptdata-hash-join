#include <iostream>
#include <vector>
#include <string>
#include <regex.h>

using namespace std;

int main() {
    string str = "lineitem.l_suppkey = nation.n_nationkey";
    regex_t regex;
    regmatch_t matches[3];
    vector<pair<string, string> > tab;

    if (regcomp(&regex, "([a-zA-Z0-9_]+)\\.([a-zA-Z0-9_]+)", REG_EXTENDED) != 0) {
        cerr << "Failed to compile regex" << endl;
        return 1;
    }

    int pos = 0;
    while (regexec(&regex, str.c_str() + pos, 3, matches, 0) == 0) {
        string token1 = str.substr(pos + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
        string token2 = str.substr(pos + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
        tab.push_back(make_pair(token1, token2));
        pos += matches[0].rm_eo;
    }

    regfree(&regex);

    for (vector < pair<string, string> >::iterator it = tab.begin(); it != tab.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }


}