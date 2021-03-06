#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <istream>
#include <map>

using namespace std;

map <string, string> m;
map < pair < string, string >, int > m_t; //station1_external_code , station2_external_code, time_second

vector<string> f_csv_read_row(istream& file, char delimiter)
{
    stringstream ss;
    bool inquotes = false;
    vector<string> row;
    while (file.good()) {
        char c = file.get();
        if (!inquotes && c == '"') {
            inquotes = true;
        }
        else if (inquotes && c == '"') {
            if (file.peek() == '"'){
                ss << (char)file.get();
            }
            else {
                inquotes = false;
            }
        }

        else if (!inquotes && c == delimiter) {
            row.push_back(ss.str());
            ss.str("");
        }

        else if (!inquotes && (c == '\r' || c == '\n')) {
            if (file.peek() == '\n') {
                file.get();
            }
            row.push_back(ss.str());
            return row;
        }
        else {
            ss << c;
        }
    }
}

int f_calc_time(string t1, string t2) {
    if (t1.length() == 5) t1 = '0' + t1;
    if (t2.length() == 5) t2 = '0' + t2;

    if (stoi(t1) < stoi(t2)) swap(t1, t2);

    int hour = stoi(t1.substr(0, 2)) - stoi(t2.substr(0, 2));
    int minute = stoi(t1.substr(2, 2)) - stoi(t2.substr(2, 2));
    int second = stoi(t1.substr(4, 2)) - stoi(t2.substr(4, 2));

    if (hour > 0) {
        hour--;
        minute += 60;
    }
    while (minute) {
        minute--;
        second += 60;
    }
    return second;
}

void f_check_all() {
    cout << "-------------No Path-------------" << "\n";
    for (auto iter : m) {
        //P144-1
        bool P = false; // P = true;
        int external_code; // 144
        int external__code = 0; // 1

        if (iter.first[0] != 'P' && iter.first[iter.first.size() - 2] != '-') { // 102 bosan
            external_code = stoi(iter.first);
        }
        else if (iter.first[0] == 'P' && iter.first[iter.first.size() - 2] != '-') { // P142 gasan Digital
            P = true;
            external_code = stoi(iter.first.substr(1, 3));
        }
        else if (iter.first[0] != 'P' && iter.first[iter.first.size() - 2] == '-') { // 211-1 yongdap
            external_code = stoi(iter.first.substr(0, 3));
            external__code = stoi(iter.first.substr(iter.first.length() - 1));
        }
        else { // p144-1 Gwangmyeong 
            P = true;
            external_code = stoi(iter.first.substr(1, 3));
            external__code = stoi(iter.first.substr(iter.first.length() - 1));
        }
        
        
        if (external__code == 0 && P == false) { // 102 bosan
            string niter = to_string(external_code + 1);

            if (m.find(niter) != m.end()) {
                if (m_t.find(make_pair(iter.first, niter)) == m_t.end()) {
                    cout << iter.first << " " << niter << "\n";
                }
            }

            if ((m.find("P" + niter) != m.end()) && (m.find("P" + to_string(external_code)) == m.end())) {
                if (m_t.find(make_pair(iter.first, "P" + niter)) == m_t.end()) {
                    cout << iter.first << " " << "P" + niter << "\n";
                }
            }

            if (m.find(to_string(external_code) + "-1") != m.end()) {
                if (m_t.find(make_pair(iter.first, to_string(external_code) + "-1")) == m_t.end()) {
                    cout << iter.first << " " << to_string(external_code) + "-1" << "\n";
                }
            }
        }
        else if (external__code == 0 && P == true) { // P142 gasan Digital
            string niter = "P" + to_string(external_code + 1);
            if (m.find(niter) != m.end()) {
                if (m_t.find(make_pair(iter.first, niter)) == m_t.end()) {
                    cout << iter.first << " " << niter << "\n";
                }
            }
            if (m.find("P" + to_string(external_code) + "-1") != m.end()) {
                if (m_t.find(make_pair(iter.first, "P" + to_string(external_code) + "-1")) == m_t.end()) {
                    cout << iter.first << " " << "P" + to_string(external_code) + "-1" << "\n";
                }
            }
        }
        else if (external__code != 0 && P == false) { // 211-1 yongdap
            string niter = to_string(external_code) + "_" + to_string(external__code + 1);
            if (m.find(niter) == m.end()) {
                if (m_t.find(make_pair(iter.first, niter)) != m_t.end()) {
                    cout << iter.first << " " << to_string(external__code + 1) << "\n";
                }
            }
        }

        else {// p144-1 Gwangmyeong 
            cout << "error : " << iter.first << "\n";
            //not have  next station
        }
    }
}

int f_initialize() {
    ifstream file1(".\\subway.csv");
    if (file1.fail()) {
        return (cout << "???? ?????? ???????? ?????? ???????? ????????." << endl) && 0;
    }

    vector<string> row = f_csv_read_row(file1, ','); //except first line

    while (file1.good()) {
        vector<string> row = f_csv_read_row(file1, ',');
        for (int i = 0, leng = row.size(); i < leng; i++) {
            m.insert(make_pair(row[3], row[1]));
        }
    }
    file1.close();

    ifstream file2(".\\subway_first.csv");
    if (file2.fail()) {
        return (cout << "???? ?????? ???????? ?????? ???????? ????????." << endl) && 0;
    }

    vector<string> row1 = f_csv_read_row(file2, ','); //except first line
    while (file2.good()) {
        vector<string> row2 = f_csv_read_row(file2, ',');
        if (m.find(row2[4]) == m.end()) {
            cout << "empty data : " << row2[4] << "\n";
            //Handling Empty Data Exceptions
            //Result : None
        }
        else {
            if (row2[9] == row1[9] && row2[10] == row1[10]) {
                m_t.insert(make_pair(make_pair(row2[4], row1[4]), f_calc_time(row2[6], row1[6])));
                m_t.insert(make_pair(make_pair(row1[4], row2[4]), f_calc_time(row2[6], row1[6])));
            }
            else if (row2[12] == row1[12] && row2[13] == row1[13]) {
                m_t.insert(make_pair(make_pair(row1[4], row2[4]), f_calc_time(row2[11], row1[11])));
                m_t.insert(make_pair(make_pair(row2[4], row1[4]), f_calc_time(row2[11], row1[11])));
            }
        }
        row1 = row2;
    }
    file2.close();

    ifstream file3(".\\subway_exception.csv");
    if (file3.fail()) {
        return (cout << "???? ?????? ???????? ?????? ???????? ????????." << endl) && 0;
    }

    vector<string> row3 = f_csv_read_row(file3, ','); //except first line
    while (file3.good()) {
        vector<string> row3 = f_csv_read_row(file3, ',');
        if (m_t.find(make_pair(row3[1], row3[3])) != m_t.end()) cout << "already exist" << "\n";
        else {
            m_t.insert(make_pair(make_pair(row3[1], row3[3]), stoi(row3[5])));
            m_t.insert(make_pair(make_pair(row3[3], row3[1]), stoi(row3[5])));
        }
    }
    file3.close();

    f_check_all();
    cout << "-----------all path -----------" << "\n";
    for (auto iter : m_t) {
        cout << iter.first.first << " " << iter.first.second << " " << iter.second << "\n";
        auto test = m_t.find(make_pair(iter.first.second, iter.first.first));
        if (test == m_t.end()) cout << "path error" << "\n";
        if (test->second != iter.second) cout << "path error";
    }
}

int main()
{
    f_initialize();
    return 0;
}

