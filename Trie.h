#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

template<typename ValueType>
class Trie
{
public:
	Trie();
	~Trie();
	void reset();
	void insert(const std::string& key, const ValueType& value);
	std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

	// C++11 syntax for preventing copying and assignment
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie&) = delete;
private:
	
	//Tries will consist of Nodes containing values,  and labels and pointers that connect nodes

	//Nodes contain Children, an array of child structs, and Values
	struct Node
	{
		~Node()
		{
			for (size_t i = 0; i < Branches.size(); i++)
			{
				delete Branches[i];
			}
		}
		std::vector<ValueType> Values;
		std::vector<char> Labels;
		std::vector<Node*> Branches;

	};

	//A child struct contains a label and a pointer to the next node

	Node* root; //Each Trie has a root, the pointer to the first empty node

	//helper function for Find
	std::vector<ValueType> finder(Trie<ValueType>::Node* cur, const int& charNumber, const std::string& key, bool exactMatchOnly) const;

};

template<typename ValueType>
Trie<ValueType>::Trie()
{
	root = new Trie<ValueType>::Node;
}

template<typename ValueType>
Trie<ValueType>::~Trie()
{
	delete root;
}


template<typename ValueType>
void Trie<ValueType>::reset()
{
	delete root;
	root = new Trie<ValueType>::Node;
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string & key, const ValueType & value)
{
	Trie<ValueType>::Node* cur = root;
	for (int i = 0; i < key.size(); i++)
	{
		//Checks for a label matching the current character of the key string in cur node
		bool CharFound = false;


		for (int j = 0; j < (cur -> Labels.size()); j++)
		{
			if ( cur -> Labels[j] == key[i])
			{
				CharFound = true;
				cur = cur->Branches[j];
				break;
			}

		}
		if (!CharFound)
		{
			//must create new Branches with correct labels. 
			cur->Labels.push_back(key[i]);
			cur->Branches.push_back(new Trie<ValueType>::Node);
			cur = cur->Branches.back();
		}
	}
	//insert the value into this node
	cur->Values.push_back(value);
}



template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
	std::vector<ValueType> Result;
	if (key != "")
	{
		for (int i = 0; i < root->Labels.size(); i++)
		{
			if (root->Labels[i] == key[0])
			{
				Result.push_back(finder(root->Branches[i], 1, key, exactMatchOnly)); //Recursive function should return final correct result
			}
		}
	}

	return Result;


}

template<typename ValueType>
 std::vector<ValueType> Trie<ValueType>::finder(Trie<ValueType>::Node* cur, const int& charNumber, const std::string & key, bool exactMatchOnly) const
{
	 std::vector<ValueType> Result;

	 if (charNumber == key.size()) //Checks to see if node needs to be checked for values
	 {
		 Result.reserve(Result.size() + cur->Values.size());
		 Result.insert(Result.end(), cur->Values.begin(), cur->Values.end());
	 }
	 else 
	 {
		 for (int i = 0; i < cur->Labels.size(); i++)
		 {
			 std::vector<ValueType> BranchVector;
			 if (cur->Labels[i] == key[charNumber])
			 {
				 BranchVector = finder(cur->Branches[i], charNumber + 1, key, exactMatchOnly);
				 Result.reserve(Result.size() + BranchVector.size());
				 Result.insert(Result.end(), BranchVector.begin(), BranchVector.end());
			 }
			 else if (!exactMatchOnly)
			 {
				 BranchVector = finder(cur->Branches[i], charNumber + 1, key, true);
				 Result.reserve(Result.size() + BranchVector.size());
				 Result.insert(Result.end(), BranchVector.begin(), BranchVector.end());
			 }
		 }
	 }

	return Result;
}



#endif TRIE_INCLUDED
