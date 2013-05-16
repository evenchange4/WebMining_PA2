# WebMining_PA2

Web Retrieval and Mining spring 2013 Programming HW 2
[page](https://ceiba.ntu.edu.tw/course/4825d0/hw/wm2013_hw2.tar.gz)

## Ranking with

1. PageRank
2. LexRank

## Run

* 查看bash 位置

```
$ which bash
=> /bin/bash
```

* 調整權限

```
$ chmod +x executeLR.sh
$ chmod +x executePR.sh
```

* 執行程式 PageRank

```
$ ./executePR.sh cs.stanford.2004-10.graph
```

* 執行程式 LexRank

```
$ ./executePR.sh cs.stanford.2004-10.graph
```

# Reference
- [Some Facts about Spearman’s ρ](http://irlab.csie.ntu.edu.tw/wm2008/contrib/wm2008-rho.pdf)
- [pagerank-tutorial](http://irlab.csie.ntu.edu.tw/wm2008/contrib/pagerank-tutorial.doc)

# 助教問答大匯整
- evaluate PageRank program 跑出來的兩個參數：
	- L1-norm is Sigma |xi-yi|
	- Spearman's rank 請看http://irlab.csie.ntu.edu.tw/wm2008/contrib/wm2008-rho.pdf
	- Your L1-norm and spearman's rho should be around 100 and 1
	- 關於配分部分, 原則上你的L1-norm跟Spearman's rho應該要跟上一篇提到的差不多才會有完整的分數. 如果差很多應該就是你的pagerank有bug
- Dangling page
	- You need to avoid adding edges explicitly to every node
for those nodes with zero out degree.
這句是說理論上你的演算法要做加到每個點的動作,但implementation上不能真的加上去因為時間複雜度會增加很多, 至於如何implement就是作業的一部分, 也可以把兩者的差異寫在report中
- 一開始weighting 時就要把idf乘進去嗎? (因為這樣內積時會乘兩遍idf 有些多餘)
	- yes
- tf計算時，要除以每個句子的總term數嗎? 還是term出現在句中的次數就好? 
	- 不需要, 只要統計出現次數就好
- default idf:"log(40252.0)" 是以10為底還是以e?
	- log e
- 做page rank時damping factor 還是 0.85 嗎?
	- Yes
- 假設句子1 和句子2  的相似度 > 0.1 是要給句子1有outlink到句子2，句子2也有out link到句子1嗎?
	- Yes, 就當作兩個有向邊 1->2 , 2->1
- 所以執行的方法是 ./executePR.sh graph_path 及 ./executeLR.sh sentence_path
	- yes
- 請問IR programming hw2 pdf 內的link都不能下載(dataset等),那是要到哪下載?? thx!
	- 所有需要的檔案都在tar.gz內
- Lex Rank output格式 請忽略第一行的總行數
- lexrank 結果 如果 pagerank 的值一樣，排序有差嗎
- pagerank 的值有需要取到第幾位嗎

