#include <iostream>
#include<string>
#include <set>
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
using std::string;
using std::set;
using std::cin;
using std::cout;
using std::endl;

struct Block
{
    int height;
    char hash[512];
    char prevHash[512];
    char merkleRoot[512];
    int nonce;
    struct Transaction *transactions;
};

struct Transaction
{
    char txid[512];
    int input_count;
    int output_count;
    struct Input *inputs;
    struct Output *outputs;
    int is_coinbase;
    struct Transaction *prior;
    struct Transaction *next;
};

struct Input
{
    int pre_block;
    char prevTxID[512];
    int prevTxOutIndex;
    char scriptSig[512];
    struct Input *prior;
    struct Input *next;
};

struct Output
{
    char txid[512];
    int index;
    long long value;
    char script[512];
    struct Output *prior;
    struct Output *next;
    int used=0;
};

typedef Block *Blockptr;
typedef Transaction *Transactionptr;
typedef Input *Inputptr;
typedef Output *Outputptr;

void blockinit(FILE *fp, Blockptr &bp, int &nowsize)
{
    if (nowsize == 0)
    {
        nowsize++;
        bp = (Blockptr)calloc(1,sizeof(Block));
    }
    int count = 0;
    char tempstr[512];
    fgets(tempstr, 512, fp);
    fgets(tempstr, 512, fp);
    while (!feof(fp))
    {
        if (tempstr == "\n")
        {
            fgets(tempstr, 512, fp);
            continue;
        }
        count++;
        if (count > nowsize)
        {
            nowsize *= 2;
            bp = (Blockptr)realloc(bp, 2 * nowsize * sizeof(Block));
            if (!bp)
                throw(0);
        }
        Block &now = bp[count - 1];
        now.height = atoi(strtok(tempstr, ","));
        strcpy(now.hash, strtok(NULL, ","));
        strcpy(now.prevHash, strtok(NULL, ","));
        strcpy(now.merkleRoot, strtok(NULL, ","));
        now.nonce = atoi(strtok(NULL, ","));
        now.transactions = NULL;
        fgets(tempstr, 512, fp);
    }
    nowsize = count;
}

void transactioninit(FILE *fp, Blockptr &bp, int &valid)
{
    char tempstr[512];
    fgets(tempstr, 512, fp);
    fgets(tempstr, 512, fp);
    while (!feof(fp))
    {
        if (tempstr == "\n")
        {
            fgets(tempstr, 512, fp);
            continue;
        }
        int height = atoi(strtok(tempstr, ","));
        Block &blocknow = bp[height];
        Transactionptr tp_now = (Transactionptr)calloc(1, sizeof(Transaction));
        if (!tp_now)
            throw(0);
        strcpy(tp_now->txid, strtok(NULL, ","));
        tp_now->is_coinbase = atoi(strtok(NULL, ","));
        tp_now->input_count = atoi(strtok(NULL, ","));
        tp_now->output_count = atoi(strtok(NULL, ","));
        tp_now->inputs = NULL;
        tp_now->outputs = NULL;
        tp_now->prior = tp_now;
        tp_now->next = tp_now;
        if (blocknow.transactions == NULL)
        {
            blocknow.transactions = tp_now;
        }
        else
        {
            tp_now->next = blocknow.transactions;
            tp_now->prior = blocknow.transactions->prior;
            blocknow.transactions->prior->next = tp_now;
            blocknow.transactions->prior = tp_now;
        }
        valid++;
        fgets(tempstr, 512, fp);
    }
}

void inputsinit(FILE *fp, Blockptr &bp)
{
    char tempstr[512];
    fgets(tempstr, 512, fp);
    fgets(tempstr, 512, fp);
    while (!feof(fp))
    {
        if (tempstr == "\n")
        {
            fgets(tempstr, 512, fp);
            continue;
        }
        int height = atoi(strtok(tempstr, ","));
        Block &blocknow = bp[height];
        Inputptr ip_now=(Inputptr)calloc(1,sizeof(Input));
        if (!ip_now)
            throw(0);
        char txid[512];
        strcpy(txid,strtok(NULL,","));
        ip_now->pre_block=atoi(strtok(NULL,","));
        strcpy(ip_now->prevTxID,strtok(NULL,","));
        ip_now->prevTxOutIndex=atoi(strtok(NULL,","));
        strcpy(ip_now->scriptSig,strtok(NULL,","));
        ip_now->prior=ip_now;
        ip_now->next=ip_now;
        Transactionptr tp_now=blocknow.transactions;
        while (true)
        {
            if (strcmp(tp_now->txid,txid)==0)
            {
                Inputptr &ip_block=tp_now->inputs;
                if (ip_block==NULL)
                {
                    ip_block=ip_now;
                }
                else
                {
                    ip_now->next=ip_block;
                    ip_now->prior=ip_block->prior;
                    ip_block->prior->next=ip_now;
                    ip_block->prior=ip_now;
                }
                break;
            }
            tp_now=tp_now->next;
        }
        fgets(tempstr, 512, fp);
    }
}

void outputsinit(FILE *fp, Blockptr &bp)
{
    char tempstr[512];
    fgets(tempstr, 512, fp);
    fgets(tempstr, 512, fp);
    while (!feof(fp))
    {
        if (tempstr == "\n")
        {
            fgets(tempstr, 512, fp);
            continue;
        }
        int height = atoi(strtok(tempstr, ","));
        Block &blocknow = bp[height];
        Outputptr op_now = (Outputptr)calloc(1, sizeof(Output));
        if (!op_now)
            throw(0);
        char txid[512];
        strcpy(txid,strtok(NULL,","));
        op_now->index=atoi(strtok(NULL,","));
        op_now->value=atoll(strtok(NULL,","));
        strcpy(op_now->script, strtok(NULL, ","));
        op_now->prior=op_now;
        op_now->next=op_now;
        Transactionptr tp_now=blocknow.transactions;
        while (true)
        {
            if (strcmp(tp_now->txid,txid)==0)
            {
                Outputptr &op_block=tp_now->outputs;
                if (op_block==NULL)
                {
                    op_block=op_now;
                }
                else
                {
                    op_now->next=op_block;
                    op_now->prior=op_block->prior;
                    op_block->prior->next=op_now;
                    op_block->prior=op_now;
                }
                break;
            }
            tp_now=tp_now->next;
        }
        fgets(tempstr, 512, fp);
    }
}

Transactionptr t_find(Transactionptr tp_now,char* txid)
{
    if(!tp_now)return tp_now;
    Transactionptr res = tp_now;
    while (true)
    {
        if (strcmp(res->txid, txid) == 0)
        {
            return res;
        }
        res = res->next;
        if(res==tp_now)
        {
            break;
        }
    }
    res = NULL;
    return res;
}

Outputptr o_find(Outputptr op_now, int index)
{
    if (!op_now)
        return op_now;
    Outputptr res = op_now;
    while (true)
    {
        if (res->index == index)
        {
            if (res->used == 0)
            {
                res->used = 1;
                return res;
            }
            else
            {
                res = NULL;
                return res;
            }
        }
        res = res->next;
        if (res == op_now)
        {
            break;
        }
    }
    res = NULL;
    return res;
}

Outputptr o_find_out(Outputptr op_now, int index)
{
    if (!op_now)
        return op_now;
    Outputptr res = op_now;
    while (true)
    {
        res = res->next;
        if (res->index == index)
        {
            return res;
        }
        if (res == op_now)
        {
            break;
        }
    }
    res = NULL;
    return res;
}

void tp_free(Transactionptr &tp_now,Transactionptr &tp_move)
{
    if (tp_move->next==tp_move)
    {
        free(tp_move);
        tp_now==NULL;
    }
    else
    {
        if(tp_move==tp_now)
        {
            tp_now=tp_move->next;
        }
        Transactionptr tp_temp = tp_move->next;
        tp_move->next->prior=tp_move->prior;
        tp_move->prior->next=tp_move->next;
        free(tp_move);
        tp_move = tp_temp;
    }
}

void validate(Blockptr &bp, const int &block_count, int &valid, int &invalid)
{
    for (int i = 0; i < block_count; i++)
    {
        Transactionptr &tp_now = bp[i].transactions;
        if (!tp_now)
            continue;
        Transactionptr tp_move = tp_now;
        while (true)
        {
            Inputptr ip_now = tp_move->inputs;
            if (!ip_now)break;
            long long in_value_sum = 0;
            Inputptr ip_move = ip_now;
            int flag = 0;
            for (int j = 0; j < tp_move->input_count; j++)
            {
                Transactionptr tp_prev = t_find(bp[ip_move->pre_block].transactions, ip_move->prevTxID);
                if (tp_prev == NULL)
                {
                    flag = 1;
                    break;
                }
                else
                {
                    Outputptr op_prev=o_find(tp_prev->outputs, ip_move->prevTxOutIndex);
                    if ( op_prev== NULL)
                    {
                        flag = 1;
                        break;
                    }
                    in_value_sum += op_prev->value;
                }
                ip_move = ip_move->next;
            }
            if (flag != 1)
            {
                long long out_value_sum = 0;
                Outputptr op_move = tp_move->outputs;
                for (int j = 0; j < tp_move->output_count; j++)
                {
                    out_value_sum += op_move->value;
                    op_move = op_move->next;
                }
                if (out_value_sum > in_value_sum)
                {
                    flag = 1;
                }
            }
            if (flag == 1)
            {
                tp_free(tp_now,tp_move);
                valid -= 1;
                invalid += 1;
            }
            else
                tp_move = tp_move->next;
            if (tp_move == tp_now)
            {
                break;
            }
        }
    }
}

Blockptr chaininit(int &block_count ,int &valid ,int &invalid)
{
    char root[512] = "demo/";
    char filename[512];
    block_count = 0;
    valid = 0;
    invalid = 0;
    //
    strcpy(filename, root);
    FILE *blockfp = fopen(strcat(filename, "blocks.csv"), "r");
    if (!blockfp)
        throw(0);
    Blockptr bp;
    blockinit(blockfp, bp, block_count);
    fclose(blockfp);
    //
    strcpy(filename, root);
    FILE *transactionsfp = fopen(strcat(filename, "transactions.csv"), "r");
    if (!transactionsfp)
        throw(0);
    transactioninit(transactionsfp, bp, valid);
    fclose(transactionsfp);
    //
    strcpy(filename, root);
    FILE *inputsfp = fopen(strcat(filename, "inputs.csv"), "r");
    if (!inputsfp)
        throw(0);
    inputsinit(inputsfp,bp);
    fclose(inputsfp);
    //
    strcpy(filename, root);
    FILE *outputsfp = fopen(strcat(filename, "outputs.csv"), "r");
    if (!outputsfp)
        throw(0);
    outputsinit(outputsfp,bp);
    fclose(outputsfp);
    //
    validate(bp,block_count,valid,invalid);
    return bp;
    //查询功能实现
    /*
    printf("区块总数：%d,合法交易总数：%d,不合法交易总数：%d\n",block_count,valid,invalid);
    int inblock=0;
    printf("请输入查询区块高度：\n");
    scanf("%d",&inblock);
    Block block=bp[inblock];
    printf("区块内容：\nhash:%s;merkleRoot:%s;nonce:%d\n",block.hash,block.merkleRoot,block.nonce);
    char intxid[512];
    printf("请输入查询交易txID：\n");
    scanf("%s",intxid);
    int flag = 0;
    for (int i = 0; i < block_count; i++)
    {
        Transactionptr tp_now = bp[i].transactions;
        if (!tp_now)
            continue;
        Transactionptr tp_move = tp_now;
        while (true)
        {
            if(strcmp(tp_move->txid,intxid)==0)
            {
                flag=1;
                printf("交易内容：\nis_coinbase:%d,input_count:%d,output_count:%d\n",tp_move->is_coinbase,tp_move->input_count,tp_move->output_count);
                break;
            }
            tp_move = tp_move->next;
            if (tp_move == tp_now)
            {
                break;
            }
        }
        if (flag==1)
        {
            break;
        }
    }
    if(flag==0)printf("交易非法！\n");
    return 0;
    */
}