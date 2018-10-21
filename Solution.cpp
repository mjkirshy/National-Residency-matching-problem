#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
private:
	int m;
	int n;
	map<int, vector<int>> students;
	map<int, vector<int>> hospitals;
	map<int, vector<int>>::iterator it;
	vector<pair<int, int>> hospital_choice;
	vector<pair<int, int>> stableMatchings;
	vector<pair<int, int>>::iterator itt;

public:
	Solution(int _m, int _n, map<int, vector<int>> _students, map<int, vector<int>> _hospitals) {
		m = _m;
		n = _n;
		students = _students;
		hospitals = _hospitals;
	}

	vector<pair<int, int>> getMatches() {
		stableMatchings.clear();
		int g = 0;
		for (it = hospitals.begin(); it != hospitals.end(); ++it) { 
			g = it->second[0];
			vector<int> slots = it->second;
			slots.erase(slots.begin());
			for (int t = 0; t < g; ++t) {
				hospital_choice.push_back(make_pair(slots[t], it->first));
			}
		}
		hospital_choice = stabalize_matches(hospital_choice); // call recursive conflict resolution unction
		for (vector<pair<int, int>>::iterator itt = hospital_choice.begin(); itt != hospital_choice.end(); ++itt) {
			stableMatchings.push_back(make_pair(itt->second, itt->first)); // switch pairs so its (HOSPITAL, STUDENT) order and insert into final vector
		}
		return stableMatchings;
	}

	vector<pair<int, int>> stabalize_matches(vector<pair<int, int>> &hospital_choice) {
		vector<int> stu_vec, hos_vec; // initalize 2 empty vector
		int flag = 0; 
		sort(hospital_choice.begin(), hospital_choice.end()); // sort vector by student (First value in the pair)
		for (itt = hospital_choice.begin(); itt != hospital_choice.end(); ++itt) {
			stu_vec.push_back(itt->first); 
			hos_vec.push_back(itt->second); 
		}
		hospital_choice.clear(); // empty hospital vec
		for (int i = 1; i < stu_vec.size(); ++i) {
			if (stu_vec[i - 1] == stu_vec[i]) { // find conflict in matchings
				flag = flag + 1; // increment flag to make it recursive
				int conflict = stu_vec[i]; // store conflicting student
				int hos_1_rank = 0, hos_2_rank = 0;
				int hos_1 = hos_vec[i - 1]; int hos_2 = hos_vec[i]; // store conflicting hospital one // store conflicting hospital two
				vector<int> hos1_temp_vec = hospitals.find(hos_1)->second; vector<int> hos2_temp_vec = hospitals.find(hos_2)->second; // store conflicting hospital one's & two's student ranking 
				hos1_temp_vec.erase(hos1_temp_vec.begin()); hos2_temp_vec.erase(hos2_temp_vec.begin()); // remove number of slots index from each hospital
				vector<int> temp3 = students.find(conflict)->second; // store student ranking
				if (hos_1 == hos_2) { // if the same student is in the same hospital
					vector<int> hos1_temp_vec = hospitals.find(hos_1)->second;
					hos1_temp_vec.erase(hos1_temp_vec.begin());
					for (int s = 0; s < hos1_temp_vec.size(); ++s) {
						if (hos1_temp_vec[s] == conflict) {
							stu_vec[i] = hos1_temp_vec[s + 1];
							break;
						}
					}
					break;
					//continue;
				}
				for (int l = 0; l < temp3.size(); ++l) { // iterate over student ranking vector
					if (temp3[l] == hos_1) {
						hos_1_rank = l;
					}
					if (temp3[l] == hos_2) {
						hos_2_rank = l;
					}
				}
				if (hos_1_rank > hos_2_rank) { // student wants hospital 2 more than hospital 1, change hospital 1's student
					for (int s = 0; s < hos1_temp_vec.size(); ++s) {
						if (hos1_temp_vec[s] == conflict) {
							stu_vec[i - 1] = hos1_temp_vec[s + 1];
							break;
						}
					}
				}
				else if (hos_1_rank < hos_2_rank) { // student wants hospital 1 more than hospital 2, change hospital 2's student
					for (int s = 0; s < hos2_temp_vec.size(); ++s) {
						if (hos2_temp_vec[s] == conflict) {
							stu_vec[i] = hos2_temp_vec[s + 1];
							break;
						}
					}
				}
				break;
			}
		}
		for (int j = 0; j < stu_vec.size(); ++j) {
			hospital_choice.push_back(make_pair(stu_vec[j], hos_vec[j])); // zip back up
		}
		if (flag > 0) {
			stabalize_matches(hospital_choice);
		} // recursive call
		return hospital_choice; // found stable match
	}
};

