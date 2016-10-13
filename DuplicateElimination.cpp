#include<bits/stdc++.h>
using namespace std;

// Global constants
#define MAX_BLOCK_HEAD 4
#define MAX_COLLISIONS 2

// Global variables

ifstream inpFileStream;
ofstream outFileStream;


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

// A BTree node
class BTreeNode {
    vector< int > keys;
    vector< BTreeNode* > children;
    bool isLeaf;
    int minDeg, numKeys;

public:
    BTreeNode(int _minDeg, bool _isLeaf) {
    	minDeg=_minDeg;
    	isLeaf=_isLeaf;
    	numKeys=0;
    	keys.resize(2*minDeg-1);
    	children.resize(2*minDeg);
    }

    void insertNonFull(int k) {
    	int i=numKeys-1;
	    if(isLeaf==true) {
        	while(i>=0 && keys[i]>k) keys[i+1] = keys[i--];
	        keys[i+1]=k;
    	    n++;
    	}
    	else {
	        while(i>=0 && keys[i]>k) i--;
	        if(children[i+1]->numKeys==2*minDeg-1) {
	            splitChild(i+1, children[i+1]);
            if(keys[i+1]<k)
                i++;
        	}
        	children[i+1]->insertNonFull(k);
    	}
    }

    void splitChild(int i, BTreeNode *y) {
    	BTreeNode *z=new BTreeNode(y->minDeg, y->leaf);
    	z->numKeys=minDeg-1;

    	for(int j=0;j<minDeg-1;j++)	
        	z->keys[j]=y->keys[j+minDeg];

	    if (y->isLeaf==false) {
	        for(int j=0;j<minDeg;j++)
    	        z->children[j]=y->children[j+minDeg];
    	}
 		
 		y->numKeys=minDeg-1;
	    
	    for(int j=numKeys;j>=i+1;j--)
    	    children[j+1]=children[j];
	    children[i+1]=z;
	    
	    for(int j=numKeys-1;j>=i;j--)
        	keys[j+1]=keys[j];
	    keys[i]=y->keys[minDeg-1];
	    
	    numKeys=numKeys+1;
    }
    
    BTreeNode *search(int k) {
    	int i = 0;
    	while (i<numKeys && k>keys[i]) i++;
	
	    if (keys[i]==k)
    	    return this;
	
	    if (isLeaf==true)
        	return NULL;
	
	    return children[i]->search(k);
    }
 
friend class BTree;
};
 
// A BTree
class BTree {
    BTreeNode *root;
    int minDeg;
public:
    BTree(int _minDeg) {
    	root=NULL;
    	minDeg=_minDeg;
    }

    BTreeNode* search(int k) {
    	return (root == NULL)?NULL:root->search(k);
    }

    void insert(int k) {
    	if(root==NULL) {
	        root = new BTreeNode(minDeg, true);
        	root->keys[0]=k;
        	root->numKeys=1;
    	}
    	else {
	        if(root->numKeys==2*minDeg-1) {
	            BTreeNode *s=new BTreeNode(minDeg, false);
	            s->children[0]=root;
            	s->splitChild(0, root);
	            int i=0;
            	if(s->keys[0]<k)
                	i++;
            	s->children[i]->insertNonFull(k);
	            root = s;
    	    }
        	else root->insertNonFull(k);
    	}

    }
};

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

void openBTree() {

}

void getnext() {

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
	else if(typeIndex == "btree")
		openBTree();
	
}

void close() {
	inpFileStream.close();
	outFileStream.close();
}

int main(int argc, char* argv[])
{
	if(argc!=6) {
		cout<<"Invalid number of arguments!\n";
		cout<<"Usage: <Input file> <Output file> <Number of attributes>";
		cout<<" <Number of Blocks in memory used> <Type of index - btree or hash>\n";
		return 0;
	}

	inpFile = argv[1];
	outFile = argv[2];
	numAttrs = stoi(argv[3]);
	numBlocks = stoi(argv[4]);
	typeIndex = argv[5];

	open();
	getnext();
	close();

	return 0;
}
