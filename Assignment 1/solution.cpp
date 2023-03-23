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
#define E1 "Error:\nE1: A state '' is not in the set of states\n"
#define E2 "Error:\nE2: Some states are disjoint\n"
#define E3 "Error:\nE3: A transition '' is not represented in the alphabet\n"
#define E4 "Error:\nE4: Initial state is not defined\n"
#define E5 "Error:\nE5: Input file is malformed\n"
#define COMPLETE "FSA is complete\n"
#define INCOMPLETE "FSA is incomplete\n"
#define W1 "W1: Accepting state is not defined\n"
#define W2 "W2: Some states are not reachable from the initial state\n"
#define W3 "W3: FSA is nondeterministic\n"

// Testing the elements of the array
void displayArr(vector<string> arr) {
    for(string i : arr) {
        cout<<i<<" : ";
    }
    cout<<endl;
}

// Display Error or Warnings
// temp is for E1 and E2 errors for inserting string inside
// finish is for Errors only
void displayMessage(ofstream& myFile, string result, string temp="", bool finish=false) {
    if(result==E1) {
        result.insert(20, temp);
    } else if(result==E3) {
        result.insert(25, temp);
    }
    // Output
    myFile << result;
    if(finish) {
        myFile.close();
        exit(0);
    }
}

// Splitting process of string into array
// chars are the main characters of the split
// order is for special operations (switch case)
// &arr splitted result
// e5 check if input valid
void makeSplit(string str, vector<char> chars, int order, vector<string>& arr, bool &e5) {
    // clean whitespaces
    while(str[str.size()-1] == ' ') str.erase(str.size()-1, 1);
    // if no close brace input
    if(order<=4 && str[str.size()-1] != ']') {
        e5=true;
    }
    // check the existence of character
    map<char, bool> check;
    // splitting characters
    for(char i : chars) check[i] = true;
    // split process
    string temp="";
    string name="";
    // if order is less than or equal to 4
    // ignore the first strings ("states=", "alpha=", etc)
    bool isOk=(order>4);
    rep(i, ll(str.size())) {
        if(check[str[i]]) {
            if(isOk) {
                // push the element to the array
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
    // check names, Input validation
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

// Check lastin letters and numbers only
bool checkLatin(string str) {
    bool isTrue = true;
    for(char i:str) {
        isTrue &= ((i>='a' && i<='z') || (i>='A' && i<='Z') || (i>='0' && i<='9'));
    }
    return isTrue;
}

// Check latin letters, numbers and _ (underscore) only
bool checkLatinChar(string str) {
    bool isTrue = true;
    for(char i:str) {
        isTrue &= ((i>='a' && i<='z') || (i>='A' && i<='Z') || (i>='0' && i<='9') || i=='_');
    }
    return isTrue;
}

int main(){
    // Input file
    ifstream inFile ("fsa.txt");
    // Output file
    ofstream myFile;
    myFile.open ("result.txt");

    string str;
    vector<string> states, alpha, init, fin, trans;
    vector<vector<string>> arr = {states, alpha, init, fin, trans};
    vector<vector<string>> transitions;
    set<string> checkDisjoint;
    bool e5=false;
    bool w1=false,w2=false,w3=false;
    bool isComplete = false;

    // split array
    rep(i, ll(5)) {
        getline(inFile, str);
        makeSplit(str, {'[',',',']'}, i, arr[i], e5);
        // states and transitions
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
    rep(i, ll(5)) {
        if(i==0) {
            // Input validation for states
            // Check E5: Input
            for(string state : arr[i]) {
                e5 |= !checkLatin(state);
            }
            if(e5) displayMessage(myFile, E5, "", true);
        }
        else if(i==1) {
            // Input validation for alphabets
            // Check E5: Input
            for(string transition : arr[1]) {
                e5 |= !checkLatinChar(transition);
            }
            if(e5) displayMessage(myFile, E5, "", true);
        }
        else if(i==2) {
            // Initial state validation
            // check states
            if(arr[i].size() == 0) displayMessage(myFile, E4, "", true);
            if(arr[i].size() > 1) displayMessage(myFile, E5, "", true);
            string temp = arr[i][0];
            bool isExist = false;
            for(string state : arr[0]) {
                isExist |= (state==temp);
            }
            // If there is inappropriate/wrong state
            if(!isExist) displayMessage(myFile, E1, temp, true);
        } else if(i==3) {
            // Final state validation
            // check states
            for(string temp : arr[i]){
                bool isExist = false;
                for(string state : arr[0]) {
                    isExist |= (state==temp);
                }
                // If there is inappropriate/wrong state
                if(!isExist) displayMessage(myFile, E1, temp, true);
            }
        }else if(i==4) {
            // check states
            for(vector<string> tran : transitions) {
                // Exception
                if(transitions.size()>1 && (tran.size()==0)) displayMessage(myFile, E5, "", true);
                if(tran.size()==0) continue;
                bool isExist = true;
                for(int j=0; j<=2; j+=1){
                    if(j==0 || j==2) {
                        // States
                        bool tempExist=false;
                        for(string state : arr[0]) {
                            tempExist |= (state==tran[j]);
                        }
                        isExist &= tempExist;
                        // If there is inappropriate/wrong state
                        if(!isExist) displayMessage(myFile, E1, tran[j], true);
                    } else {
                        // transitions
                        // Check E3: transition
                        bool isExist = false;
                        for(string alphabet : arr[1]) {
                            isExist |= (alphabet==tran[j]);
                        }
                        if(!isExist) {
                            if(!isExist) displayMessage(myFile, E3, tran[j], true);
                        };
                    }
                }
            }
        }
    }

    // Check E2 - disjoint
    // ADTs for storing joinable states
    map<string, bool> joinable;
    set<string> joined;
    // initial state is always joinable
    joinable[arr[2][0]] = true;
    joined.insert(arr[2][0]);
    ll cnt;
    do {
        cnt=0;
        for(vector<string> tran : transitions) {
            if(joinable[tran[0]] != joinable[tran[2]]) {
                // if joinable then add
                joinable[tran[0]] = true;
                joined.insert(tran[0]);
                joinable[tran[2]] = true;
                joined.insert(tran[2]);
                cnt++;
            }
        }
    } while(cnt!=0);
    // Check
    if(joined.size() != arr[0].size()) {
        // disjoint
        displayMessage(myFile, E2, "", true);
    }


    /* REPORT AND WARNINGS */

    // Check W1 - accepting state
    if(arr[3].size()==0) {
        // no accepting state
        w1=true;
    }

    // Check W2 - not reachable
    // ADTs for storing reachable states
    map<string, bool> reachable;
    set<string> reached;
    // initial state is always reachable
    reachable[arr[2][0]] = true;
    reached.insert(arr[2][0]);
    cnt = 0;
    do {
        cnt=0;
        for(vector<string> tran : transitions) {
            if(reachable[tran[0]] && !reachable[tran[2]]) {
                // if reachable then add
                reachable[tran[2]] = true;
                reached.insert(tran[2]);
                cnt++;
            }
        }
    } while(cnt!=0);
    // Check
    if(reached.size() != arr[0].size()) {
        // not reachable
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
        // if all transitions number is equal to unique transitions -> deterministic
        if(prevSize == ll(mapChecking[tran[0]].uniTrans.size())) w3=true;
        mapChecking[tran[0]].numOfTrans++;
    }

    for(string i : arr[0]) {
        if(mapChecking.find(i)==mapChecking.end()) {
            // if some transition ignored -> incomplete
            isComplete = false;
            break;
        }
        st elem = mapChecking[i];

        if(elem.numOfTrans >= ll(arr[1].size()) &&
            elem.uniTrans.size() == arr[1].size()) {
                isComplete = true;
            } else {
                isComplete = false;
                break;
            }
    }

    // TODO: consider all conditions
    if(isComplete) displayMessage(myFile, COMPLETE);
    else displayMessage(myFile, INCOMPLETE);
    if(w1 || w2 || w3) displayMessage(myFile, WARNING);
    if(w1) displayMessage(myFile, W1);
    if(w2) displayMessage(myFile, W2);
    if(w3) displayMessage(myFile, W3);
    myFile.close();
    return 0;
}
