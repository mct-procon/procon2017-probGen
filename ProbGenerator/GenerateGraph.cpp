#include "GenerateGraph.h"
#include "Bridge.h"

void GenerateGraph::drawLines(vector<Line>& lines)
{
	for (int i = 0; i < lines.size(); i++) {
		lines[i].draw(drawInfo);
	}
}

void GenerateGraph::drawGraph(vector<Point>& kotens, vector<vector<int>>& edges)
{
	for (int i = 0; i < kotens.size(); i++) {
		int x = drawInfo->toDrawX(kotens[i].real());
		int y = drawInfo->toDrawY(kotens[i].imag());
		DrawCircle(x, y, 3, GetColor(255, 0, 0), TRUE);
		DrawFormatString(x + 7, y - 30, 0, "%d", i);
	}
	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < edges[i].size(); j++) {
			int from = i;
			int to = edges[i][j];
			Line(kotens[from], kotens[to]).draw(drawInfo, GetColor(0, 255, 0), 2);
		}
	}
}

//�R���X�g���N�^
GenerateGraph::GenerateGraph(DrawInfo * drawInfo, Keyboard * keyboard)
{
	this->drawInfo = drawInfo;
	this->keyboard = keyboard;
}

//�����W������, ��_�����o��, �ׂ荇����_��o�����Ɍ��񂾃O���tG = <V, E>�𐶐�����B
//���ʂȕӂ̓J�b�g����B������0�̒��_�i��_�j���ł���ꍇ������̂Œ��ӁB
//�O��F�����̎n�_, �I�_, ��_�͐������W�Ƃ���B���s��2����������邱�Ƃ��Ȃ��Ƃ���B
tuple<int, vector<Point>, vector<vector<int>>> GenerateGraph::main(vector<Line> lines)
{
	vector<Point> kotens;

	for (int i = 0; i < lines.size(); i++) {
		for (int j = i + 1; j < lines.size(); j++) {
			if (!lines[i].ishit(lines[j])) continue;
			kotens.push_back(lines[i].cross_point(lines[j]));
		}
	}
	sort(kotens.begin(), kotens.end());
	kotens.erase(unique(kotens.begin(), kotens.end()), kotens.end());

	vector<vector<int>> edges;
	edges.resize(kotens.size());

	for (int i = 0; i < lines.size(); i++) {
		vector<pair<Point, int>> points;
		for (int j = 0; j < kotens.size(); j++) {
			if (lines[i].on_line(kotens[j])) {
				points.push_back(pair<Point, int>(kotens[j], j));
			}
		}
		sort(points.begin(), points.end());
		for (int j = 0; j + 1 < points.size(); j++) {
			edges[points[j].second].push_back(points[j + 1].second);
			edges[points[j + 1].second].push_back(points[j].second);
		}
	}

	//���폜
	Bridge bridge;
	vector<vector<int>> newEdges = bridge.eraseBridge(edges);

	//���ʕ\��
	int endKeycode[3] = { KEY_INPUT_ESCAPE, KEY_INPUT_LEFT, KEY_INPUT_RIGHT };		//�I���L�[�̏W��. ���ԓ���ւ���.
	
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		keyboard->update();
		drawInfo->update(keyboard);
	
		for (int i = 0; i < 3; i++) {
			if (keyboard->is_click(endKeycode[i])) {
				return tuple<int, vector<Point>, vector<vector<int>>>(endKeycode[i], kotens, newEdges);
			}
		}

		drawLines(lines);
		drawGraph(kotens, newEdges);
	}
	return tuple<int, vector<Point>, vector<vector<int>>>(endKeycode[0], kotens, newEdges);
}
