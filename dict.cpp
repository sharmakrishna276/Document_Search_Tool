#include "dict.h"

class LLNode{
public:

    string word;
    int count;

    LLNode() {
        word="";
        count=0;
    }
};

void lowercase(string &word) {
    int len = word.size();
    for (int i=0;i<len;i++) {
        if (word[i]>='A' && word[i]<='Z') {
            word[i]+=32;
        }
    }
}

int hashvalue(string word) {
    long long ans=0;
    int len = word.size();
    long long power = 1;
    for (int i=0;i<len;i++) {
        ans+=(abs(word[i]-'\0'))*power;
        ans%=9973;
        if (power<1e16) {
            power*=10;
        }
        else {
            power%=9973;
            power*=10;
        }
    }
    ans = abs(ans);
    ans%=9973;
    return ans;
}

Dict::Dict(){
    for (int i=0;i<9973;i++) {
        vector<LLNode*> v;
        hashtable.push_back(v);
    }
}

Dict::~Dict(){
    for (int i=0;i<9973;i++) {
        int p = hashtable[i].size();
        for (int j=0;j<p;j++) {
            if (!hashtable[i][j]) {
                delete hashtable[i][j];
            }
        }
    }
}

void split_sentence(vector<string> &words,string sentence) {
    string word="";
    int len = sentence.size();
    for (int i=0;i<len;i++) {
        char x = sentence[i];
        if (x==' ' || x=='.' || x==',' || x=='-' || x==':' || x=='!' || x=='"' || x=='\'' || x=='(' || x==')' || x=='?' || x=='[' || x==']' || x==';' || x=='@' || x<=31 || x==127) {
            int num = word.size();
            if (num>0) {
                words.push_back(word);
            }
            word="";
        }
        else {
            word+=x;
        }
    }
    int num = word.size();
    if (num>0) {
        words.push_back(word);
    }
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    vector<string> v; split_sentence(v,sentence);
    int p = v.size();
    for (int i=0;i<p;i++) {
        lowercase(v[i]);
        string lower_word = v[i];
        int idx = hashvalue(v[i]);
        int num = hashtable[idx].size();
        bool flag=true;
        for (int i=0;i<num;i++) {
            if (hashtable[idx][i]->word==lower_word) {
                hashtable[idx][i]->count+=1;
                flag=false;
            }
        }
        if (flag) {
            LLNode* node = new LLNode();
            node->count=1;
            node->word=lower_word;
            hashtable[idx].push_back(node);
        }
    }
    return;
}

int Dict::get_word_count(string word){
    lowercase(word);
    string lower_word = word;
    int idx=hashvalue(lower_word);
    int num = hashtable[idx].size();
    for (int i=0;i<num;i++) {
        if (hashtable[idx][i]->word==lower_word) {
            return hashtable[idx][i]->count;
        }
    }
    return 0;
}

void Dict::dump_dictionary(string filename){
    fstream my_output;
    my_output.open(filename, std::ios::out);
    for (int i=0;i<9973;i++) {
        int num = hashtable[i].size();
        for (int j=0;j<num;j++) {
            if (hashtable[i][j]) {
                my_output<<hashtable[i][j]->word<<", "<<hashtable[i][j]->count<<endl;
            }
        }
    }
    my_output.close();
    return;
}
