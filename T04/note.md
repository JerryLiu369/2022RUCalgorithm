# class
## 交易
```cpp
typedef struct {
	string txid;
	int input_count;
	int output_count;
	input *inputs;
	output *outputs;
	int is_coinbase;
	bool is_valid;//初始化合法
} transaction;
```

## 区块
```cpp
typedef struct block {
    int height;
    string hash;
    string prevHash;
    string merkleRoot;
    int nonce;
    transaction *transactions;
    int transNum;
    struct block *prev;
    struct block *next;
} block, *blockList;
```
# 文件
## node1
clientmsg1
blockmsg1
## node2
clientmsg2
blockmsg2

# client
## data
### 交易池

## function

## message
### 客户消息队列
### 区块消息队列
- 1.txt
2.txt
height,hash,prevHash,merkleRoot,nonce,transNum
`1`txid,is_coinbase,input_countoutput_count
`2`txid,is_coinbase,input_countoutput_count
`.`

# Server
## function
### 交易
### 查询