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
#define E1 "E1: Input file is malformed\n"
#define E2 "E2: Initial state is not defined\n"
#define E3 "E3: Set of accepting states is empty\n"
#define E4 "E4: A state '' is not in the set of states\n"
#define E5 "E5: A transition '' is not represented in the alphabet\n"
#define E6 "E6: Some states are disjoint\n"
#define E7 "E7: FSA is nondeterministic\n"
#define COMPLETE "FSA is complete\n"
#define INCOMPLETE "FSA is incomplete\n"
#define W2 "W2: Some states are not reachable from the initial state\n"
#define EPS "eps"
#define NONE "{}"

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
void displayMessage(string result, string temp="", bool finish=false) {
    if(result==E4) {
        result.insert(13, temp);
    } else if(result==E5) {
        result.insert(18, temp);
    }
    // Output
    cout << result;
    if(finish) {
        exit(0);
    }
}

// Splitting process of string into array
// chars are the main characters of the split
// order is for special operations (switch case)
// &arr splitted result
// e5 check if input valid
void makeSplit(string str, vector<char> chars, int order, vector<string>& arr, bool &e1) {
    // clean whitespaces
    while(str[str.size()-1] == ' ') str.erase(str.size()-1, 1);
    // if no close brace input
    if(order<=4 && str[str.size()-1] != ']') {
        e1=true;
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
            if(name!="states=") e1=true;
            break;
        case 1:
            if(name!="alpha=") e1=true;
            break;
        case 2:
            if(name!="initial=") e1=true;
            break;
        case 3:
            if(name!="accepting=") e1=true;
            break;
        case 4:
            if(name!="trans=") e1=true;
            break;
        default:
            break;
    }
}

// Check latin letters and numbers only
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
    ifstream inFile ("input.txt");
    string str;
    vector<string> states, alpha, init, fin, trans;
    vector<vector<string>> arr = {states, alpha, init, fin, trans};
    vector<vector<string>> transitions;
    map<string, ll> indexOfState;
    string INITIAL;
    set<string> checkDisjoint;
    bool e1=false;

    // GET THE INPUT AND SPLIT IT
    rep(i, ll(5)) {
        getline(inFile, str);
        makeSplit(str, {'[',',',']'}, i, arr[i], e1);
        // states and transitions
        if(i==4) {
            for(string temp : arr[i]) {
                vector<string> tran;
                makeSplit(temp, {'>'}, 5, tran, e1);
                // Input Error
                if(tran.size()!=3) e1=true;
                transitions.push_back(tran);
            }

        }
    }

    // CHECKING PROCESS
    rep(i, ll(5)) {
        if(i==0) {
            // Input validation for states
            // Check E1: Input
            if(arr[i].size() == 0) e1=true;
            for(string state : arr[i]) {
                e1 |= !checkLatin(state);
            }
            if(e1) displayMessage(E1, "", true);
        }
        else if(i==1) {
            // Input validation for alphabets
            // Check E1: Input
            for(string transition : arr[1]) {
                e1 |= !checkLatinChar(transition);
            }
            if(e1) displayMessage(E1, "", true);
        }
        else if(i==2) {
            // Initial state validation E2
            // check states
            if(arr[i].size() == 0) displayMessage(E2, "", true);
            if(arr[i].size() > 1) displayMessage(E1, "", true);
            string temp = arr[i][0];
            bool isExist = false;
            for(string state : arr[0]) {
                isExist |= (state==temp);
            }
            // If there is inappropriate/wrong state E4
            if(!isExist) displayMessage(E4, temp, true);
            INITIAL = temp;
        } else if(i==3) {
            // Accepting state validation
            // check states
            if(arr[i].size() == 0) displayMessage(E3, "", true);
            for(string temp : arr[i]){
                bool isExist = false;
                for(string state : arr[0]) {
                    isExist |= (state==temp);
                }
                // If there is inappropriate/wrong state E4
                if(!isExist) displayMessage(E4, temp, true);
            }
        }else if(i==4) {
            // check transitions
            for(vector<string> tran : transitions) {
                // Exception
                if(transitions.size()>1 && (tran.size()==0)) displayMessage(E1, "", true);
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
                        // If there is inappropriate/wrong state E4
                        if(!isExist) displayMessage(E4, tran[j], true);
                    } else {
                        // transitions
                        // Check E5: transition
                        bool isExist = false;
                        for(string alphabet : arr[1]) {
                            isExist |= (alphabet==tran[j]);
                        }
                        if(!isExist) displayMessage(E5, tran[j], true);
                    }
                }
            }
        }
    }

    // Check E6 - disjoint
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
        displayMessage(E6, "", true);
    }

    // Check E7 - nondeterministic
    struct st {
        ll numOfTrans=0;
        set<string> uniTrans;
    };
    map<string, st> mapChecking;

    for(vector<string> tran : transitions) {
        ll prevSize = mapChecking[tran[0]].uniTrans.size();
        mapChecking[tran[0]].uniTrans.insert(tran[1]);
        // if all transitions number is equal to unique transitions -> deterministic
        if(prevSize == ll(mapChecking[tran[0]].uniTrans.size()))  {
            // nondeterministic
            displayMessage(E7, "", true);
        }
        mapChecking[tran[0]].numOfTrans++;
    }

    // ###########################################################
    // TODO: CALCULATION PART

    ll numberOfStates = arr[0].size();
    // Make initial state at 0 index
    for(ll i=0; i<numberOfStates; i++) {
        string state = arr[0][i];
        if(state == INITIAL) {
            swap(arr[0][0], arr[0][i]);
            break;
        }
    }
    
    // find the index from the name of the state
    for(ll i=0; i<numberOfStates; i++) {
        indexOfState[arr[0][i]] = i;   
    }

    map<pair<ll, pair<ll, ll>>, string> R; // R <k, <i, j>> -> value (string)
    ll k = -1;
    rep(i, numberOfStates) {
        rep(j, numberOfStates) {
            string res = "";
            vector<string> transList;
            if(i==j) transList.push_back(EPS);
            for(vector<string> tran : transitions) {
                if(tran[0] == arr[0][i] && tran[2] == arr[0][j]) {
                    // If there is a transition from state1 to state2
                    transList.push_back(tran[1]);
                }
            }
            // sort the transitions
            sort(transList.begin(), transList.end());
            // make it 'a|b|eps' etc.
            rep(i, transList.size()) {
                res += transList[i];
                if(i != transList.size()-1) res += "|";
            }
            // save it in the map
            if(res=="") res = NONE;
            R[{k, {i, j}}] = res;
        }
    }

    // iterate k from 0 to n
    rep(k, numberOfStates) {
        rep(i, numberOfStates) {
            rep(j, numberOfStates) {
                string res = "";
                res += "("+R[{k-1, {i, k}}]+")";
                res += "("+R[{k-1, {k, k}}]+")*";
                res += "("+R[{k-1, {k, j}}]+")|";
                res += "("+R[{k-1, {i, j}}]+")";
                R[{k, {i, j}}] = res;
            }
        }
    }

    // TODO: iterate through all accepting states
    vector<ll> accIndices;
    for(string acc : arr[3]) {
        accIndices.push_back(indexOfState[acc]);
    }

    // OUTPUT
    ll countForTest=0;
    
    for(ll j : accIndices) {
        cout<<"(";
        cout << R[{numberOfStates-1, {0, j}}];
        cout<<")";
        if(++countForTest!=accIndices.size()) cout<<"|";
    }
    return 0;
}
