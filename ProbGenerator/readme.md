# パズル生成
パズルを生成します。  
  
## 工程：  
1. 線を描く					(PaintLines.cpp)  
2. グラフ(G = <V, E>)生成	(GenerateGraph.cpp)  
3. {枠穴}{ピース}生成		(GeneratePolys.cpp)  
4. 問題生成					(GenerateProblem.cpp)  
→ main側で、GenerateProblem.main()を呼び出すときに、最後の引数をfalseにすると、「反転なし」にできます。  
  
## 操作説明：
- Escape           … プログラムの終了
- ←→             … 工程を戻す／進める
  
- マウス左ドラッグ … 線を引く
- Backspace        … 最後に引いた線を消す
- Z, X … 縮小／拡大
- WDSA … 平行移動
