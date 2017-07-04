//�������d���i�֐�������������j�̂ŁA�g�b�v�_�E���Łi�傫�Ȋ֐��قǃR�[�h�̐擪�ɂȂ�悤�Ɂj�������܂��B
//�g�̌��o�ɂāA�֐ߓ_��T���J�n�_�ɂ���ƃo�O��̂ŁA�֐ߓ_�ȊO���ŏ��ɑI������悤�ɂ��܂��傤�B�֐ߓ_��makeWaku()������ŗ񋓂��Ă����Α��v���Ǝv���܂��B(���͎������܂���)
//�ŏI�I��, �g���͎��v���, �s�[�X�͔����v���Ŏ��܂��B
#include "GeneratePolys.h"
#include "DxLib.h"
#include <stack>
#include "Line.h"

//�R���X�g���N�^
GeneratePolys::GeneratePolys(DrawInfo * drawInfo, Keyboard * keyboard)
{
	this->drawInfo = drawInfo;
	this->keyboard = keyboard;
}

//���C��
tuple<int, vector<Poly>, vector<Poly>> GeneratePolys::main(vector<Point> kotens, vector<vector<int>> edges)
{
	init();
	makeConnectedGraphs(Graph(kotens, edges));
	for (int i = 0; i < graphs.size(); i++) {
		makePolys(graphs[i]);
	}

	//�璷�_�폜
	for (int i = 0; i < wakus.size(); i++) { wakus[i] = fixPoly(wakus[i]); }
	for (int i = 0; i < pieces.size(); i++) { pieces[i] = fixPoly(pieces[i]); }

	//���ʕ\��
	int endKeycode[3] = { KEY_INPUT_ESCAPE, KEY_INPUT_LEFT, KEY_INPUT_RIGHT };		//�I���L�[�̏W��. ���ԓ���ւ���.
	int t = 0;

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		keyboard->update();
		drawInfo->update(keyboard);

		for (int i = 0; i < 3; i++) {
			if (keyboard->is_click(endKeycode[i])) {
				return tuple<int, vector<Poly>, vector<Poly>>(endKeycode[i], wakus, pieces);
			}
		}
		draw(t);
		t++;
	}

	return tuple<int, vector<Poly>, vector<Poly>>(endKeycode[0], wakus, pieces);
}

//�ϐ��̏�����
void GeneratePolys::init()
{
	graphs.clear();
	wakus.clear();
	pieces.clear();
}

//(���̂Ȃ�)�O���t��, �A���ȃO���t�̏W��graphs�ŕ\��
void GeneratePolys::makeConnectedGraphs(Graph & graph)
{
	int n = graph.first.size();
	vector<int> label(n, -1);
	queue<int> que;
	int id = 0;

	for (int i = 0; i < n; i++) {
		if (label[i] != -1) continue;
		que.push(i);
		label[i] = id;
		while (!que.empty()) {
			int v = que.front(); que.pop();
			for (int j = 0; j < graph.second[v].size(); j++) {
				if (label[graph.second[v][j]] == -1) {
					que.push(graph.second[v][j]);
					label[graph.second[v][j]] = id;
				}
			}
		}
		id++;
	}

	vector<vector<int>> labelUnion;
	labelUnion.resize(id);
	for (int i = 0; i < n; i++) {
		if (label[i] == -1) continue;
		labelUnion[label[i]].push_back(i);
	}

	vector<int> soutaiId(n);
	for (int i = 0; i < labelUnion.size(); i++) {
		for (int j = 0; j < labelUnion[i].size(); j++) {
			soutaiId[labelUnion[i][j]] = j;
		}
	}

	for (int i = 0; i < labelUnion.size(); i++) {
		if (labelUnion[i].size() <= 1) continue;
		//labelUnion[i]�𒸓_�W��V, V�̎����Ă���ӂ�ӏW��E�Ƃ����O���tG = <V, E>���쐬����B
		//���_�̔ԍ��ɒ��ӁI�I�ӂ̍s�撸�_�ԍ���graph�S�̂̔ԍ��ł͂Ȃ��Agraphs[i]�ɂ��Ă̔ԍ��ɂ��邱�ƁI
		vector<Point> points;
		vector<vector<int>> edges;
		for (int j = 0; j < labelUnion[i].size(); j++) {
			int v = labelUnion[i][j];
			points.push_back(graph.first[v]);
			
			vector<int> edgeV;
			for (int k = 0; k < graph.second[v].size(); k++) {
				edgeV.push_back(soutaiId[graph.second[v][k]]);
			}
			edges.push_back(edgeV);
		}
		graphs.push_back(Graph(points, edges));
	}
}

//���_��2�ȏ�̘A���ȃO���tgraph����, �s�[�X�Ƙg���쐬����B
void GeneratePolys::makePolys(Graph & graph)
{
	vector<vector<bool>> usedEdges;
	vector<int> remEdgeNums;
	initMakePolysFlag(graph, usedEdges, remEdgeNums);
	if (!makeWaku(graph, usedEdges, remEdgeNums)) return;
	while (makePiece(graph, usedEdges, remEdgeNums));
}

//�璷�_���폜�������Ƃ̑��p�`��Ԃ�
Poly GeneratePolys::fixPoly(Poly & poly)
{
	int n = poly.size() - 1;
	Poly ret;

	if (ccw(poly[n - 1], poly[0], poly[1]) != -2) ret.push_back(poly[0]);
	for (int i = 1; i < n; i++) {
		if (ccw(poly[i - 1], poly[i], poly[i + 1]) != -2) ret.push_back(poly[i]);
	}
	ret.push_back(ret[0]);
	return ret;
}

//�`��
void GeneratePolys::draw(int t)
{
	//�s�[�X
	for (int i = 0; i < pieces.size(); i++) {
		Point pos = getDrawPosition(pieces[i], drawInfo);
		DrawFormatString(pos.real(), pos.imag(), GetColor(0, 0, 255), "%d", i);
		for (int j = 0; j < pieces[i].size() - 1; j++) {
			Line(pieces[i][j], pieces[i][j + 1]).draw(drawInfo, GetColor(0, 255, 0));
		}
	}
	//�g��
	if (t % 90 >= 45) {
		for (int i = 0; i < wakus.size(); i++) {
			for (int j = 0; j < (int)wakus[i].size() - 1; j++) {
				Line(wakus[i][j], wakus[i][j + 1]).draw(drawInfo, GetColor(255, 0, 255));
			}
		}
	}
	//��
	DrawFormatString(drawInfo->window_width() - 300, drawInfo->window_height() - 110, GetColor(255, 0, 255), "�g�̌� = %d", wakus.size());
	DrawFormatString(drawInfo->window_width() - 300, drawInfo->window_height() - 70, GetColor(0, 255, 0), "�s�[�X�̌� = %d", pieces.size());
}


//makePolys()�֐��Ŏg���ϐ��̏�����
void GeneratePolys::initMakePolysFlag(Graph &graph, vector<vector<bool>>& usedEdges, vector<int>& remEdgeNums)
{
	int n = graph.first.size();
	usedEdges.resize(n);
	remEdgeNums.resize(n);
	for (int i = 0; i < n; i++) {
		usedEdges[i] = vector<bool>(graph.second[i].size(), false);
		remEdgeNums[i] = graph.second[i].size();
	}
}

//�g[1 + �֐ߓ_�̌�]���쐬
bool GeneratePolys::makeWaku(Graph & graph, vector<vector<bool>>& usedEdges, vector<int>& remEdgeNums)
{
	if (makeWaku(graph, false, usedEdges, remEdgeNums)) return true;
	if (makeWaku(graph, true, usedEdges, remEdgeNums)) return true;
	return false;
}

//�s�[�X1���쐬
bool GeneratePolys::makePiece(Graph & graph, vector<vector<bool>>& usedEdges, vector<int>& remEdgeNums)
{
	if (makePiece(graph, false, usedEdges, remEdgeNums)) return true;
	if (makePiece(graph, true, usedEdges, remEdgeNums)) return true;
	return false;
}

//is_clockwise = true�Ȃ玞�v������ۂ����Ȃ���g���쐬. �i���v���̘g���ł���Ƃ͌���Ȃ����ǁj
bool GeneratePolys::makeWaku(Graph & graph, bool is_clockwise, vector<vector<bool>> &usedEdges, vector<int> &remEdgeNums)
{
	vector<Point> &points = graph.first;
	vector<vector<int>> &edges = graph.second;
	vector<int> cycle;

	int startPos = startPointId(points, remEdgeNums); if (startPos == -1) return false;
	int eid = startEdgeId(startPos, points, edges[startPos], usedEdges[startPos]); if (eid == -1) return false;
	int pos = edges[startPos][eid];
	Point lastMove = points[pos] - points[startPos];

	cycle.push_back(startPos);
	while (pos != startPos) {
		cycle.push_back(pos);
		eid = nextEdgeId(graph, lastMove, pos, usedEdges[pos], is_clockwise);
		if (eid < 0) return false;
		int nextPos = edges[pos][eid];
		lastMove = points[nextPos] - points[pos];
		pos = nextPos;
	}
	cycle.push_back(startPos);

	//cycle���֐ߓ_���o�R���Ȃ�, �܂Ƃ���cycle�ɕ�������
	vector<bool> used(points.size(), false);
	stack<int> stk;
	vector<vector<int>> wakuCycles;

	for (int i = 0; i < cycle.size(); i++) {
		stk.push(cycle[i]);
		if (used[cycle[i]]) {	//�lcycle[i]��stk��2�����Ă���ꍇ
			vector<int> wakuCycle;
			wakuCycle.push_back(stk.top()); stk.pop();
			while (stk.top() != cycle[i]) {
				wakuCycle.push_back(stk.top()); stk.pop();
			}
			wakuCycle.push_back(stk.top());
			wakuCycles.push_back(wakuCycle);
		}
		used[cycle[i]] = true;
	}

	//�g(ring-buffer)���쐬����
	vector<Poly> createdPolys;
	for (int i = 0; i < wakuCycles.size(); i++) {
		Poly waku;
		for (int j = wakuCycles[i].size() - 1; j >= 0; j--) {
			int v = wakuCycles[i][j];
			waku.push_back(points[v]);
		}
		createdPolys.push_back(waku);
	}

	//�G���[����(�ǂ̘g���ɂ������Ă��Ȃ��_, �ӂ̒��_���������疵��)
	for (int i = 0; i < used.size(); i++) {
		if (used[i]) continue;
		int j;
		for (j = 0; j < createdPolys.size(); j++) {
			if (contain(createdPolys[j], points[i]) != out) break;
		}
		if (j == createdPolys.size()) {
			return false;
		}
	}
	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < edges[i].size(); j++) {
			Point p = points[i] + points[edges[i][j]]; p /= 2;
			int k;
			for (k = 0; k < createdPolys.size(); k++) {
				if (contain(createdPolys[k], p) != out) break;
			}
			if (k == createdPolys.size()) {
				return false;
			}
		}
	}

	//�g���X�V
	bool revCycleFlag = false;
	for (int i = 0; i < createdPolys.size(); i++) {
		if (area(createdPolys[i]) > 0) {
			reverse(createdPolys);
			revCycleFlag = true;
		}
		wakus.push_back(createdPolys[i]);
	}

	if (revCycleFlag) { reverse(cycle); }

	//usedEdges, remEdgeNums���X�V
	for (int i = 0; i < (int)cycle.size() - 1; i++) {
		int id;
		for (id = 0; id < edges[cycle[i]].size(); id++) {
			if (edges[cycle[i]][id] == cycle[i + 1]) break;
		}
		usedEdges[cycle[i]][id] = true;
		remEdgeNums[cycle[i]]--;
	}
	return true;
}

//is_clockwise = true�Ȃ玞�v�����ɂ��ǂ��ăs�[�X���쐬. ���v���̃s�[�X���ł���Ƃ͌���Ȃ��B
bool GeneratePolys::makePiece(Graph & graph, bool is_clockwise, vector<vector<bool>> &usedEdges, vector<int> &remEdgeNums)
{
	vector<Point> &points = graph.first;
	vector<vector<int>> &edges = graph.second;
	vector<int> cycle;

	int startPos = startPointId(points, remEdgeNums); if (startPos == -1) return false;
	int eid = startEdgeId(startPos, points, edges[startPos], usedEdges[startPos]); if (eid == -1) return false;
	int pos = edges[startPos][eid];
	Point lastMove = points[pos] - points[startPos];

	cycle.push_back(startPos);
	while (pos != startPos) {
		cycle.push_back(pos);
		eid = nextEdgeId(graph, lastMove, pos, usedEdges[pos], is_clockwise);
		if (eid < 0) return false;
		int nextPos = edges[pos][eid];
		lastMove = points[nextPos] - points[pos];
		pos = nextPos;
	}
	cycle.push_back(startPos);

	//�s�[�X�𐶐����܂�
	Poly piece;
	for (int i = 0; i < cycle.size(); i++) {
		piece.push_back(points[cycle[i]]);
	}
	
	//�����v���ɂ��܂�
	bool revCycleFlag = false;
	if (area(piece) < 0) {
		reverse(piece);
		revCycleFlag = true;
	}
	if (revCycleFlag) { reverse(cycle); }

	//�G���[�����i{�T�C�N��} - {�I�_}�ɓ������_��2�ȏ�܂܂�Ă����疵���B�j->����ȊO�Ȃ�A�P�����p�`��������B
	//���łɑS�������s�[�X�����݂����疵��
	//�i�T�C�N���̒��_�W����K�Ƃ����BK->K��̖��g�p�ӂł����āApiece�̓����ɂ�����̂����݂����疵���Bpiece�̓����ɓ_������Ζ����j
	vector<bool> used(points.size(), false);
	for (int i = 0; i < cycle.size() - 1; i++) {
		if (used[cycle[i]]) return false;
		used[cycle[i]] = true;
	}
	for (int i = 0; i < pieces.size(); i++) {
		int j;
		for (j = 0; j < pieces[i].size(); j++) {
			if (pieces[i][j] != piece[j]) break;
		}
		if (j == pieces[i].size()) return false;
	} 
	for (int i = 0; i < cycle.size() - 1; i++) {
		int u = cycle[i];
		for (int j = 0; j < edges[u].size(); j++) {
			int v = edges[u][j];
			if (usedEdges[u][j] || !used[v]) continue;
			if (contain(piece, (points[u] + points[v]) * 0.5) == in) return false;
		}
	}
	for (int i = 0; i < points.size(); i++) {
		if (contain(piece, points[i]) == in) return false;
	}

	//�s�[�X�W���̍X�V
	pieces.push_back(piece);

	//usedEdges, remEdgeNums���X�V
	for (int i = 0; i < (int)cycle.size() - 1; i++) {
		int id;
		for (id = 0; id < edges[cycle[i]].size(); id++) {
			if (edges[cycle[i]][id] == cycle[i + 1]) break;
		}
		usedEdges[cycle[i]][id] = true;
		remEdgeNums[cycle[i]]--;
	}
	return true;
}

//�T���J�n�_�i�O���ɂ���_�j��Ԃ�. �Ȃ����-1��Ԃ�.
int GeneratePolys::startPointId(vector<Point>& points, vector<int>& remEdgeNums)
{
	double minX = 1145141919;
	int ret = -1;
	for (int i = 0; i < points.size(); i++) {
		if (remEdgeNums[i] == 0) {
			continue;
		}
		if (minX > points[i].real()) {
			minX = points[i].real();
			ret = i;
		}
	}
	return ret;
}

//points[pos]���玟�Ɍ������ꏊ���l����B���̂Ƃ��Ɏg���ӂ̔ԍ���Ԃ��B
int GeneratePolys::startEdgeId(int pos, vector<Point>& points, vector<int>& edges, vector<bool>& usedEdges)
{
	double minDot = 1145141919;
	int ret = -1;

	for (int i = 0; i < edges.size(); i++) {
		if (usedEdges[i]) {
			continue;
		}
		Point vec = points[edges[i]] - points[pos]; vec /= abs(vec);
		if (minDot > dot(vec, Point(1, 0))) {
			minDot = dot(vec, Point(1, 0));
			ret = i;
		}
	}
	return ret;
}

//���ɉ��Ԃ̕ӂ��g������Ԃ��B�Ȃ����-1��Ԃ��B
int GeneratePolys::nextEdgeId(Graph & graph, Point lastMove, int pos, vector<bool> &usedEdges, bool is_clockwise)
{
	const double PAI = 3.14159265358979;
	double tmp;
	bool first_loop = true;
	int ret = -1;

	lastMove *= -1;

	for (int i = 0; i < usedEdges.size(); i++) {
		if (usedEdges[i]) continue;

		Point vec = graph.first[graph.second[pos][i]] - graph.first[pos];
		if (vec == lastMove) continue;
		vec /= lastMove;
		double angle = arg(vec);
		if (angle < 0) angle += 2 * PAI;

		if (is_clockwise) {	//angle�ő�����
			if (first_loop || tmp < angle) {
				tmp = angle;
				ret = i;
				first_loop = false;
			}
		}
		else {	//angle�ŏ������
			if (first_loop || tmp > angle) {
				tmp = angle;
				ret = i;
				first_loop = false;
			}
		}
	}
	return ret;
}

//���_��̌������t�ɂ���
template<class T> void GeneratePolys::reverse(T & value)
{
	int l = 0, r = value.size() - 1;
	while (l < r) {
		swap(value[l], value[r]);
		l++;
		r--;
	}
}
