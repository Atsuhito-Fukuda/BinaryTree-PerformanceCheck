#pragma once
#include <vector>
#include <iostream>
#include <string>

extern int collisioncount;

/***************************************************************************

	BaseBinaryTree

****************************************************************************/

template <typename Instance>
class BaseBinaryTree {

public:
	BaseBinaryTree(unsigned int dimension, unsigned int tablecap = defaulttablecap);
	~BaseBinaryTree(void);

	void Register(const Instance& instance, const std::vector<char>& code, unsigned int codelength);
	bool Update(void);
	bool Update_compare2(void); //î‰ärÇÃÇΩÇﬂÇ…í«â¡
	void Update_compare5(void); //î‰ärÇÃÇΩÇﬂÇ…í«â¡
	void Clear(void);

	bool NextNode(void);
	bool NextEdge(void);
	bool NextParent(void);
	const Instance& present(void); 

protected:
	class Node;

	const unsigned int dimension;
	const unsigned int branchlength;	
	unsigned int getcodelength(const std::vector<char>& code1, const std::vector<char>& code2);
	Node* getnode(const std::vector<char>& code, unsigned int codelength);
	void Remove(Node* node);
public:
	Node* presentnode_compare1(void); //î‰ärÇÃÇΩÇﬂÇ…í«â¡
private:
	static void remove(Node* node);
	static bool startreach;

	void noderegister(void);
	void noderegister_compare2(void); //î‰ärÇÃÇΩÇﬂÇ…í«â¡
	void removenodesearch_compare5(Node* node); //î‰ärÇÃÇΩÇﬂÇ…í«â¡
public:
	std::vector<Instance> deletetable_compare5; //î‰ärÇÃÇΩÇﬂÇ…í«â¡
protected:
	std::vector<Node*> deletetable_compare2; //î‰ärÇÃÇΩÇﬂÇ…í«â¡
	void removeone_compare6(Node* node); //î‰ärÇÃÇΩÇﬂÇ…í«â¡
//private: //î‰ärÇÃÇΩÇﬂàÍéûìIÇ…
	std::vector<Node*> nodetable;
	Node* startnode;
	Node* presentnode;
	unsigned int tablenum;
	Node dummy;
protected:
	static const unsigned int defaulttablecap;

private:
	BaseBinaryTree(void);

// debug - - - - - - - - - - - - - - - - - - - - - - -
	static int wholenodenum;
public:
	void Showresult(bool wholenumonly = false) {

		if (startnode) draw(startnode, "", wholenumonly);

		std::cout << "(whole node number : " << wholenodenum << ")\n";

		wholenodenum = 0;

	}
	void ShowTable(void){

		for (auto node : nodetable) {
			std::cout << "< ";
			show(node);
			std::cout << ">\n";
		}

	}
	void Showtablenum(void) {
		std::cout << nodetable.size() << "\n";
	}

private:
	void draw(Node* node, std::string string, bool wholenumonly = false) {

		++wholenodenum;

		for (unsigned int n(0); n < branchlength; ++n) {
			if (node->branch[n]) draw(node->branch[n], string + std::to_string(n), wholenumonly);
		}

		if (!wholenumonly) {
			std::cout << "[" << string << "] : ";
			show(node);
			std::cout << "\n";
		}

	}
	virtual void show(Node* node) {
		std::cout <<  "* ";
	}

};

/***************************************************************************

	BaseBinaryTree::Node

****************************************************************************/

template <typename Instance>
class BaseBinaryTree<Instance>::Node {		

	friend class BaseBinaryTree;

public:
	Node(const BaseBinaryTree<Instance>* tree);
	bool IsNulledge(void);
	bool Isedge(void);
	Instance instance;

//private: //î‰ärÇÃÇΩÇﬂàÍéûìIÇ…
	Node* parent;
	std::vector<Node*> branch;
	static void deletetree(Node* start);
private:
	Node(void);
};

/***************************************************************************

	BinaryTree

****************************************************************************/

template <typename Instance>
class BinaryTree : public BaseBinaryTree<std::vector<Instance>> {

public:
	BinaryTree(unsigned int dimension, unsigned int tablecap = BaseBinaryTree<std::vector<Instance>>::defaulttablecap, unsigned int removetablecap = defaultremovetablecap);
	~BinaryTree(void);

	class NodeInfo;
	void Register(NodeInfo& returninfo, const Instance& instance, const std::vector<char>& code1, const std::vector<char>& code2);
	void Register_compare1(NodeInfo& returninfo, const Instance& instance, const std::vector<char>& code1, const std::vector<char>& code2);//î‰ärÇÃÇΩÇﬂÇ…í«â¡
	void Register_compare3(NodeInfo& returninfo, const Instance& instance, const std::vector<char>& code1, const std::vector<char>& code2);//î‰ärÇÃÇΩÇﬂÇ…í«â¡
	void Register_compare4(NodeInfo& returninfo, const Instance& instance, const std::vector<char>& code1, const std::vector<char>& code2);//î‰ärÇÃÇΩÇﬂÇ…í«â¡
	void Remove(NodeInfo& nodeinfo);
	bool Update(void);
	bool Update_compare1(void);//î‰ärÇÃÇΩÇﬂÇ…í«â¡
	bool Update_compare2(void);//î‰ärÇÃÇΩÇﬂÇ…í«â¡
	bool Update_compare5(void);//î‰ärÇÃÇΩÇﬂÇ…í«â¡
	void Collision_compare5(void);//î‰ärÇÃÇΩÇﬂÇ…í«â¡
	void Collision_compare6(void);//î‰ärÇÃÇΩÇﬂÇ…í«â¡

private:
	void remove(NodeInfo& info);
	void remove_compare1(NodeInfo& info);//î‰ärÇÃÇΩÇﬂÇ…í«â¡
	void collide_compare5(BaseBinaryTree<std::vector<Instance>>::Node* node) {//î‰ärÇÃÇΩÇﬂÇ…í«â¡

		std::vector<Instance>& main(node->instance);

		for (unsigned int i(0); i < main.size(); ++i) {//size0Ç»ÇÁí âﬂ

			for (unsigned int j(i + 1); j < main.size(); ++j) { ++collisioncount;/*è’ìÀèàóùÇÕè»ó™*/ }

			for (auto par : parenttable_compare5) {
				for (auto obj : *par) { ++collisioncount;/*è»ó™*/ }
			}

		}

		if (main.size()) parenttable_compare5.emplace_back(&main);  //ÉXÉ^ÉbÉNÇ…push

		for (auto br : node->branch) {

			if (br) collide_compare5(br);

		}

		if (main.size()) parenttable_compare5.pop_back(); //ÉXÉ^ÉbÉNÇ©ÇÁpop

		//if (node->IsNulledge()){} //Ç±ÇÍÇ™Ç†ÇÈÇ∆éûä‘Ç™âÑÇ—ÇÈÇ±Ç∆ÇämîF

	}

	void collide_compare6(BaseBinaryTree<std::vector<Instance>>::Node* node) {//î‰ärÇÃÇΩÇﬂÇ…í«â¡

		std::vector<Instance>& main(node->instance);

		for (unsigned int i(0); i < main.size(); ++i) {//size0Ç»ÇÁí âﬂ

			for (unsigned int j(i + 1); j < main.size(); ++j) { ++collisioncount;/*è»ó™*/ }

			for (auto par : parenttable_compare5) {
				for (auto obj : *par) { ++collisioncount;/*è»ó™*/ }
			}

		}

		if (main.size()) parenttable_compare5.emplace_back(&main); 

		for (auto br : node->branch) {

			if (br) collide_compare6(br);

		}

		if (main.size()) parenttable_compare5.pop_back();

		if (node->IsNulledge()) BaseBinaryTree<std::vector<Instance>>::removeone_compare6(node); //Ç±Ç±Ç≈É`ÉFÉbÉN&ÉmÅ[Éh1Ç¬ÇæÇØè¡ãé

	}


	bool isvergininfo(NodeInfo& nodeinfo);

	BaseBinaryTree<BaseBinaryTree<std::vector<Instance>>::Node*> removetree;
	static const unsigned int defaultremovetablecap;

	std::vector<std::vector<Instance>*> parenttable_compare5; //î‰ärÇÃÇΩÇﬂÇ…í«â¡

	BinaryTree(void);

// hide - - - - - - - - - - - - - - - - - - - - - - - 
	void Register(const std::vector<Instance>& instance, const std::vector<char>& code, unsigned int codelength){}// Å¶ âΩÇ≈Ç©ï™Ç©ÇÁÇ»Ç¢Ç™Ç±ÇÍïsóvÅHÅH
	void Clear(void) {}	

// debug - - - - - - - - - - - - - - - - - - - - - - -
	virtual void show(BaseBinaryTree<std::vector<Instance>>::Node* node) override {
		for (const auto& e : node->instance) {
			std::cout << e << " ";
		}
	}

};

/***************************************************************************

	BinaryTree::NodeInfo

****************************************************************************/

template <typename Instance>
class BinaryTree<Instance>::NodeInfo {

	friend class BinaryTree;

public:
	NodeInfo(void);
	~NodeInfo(void);
private:
	std::vector<char> code;
	unsigned int codelength;
	Instance instance;		
	BinaryTree<Instance>* tree;	

	NodeInfo& operator =(const NodeInfo& nodeinfo);
	NodeInfo(const NodeInfo& nodeinfo);

};