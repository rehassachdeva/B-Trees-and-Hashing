#include<bits/stdc++.h>
using namespace std;

// Global constants and macros
#define MAX_BLOCK_SIZE 5
#define BTREE_MINDEG 20
#define LC(veca, vecb) lexicographical_compare(veca.begin(), veca.end(), \
		vecb.begin(), vecb.end())

// Global variables
ifstream inpFileStream;
ofstream outFileStream;
string inpFile;
string outFile;

// Assuming buffer size is equal to block size. Last block is output buffer.
vector<string> blocks, outputBlock;

int curBlockHead;
int outputBlockHead;

int numAttrs;
int numBlocks;

string typeIndex;

// A BTree node
class BTreeNode {
    vector< vector<int> > keys;
    vector< BTreeNode* > children;
    bool isLeaf;
    int minDeg, numKeys;

public:
    BTreeNode(int _minDeg, bool _isLeaf) {
    	minDeg=_minDeg;
    	isLeaf=_isLeaf;
    	numKeys=0;
    	keys.resize(2*minDeg-1);
    	for(int i=0;i<2*minDeg-1;i++)
    		keys[i].resize(numAttrs);
    	children.resize(2*minDeg);
    }

    void insertNonFull(vector<int> k) {
    	int i=numKeys-1;
	    if(isLeaf==true) {
        	while(i>=0 && LC(keys[i], k)!=0) {
        		keys[i+1]=keys[i];
        		i--;
        	}
	        keys[i+1]=k;
    	    numKeys++;
    	}
    	else {
	        while(i>=0 && LC(keys[i], k)!=0)
	        	i--;
	        if(children[i+1]->numKeys==2*minDeg-1) {
	            splitChild(i+1, children[i+1]);
            if(LC(k, keys[i+1])!=0)
            	i++;
        	}
        	children[i+1]->insertNonFull(k);
    	}
    }

    void splitChild(int i, BTreeNode *y) {
    	BTreeNode *z=new BTreeNode(y->minDeg, y->isLeaf);
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
    
    BTreeNode *search(vector<int> k) {
    	int i = 0;
    	while(i<numKeys && LC(k, keys[i])!=0)
    		i++;

	    if(k==keys[i])
    	    return this;
	
	    if(isLeaf==true)
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
    BTree() {
    	root=NULL;
    	minDeg=BTREE_MINDEG;
    }

    BTreeNode* search(vector<int> k) {
    	return (root == NULL)?NULL:root->search(k);
    }

    void insert(vector<int> k) {
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
            	if(LC(k, s->keys[0])!=0)
                	i++;
            	s->children[i]->insertNonFull(k);
	            root=s;
    	    }
        	else {
        		root->insertNonFull(k);
        	}
    	}

    }
};

// A HashMap Class
class HashMap {
	unordered_map<string, bool> recordPresence;
public:
	bool search(string key) {
		if(recordPresence.find(key) == recordPresence.end()) return false;
		return true;
	}
	void insert(string key) {
		if(search(key)==false)
			recordPresence[key]=true;
	}
};

BTree tree;
HashMap hash_;

void manage_output(string line) {
	outputBlock[outputBlockHead++]=line;
	if(outputBlockHead==MAX_BLOCK_SIZE) {
		outputBlockHead=0;
		while(outputBlockHead!=MAX_BLOCK_SIZE) {
			outFileStream<<outputBlock[outputBlockHead++]<<"\n";
		}
		outputBlockHead=0;
	}
}

void flush_output() {
	int outputBlockHeadIt=0;
	while(outputBlockHeadIt!=outputBlockHead) {
		outFileStream<<outputBlock[outputBlockHeadIt]<<"\n";
		outputBlockHeadIt++;
	}
}

void process_btree(string line) {
	stringstream linestream(line);
	string curnum;
	vector<int> record;
	while(getline(linestream, curnum, ',')) {
		record.push_back(stoi(curnum));
	}
	if(tree.search(record)) return;
	else {
		tree.insert(record);
		manage_output(line);
	}
}

void process_hash(string line) {
	if(hash_.search(line)) return;
	else {
		hash_.insert(line);
		manage_output(line);
	}
}

void process() {
	int curBlockHeadIt=0;
	string line;
	while(true) {
		if(curBlockHeadIt==curBlockHead)  {
			curBlockHeadIt=0;
			break;
		}
		line=blocks[curBlockHeadIt];
		if(typeIndex=="btree") process_btree(line);
		else if(typeIndex=="hash") process_hash(line);
		curBlockHeadIt++;
	}
}

void getnext() {
	string line;
	while(getline(inpFileStream, line)) {
		if(curBlockHead==MAX_BLOCK_SIZE*(numBlocks-1)) {
			process();
			curBlockHead=0;
		}
		blocks[curBlockHead++]=line;
	}
	process();
	flush_output();
}

void open() {
	inpFileStream.open(inpFile);
	outFileStream.open(outFile);
	blocks.resize(MAX_BLOCK_SIZE*(numBlocks-1));
	outputBlock.resize(MAX_BLOCK_SIZE);
	curBlockHead=0;
	outputBlockHead=0;
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