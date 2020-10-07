#include <algorithm> // for min
#include <functional> // for function
#include <iostream>
#include <vector>
#include<string>

using namespace std;

//
// Qudrat Rabbi
// Prof : John Baugh
// CIS 3501
// Assignment 4
//




vector<vector<int>> BiconComponents(const vector<vector<int>>& points) { // Parts of graph that are biconnected
	vector<vector<int>> bicons;

	vector<int> idx(points.size(), -1), staq; // initializing vectors idx with all -1s


	function<int(int)> doit = [&](int x) {

		idx[x] = staq.size();
		staq.push_back(x);
		int low = idx[x];

		if (points[x].size() == 0) {
			bicons.emplace_back(1, x);
		}
		for (int i = 0; i < points[x].size(); i++) {
			if (idx[points[x][i]] == -1) {
				int curs = staq.size();
				int v = doit(points[x][i]);
				if (v >= idx[x]) {
					bicons.emplace_back(staq.begin() + curs, staq.end());
					bicons.back().push_back(x);
					staq.resize(curs);
				}
				else {
					low = min(low, v);
				}
			}
			else {
				low = min(low, idx[points[x][i]]);
			}
		}
		return low;
	};
	for (int x = 0; x < points.size(); x++) if (idx[x] == -1) doit(x);
	return bicons;
}

int main() {

	string input_one, input_two; // Allows to receive inputs in anyform
	int  num_of_loc, num_of_streets,V, W;

	while (cin >> input_one >> input_two)//Run as long as inputs detected 
	{	
		try 
		{
			num_of_loc = stoi(input_one);  // using stoi to convert int to string
			num_of_streets = stoi(input_two);
		}
		catch(invalid_argument const &e){ // For invalid conversion, error is printed and moves to the next input 
			cout << "Invalid input received." << endl;
			cout << "Please try again." << endl;
			continue;
		}

		if (num_of_loc <= 0 ) { // no locations no streets
			cout << "Invalid number of locations. No signs required. Please try again" << endl;
			continue;			
		}
		if (num_of_streets <= 0) { // no streets no signs
			cout << "Invalid number of streets. No signs required. Please try again" << endl;
			continue;
		}		

		vector<vector<int>> g(num_of_loc), gi(num_of_loc), d(num_of_loc);


		for (int i = 0; i < num_of_streets; i++) {

			cin >> V >> W;
			V--; W--;
			gi[V].push_back(g[W].size()); // Since the graph is undirected
			gi[W].push_back(g[V].size()); // They have to paired up
			g[V].push_back(W);
			g[W].push_back(V);
			d[V].push_back(1);
			d[W].push_back(1);
		}

		vector<vector<int>> components = BiconComponents(g); // initializing componenets as biconnected parts of graph

		vector<int> seen(num_of_loc);

		function<void(int, int)> clearD = [&](int x, int i) 
		{
			if (!d[x][i]) 
				return;

			d[x][i] = 0;
			int y = g[x][i];
			if (++seen[y] > 3)
				return;

			for (int j = 0; j < g[y].size(); j++)
				if (g[y][j] != x)
					clearD(y, j);

		};

		for (auto const& comp : components)
			if (comp.size() >= 3) { // if the biconnected component has size of 3 or more, its part of a cycle, therefore not dead end
			for (auto x : comp)
				for (int i = 0; i < g[x].size(); i++)
					clearD(x, i);
		}


		for (int x = 0; x < num_of_loc; x++) // gets rid of redundant signs
			for (int i = 0; i < g[x].size(); i++) {
			if (x < g[x][i])
				swap(d[x][i], d[g[x][i]][gi[x][i]]);
		}

		seen = vector<int>(num_of_loc);				// for loop to stop program from double counting each vertices for dead end signs
		for (int x = 0; x < num_of_loc; x++)		
			for (int i = 0; i < g[x].size(); i++)
				if (d[x][i]) 
				{
			clearD(x, i);
			d[x][i] = 1;
		}

		vector<pair<int, int> > dead_post_spots;

		for (int x = 0; x < num_of_loc; x++) // for every detected dead end spot, its appended to vector dead_post_sports 
			for (int i = 0; i < g[x].size(); i++)
				if (d[x][i]) {
					dead_post_spots.emplace_back(x, g[x][i]);
				}

		cout << dead_post_spots.size() << endl; // Prints out the number of dead end signs

		for (auto const& p : dead_post_spots) cout << p.first + 1 << ' ' << p.second + 1 << endl;// range based loop to show the edges needing signs

	}
}