#include<bits/stdc++.h>
using namespace std;

#define MAX_BLOCK_HEAD 4
#define MAX_COLLISIONS 2

vector< vector<string> > inputBlocks;
vector<string> outputBlock;

int curBlock;
int curBlockHead;
int outputBlockHead;

map<string, bool> recordPresence;

string inpFile;
string outFile;
int numAttrs;
int numBlocks;
string typeIndex;

ifstream inpFileStream;
ofstream outFileStream;

void GetnextHash() {
	int curBlockIt=0, curBlockHeadIt=0;
	while(true) {
		if(curBlockIt==curBlock and curBlockHeadIt==curBlockHead) break;
		if(curBlockHeadIt==MAX_BLOCK_HEAD+1) {
			curBlockIt++;
			curBlockHeadIt=0;
		}
		if(recordPresence[inputBlocks[curBlockIt][curBlockHeadIt]] == true) {
			curBlockHeadIt++;
			continue;
		}
		else {
			recordPresence[inputBlocks[curBlockIt][curBlockHeadIt]] = true;
			outputBlock[outputBlockHead++]=inputBlocks[curBlockIt][curBlockHeadIt];
			if(outputBlockHead==MAX_BLOCK_HEAD+1) {
				outputBlockHead=0;
				while(outputBlockHead!=MAX_BLOCK_HEAD+1) {
					outFileStream<<outputBlock[outputBlockHead]<<"\n";
					outputBlockHead++;
				}
				outputBlockHead=0;
			}
		}
		curBlockHeadIt++;
	}
}

void openHash() {
	string line;
	recordPresence.clear();
	while(getline(inpFileStream, line)) {

		if(curBlockHead==MAX_BLOCK_HEAD+1) {
			curBlock++;
			curBlockHead=0;
		}

		if(curBlockHead==MAX_BLOCK_HEAD+1 and curBlock==numBlocks-2) {
			GetnextHash();
			curBlock=curBlockHead=0;
		}
		else
			inputBlocks[curBlock][curBlockHead++]=line;
	}
	GetnextHash();
	int outputBlockHeadIt=0;
	while(outputBlockHeadIt!=outputBlockHead) {
		outFileStream<<outputBlock[outputBlockHeadIt]<<"\n";
		outputBlockHeadIt++;
	}
}

void open() {
	inpFileStream.open(inpFile);
	outFileStream.open(outFile);

	curBlock=0;
	curBlockHead=0;
	outputBlockHead=0;

	inputBlocks.resize(numBlocks-1);
	for(int i=0;i<numBlocks-1;i++)
		inputBlocks[i].resize(MAX_BLOCK_HEAD+1);
	outputBlock.resize(MAX_BLOCK_HEAD+1);
	
	if(typeIndex == "hash")
		openHash();
	//else if(typeIndex == "btree") openBTree();
	outFileStream.close();
}

int main(int argc, char* argv[])
{
	if(argc!=6) {
		cout<<"Invalid number of arguments!\n";
		cout<<"Usage: <Input file> <Output file> <Number of attributes>";
		cout<<" <Number of Blocks in memory used> <Type of index - B+ Tree or Hashing>\n";
		return 0;
	}

	inpFile = argv[1];
	outFile = argv[2];
	numAttrs = stoi(argv[3]);
	numBlocks = stoi(argv[4]);
	typeIndex = argv[5];

	open();

	return 0;
}
