//�O���tG = <���_�W��, �ӏW��>���^����ꂽ�Ƃ��A���ƂȂ�ӈȊO�����ׂĕԂ�
#pragma once
#include "stdafx.h"

class Bridge {
	int n;
	vector<vector<int>> edges;
	vector<int> ord;
	vector<int> lowlink;
	vector<bool> visited;
	vector<int> parent;
	int t;

	int dfs(int v);
	bool is_bridge(int a, int b);

public:
	vector<vector<int>> eraseBridge(vector<vector<int>> edges);
};