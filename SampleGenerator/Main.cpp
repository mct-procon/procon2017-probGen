//フォントのサイズは（変数に入れておくのが面倒くさいため）, PaintLinesでは20, それ以外では30とします。
#include "DxLib.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "DrawInfo.h"
#include "PaintLines.h"
#include "GenerateGraph.h"
#include "GeneratePolys.h"
#include "GenerateProblem.h"
#include "FileWriter.h"

const int windowSizeX = 3012 / 2;
const int windowSizeY = 1984 / 2;
Mouse mouse;
Keyboard keyboard;
DrawInfo drawInfo(windowSizeX, windowSizeY, 14.85, 2, 2);
PaintLines paintLines(&keyboard, &mouse, &drawInfo);
GenerateGraph generateGraph(&drawInfo, &keyboard);
GeneratePolys generatePolys(&drawInfo, &keyboard);
GenerateProblem generateProblem(&drawInfo, &keyboard);

FileWriter fileWriter;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(TRUE);
	SetBackgroundColor(255, 255, 255);
	SetGraphMode(windowSizeX, windowSizeY, 32);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	//写真（0.5倍の大きさで表示される）
	int photoHandle = LoadGraph("sample.jpg");
	paintLines.setPhotoHandle(photoHandle);

	//力技ページ制御
	pair<int, vector<Line>> lines;
	tuple<int, vector<Point>, vector<vector<int>>> graph;
	tuple<int, vector<Poly>, vector<Poly>> polys;	//枠, ピース

	int call_id = 0;
	while (true) {
		int state;
		
		//呼び出し
		if (call_id == 0) {
			SetFontSize(20);
			lines = paintLines.main();
			state = lines.first;
		}
		else if (call_id == 1) {
			SetFontSize(30);
			graph = generateGraph.main(lines.second);
			state = get<0>(graph);
		}
		else if (call_id == 2) {
			polys = generatePolys.main(get<1>(graph), get<2>(graph));
			state = get<0>(polys);
		}
		else if (call_id == 3) {
			fileWriter.writeFile(get<1>(polys), "answer.txt", true, true);
			fileWriter.writeFile(get<2>(polys), "answer.txt", true, false);
			polys = generateProblem.main(get<1>(polys), get<2>(polys), false);	//反転なしの場合は、最後の引数をfalseに設定
			state = get<0>(polys);
			fileWriter.writeFile(get<1>(polys), "problem.txt", true, true);
			fileWriter.writeFile(get<2>(polys), "problem.txt", true, false);
		}
		else {
			break;
		}

		//ページ番号制御
		if (state == KEY_INPUT_ESCAPE) break;
		if (state == KEY_INPUT_RIGHT) call_id++;
		if (state == KEY_INPUT_LEFT) call_id--;
	}
	DxLib_End();
	return 0;
}