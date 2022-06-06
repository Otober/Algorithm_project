#include "main.h"
#include <queue>

map < string, pair < double, double > > m_c; // external code , x , y
map < string, int > visit; // external code , dist(double)

priority_queue < pair < double, string> > pq; // dist(double) external code

double f_dist(pair <double, double > a, pair < double, double > b) {
    return (a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second);
}

int f_coordinates() {
    ifstream file4(".\\subway_coordinates.csv");
    if (file4.fail()) {
        return (cout << "해당 경로에 위치하는 파일이 존재하지 않습니다." << endl) && 0;
    }

    while (file4.good()) {
        vector<string> row4 = f_csv_read_row(file4, ',');
        if (row4[3] == "#N/A") continue;
        if (m.find(row4[3]) != m.end()) {
            m_c.insert(make_pair(row4[3], make_pair(stod(row4[1]), stod(row4[2]))));
        }
        else cout << "Error : " << row4[0] << "\n";
    }

    for (auto iter : m) {
        if (m_c.find(iter.first) == m_c.end()) cout << iter.first << '\n';
    }
    file4.close();

    ifstream file5(".\\subway_transfer.csv");
    if (file5.fail()) {
        return (cout << "해당 경로에 위치하는 파일이 존재하지 않습니다." << endl) && 0;
    }

    vector<string> row5 = f_csv_read_row(file5, ',');

    while (file5.good()) {
        vector<string> row5 = f_csv_read_row(file5, ',');
        vector < string > temp = m_n.find(row5[1])->second;

        for (int i = 0; i < temp.size(); i++) {
            for (int j = 0; j < temp.size(); j++) {
                if (i == j) continue;
                
                
                //m_t.insert(make_pair(make_pair(temp[i], temp[j]), stoi(row5[5])));
            }
        }
    }

    file5.close();

    return 0;
}

int main() {
    f_initialize();
    f_coordinates();
    while (true) {
        string st, et;
        vector < string > st_n, et_n;
        while (true) {
            cout << "Please input the departure subway station name : ";
            //cin >> st;
            st = "충무로";
            if (m_n.find(st) != m_n.end()) {
                st_n = m_n.find(st)->second;
                break;
            }
            else cout << "Wrong name" << "\n";
        }
        
        while (true) {
            cout << "Please input the arrival subway station name : ";
            //cin >> et;
            et = "서울역";
            if (m_n.find(et) != m_n.end()) {
                et_n = m_n.find(et)->second;
                break;
            }
            else cout << "Wrong name" << "\n";
        }
        visit.clear();
        while (!pq.empty()) {
            pq.pop();
        }
        
        auto et_iter = m_c.find(et_n[0]);
        for (int i = 0; i < st_n.size(); i++) {
            auto iter = m_c.find(st_n[i]);
            pq.push(make_pair(-f_dist(iter->second, et_iter->second), iter->first));
            visit.insert(make_pair(iter->first, f_dist(iter->second, et_iter->second)));
        }

        while (pq.empty()) {
            double cost = pq.top().first;
            string now = pq.top().second;
            pq.pop();
        }

    }
}