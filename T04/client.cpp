#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <windows.h>
#include <time.h>
#include <direct.h>
using std::fstream;
using std::hash;
using std::ifstream;
using std::ios;
using std::list;
using std::ofstream;
using std::random_shuffle;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

void write_next(const string &content)
{
    string folder;
    if (rand() % 2 == 0)
    {
        folder = "cmsg1/";
    }
    else
    {
        folder = "cmsg2/";
    }

    // root处理
    fstream root_f;
    root_f.open(folder + "root.txt", ios::in);
    string head;
    string tail;
    root_f >> head;
    root_f.close();
    if (head.empty())
    {
        root_f.open(folder + "root.txt", ios::out);
        root_f << "1.txt\n1.txt\n";
        tail = "1.txt";
        root_f.close();
    }
    else
    {
        root_f.open(folder + "root.txt", ios::in);
        root_f >> head;
        root_f >> tail;
        root_f.close();

        fstream tail_f;
        tail_f.open(folder + tail, ios::in);
        string temp;
        tail_f >> temp;
        tail_f >> temp;
        tail_f.close();

        tail_f.open(folder + tail, ios::out);
        tail = to_string(stoi(tail.substr(0, tail.find("."))) + 1) + ".txt";
        tail_f << tail << "\n";
        tail_f << temp;
        tail_f.close();

        root_f.open(folder + "root.txt", ios::out);
        root_f << head << "\n";
        root_f << tail;
        root_f.close();
    }

    // tail处理
    ofstream tail_f(folder + tail);
    tail_f << "NULL\n";
    tail_f << content;
    tail_f.close();
}

int main()
{
    srand(unsigned(time(NULL)));
    ifstream transactions_f("full/transactions.csv");
    vector<string> transactions;
    string temp;
    transactions_f >> temp;
    while (!transactions_f.eof())
    {
        transactions_f >> temp;
        if(temp.empty())continue;
        temp=temp.substr(temp.find(",")+1,temp.size()-temp.find(",")-1);
        transactions.push_back(temp);
    }
    transactions_f.close();

    vector<string> transactions_copy(transactions);
    random_shuffle(transactions.begin(), transactions.end());

    _mkdir("cmsg1");
    _mkdir("cmsg2");

    while (1)
    {
        if (rand() % 2 == 0)
        {
            string now(transactions[0]);
            transactions.erase(transactions.begin());
            write_next(now);
        }
        else
        {
            string term;
            int choice = rand() % 3;
            switch (choice)
            {
            case 0:
                term = "height,";
                term += to_string(rand() % 100000);
                break;
            case 1:
                term = "hash,";
                hash<string> strhash;
                term += to_string(strhash(to_string(rand() % 100000)));
                break;
            case 2:
                term = "id,";
                term += transactions_copy[rand() % transactions_copy.size()];
                break;
            }
            write_next("search," + term);
        }
        Sleep(500);
    }
    return 0;
}
