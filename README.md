# 中國郵差問題(Chinese Postman Problem, CPP)

1. 問題:
>給定一個無向圖，圖中有數個vertices與edges，找出一條最短路徑封閉路徑，皆能經過所有edges至少一次。

2. 解析:
>無向圖中所有vertices必須找出一條通過所有vertices, edges的最短封閉路徑，而edges能通過多次，因此必須先確認圖是否為connected，確保從任意起點開始皆能到達其他vertex。
>由於圖中的edges可以重複經過，因此可以先假定重複通過的edges，為一條新的edges，即當通過edge次數大於1次時，每多通過一次，vertices之間就多一條edge。
>從任意起點開始，每個點皆能找出一條回到自身的封閉路徑，代表最終解出來的圖是一個Euler cycle，圖中所有vertices的degree皆為偶數。
>但給定的圖不一定是Euler cycle，因此可以找出圖中所有的奇點(degree為奇數的vertex)，奇點數量必為偶數。接著找出成對奇點間的最短路徑，在這條最短路徑上的每個edge，加上一條新edge，路徑上中繼點的degree由於加上了2條edge，degree增加2，奇偶性質不變；而該成對奇點上各加了一條edge，因此degree也被補正為偶數。
>因此經過修正後，新圖上的vertices的degree皆為偶數，必能使用一筆畫定理的演算法，找到一條最短封閉路徑，即為問題所求答案。

3. 文件說明:
>測試資料統一放置在Benchmark中，程式會由此讀取資料，而fake-mininet-master為Fake-mininet API套件。include與src資料夾分別放置header與source code檔案。而Output資料夾放置測試資料後所產生的結果txt檔。

4. 執行說明:
>開啟終端介面，將所在位置移至本專案資料夾，輸入指令”make”，makefile會自動執行編譯程序，產生執行檔CPP.exe。輸入測試資料時，可執行指令:
>./CPP.exe ./Benchmark/資料名稱.txt
	而可接受的資料檔案格式如下:
	a. <first node name> <second node name> <capacity> <flow value>
	b. <first node name> <second node name>
	example:
	a.                      b.
	    a b 1 1                 a b
	    b c 1 1                 b c
	    ......                  ......

	除了上述方式，也能執行腳本檔run.sh，能夠批次把Benchmark內的測試資料執行計算結果。

5. 演算法說明
	如同上述解析的流程，首先判斷輸入的圖是否為connected，若不是的話則在輸出檔案顯示無解；是的話，則會先判斷該圖是否為Euler cycle，若是的話，則直接執行演算法，如果不是的話，就計算最短路徑的距離，補上edges使奇點消失，得到Euler cycle後再執行演算法。
	演算法使用Hierholzer's Algorithm，流程如下:
	1. 選定一個起點，接著沿著起點上的一條edge往下一個vertex前進，並標記edge已被走過。
	2. 重複上述步驟，直到到達起始點上，若起始點上的edge都已被走過，代表路徑已經過一個cycle，接著沿著路徑回溯，紀錄回溯的路徑，尋找仍有edge還沒走過的vertex。從那個vertex開始，繼續沿edge往下走。
	3. 重複直到所有edge都被走過，將回溯的路徑紀錄完，便是最終的最短封閉路徑。
	該演算法原理是藉由每個vertex上必有一個或一個以上的cycle，藉由回溯路徑，以起點為始，在路徑上遇到另一cycle時，先將該cycle走完，回到原cycle之中繼續下去，因此終點也為起始點。

6. 程式說明
	a. class VERTEX
		紀錄vertex的名字及與之相聯的edges陣列。
	b. class EDGE
		紀錄edge的兩端點，與是否被走過的旗標through。
	c. class CPP
		為整體演算法的資料結構，將圖存進結構中便能以內部函式運算
 		Function:
		(1) read_graph : 將圖讀入
		(2) check_connected : 檢驗圖是否connected
		(3) check_Euler : 檢驗圖是否為Euler cycle
		(4) Find_shortest_path : 找出各點間最短路徑
		(5) cal_minw : 計算奇點間最短路徑權重
		(6) create_Euler : 根據權重，在奇點間的path增加edge
		(7) Euler_path : 演算法算出最短封閉路徑
		(8) dump_solution : 輸出結果
 		Object:
		(1) NetworkManager graph : Fake-mininet套件的物件，用於計算最短路徑
		(2) v_list : 紀錄vertex的陣列
		(3) e_list : 紀錄edge的陣列
		(4) v_index : 根據我自己的結構與Fake-mininet套件的結構，以map來轉換，將vertex名字對應到Fake-mininet中vertex陣列的索引值。
		(5) odd_v : 紀錄圖中的奇點
		(6) optw_vlist : 經計算權重後的奇點排序。比如有奇點b, c, d, e，經計算後，最短路徑權重總和最小的組合為bd, ce，則陣列排序為b, d, c, e。以便後續要建立edge使用
		(7) shortest_path : 以Fake-mininet套件所建立的各點間最短路徑陣列
		(8) shortest_distance : 各點間最短路徑距離的陣列，即權重值
		(9) final_path : 紀錄最短封閉路徑流程的vertex點

7. 參考資料
	Hierholzer's Algorithm
	https://iampandiyan.blogspot.com/2013/10/c-program-to-find-euler-path-or-euler.html?fbclid=IwAR3lx-pnj_xg_yenxQ31Tc_cyaLDRwKNOOJ31Xtj9L_EPizKDMsbEyaOIlw
