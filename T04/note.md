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
    int height;//作为头结点时记录的是总长度
    string hash;
    string prevHash;
    string merkleRoot;
    int nonce;
    transaction *transactions;
    int transNum;//���еĽ�����
    struct block *prev;
    struct block *next;
} block, *blockList;
```

# guest
## data
### 交易池

## function

## message
### 客户消息队列
### 区块消息队列

# server
## function
### 交易
### 查询