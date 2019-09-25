#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <iostream>
#include <unordered_map>

using namespace std;

class huffman_tree {
	private:
		char c;
    int freq;
    bool isLeaf;

    huffman_tree *left;
    huffman_tree *right;
    huffman_tree *par;
    huffman_tree *theroot;

		unordered_map<char, huffman_tree*> leaves;
		unordered_map<char,string> thecodes;
	public:
		//leaf node
		huffman_tree(char c, int freq);
    //nonleaf node
    huffman_tree(int freq, huffman_tree *left, huffman_tree *right);

		huffman_tree(const std::string &file_name);
		~huffman_tree();

		std::string get_character_code(char character) const;
		std::string encode(const std::string &file_name) const;
		std::string decode(const std::string &string_to_decode) const;
		void code_map();

		void set_root(huffman_tree *root);
		huffman_tree* get_root()const;
		void set_c(char c1);
		char get_c()const;
		void set_freq(int freq1);
		int get_freq()const;
		void set_left(huffman_tree *left1);
		huffman_tree* get_left()const;
		void set_right(huffman_tree *right1);
		huffman_tree* get_right()const;
		void set_par(huffman_tree *par1);
		huffman_tree* get_par()const;

};

struct Comp{
	bool operator()(huffman_tree* lhs, huffman_tree* rhs){
		return (lhs->get_freq())>(rhs->get_freq());
	}
};

#endif
