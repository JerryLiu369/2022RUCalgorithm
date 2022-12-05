#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
using std::find;
using std::fstream;
using std::ios;
using std::stoi;
using std::string;
using std::vector;
using std::to_string;

typedef struct block
{
    int height;
    string hash;
    string prevHash;
    string merkleRoot;
    int nonce;
    vector<string> transactions;
    int transNum;
} block;

block str2blk(const string &input)
{
    block res;
    int pos = 0;
    res.height = stoi(input.substr(pos, input.find(",", pos + 1) - pos));
    pos = input.find(",", pos + 1);
    res.hash = input.substr(pos + 1, input.find(",", pos + 1) - pos);
    pos = input.find(",", pos + 1);
    res.prevHash = input.substr(pos + 1, input.find(",", pos + 1) - pos);
    pos = input.find(",", pos + 1);
    res.merkleRoot = input.substr(pos + 1, input.find(",", pos + 1) - pos);
    pos = input.find(",", pos + 1);
    res.nonce = stoi(input.substr(pos + 1, input.find(",", pos + 1) - pos));
    pos = input.find(",", pos + 1);
    res.transNum = stoi(input.substr(pos + 1, input.size() - 1 - pos));
    return res;
}

string blk2str(const block &input)
{
    return to_string(input.height) + "," + input.hash + "," + input.prevHash + "," + input.merkleRoot + "," + to_string(input.nonce) + "," + to_string(input.transNum);
}

int main()
{
    vector<string> transaction_pool;
    string cfolder = "cmsg2/";
    string bfolder = "bmsg2/";
    vector<block> blocks;
    while (1)
    {
        if (rand()%10==0)
        {
            block newblk;
            std::hash<string> strhash;
            newblk.height=0;
            newblk.hash=to_string(strhash(to_string(newblk.height)));
            newblk.prevHash=to_string(strhash(to_string(newblk.height)));
            newblk.transNum=transaction_pool.size();
            for (auto &&tran : transaction_pool)
            {
                newblk.transactions.push_back(tran);
            }
            transaction_pool.clear();
            blocks.push_back(newblk);
        }
        else
        {
            fstream root_b;
            root_b.open(bfolder + "root.txt", ios::in);
            string head;
            string tail;
            root_b >> head;
            root_b >> tail;
            root_b.close();

            if (!head.empty())
            {
                //区块消息队列不为空
                fstream head_b;
                head_b.open(bfolder + head, ios::in);
                if (head == tail)
                {
                    string head_new;
                    head_b >> head_new;
                    root_b.open(bfolder + "root.txt", ios::out);
                    root_b.close();
                }
                else
                {
                    string head_new;
                    head_b >> head_new;

                    root_b.open(bfolder + "root.txt", ios::out);
                    root_b << head_new << "\n";
                    root_b << tail;
                    root_b.close();
                }
                //处理消息
                string content;
                head_b >> content;
                block blk = str2blk(content);
                for (int i = 0; i < blk.transNum; i++)
                {
                    string trantemp;
                    head_b >> trantemp;
                    blk.transactions.push_back(trantemp);
                }
                int flag = 1;
                for (auto &&block : blocks)
                {
                    if (blk.prevHash == block.prevHash)
                    {
                        break;
                        flag = 0;
                    }
                }
                if (flag == 1)
                {
                    blocks.push_back(blk);
                    for (auto &&tran : blk.transactions)
                    {
                        auto pos=find(transaction_pool.begin(),transaction_pool.end(),tran);
                        if (pos != transaction_pool.end())
                            transaction_pool.erase(pos);
                    }
                }
            }
            else
            {
                fstream root_c;
                root_c.open(cfolder + "root.txt", ios::in);
                root_c >> head;
                root_c >> tail;
                root_c.close();
                if (!head.empty())
                {
                    //用户消息队列不为空
                    fstream head_c;
                    head_c.open(cfolder + head, ios::in);
                    string content;
                    if (head == tail)
                    {
                        string head_new;
                        head_c >> head_new;
                        root_c.open(cfolder + "root.txt", ios::out);
                        root_c.close();
                    }
                    else
                    {
                        string head_new;
                        head_c >> head_new;

                        root_c.open(cfolder + "root.txt", ios::out);
                        root_c << head_new << "\n";
                        root_c << tail;
                        root_c.close();
                    }
                    head_c >> content;
                    head_c.close();
                    remove((cfolder + head).c_str());
                    //处理消息
                    string msgtype(content.substr(0, content.find(",")));
                    if (!(msgtype == "height" || msgtype == "hash" || msgtype == "id"))
                    {
                        if (find(transaction_pool.begin(), transaction_pool.end(), content) == transaction_pool.end())
                            transaction_pool.push_back(content);
                    }
                    else
                    {
                        /* code */
                    }
                }
            }
        }
        Sleep(200);
    }
    return 0;
}
