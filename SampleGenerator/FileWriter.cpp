#include "FileWriter.h"

bool FileWriter::writeFile(vector<vector<Point>>& polys, string filename, bool isCastInt, bool isOverWrite)
{
	int mode = isOverWrite ? ios::out : ios::app;

	ofstream ofs(filename, mode);
	if (ofs.fail()) return false;

	ofs << polys.size() << endl;
	for (int i = 0; i < polys.size(); i++) {
		ofs << polys[i].size() - 1 << endl;
		for (int j = 0; j < polys[i].size() - 1; j++) {
			double x = polys[i][j].real();
			double y = polys[i][j].imag();
			if (isCastInt) {
				ofs << (int)x << " " << (int)y << endl;
			}
			else {
				ofs << x << " " << y << endl;
			}
		}
	}
	ofs << endl;
	ofs.close();
}
