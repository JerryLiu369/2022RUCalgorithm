#include "chaininit.h"
#include "rsa.h"
#include <stack>
using std::stack;
using std::stoi;
using std::to_string;
using std::hash;

int isscriptvalid(char *fullscript)
{
    string txt(fullscript);
    stack<string> validstack;
    set<string> operators{"OP_ADD", "OP_HASH160", "OP_EQUALVERIFY", "OP_DUP", "OP_CHECKSIG"};
    int ind = txt.find(" ");
    while (ind != -1)
    {
        string now = txt.substr(0, ind);
        if (operators.count(now))
        {
            if (now == "OP_ADD")
            {
                int a,b;
                a=stoi(validstack.top());
                validstack.pop();
                b=stoi(validstack.top());
                validstack.pop();
                validstack.push(to_string(a+b));
            }
            else if (now == "OP_HASH160")
            {
                hash<string> strhash;
                string res=to_string(strhash(validstack.top()));
                validstack.pop();
                validstack.push(res);
            }
            else if (now == "OP_EQUALVERIFY")
            {
                string a,b;
                a=validstack.top();
                validstack.pop();
                b=validstack.top();
                validstack.pop();
                if(a!=b)return 0;
            }
            else if (now == "OP_DUP")
            {
                validstack.push(validstack.top());
            }
            else if (now == "OP_CHECKSIG")
            {
                string sig,pubK;
                pubK=validstack.top();
                validstack.pop();
                sig=validstack.top();
                validstack.pop();
                Rsa rsa;
                rsa.init(64);
                rsa.setPu(stoi(pubK));
                BigInt ss(sig);
                if(verify(rsa,ss,pubK)==0)return 0;
            }
        }
        else
            validstack.push(now);
        txt = txt.substr(ind + 1, txt.length() - ind);
        ind = txt.find(" ");
    }
    return 1;
}

int isvalid(Inputptr ip, Blockptr chain)
{
    Inputptr ip_move = ip;
    while (true)
    {
        ip_move = ip_move->next;
        char fullscript[512];
        char *temp = ip_move->scriptSig;
        temp[strlen(temp) - 1] = '\0';
        strcpy(fullscript, temp);
        strcat(fullscript, " ");
        Transactionptr tp_pre = chain[ip_move->pre_block].transactions;
        Transactionptr tp_move = tp_pre;
        while (true)
        {
            tp_move = tp_move->next;
            if (strcmp(tp_move->txid, ip_move->prevTxID) == 0)
            {
                char *temp = o_find_out(tp_move->outputs, ip_move->prevTxOutIndex)->script;
                temp[strlen(temp) - 1] = ' ';
                strcat(fullscript, temp);
                int status = isscriptvalid(fullscript);
                if (status == 0)
                    return 0;
                break;
            }
        }
        if (ip_move == ip)
            break;
    }
    return 1;
}

int main()
{
    int n, a, b;
    Blockptr bp = chaininit(n, a, b);
    int k;
    char txid[512];
    printf("请输入要验证的区块高度：\n");
    scanf("%d", &k);
    printf("请输入要验证的交易txid：\n");
    scanf("%s", txid);
    Transactionptr tp = bp[k].transactions;
    Transactionptr tp_move = tp;
    int valid = 0;
    while (true)
    {
        tp_move = tp_move->next;
        if (strcmp(tp_move->txid, txid) == 0)
        {
            valid = isvalid(tp_move->inputs, bp);
            break;
        }
        if (tp_move == tp)
        {
            break;
        }
    }
    printf("此交易验证结果：\n%d\n", valid);
    return 0;
}
