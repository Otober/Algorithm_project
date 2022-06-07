#include "main.h"
#include <queue>

map < string, pair < double, double > > m_c; // external code , x , y

double f_dist(string now, string next) {
    pair < double, double > a = m_c.find(now)->second;
    pair < double, double > b = m_c.find(next)->second;
    return ((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second)) * 1404878;
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

    int cnt = 0;
    while (file5.good()) {
        cnt++;
        cout << cnt << "\n";
        vector<string> row5 = f_csv_read_row(file5, ',');
        string nstation = row5[6];
        auto iter = m_t.find(row5[5]);
        auto f_iter = find_if(iter->second.begin(), iter->second.end(), [&nstation](const pair<string, int>elem) {return elem.first == nstation; });
        if (f_iter == iter->second.end()) {
            vector < pair < string, int > > temp = iter->second;
            temp.push_back(make_pair(row5[6], stoi(row5[4])));
            m_t.erase(iter);
            m_t.insert(make_pair(row5[5], temp));
        }
        nstation = row5[5];
        iter = m_t.find(row5[6]);

        f_iter = find_if(iter->second.begin(), iter->second.end(), [&nstation](const pair<string, int>elem) {return elem.first == nstation; });
        if (f_iter == iter->second.end()) {
            vector < pair < string, int > > temp = iter->second;
            temp.push_back(make_pair(row5[5], stoi(row5[4])));
            m_t.erase(iter);
            m_t.insert(make_pair(row5[6], temp));
        }
    }

    file5.close();

    return 0;
}

int main() {
    f_initialize();
    f_coordinates();
    vector < string > closed;
    vector < string > ans;
    priority_queue < pair < double, string> > pq; // dist(double) external code
    for (auto iter : m_t) {
        cout << iter.first << " ";
        for (int i = 0; i < iter.second.size(); i++) {
            cout << iter.second[i].first << " " << iter.second[i].second << " ";
        }
        cout << "\n";
    }
    bool flag1 = true;
    while (flag1) {
        flag1 = false;
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
        bool flag = true;
        pq.push(make_pair(-0.0, st_n[0]));
        while (flag) {
            string now = pq.top().second;
            double cost = -pq.top().first;
            pq.pop();

            closed.push_back(now);
            ans.push_back(now);
            pq = priority_queue < pair < double, string> >();

            for (int i = 0; i < et_n.size(); i++) {
                if (et_n[i] == ans.back()) flag = false;
            } 

            auto iter = m_t.find(now);
            for (int i = 0; i < iter->second.size(); i++) {
                string next = iter->second[i].first;
                if (find(closed.begin(), closed.end(), next) != closed.end()) continue;
                pq.push(make_pair( -(f_dist(now, et_n[0]) + iter->second[i].second), next));
            }
        }
        if (ans.size() >= 2) {
            if (m.find(ans[0])->second == m.find(ans[1])->second) ans.erase(ans.begin());
            if (m.find(ans[ans.size() - 1])->second == m.find(ans[ans.size() - 2])->second) ans.erase(ans.begin());
        }
        for (int i = 0; i < ans.size(); i++) {
            cout << ans[i] << " ";
        }
    }
}