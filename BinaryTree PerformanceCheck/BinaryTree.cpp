#include "BinaryTree.h"

/***************************************************************************

	template <typename Instance> BaseBinaryTree

****************************************************************************/

/*

	[ BaseBinaryTree ]

	�E�e�m�[�h��Instance��ϐ��ɂ��B

	�E8bit����(code)�Ɠǂݎ��u����(codelength)�v��2��Tree�̈ʒu���w���B(code�~2�ł������������Ɏ��Ԃ������邩)
		�Ecode��bit�����͎}������ԍ��������Bcodelength�̒������擪bit����}��������J��Ԃ��B

	�E�u2^n���؁v�Ƃ������Ƃ�n��char�^�z��������Ƃɂ���B

	�EBaseBinaryTree�͓o�^�ƍ폜�͕ʓr�s���B(NodeInfo�������������폜�@�\���͂Ȃ�)

	�EBaseBinaryTree��Update()���s�����Ƃ�Tree�𑖍����A�S�m�[�h�𕝗D��T���Ŕz��ɏW������
		�E�o�^(Register())���Update���Ȃ��Ɣ��f����Ȃ�

	�ENextNode()�̓m�[�h���[�z��̗v�f��i�߁ANextParent()�͐e�m�[�h�ɃA�N�Z�X����Bpresent()��const Instance&��Ԃ��B
	�@NextEdge()�͎���Tree�̖��[�܂ŗv�f��i�߂�B
	
	�ERemove()�͎w��node�Ɋւ�炸�폜����B���̌�parent��branch�z��̊Y��ptr��nullptr�ɒu�������A
	�@parent�m�[�h��Nulledge�������ꍇ�ɂ�parent�������č폜����BNulledge�łȂ��Ȃ�܂ł�����J��Ԃ��B
		
	�E�h���N���X��Remove()(protected)��p����ꍇ�ɂ͎��[�z�����ɍX�V���Ȃ���΂Ȃ�Ȃ��B

	�EClear()�ł͑S�폜��Ɏ��[�z����X�V����B


*/

template <typename Instance>
bool BaseBinaryTree<Instance>::startreach(false);

template <typename Instance>
const unsigned int BaseBinaryTree<Instance>::defaulttablecap(5000);//�m�[�h���[�z��̃f�t�H���gcapacity

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
void BaseBinaryTree<Instance>::Register(const Instance& instance, const std::vector<char>& code, unsigned int codelength) {	// copy�֎~�̂��̂�retturn�ł��Ă��オ�ʓ|�Ȃ̂�
	if(code.size() != dimension) std::cout << "dimensionmismatch";	// �ėp���p��z�肵���ꍇ��error check
	getnode(code, codelength)->instance = instance;
}

template <typename Instance>
bool BaseBinaryTree<Instance>::Update(void) {

	bool exist(true);

	noderegister();	// clear�͕K�������ōs����B������node��z��Ɏ��[���Ă����B

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
void BaseBinaryTree<Instance>::removenodesearch_compare5(Node* node) {//removetree�̕���instance�����Ă��邱�Ƃɒ���

	bool isedge(true);

	for (auto branch : node->branch) {

		if (branch) {
			isedge = false;
			removenodesearch_compare5(branch);
		}

	}

	if (isedge) deletetable_compare5.emplace_back(node->instance); //�����ł͍폜�ł��Ȃ��̂Ŏd���Ȃ�

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
bool BaseBinaryTree<Instance>::NextNode(void) {	// �؂̏�->���A��->�E�ւƁv�i��

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

	remove(node); //node�����̂���Instance�łȂ��Ƃ��폜

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
				if (branch == node) branch = nullptr;	//�e�̎}����폜
				else unbranch = false;
			}

		}

		if (unbranch && (parent->instance == Instance())) //�e��Nulledge�Ȃ�e�������č폜
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

	} //�e�̍폜�͂��Ȃ�
	else
		startnode = nullptr;

}

template <typename Instance>
void BaseBinaryTree<Instance>::noderegister(void) {	//node�̕��D��T���Ɣz��ւ̎��[

	tablenum = 0;
	nodetable.clear();

	if (startnode) nodetable.emplace_back(startnode);

	for (unsigned int n(0); n < nodetable.size(); ++n) { //�������i�߂�size()�͑����Ă���

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

		if (isedge && (nodetable[n]->instance == Instance())) //������Nulledge���𔻒肵�ʔz��Ɏ��[
			deletetable_compare2.emplace_back(nodetable[n]);

	}


}

template <typename Instance>
unsigned int BaseBinaryTree<Instance>::getcodelength(const std::vector<char>& code1, const std::vector<char>& code2) {

	unsigned int length(0); int bitpoint(0b10000000);

	for (; length < 8; ++length) {	//2��code�̐擪����̋���bit�����̒������擾

		for (unsigned int i(0); i < dimension; ++i) { 

			if ((code1[i] & bitpoint) != (code2[i] & bitpoint)) goto End;

		}

		bitpoint >>= 1;

	}

End:
	return length;

} //���ǒ���copy�͕K�v�Ȃ̂ŃR�s�[�n���Bvector�͐����Ɏ��Ԃ�������H

template <typename Instance> 
typename BaseBinaryTree<Instance>::Node* BaseBinaryTree<Instance>::getnode(const std::vector<char>& code, unsigned int codelength) {

	if (!startnode) startnode = new Node(this);	
	
	Node* result(startnode);  int bitpoint(0b10000000);
	
	while (codelength--) {

		int branchnum(0);

		for (unsigned int i(0); i < dimension; ++i) { //�e���̓����ʒu��bit����ׂ�1�̐����ɂ���
			branchnum <<= 1;
			branchnum |= (int)((bool)(code[i] & bitpoint));
		}

		bitpoint >>= 1;

		Node** temp(&result->branch[branchnum]); //��Ŏ��o���������Ԗڂ̎}�ֈڂ�

		if (!(*temp)) {	//node�����݂��Ă��Ȃ��Ȃ炱���Ő���
			*temp = new Node(this);
			(*temp)->parent = result;
		}

		result = *temp;

	}// codelength�����̏������J��Ԃ�

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
void BaseBinaryTree<Instance>::Node::deletetree(Node* start) { //������[�����֍폜

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

	�ENodeInfo�ɓo�^�ʒu��code,codelength,�o�^����Instance,TreeID���L�^����B

	�ERegister��Remove���s���ۂɂ͂���NodeInfo���K�v�ƂȂ�A�����őO��̓o�^���C�����Ă����B

	�ETreeID����v���Ă��邩�̃`�F�b�N������BTreeID�������Ă����Tree�ɔz�u����Ă���͂��B
	�E�قȂ�Tree�̃m�[�h�ւ̃A�N�Z�X or Tree�ɖ��o�^��node��Remove()���Ă����v�ɂ��Ă���B

	�E�I�u�W�F�N�g��Load���ɏ����o�^��Y��Ȃ��悤��


	�E�]���ƂȂ����}��"����"���s�����߂�"removetree"��p�ӂ��Ă���B"

		�Eremovetree�͂sree�\���ŁA�{Tree��Node��Remove()���Ƀm�[�h��Nulledge�Ȃ�(�{Tree�Ɠ����ʒu��)�o�^����B
		�ERegister�i�K�ł���Node�͏㏑�������\���͂��邵�Aedge�łȂ��Ȃ�\��������
		�E�I�u�W�F�N�g�̈ړ����ƏՓˏ�������2��ŃI�u�W�F�N�g�͓������A�d���o�^���l������̂�Tree�\�����̗p����B
		
		�E�{Tree��Update()����removetree��Update()���Aremovetree�̑S�m�[�h��z��Ɏ��[�B
		�E���̌�removetree��edge�����ǂ�A�{Tree�̊Y���m�[�h��Nulldge�Ȃ�e���܂߂��]���Ȏ}���폜�B
		�E�]���Ȏ}�̍폜��ɖ{Tree�̑S�m�[�h��z��Ɏ��[�B

	�ERegister()����Tree�ł̔z�u�ύX�̗L���ɂ�����炸��x��菜����removetree�ɓo�^��A�Ăі{Tree�ɓo�^���Ă���B


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
void BinaryTree<Instance>::Register(NodeInfo& returninfo, const Instance& instance, const std::vector<char>& code1, const std::vector<char>& code2) {	// copy�֎~�̂��̂�retturn�ł��Ă��オ�ʓ|�Ȃ̂�

	if ((code1.size() != this->dimension) || (code2.size() != this->dimension))	
		std::cout << "dimensionmismatch";

	remove(returninfo);	// treeID�������Ȃ��Ȃ炱���œo�^�B���ĂΑO��ʒu���C���B

	returninfo.code = code1;
	returninfo.codelength = BaseBinaryTree<std::vector<Instance>>::getcodelength(code1, code2);
	returninfo.instance = instance;

	BaseBinaryTree<std::vector<Instance>>::getnode(code1, returninfo.codelength)->instance.emplace_back(instance);

}

template <typename Instance>
void BinaryTree<Instance>::Remove(NodeInfo& nodeinfo) {
	remove(nodeinfo);
	nodeinfo.tree = nullptr; //treeID��0�ɁB������nullptr��node��Remove()���Ă����v�B
}

template <typename Instance>
void BinaryTree<Instance>::remove(NodeInfo& info) {

	if (isvergininfo(info)) return; // ������treeID�̃`�F�b�Nor�o�^
		
	auto node(BaseBinaryTree<std::vector<Instance>>::getnode(info.code, info.codelength)/*info.node*/);

	for (auto iter(node->instance.begin()); iter != node->instance.end(); ++iter) {

		if (*iter == info.instance) {
			node->instance.erase(iter); // �O��̓o�^���폜
			break;
		}

	}

	if (node->IsNulledge()) // node��Nulledge�Ȃ炱����removetree�ɓo�^
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
	// removetree�͗p���Ȃ�
}

template <typename Instance>
bool BinaryTree<Instance>::Update(void) {

	if (removetree.Update()) {

		if (!removetree.present()) removetree.NextEdge();	// ������true�Ȃ�NextEdge�͕K������

		do {// �{Tree�̔���

			if (removetree.present()->IsNulledge())
				BaseBinaryTree<std::vector<Instance>>::Remove(removetree.present());

		} while (removetree.NextEdge());

	}

	removetree.Clear();

	return ((BaseBinaryTree<std::vector<Instance>>*)this)->Update(); //�{Tree�̍X�V

}

template <typename Instance>
bool BinaryTree<Instance>::Update_compare1(void) {

	if (((BaseBinaryTree<std::vector<Instance>>*)this)->Update()) {//removetree�łȂ��{Tree�̍X�V

		do { //�{Tree�𒼂ɒT��Nulledge��T���č폜�B���D��T���Ȃ̂ő����č폜����e�m�[�h�ɂ��̌�A�N�Z�X���邱�Ƃ͖����B

			if (((BaseBinaryTree<std::vector<Instance>>*)this)->presentnode_compare1()->IsNulledge())
				BaseBinaryTree<std::vector<Instance>>::Remove(((BaseBinaryTree<std::vector<Instance>>*)this)->presentnode_compare1());

		} while (((BaseBinaryTree<std::vector<Instance>>*)this)->NextEdge());

	}

	return ((BaseBinaryTree<std::vector<Instance>>*)this)->Update(); // �Ăі{Tree���X�V

}

template <typename Instance>
bool BinaryTree<Instance>::Update_compare2(void) { 

	for (auto delnode : BaseBinaryTree<std::vector<Instance>>::deletetable_compare2) { // �S�t���[���̍X�V���ɋL�^���Ă������폜�����`�F�b�N��폜

		if (delnode->BaseBinaryTree<std::vector<Instance>>::Node::IsNulledge())
			BaseBinaryTree<std::vector<Instance>>::Remove(delnode);

	}// �����Ŕ���

	return ((BaseBinaryTree<std::vector<Instance>>*)this)->Update_compare2();

}

template <typename Instance>
bool BinaryTree<Instance>::Update_compare5(void) {
	
	removetree.Update_compare5(); //removetree�ł��m�[�h����X�z��Ɏ��[���Ȃ�

	for (auto del : removetree.deletetable_compare5) {

		if (del->IsNulledge()) BaseBinaryTree<std::vector<Instance>>::Remove(del);

	}

	removetree.Clear();

	return true;

}

template <typename Instance>
void BinaryTree<Instance>::Register_compare1(NodeInfo& returninfo, const Instance& instance, const std::vector<char>& code1, const std::vector<char>& code2) {	// copy�֎~�̂��̂�retturn�ł��Ă��オ�ʓ|�Ȃ̂�

	if ((code1.size() != this->dimension) || (code2.size() != this->dimension))
		std::cout << "dimensionmismatch";	

	remove_compare1(returninfo); //removetree��g�p

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

	if ( // �z�u�ύX�������
		(returninfo.code != code1) ||
		(returninfo.codelength != codelength) ||
		(returninfo.instance != instance) // ���o�^�m�[�h�ł������ő��v��(�ˊэH��)
		)
	{

		typename BaseBinaryTree<std::vector<Instance>>::Node* node(nullptr);

		if (!isvergininfo(returninfo)) {

			node = BaseBinaryTree<std::vector<Instance>>::getnode(returninfo.code, returninfo.codelength/*info.node*/);

			for (auto iter(node->instance.begin()); iter != node->instance.end(); ++iter) {

				if (*iter == returninfo.instance) {
					node->instance.erase(iter); // �O��̓o�^���폜
					break;
				}

			}

		}

		BaseBinaryTree<std::vector<Instance>>::getnode(code1, codelength)->instance.emplace_back(instance);

		if (node && node->IsNulledge()) removetree.Register(node, returninfo.code, returninfo.codelength);

		returninfo.code = code1;
		returninfo.codelength = codelength;
		returninfo.instance = instance;

	}// ���ʂȏ��������炷����removetree�ւ̓o�^������getnode()�̌�Ɉړ��B

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

		// removetree�͗p���Ȃ�
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
 [ NodeInfo �d�l ]

	�E�O������̓A�N�Z�X�֎~&�R�s�[�֎~��Tree�ɓo�^����Ă���I�u�W�F�N�g��1��1�ɑΉ�����悤�ɁB

	�ENodeInfo��removetree�ɓo�^���邱�Ƃ��l����code��codelength���ʒu���Ƃ��Ď��B

	�ENodeInfo�������o�Ɏ��I�u�W�F�N�g���f�X�g���N�g������Tree�̕�������폜����悤�ɂ��Ă���B

	�E���Tree��delete�����ꍇ��NodeInfo�̃f�[�^�͎c��B���ӂ��ׂ��B

*/

template <typename Instance>
BinaryTree<Instance>::NodeInfo::NodeInfo(void) : code(), codelength(), instance(), tree()/*, node()*/ {}

template <typename Instance>
BinaryTree<Instance>::NodeInfo::~NodeInfo(void) {
	if (tree) tree->Remove(*this);	
}	