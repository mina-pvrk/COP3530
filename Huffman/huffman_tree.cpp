#include "huffman_tree.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>

using namespace std;

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/

void huffman_tree::set_root(huffman_tree *root){
	theroot=root;
}

huffman_tree* huffman_tree::get_root()const{
	return theroot;
}

void huffman_tree::set_c(char c1){
	c=c1;
}

char huffman_tree::get_c()const {
	return c;
}

void huffman_tree::set_freq(int freq1){
	freq=freq1;
}

int huffman_tree::get_freq()const {
	return freq;
}

void huffman_tree::set_left(huffman_tree *left1){
	left=left1;
}

huffman_tree* huffman_tree::get_left()const {
	return left;
}

void huffman_tree::set_right(huffman_tree *right1){
	right=right1;
}

huffman_tree* huffman_tree::get_right()const {
	return right;
}

void huffman_tree::set_par(huffman_tree *par1){
	par=par1;
}

huffman_tree* huffman_tree::get_par()const {
	return par;
}

//Constructor for a leaf node
huffman_tree::huffman_tree(char c, int freq){
	this->freq=freq;
	this->c=c;
	this->isLeaf=true;
	this->left=NULL;
	this->right=NULL;
	this->par=NULL;
}

//Constructor for a nonleaf node
huffman_tree::huffman_tree(int freq, huffman_tree *left, huffman_tree *right){
	this->freq=freq;
	this->c='\0';
	this->isLeaf=false;
	this->left=left;
	this->right=right;
	left->par=this;
	right->par=this;
}

huffman_tree::huffman_tree(const std::string &file_name){
	huffman_tree *theroot=NULL;
  unordered_map<char,int> cfreq;

	//Gets each char of file and their frequencies and inserts into map
  ifstream inf(file_name);
  char in;

  while(inf.get(in)){
    if(cfreq.find(in)!=cfreq.end()){
      ++cfreq.at(in);
    }
    else{
      cfreq.insert({in,1});
    }
  }
  inf.close();

	//Priority queue to create huffman tree
  priority_queue<huffman_tree*, vector<huffman_tree*>, Comp> charq;

	//For each char, create a map that maps it to its leaf node
  for(auto it=cfreq.begin();it!=cfreq.end();it++){
    leaves[it->first]=new huffman_tree(it->first, it->second);
    charq.push(leaves[it->first]);
  }

	//While queue>1, merge the two smallest nodes until tree completed
  while(charq.size()>1){
    huffman_tree* lt=charq.top();
    charq.pop();
    huffman_tree* rt=charq.top();
    charq.pop();

    huffman_tree* combine=new huffman_tree((lt->get_freq())+(rt->get_freq()),lt,rt);
    charq.push(combine);
  }

	//Set the root of completed huffman tree
  theroot=charq.top();
  charq.pop();
  huffman_tree *rootpar=NULL;
  theroot->set_par(rootpar);
  set_root(theroot);

	//Create the map of leaves and their traversal codes
  code_map();
}

huffman_tree::~huffman_tree(){

}


/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/
std::string huffman_tree::get_character_code(char character) const {
	//If character is in our map of codes, return its code as string
	//otherwise, return empty string
	if(thecodes.find(character)!=thecodes.end()){
		string res=thecodes.at(character);
		return res;
	}
	else{
		return " ";
	}
}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/
std::string huffman_tree::encode(const std::string &file_name) const {
	char ch;
	ifstream file(file_name);

	string str="";

	//Return empty string if file can't be opened
	if(!file){
		return "";
	}

	else{
		//Find code of each char from map and add to string to return
		//Return empty string if contains letter not in tree
		while(file.get(ch)){
			if(thecodes.find(ch)!=thecodes.end()){
			str+=thecodes.at(ch);
			}
			else{
			return "";
			}
		}
	return str;
	}
}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/
std::string huffman_tree::decode(const std::string &string_to_decode) const {
	string result = "";

	int i = 0;
	const huffman_tree* now=NULL;
	const huffman_tree* len1=NULL;

	int slength=string_to_decode.length();

	//If length of string is 1, return the single value
	if(slength==1){
		len1=get_root();
		char tempchar=len1->get_c();
		string s(1,tempchar);
		return s;
	}
	//Else,traverse tree to obtain chars of the given code
	else{
			 while (i < slength)
			{
			now=get_root();
			while (!(now->isLeaf) && i < slength){

					if (string_to_decode[i]=='0') {
						now=now->get_left();
					}
					else if(string_to_decode[i]=='1'){
						now=now->get_right();
					}
					i++;
			}
			result+=now->get_c();
		 }

}

//If resulting string doesn't contain invalid Huffman Encoding
//reutrn the string; otherwise return empty string
 for(int k=0;k<result.length();k++){
	 char temp2=result.at(k);
	 if(leaves.find(temp2)!=leaves.end()){
		 return result;
	 }
	 else {
		 return "";
	 }
 }
}

void huffman_tree::code_map(){
	string result="";

	//For each leaf in tree, find its code traversal up the tree(backwards)
	//and reverse the string to get final code.
	//Insert char and its code into the code map for faster encoding
	for(auto it=leaves.begin(); it!=leaves.end(); ++it){
		result="";
		char ccur=it->first;
		huffman_tree* now=it->second;

		if(leaves.size()==1){
			thecodes.insert({ccur,"0"});
		}

		while (now->get_par()!=NULL){
			if(now==now->get_par()->get_left()){
				result+="0";
			}

			else if(now==now->get_par()->get_right()) {
				result+="1";
			}
			now=now->get_par();
		}

		reverse(result.begin(), result.end());
		thecodes.insert({ccur,result});
	}
}
