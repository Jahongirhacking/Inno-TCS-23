// Jahongir Hayitov CS-01
#include"bits/stdc++.h"
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
#define rep(i,n) for (ll i = 0; i < (n); ++i)
#define pb(x) push_back(x);
#define ENDL cout<<endl;
#define WARNING "Warning:\n"
#define ERROR "Error:\n"
#define E1 "E1: A state '' is not in the set of states\n"
#define E2 "E2: Some states are disjoint\n"
#define E3 "E3: A transition '' is not represented in the alphabet\n"
#define E4 "E4: Initial state is not defined\n"
#define E5 "E5: Input file is malformed\n"
#define COMPLETE "FSA is complete\n"
#define INCOMPLETE "FSA is incomplete\n"
#define W1 "W1: Accepting state is not defined\n"
#define W2 "W2: Some states are not reachable from the initial state\n"
#define W3 "W3: FSA is nondeterministic\n"

ll toInt(string str){
    ll n=0;
    for(char i:str){
        n*=10;
        n+=ll(i-'0');
    }
    return n;
}

void displayArr(vector<string> arr) {
    for(string i : arr) {
        cout<<i<<" : ";
    }
    cout<<endl;
}

void displayMessage(ofstream& myFile, string result, string temp="") {
    if(result==E1) {
        result.insert(13, temp);
    } else if(result==E3) {
        result.insert(18, temp);
    }

    myFile << result;
}

void makeSplit(string str, vector<char> chars, int order, vector<string>& arr, bool &e5) {
    // clean whitespaces
    while(str[str.size()-1] == ' ') str.erase(str.size()-1, 1);
    // if no close brace input
    if(order<=4 && str[str.size()-1] != ']') {
        e5=true;
    }
    map<char, bool> check;
    // splitting characters
    for(char i : chars) check[i] = true;
    // split process
    string temp="";
    string name="";
    bool isOk=(order>4);
    rep(i, str.size()) {
        if(check[str[i]]) {
            if(isOk) {
               if(temp.size()>0) arr.push_back(temp);
            }
            else name=temp;
            temp = "";
            isOk = true;
            continue;
        }
        temp += str[i];
    }
    // extra check
    if(temp.size()>0) {
        arr.push_back(temp);
    }
    // check names
    switch(order) {
        case 0:
            if(name!="states=") e5=true;
            break;
        case 1:
            if(name!="alpha=") e5=true;
            break;
        case 2:
            if(name!="init.st=") e5=true;
            break;
        case 3:
            if(name!="fin.st=") e5=true;
            break;
        case 4:
            if(name!="trans=") e5=true;
            break;
        default:
            break;
    }
}

bool checkLatin(string str) {
    bool isTrue = true;
    for(char i:str) {
        isTrue &= (i>='a' && i<='z' || i>='A' && i<='Z' || i>='0' && i<='9');
    }
    return isTrue;
}

bool checkLatinChar(string str) {
    bool isTrue = true;
    for(char i:str) {
        isTrue &= (i>='a' && i<='z' || i>='A' && i<='Z' || i>='0' && i<='9' || i=='_');
    }
    return isTrue;
}

int main(){
    ifstream inFile ("fsa.txt");
    // Output
    ofstream myFile;
    myFile.open ("result.txt");

    string str;
    vector<string> states, alpha, init, fin, trans;
    vector<vector<string>> arr = {states, alpha, init, fin, trans};
    vector<vector<string>> transitions;
    set<string> checkDisjoint;
    bool e1=false,e2=false,e3=false,e4=false,e5=false;
    bool w1=false,w2=false,w3=false;
    bool isComplete = false;
    set<string> tempForE1, tempForE3;

    // split array
    rep(i, 5) {
        getline(inFile, str);
        makeSplit(str, {'[',',',']'}, i, arr[i], e5);

        if(i==4) {
            for(string temp : arr[i]) {
                vector<string> tran;
                makeSplit(temp, {'>'}, 5, tran, e5);
                // Input Error
                if(tran.size()!=3) e5=true;
                transitions.push_back(tran);
            }

        }
    }


    // Check E1: A state  is not in the set of states
    rep(i, 5) {
        if(i==2 || i==3) {
            // check states
            for(string temp : arr[i]) {
                // Exception
                if(temp=="") continue;

                bool isExist = false;
                for(string state : arr[0]) {
                    isExist |= (state==temp);
                }
                // If there is inappropriate/wrong state
                if(!isExist) {
                    tempForE1.insert(temp);
                    e1=true;
                }
            }
        } else if(i==4) {
            // check states
            for(vector<string> tran : transitions) {
                // Exception
                if(transitions.size()>1 && (tran.size()==0)) e5=true;
                if(tran.size()==0) continue;
                bool isExist = true;
                for(int j=0; j<=2; j+=2){
                    bool tempExist=false;
                    for(string state : arr[0]) {
                        tempExist |= (state==tran[j]);
                    }
                    isExist &= tempExist;
                    // If there is inappropriate/wrong state
                    if(!isExist) {
                        tempForE1.insert(tran[j]);
                        e1=true;
                    }
                }
            }
        }
    }

    // Check E5: Input
    for(string i : arr[0]) {
        e5 |= !checkLatin(i);
    }
    for(string i : arr[1]) {
        e5 |= !checkLatinChar(i);
    }

    // Check E2: Some states are disjoint
    if(transitions.size()==0 && arr[0].size()>1) {
        e2=true;
    }

    // Check E3: transition
    for(vector<string> tran : transitions) {
        // Exception
        bool isExist = false;
        for(string alphabet : arr[1]) {
            isExist |= (alphabet==tran[1]);
        }
        if(!isExist) {
            tempForE3.insert(tran[1]);
            e3 = true;
        };
    }

    // Check E2: Some states are disjoint
    for(vector<string> tran : transitions) {
        if(tran[0]!=tran[2]){
            checkDisjoint.insert(tran[0]);
            checkDisjoint.insert(tran[2]);
        }
    }
    if(arr[0].size()!=1 && arr[0].size()!=checkDisjoint.size()) {
        e2=true;
    }

    // Check E3: initial state
    if(arr[2].size()!=1) e4=true;

    if(e1 || e2 || e3 || e4 || e5) {
        displayMessage(myFile, ERROR);
        if(e1) {
            for(auto temp1:tempForE1) displayMessage(myFile, E1, temp1);
        }
        if(e2) displayMessage(myFile, E2);
        if(e3) {
            for(auto temp3:tempForE3) displayMessage(myFile, E3, temp3);
        }
        if(e4) displayMessage(myFile, E4);
        if(e5) displayMessage(myFile, E5);
        myFile.close();
        return 0;
    }



    // Check W1 - accepting state
    if(arr[3].size()==0) {
        w1=true;
    }

    // Check W2 - not reachable
    map<string, bool> reachable;
    set<string> reached;
    reachable[arr[2][0]] = true;
    reached.insert(arr[2][0]);
    ll cnt;
    do {
        cnt=0;
        for(vector<string> tran : transitions) {
            if(reachable[tran[0]] && !reachable[tran[2]]) {
                reachable[tran[2]] = true;
                reached.insert(tran[2]);
                cnt++;
            }
        }
    } while(cnt!=0);
    // Check
    if(reached.size() != arr[0].size()) {
        w2=true;
    }

    // Check W3 - nondeterministic
    struct st {
        ll numOfTrans=0;
        set<string> uniTrans;
    };
    map<string, st> mapChecking;

    for(vector<string> tran : transitions) {
        ll prevSize = mapChecking[tran[0]].uniTrans.size();
        mapChecking[tran[0]].uniTrans.insert(tran[1]);
        // extra
        if(prevSize == mapChecking[tran[0]].uniTrans.size()) w3=true;
        mapChecking[tran[0]].numOfTrans++;
    }

    for(string i : arr[0]) {
        if(arr[0].size()==1) {
            isComplete=true;
            break;
        }
        if(mapChecking.find(i)==mapChecking.end()) {
            isComplete = false;
            break;
        }
        st elem = mapChecking[i];

        if(elem.numOfTrans >= arr[1].size() &&
            elem.uniTrans.size() == arr[1].size()) {
                isComplete = true;
            } else {
                isComplete = false;
                break;
            }
    }

    // complete
    if(isComplete) displayMessage(myFile, COMPLETE);
    else displayMessage(myFile, INCOMPLETE);
    if(w1 || w2 || w3) displayMessage(myFile, WARNING);
    if(w1) displayMessage(myFile, W1);
    if(w2) displayMessage(myFile, W2);
    if(w3) displayMessage(myFile, W3);
}
