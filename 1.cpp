#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <sstream>

using namespace std;

typedef int Item;

struct Node {
	Item data;
	Node* next;
};

typedef Node* NodePtr;

int RunComFileOps(const vector<string>& vectcomfile, vector<string>& vectoutfile);
void SortList(NodePtr& hdlist);
void SmoothList(NodePtr hdlist);
void InsertElement(Item number, NodePtr& hdlist, bool ordered);
void DeleteElement(Item number, NodePtr& hdlist) ;
void FindMinEntry(vector<string>& vectoutfile, NodePtr hdlist);
void FindNoOfEntries(vector<string>& vectoutfile, NodePtr hdlist);
void ReadFile(int& filenumber, NodePtr& hdlist);
void WriteFile(vector<string>& vectoutfile, NodePtr hdlist);
Item GetFromList(NodePtr& hdlist);
void AddToHeadofList(Item number, NodePtr& hdlist);

int main() {
	//HELLO
	//Read command file name and open file
	string commandfilename;
	ifstream commandfile;
	cout << "Enter Command File Name" << endl;
	cin >> commandfilename;
	commandfile.open(commandfilename.c_str());

	//If execution error, exit the program
	if(!commandfile.is_open()){
		cout << "Couldn't open" << endl;
		exit(EXIT_FAILURE);
	}

	//If not, real file names into a structure and close the file
	string cftemp;
	vector<string> vectcomfile;

	while(commandfile >> cftemp) {
		vectcomfile.push_back(cftemp);
	}

	commandfile.close();

	//Write the output of the command file to an output file
    	vector<string> vectoutfile;
	int outfilenumber = RunComFileOps(vectcomfile, vectoutfile);

	if(outfilenumber == NULL) {
		return 0;
	}

	//Read output file name into a string
	stringstream outfiless;
	outfiless << "output_" << outfilenumber << ".txt";
	string outfilename = outfiless.str();

	//Open data file
	ofstream outfile;
	outfile.open(outfilename.c_str());

	//If execution error, exit the program
	if(!outfile.is_open()){
		cout << "Couldn't open" << endl;
		exit(EXIT_FAILURE);
	}

	//Input items of data file into new list and close data file

	for (int index = 0; index < vectoutfile.size(); index++) {
		outfile << vectoutfile[index] << endl ;
	}
	return 0;
}

int RunComFileOps(const vector<string>& vectcomfile, vector<string>& vectoutfile) {
        int filenumber = NULL;
	bool ordered = false;
	int comfileindex = 0;
	NodePtr hdlist = new Node;
	hdlist = NULL;
        while(comfileindex < vectcomfile.size()) {
            cout << vectcomfile[comfileindex] << "\t" << comfileindex << endl;
			if(vectcomfile[comfileindex] == "r" or vectcomfile[comfileindex] == "i" or vectcomfile[comfileindex] == "d") {
				if(vectcomfile[comfileindex] == "r") {
					filenumber = atoi(vectcomfile[comfileindex + 1].c_str());
					ReadFile(filenumber, hdlist);
				}
				else if(vectcomfile[comfileindex] == "i") {
                    			Item numtoinsert = atoi(vectcomfile[comfileindex + 1].c_str());
					InsertElement(numtoinsert, hdlist, ordered);
				}
				else {
					Item numtodelete = atoi(vectcomfile[comfileindex + 1].c_str());
					DeleteElement(numtodelete, hdlist);
				}
				comfileindex += 2;
			}
			else {
				if(vectcomfile[comfileindex] == "s") {
					SortList(hdlist);
					ordered = true;
				}
				else if(vectcomfile[comfileindex] == "w") {
					WriteFile(vectoutfile, hdlist);
				}
				else if(vectcomfile[comfileindex] == "e") {
					FindNoOfEntries(vectoutfile, hdlist);
				}
				else if(vectcomfile[comfileindex] == "m") {
					FindMinEntry(vectoutfile, hdlist);
				}
				else {
					SmoothList(hdlist);
					ordered = true;
				}
				comfileindex++;
			}
		}
	cout << filenumber << endl;
	return filenumber;
}

void InsertElement(Item number, NodePtr& hdlist, bool ordered) {
	if(!ordered) {
		AddToHeadofList(number, hdlist);
	}
	else {
		bool found = false;
		NodePtr searchPtr, lastPtr, newPtr;

		newPtr = new Node;
		newPtr->data = number;
		newPtr->next = NULL;

		if(hdlist == NULL) {
			hdlist = newPtr;
			return;
		}
		else if(hdlist->data >= number) {
			AddToHeadofList(number, hdlist);
			return;
		}
		else {
			searchPtr = hdlist;
			lastPtr = hdlist;
			while((searchPtr != NULL) && (!found)) {
				if(searchPtr->data >= number) {
					found = true;
				}
				else {
					lastPtr = searchPtr;
					searchPtr = searchPtr->next;
				}
			}
			newPtr->next = searchPtr;
			lastPtr->next = newPtr;
		}

	}
}

void SortList(NodePtr& hdlist) {
	if(hdlist == NULL || hdlist->next == NULL) {
		return;
	}
    	bool swapping = false;
	NodePtr trailPtr, testPtr1, testPtr2;
	trailPtr = hdlist;
	testPtr1 = hdlist;
	testPtr2 = hdlist->next;
	while(testPtr2 != NULL) {
		if(testPtr2->data < testPtr1->data) {
            swapping = true;
			if(testPtr1 == hdlist) {
				hdlist->next = testPtr2->next;
				testPtr2->next = hdlist;
				hdlist = testPtr2;
			}
			else {
				trailPtr->next = testPtr2;
				testPtr1->next = testPtr2->next;
				testPtr2->next = testPtr1;
			}
		}
		trailPtr = testPtr1;
		testPtr1 = testPtr2;
		testPtr2 = testPtr2->next;
	}
	if(swapping) {
        SortList(hdlist);
        return;
	}
	else {
	    return;
	}
}

void SmoothList(NodePtr hdlist) {
	if(hdlist == NULL || hdlist->next == NULL) {
		return;
	}
    	bool insertion;
	insertion = false;
	NodePtr frontPtr, backPtr;
	backPtr = hdlist;
	frontPtr = hdlist->next;
	int tempsum;
	while(frontPtr != NULL) {
		NodePtr newPtr = new Node;
		if(abs(backPtr->data - frontPtr->data) > 5) {
			tempsum = (backPtr->data + frontPtr->data);
			if(tempsum%2 == 1) {
                newPtr->data = (tempsum+1)/2;
			}
			else {
                newPtr->data = tempsum/2;
			}
			newPtr->next = frontPtr;
			backPtr->next = newPtr;
			insertion = true;
		}
		backPtr = frontPtr;
		frontPtr = frontPtr->next;
		//delete newPtr;
	}
	if(insertion) {
		SmoothList(hdlist);
		return;
	}
	else {
		return;
	}
}

void DeleteElement(Item number, NodePtr& hdlist) {
	bool found = false;
	NodePtr oldPtr;
	if(hdlist == NULL) {
		return;
	}
	else if(hdlist->data == number) {
		oldPtr = hdlist;
		hdlist = hdlist->next;
		delete oldPtr;
	}
	else {
        NodePtr searchPtr = hdlist;
		NodePtr lastPtr = hdlist;
		while((searchPtr != NULL) && (!found)) {
			if(searchPtr->data == number) {
				found = true;
				lastPtr->next = searchPtr->next;
				delete searchPtr;
				return;
			}
			else {
				lastPtr = searchPtr;
				searchPtr = searchPtr->next;
			}
		}
	}
}

void FindMinEntry(vector<string>& vectoutfile, NodePtr hdlist) {
    Item minentry;
	if(hdlist == NULL) {
        minentry = NULL;
	}
	else {
        minentry = hdlist->data;
	}
	while (hdlist != NULL) {
		if(hdlist->data < minentry) {
            minentry = hdlist->data;
		}
		hdlist = hdlist->next;
	}
	stringstream outss;
	if(minentry == NULL) {
        outss << "No entries, Therefore no minimum element" << endl;
	}
    else {
        outss << "Minimum value: " << minentry << endl;
    }
    string out = outss.str();
	vectoutfile.push_back(out);
}

void FindNoOfEntries(vector<string>& vectoutfile, NodePtr hdlist) {
	int noofentries = 0;
	while (hdlist != NULL) {
		noofentries++;
		hdlist = hdlist->next;
	}
	stringstream outss;
    outss << "Number of elements in the list: " << noofentries << endl;
    string out = outss.str();
	vectoutfile.push_back(out);
}


void ReadFile(int& filenumber, NodePtr& hdlist) {
	//Read data file name into a string
	stringstream datafiless;
	datafiless << "data_" << filenumber << ".txt";
	string datafilename = datafiless.str();

	//Open data file
	ifstream datafile;
	datafile.open(datafilename.c_str());

	//If execution error, exit the program
	if(!datafile.is_open()){
		cout << "Couldn't open" << endl;
		exit(EXIT_FAILURE);
	}

    Item temp;

	//Input items of data file into new list and close data file
	while(datafile >> temp) {
		AddToHeadofList(temp,hdlist);
	}

	datafile.close();
}

void WriteFile(vector<string>& vectoutfile, NodePtr hdlist) {
	while (hdlist != NULL) {
		stringstream outss;
        	outss << GetFromList(hdlist) << endl;
        	string out = outss.str();
		vectoutfile.push_back(out);
	}
}

Item GetFromList(NodePtr& hdlist) {
	int outdata;
	NodePtr tempPtr = new Node;
	tempPtr = hdlist;
	outdata = tempPtr->data;
	hdlist = tempPtr->next;
	//delete tempPtr;
	return outdata;
}

void AddToHeadofList(Item number, NodePtr& hdlist) {
	NodePtr newnode = new Node;
	newnode->data = number;
	newnode->next = hdlist;
	hdlist = newnode;
}






