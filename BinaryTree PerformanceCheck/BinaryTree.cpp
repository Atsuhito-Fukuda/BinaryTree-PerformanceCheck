#include "BinaryTree.h"

/***************************************************************************

	template <typename Instance> BaseBinaryTree

****************************************************************************/

/*

	[ BaseBinaryTree ]

	・各ノードはInstanceを変数にもつ。

	・8bit整数(code)と読み取る「長さ(codelength)」の2つがTreeの位置を指す。(code×2でもいいが処理に時間がかかるか)
		・codeのbit数字は枝分かれ番号を示す。codelengthの長さ分先頭bitから枝分かれを繰り返す。

	・「2^n分木」ということでn個のchar型配列を持つことにする。

	・BaseBinaryTreeは登録と削除は別途行う。(NodeInfoをもたず自動削除機能等はない)

	・BaseBinaryTreeはUpdate()を行うことでTreeを走査し、全ノードを幅優先探索で配列に集草する
		・登録(Register())後はUpdateしないと反映されない

	・NextNode()はノード収納配列の要素を進め、NextParent()は親ノードにアクセスする。present()はconst Instance&を返す。
	　NextEdge()は次のTreeの末端まで要素を進める。
	
	・Remove()は指定nodeに関わらず削除する。その後parentのbranch配列の該当ptrをnullptrに置き換え、
	　parentノードがNulledgeだった場合にはparentも続けて削除する。Nulledgeでなくなるまでこれを繰り返す。
		
	・派生クラスがRemove()(protected)を用いる場合には収納配列も後に更新しなければならない。

	・Clear()では全削除後に収納配列も更新する。


*/

template <typename Instance>
bool BaseBinaryTree<Instance>::startreach(false);

template <typename Instance>
const unsigned int BaseBinaryTree<Instance>::defaulttablecap(5000);//ノード収納配列のデフォルトcapacity

template <typename Instance>
BaseBinaryTree<Instance>::BaseBinaryTree(unsigned int dimension, unsigned int tablecap/* = defaulttablecap*/) :
	startnode(), nodetable(), presentnode(&dummy), tablenum(), dimension(dimension),
	branchlength((unsigned int)pow(2,dimension)), dummy(this), deletetable_compare2(), deletetable_compare5()
{
	nodetable.reserve(tablecap);
	deletetable_compare2.reserve(3000);
}

template <typename Instance>
BaseBinaryTree<Instance>::~BaseBinaryTree(void) {
	Clear();
}

template <typename Instance>
void BaseBinaryTree<Instance>::Register(const Instance& instance, const std::vector<char>& code, unsigned int codelength) {	// copy禁止のものをretturnできても後が面倒なので
	if(code.size() != dimension) std::cout << "dimensionmismatch";	// 汎用利用を想定した場合のerror check
	getnode(code, codelength)->instance = instance;
}

template <typename Instance>
bool BaseBinaryTree<Instance>::Update(void) {

	bool exist(true);

	noderegister();	// clearは必ずここで行われる。ここでnodeを配列に収納していく。

	if (nodetable.size())
		presentnode = nodetable[tablenum];

	else {
		presentnode = &dummy;
		exist = false;
	}

	return exist;

}

template <typename Instance>
bool BaseBinaryTree<Instance>::Update_compare2(void) { 

	bool exist(true);

	noderegister_compare2();	

	if (nodetable.size()) 
		presentnode = nodetable[tablenum];

	else {
		presentnode = &dummy;
		exist = false;
	}

	return exist;

}

template <typename Instance>
void BaseBinaryTree<Instance>::removenodesearch_compare5(Node* node) {//removetreeの方のinstanceを見ていることに注意

	bool isedge(true);

	for (auto branch : node->branch) {

		if (branch) {
			isedge = false;
			removenodesearch_compare5(branch);
		}

	}

	if (isedge) deletetable_compare5.emplace_back(node->instance); //ここでは削除できないので仕方なく

}

template <typename Instance>
void BaseBinaryTree<Instance>::Update_compare5(void) {

	deletetable_compare5.clear();

	if(startnode) removenodesearch_compare5(startnode);

}

template <typename Instance>
void BaseBinaryTree<Instance>::Clear(void) {
	if (startnode) Node::deletetree(startnode);
		startnode = nullptr;
		Update();
}

template <typename Instance>
bool BaseBinaryTree<Instance>::NextNode(void) {	// 木の上->下、左->右へと」進む

	bool exist(true);

	if ((++tablenum) < nodetable.size()) 
		presentnode = nodetable[tablenum];

	else {	
		presentnode = &dummy;
		tablenum = (unsigned int)nodetable.size(); 
		exist = false;
	}

	return exist;

}

template <typename Instance>
bool BaseBinaryTree<Instance>::NextEdge(void) {	

	bool exist(false);	

	while (NextNode()) {	

		if (presentnode->Isedge()) {
			exist = true;
			break;
		}

	}

	return exist;

}

template <typename Instance>
bool BaseBinaryTree<Instance>::NextParent(void) {

	bool exist(true);

	if (presentnode->parent) 
		presentnode = presentnode->parent;
	
	else {
		presentnode = &dummy;
		exist = false;
	}

	return exist;

}

template <typename Instance>
const Instance& BaseBinaryTree<Instance>::present(void) {
	return presentnode->instance;
}

template <typename Instance>
typename BaseBinaryTree<Instance>::Node* BaseBinaryTree<Instance>::presentnode_compare1(void) {
	return presentnode;
}

template <typename Instance>
void BaseBinaryTree<Instance>::Remove(Node* node) {	

	remove(node); //nodeが持つのが空Instanceでなくとも削除

	if (startreach) {
		startnode = nullptr;
		startreach = false;	
	}

}


template <typename Instance>
void BaseBinaryTree<Instance>::remove(Node* node) {

	Node* parent(node->parent);

	delete node;

	if (parent) {

		bool unbranch(true);

		for (auto& branch : parent->branch) {

			if (branch) {
				if (branch == node) branch = nullptr;	//親の枝から削除
				else unbranch = false;
			}

		}

		if (unbranch && (parent->instance == Instance())) //親がNulledgeなら親も続けて削除
			remove(parent);

	}
	else
		startreach = true;

}

template <typename Instance>
void BaseBinaryTree<Instance>::removeone_compare6(Node* node) {

	Node* parent(node->parent);

	delete node;

	if (parent) {

		for (auto& branch : parent->branch) {

			if (branch == node) {
				branch = nullptr;
				break;
			}

		}

	} //親の削除はしない
	else
		startnode = nullptr;

}

template <typename Instance>
void BaseBinaryTree<Instance>::noderegister(void) {	//nodeの幅優先探索と配列への収納

	tablenum = 0;
	nodetable.clear();

	if (startnode) nodetable.emplace_back(startnode);

	for (unsigned int n(0); n < nodetable.size(); ++n) { //走査が進めばsize()は増えていく

		for (auto branch : nodetable[n]->branch) {
			if (branch) nodetable.emplace_back(branch);
		}

	}


}

template <typename Instance>
void BaseBinaryTree<Instance>::noderegister_compare2(void) {

	tablenum = 0;	
	nodetable.clear();
	deletetable_compare2.clear();

	if (startnode) nodetable.emplace_back(startnode);

	for (unsigned int n(0); n < nodetable.size(); ++n) {	

		bool isedge(true);

		for (auto branch : nodetable[n]->branch) {

			if (branch) {
				nodetable.emplace_back(branch);
				isedge = false;
			}

		}

		if (isedge && (nodetable[n]->instance == Instance())) //ここでNulledgeかを判定し別配列に収納
			deletetable_compare2.emplace_back(nodetable[n]);

	}


}

template <typename Instance>
unsigned int BaseBinaryTree<Instance>::getcodelength(const std::vector<char>& code1, const std::vector<char>& code2) {

	unsigned int length(0); int bitpoint(0b10000000);

	for (; length < 8; ++length) {	//2つのcodeの先頭からの共通bit部分の長さを取得

		for (unsigned int i(0); i < dimension; ++i) { 

			if ((code1[i] & bitpoint) != (code2[i] & bitpoint)) goto End;

		}

		bitpoint >>= 1;

	}

End:
	return length;

} //結局中でcopyは必要なのでコピー渡し。vectorは生成に時間がかかる？

template <typename Instance> 
typename BaseBinaryTree<Instance>::Node* BaseBinaryTree<Instance>::getnode(const std::vector<char>& code, unsigned int codelength) {

	if (!startnode) startnode = new Node(this);	
	
	Node* result(startnode);  int bitpoint(0b10000000);
	
	while (codelength--) {

		int branchnum(0);

		for (unsigned int i(0); i < dimension; ++i) { //各軸の同じ位置のbitを並べて1つの数字にする
			branchnum <<= 1;
			branchnum |= (int)((bool)(code[i] & bitpoint));
		}

		bitpoint >>= 1;

		Node** temp(&result->branch[branchnum]); //上で取り出した数字番目の枝へ移る

		if (!(*temp)) {	//nodeが存在していないならここで生成
			*temp = new Node(this);
			(*temp)->parent = result;
		}

		result = *temp;

	}// codelength分この処理を繰り返す

	return result;

}

template <typename Instance>
int BaseBinaryTree<Instance>::wholenodenum(0);

/***************************************************************************

	BaseBinaryTree::Node

****************************************************************************/

template <typename Instance>
BaseBinaryTree<Instance>::Node::Node(const BaseBinaryTree<Instance>* tree) : parent(), branch(tree->branchlength), instance()
{}

template <typename Instance>
bool BaseBinaryTree<Instance>::Node::Isedge(void) {

	bool unbranch(true);

	for (auto branch : this->branch) {

		if (branch) {
			unbranch = false;
			break;	
		}

	}

	return unbranch;

}

template <typename Instance>
bool BaseBinaryTree<Instance>::Node::IsNulledge(void) {	
	return Isedge() && (instance == Instance());
}

template <typename Instance>
void BaseBinaryTree<Instance>::Node::deletetree(Node* start) { //根から端方向へ削除

	for (auto branch : start->branch) {
		if (branch) deletetree(branch);
	}

	delete start;

}

/***************************************************************************

	BinaryTree

****************************************************************************/

/*
	[ BinaryTree ]

	・NodeInfoに登録位置のcode,codelength,登録したInstance,TreeIDを記録する。

	・RegisterやRemoveを行う際にはこのNodeInfoが必要となり、自動で前回の登録を修正してくれる。

	・TreeIDが一致しているかのチェックをする。TreeIDを持っていればTreeに配置されているはず。
	・異なるTreeのノードへのアクセス or Treeに未登録のnodeをRemove()しても大丈夫にしている。

	・オブジェクトのLoad時に初期登録を忘れないように


	・余分となった枝の"伐採"を行うために"removetree"を用意している。"

		・removetreeはＴree構造で、本TreeでNodeのRemove()時にノードがNulledgeなら(本Treeと同じ位置に)登録する。
		・Register段階でそのNodeは上書きされる可能性はあるし、edgeでなくなる可能性もある
		・オブジェクトの移動時と衝突処理時の2回でオブジェクトは動き回り、重複登録が考えられるのでTree構造を採用する。
		
		・本TreeのUpdate()時にremovetreeをUpdate()し、removetreeの全ノードを配列に収納。
		・その後removetreeのedgeをたどり、本Treeの該当ノードがNulldgeなら親を含めた余分な枝を削除。
		・余分な枝の削除後に本Treeの全ノードを配列に収納。

	・Register()時にTreeでの配置変更の有無にかかわらず一度取り除いてremovetreeに登録後、再び本Treeに登録している。


*/

namespace object {
	using ObjectID = unsigned int;
}

template class BaseBinaryTree<std::vector<object::ObjectID>>;
template class BinaryTree<object::ObjectID>;
template class BaseBinaryTree<BaseBinaryTree<std::vector<object::ObjectID>>::Node*>;

template <typename Instance>
const unsigned int BinaryTree<Instance>::defaultremovetablecap(BaseBinaryTree<std::vector<Instance>>::defaulttablecap);

template <typename Instance>
BinaryTree<Instance>::BinaryTree(unsigned int dimension, unsigned int tablecap/* = defaulttablecap*/, unsigned int removetablecap/* = defaultremovetablecap*/) :
	BaseBinaryTree<std::vector<Instance>>(dimension, tablecap), removetree(dimension, removetablecap), parenttable_compare5() {
	parenttable_compare5.reserve(9);
}

template <typename Instance>
BinaryTree<Instance>::~BinaryTree(void) {
	//std::cout << "Nodeinfoleft";
}

template <typename Instance>
void BinaryTree<Instance>::Register(NodeInfo& returninfo, const Instance& instance, const std::vector<char>& code1, const std::vector<char>& code2) {	// copy禁止のものをretturnできても後が面倒なので

	if ((code1.size() != this->dimension) || (code2.size() != this->dimension))	
		std::cout << "dimensionmismatch";

	remove(returninfo);	// treeIDを持たないならここで登録。持てば前回位置を修正。

	returninfo.code = code1;
	returninfo.codelength = BaseBinaryTree<std::vector<Instance>>::getcodelength(code1, code2);
	returninfo.instance = instance;

	BaseBinaryTree<std::vector<Instance>>::getnode(code1, returninfo.codelength)->instance.emplace_back(instance);

}

template <typename Instance>
void BinaryTree<Instance>::Remove(NodeInfo& nodeinfo) {
	remove(nodeinfo);
	nodeinfo.tree = nullptr; //treeIDは0に。元からnullptrのnodeをRemove()しても大丈夫。
}

template <typename Instance>
void BinaryTree<Instance>::remove(NodeInfo& info) {

	if (isvergininfo(info)) return; // ここでtreeIDのチェックor登録
		
	auto node(BaseBinaryTree<std::vector<Instance>>::getnode(info.code, info.codelength)/*info.node*/);

	for (auto iter(node->instance.begin()); iter != node->instance.end(); ++iter) {

		if (*iter == info.instance) {
			node->instance.erase(iter); // 前回の登録を削除
			break;
		}

	}

	if (node->IsNulledge()) // nodeがNulledgeならここでremovetreeに登録
		removetree.Register(node, info.code, info.codelength);

}

template <typename Instance>
void BinaryTree<Instance>::remove_compare1(NodeInfo& info) {

	if (isvergininfo(info)) return;

	auto node(BaseBinaryTree<std::vector<Instance>>::getnode(info.code, info.codelength)/*info.node*/);

	for (auto iter(node->instance.begin()); iter != node->instance.end(); ++iter) {

		if (*iter == info.instance) {
			node->instance.erase(iter);
			break;
		}

	}
	// removetreeは用いない
}

template <typename Instance>
bool BinaryTree<Instance>::Update(void) {

	if (removetree.Update()) {

		if (!removetree.present()) removetree.NextEdge();	// 条件がtrueならNextEdgeは必ずある

		do {// 本Treeの伐採

			if (removetree.present()->IsNulledge())
				BaseBinaryTree<std::vector<Instance>>::Remove(removetree.present());

		} while (removetree.NextEdge());

	}

	removetree.Clear();

	return ((BaseBinaryTree<std::vector<Instance>>*)this)->Update(); //本Treeの更新

}

template <typename Instance>
bool BinaryTree<Instance>::Update_compare1(void) {

	if (((BaseBinaryTree<std::vector<Instance>>*)this)->Update()) {//removetreeでなく本Treeの更新

		do { //本Treeを直に探りNulledgeを探して削除。幅優先探索なので続けて削除する親ノードにその後アクセスすることは無い。

			if (((BaseBinaryTree<std::vector<Instance>>*)this)->presentnode_compare1()->IsNulledge())
				BaseBinaryTree<std::vector<Instance>>::Remove(((BaseBinaryTree<std::vector<Instance>>*)this)->presentnode_compare1());

		} while (((BaseBinaryTree<std::vector<Instance>>*)this)->NextEdge());

	}

	return ((BaseBinaryTree<std::vector<Instance>>*)this)->Update(); // 再び本Treeを更新

}

template <typename Instance>
bool BinaryTree<Instance>::Update_compare2(void) { 

	for (auto delnode : BaseBinaryTree<std::vector<Instance>>::deletetable_compare2) { // 全フレームの更新時に記録しておいた削除候補をチェック後削除

		if (delnode->BaseBinaryTree<std::vector<Instance>>::Node::IsNulledge())
			BaseBinaryTree<std::vector<Instance>>::Remove(delnode);

	}// ここで伐採

	return ((BaseBinaryTree<std::vector<Instance>>*)this)->Update_compare2();

}

template <typename Instance>
bool BinaryTree<Instance>::Update_compare5(void) {
	
	removetree.Update_compare5(); //removetreeでもノードを一々配列に収納しない

	for (auto del : removetree.deletetable_compare5) {

		if (del->IsNulledge()) BaseBinaryTree<std::vector<Instance>>::Remove(del);

	}

	removetree.Clear();

	return true;

}

template <typename Instance>
void BinaryTree<Instance>::Register_compare1(NodeInfo& returninfo, const Instance& instance, const std::vector<char>& code1, const std::vector<char>& code2) {	// copy禁止のものをretturnできても後が面倒なので

	if ((code1.size() != this->dimension) || (code2.size() != this->dimension))
		std::cout << "dimensionmismatch";	

	remove_compare1(returninfo); //removetree非使用

	returninfo.code = code1;
	returninfo.codelength = BaseBinaryTree<std::vector<Instance>>::getcodelength(code1, code2);
	returninfo.instance = instance;

	BaseBinaryTree<std::vector<Instance>>::getnode(code1, returninfo.codelength)->instance.emplace_back(instance);
	
}

template <typename Instance>
void BinaryTree<Instance>::Register_compare3(NodeInfo& returninfo, const Instance& instance, const std::vector<char>& code1, const std::vector<char>& code2) {

	if ((code1.size() != this->dimension) || (code2.size() != this->dimension))
		std::cout << "dimensionmismatch";

	int codelength(BaseBinaryTree<std::vector<Instance>>::getcodelength(code1, code2));

	if ( // 配置変更があれば
		(returninfo.code != code1) ||
		(returninfo.codelength != codelength) ||
		(returninfo.instance != instance) // 未登録ノードでもここで大丈夫か(突貫工事)
		)
	{

		typename BaseBinaryTree<std::vector<Instance>>::Node* node(nullptr);

		if (!isvergininfo(returninfo)) {

			node = BaseBinaryTree<std::vector<Instance>>::getnode(returninfo.code, returninfo.codelength/*info.node*/);

			for (auto iter(node->instance.begin()); iter != node->instance.end(); ++iter) {

				if (*iter == returninfo.instance) {
					node->instance.erase(iter); // 前回の登録を削除
					break;
				}

			}

		}

		BaseBinaryTree<std::vector<Instance>>::getnode(code1, codelength)->instance.emplace_back(instance);

		if (node && node->IsNulledge()) removetree.Register(node, returninfo.code, returninfo.codelength);

		returninfo.code = code1;
		returninfo.codelength = codelength;
		returninfo.instance = instance;

	}// 無駄な処理を減らすためremovetreeへの登録処理をgetnode()の後に移動。

}


template <typename Instance>
void BinaryTree<Instance>::Register_compare4(NodeInfo& returninfo, const Instance& instance, const std::vector<char>& code1, const std::vector<char>& code2) {

	if ((code1.size() != this->dimension) || (code2.size() != this->dimension))
		std::cout << "dimensionmismatch";

	int codelength(BaseBinaryTree<std::vector<Instance>>::getcodelength(code1, code2));

	if (
		(returninfo.code != code1) ||
		(returninfo.codelength != codelength) ||
		(returninfo.instance != instance) 
		)
	{

		//typename BaseBinaryTree<std::vector<Instance>>::Node* node(nullptr);

		if (!isvergininfo(returninfo)) {

			auto node = BaseBinaryTree<std::vector<Instance>>::getnode(returninfo.code, returninfo.codelength/*info.node*/);

			for (auto iter(node->instance.begin()); iter != node->instance.end(); ++iter) {

				if (*iter == returninfo.instance) {
					node->instance.erase(iter); 
					break;
				}

			}

		}

		BaseBinaryTree<std::vector<Instance>>::getnode(code1, codelength)->instance.emplace_back(instance);

		// removetreeは用いない
		//if (node && node->IsNulledge()) removetree.Register(node, returninfo.code, returninfo.codelength);

		returninfo.code = code1;
		returninfo.codelength = codelength;
		returninfo.instance = instance;

	}

}

template <typename Instance>
void BinaryTree<Instance>::Collision_compare5(void) {

	if (!BaseBinaryTree<std::vector<Instance>>::startnode) return;

	collide_compare5(BaseBinaryTree<std::vector<Instance>>::startnode);

}

template <typename Instance>
void BinaryTree<Instance>::Collision_compare6(void) {

	if (!BaseBinaryTree<std::vector<Instance>>::startnode) return;

	collide_compare6(BaseBinaryTree<std::vector<Instance>>::startnode);

}

template <typename Instance>
bool BinaryTree<Instance>::isvergininfo(NodeInfo& nodeinfo) {

	bool virgin(false);

	if (!nodeinfo.tree) {
		nodeinfo.tree = this;
		nodeinfo.code.resize(this->dimension);	
		virgin = true;
	}
	else if (nodeinfo.tree != this)	
		std::cout << "treemismatch";

	return virgin;

}	

/***************************************************************************

	BinaryTree::NodeInfo

****************************************************************************/

/*	
 [ NodeInfo 仕様 ]

	・外部からはアクセス禁止&コピー禁止でTreeに登録されているオブジェクトと1対1に対応するように。

	・NodeInfoはremovetreeに登録することを考えてcodeとcodelengthを位置情報として持つ。

	・NodeInfoをメンバに持つオブジェクトがデストラクトされるとTreeの方からも削除するようにしている。

	・先にTreeをdeleteした場合はNodeInfoのデータは残る。注意すべし。

*/

template <typename Instance>
BinaryTree<Instance>::NodeInfo::NodeInfo(void) : code(), codelength(), instance(), tree()/*, node()*/ {}

template <typename Instance>
BinaryTree<Instance>::NodeInfo::~NodeInfo(void) {
	if (tree) tree->Remove(*this);	
}	