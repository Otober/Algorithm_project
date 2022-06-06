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
        return (cout << "�ش� ��ο� ��ġ�ϴ� ������ �������� �ʽ��ϴ�." << endl) && 0;
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
        return (cout << "�ش� ��ο� ��ġ�ϴ� ������ �������� �ʽ��ϴ�." << endl) && 0;
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
    for (auto iter : m_t) {
        cout << iter.first << " ";
        for (int i = 0; i < iter.second.size(); i++) {
            cout << iter.second[i].first << " " << iter.second[i].second << " ";
        }
        cout << "\n";
    }
    while (true) {
        string st, et;
        vector < string > st_n, et_n;
        while (true) {
            cout << "Please input the departure subway station name : ";
            //cin >> st;
            st = "�湫��";
            if (m_n.find(st) != m_n.end()) {
                st_n = m_n.find(st)->second;
                break;
            }
            else cout << "Wrong name" << "\n";
        }
        
        while (true) {
            cout << "Please input the arrival subway station name : ";
            //cin >> et;
            et = "���￪";
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