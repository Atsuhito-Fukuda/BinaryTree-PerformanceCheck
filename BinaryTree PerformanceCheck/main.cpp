#include <iostream>
#include <cstdlib>   
#include <ctime>  
#include <chrono>
#include "BinaryTree_comparetest.h"
#include "Vector.h"

extern int simpletreetest(void); //Tree簡易test用
extern void simplecollisiontest(void);

enum Type : unsigned int {	

	Dot = 0b1, LineSegment = 0b10, Plane = 0b100,
	AABB = 0b1000, max = AABB,

	isotropic = max << 1,	
	vertical = isotropic << 1,	

	Sphere = Dot | isotropic,	
	Capsule = LineSegment | isotropic,

	sort = (max << 1) - 1,
	attribute = ~sort

};
enum enumpoints : unsigned int { //Shape::pointsの要素番号
	p1, p2, p3, att1	
};
enum Max : unsigned int {
	POINTSNUM = enumpoints::att1 + 1
};
namespace adds {

	enum dot : unsigned int {
		point = enumpoints::p1
	};
	enum lineseg : unsigned int {
		start = enumpoints::p1, end = enumpoints::p2
	};
	enum plane : unsigned int {
		plpoint = enumpoints::p1, normal = enumpoints::p2
	};
	enum aabb : unsigned int {
		min = enumpoints::p1, max = enumpoints::p2
	};
}
unsigned int number(0);
class Shape  { 

public:

	GLMatrix mat; //posベクトルとユニタリー行列
	unsigned int id;

	Type type; //collisionの型
	std::vector<Vector3> points; //collisionの情報を保持
	Vector3 minboundary, maxboundary;	//境界AABB
	bool reprocess;	//未使用
	GLMatrix previous;	//未使用
	Vector3 velocity;		

	BinaryTree<unsigned int>::NodeInfo nodeinfo; //BinaryTreeの登録情報

	Shape(void) : mat(), id(++number),
		type(Type::Dot), velocity(), reprocess(), points(), previous(), minboundary(), maxboundary()
	{

		type = Type::Sphere; // 球を指定

		points.emplace_back(Vector3());
		points.emplace_back(Vector3());
		points.emplace_back(Vector3());
		points.emplace_back(Vector3(2.0f, 0.0f, 0.0f)); //半径2を指定

	}

};

void FirstAct(Shape& shape);
void TreeRegister(Shape& shape, BinaryTree_comparetest& bin);
void GetBoundary(Shape& shape);
void Collision(BinaryTree<unsigned int>& actiontree);

// ↑ namespace actionより借用 --------------------------------

void Make(Shape& shape);
void BoundaryCheck(Shape& shape);
const Vector3 sectionlength(Vector3(1.0f, 1.0f, 1.0f) * 10); //最小区間10x10x10
const Vector3 boundary(sectionlength * 128); //一軸128x2の区間

unsigned int Test(void);

/*

  ---------------------------------------------

	・compare0 : "remove tree"を使用。配置変更の有無に関わらず再配置&remove tree呼び出し。
	・compare1 : "remove tree"は用いないがBinaryTreeのUpdateでNulledgeノードを捜索・削除後続けてUpdate。
	・compare2 : Updateの走査時にNulledgeを配列に収納。1フレーム遅れて伐採。
	・compare3 : compare0と同じだが、配置変更時のみに再登録&remove treeを呼び出す。
	・compare4 : compare2と同じだcompare3と比較するためにこちらも配置変更時のみに再登録

	・compare5 : collisionをスタック方式に移行。"remove tree"(こちらもノードの配列収用をやめる)を用いたver。
	・compare6 : compare5と同じだが"remove tree"を用いず捜査時にNulledgeでないか全ノードを調べる。
	・compare7 : 200フレーム間全く伐採することなく衝突処理を行う。

	結果はallresult.txt(ソースコードと同じ場所に)に。

　---------------------------------------------

  比較はあくまでもcompare0～compare4同士、またはcompare5～compare7の中で行うのがよい。

  [compare0～compare4]

  ・compare0～compare4では、本Tree、removetreeともに一度配列に全ノードを収納するようにする。
		・配列はstd::vectorでemplace_backを用いて収納している(reserve()は用いている)。

  ・衝突処理では各ノードは毎回親ノードをチェックし配列に収納する。(こちらもemplace_back)

  [compare5～compare7]

  ・こちらでは衝突処理は関数を再帰的に呼び出して行う。

  ・スタックにpushするのはemplace_back。

　compare0～compare4、compare5～compare7の2つの方式を比較したいとしても、互いに最善のコーディングをしてはおらず
  数値はあくまでも目安か。

  ---------------------------------------------


*/


const Mode mode = Mode::compare7; //Treeの登録・更新・衝突の方式
const unsigned int allobjectsN(1000), dynamicobjectsN(500); //オブジェクト総数とその中での動的なオブジェクトの数
const float maxspeed(2.0f); // 距離毎フレーム

int main(void) {

	if (0) { simpletreetest(); return 0; } //各modeでの、登録・消去・更新の簡易テスト
	else if (0) { simplecollisiontest(); return 0; } //各modeでの、衝突の簡易テスト

	const unsigned int N(50); //Test回数

	float mean(0), var(0);

	std::cout <<
		"* allobjectN = " << allobjectsN <<
		", dynamicobject = " << dynamicobjectsN <<
		", maxspeed = " << maxspeed <<
		" ";

	BinaryTree_comparetest::testmode();

	for (unsigned int sample(0); sample < N; ++sample) { //N回のTest()とその平均値・偏差

		unsigned int result(Test());

		mean += (float)result;
		var += (float)pow(result, 2);

		if (!(sample % 10)) std::cout << "\r" << "process:" << sample;

	}

	mean /= N;
	var = (var - (float)pow(mean, 2) * N) / (N - 1);

	std::cout << "\rmean:" << mean << "[ms] sqrt(var):" << sqrt(var) << "[ms]\n";

}

unsigned int Test(void) { //八分木でtest

	BinaryTree_comparetest bin; //各modeごとのBinaryTree<unsigned int>関数を使い分ける(BinaryTree_comparetest.hに)

	const auto start = std::chrono::system_clock::now();

	std::srand((unsigned int)time(NULL));
	std::vector<Shape> table(allobjectsN);

	for (auto& shape : table) { //初期化
		Make(shape); 
		BoundaryCheck(shape);
		TreeRegister(shape, bin);
	}

	for (unsigned int plays(0); plays < 200; ++plays) { //200フレーム処理

		for (unsigned int n(0); n < dynamicobjectsN; ++n) {// 動的なのは一部のみ
			FirstAct(table[n]);
			BoundaryCheck(table[n]);
			TreeRegister(table[n], bin);
		}

		bin.Update();

		bin.cCollision();

		/*static int a(0); if (++a == 1) {
			std::cout << collisioncount << "\n";
			bin.Showresult(true);
			collisioncount = 0;
			bin.actiontree.Collision_compare5();
			std::cout << collisioncount << "\n";
		}*/

	}

	const auto end = std::chrono::system_clock::now();

	return (unsigned int)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

}

void Make(Shape& shape) {

	for (unsigned int i(0); i < 3; ++i) {
		
		shape.mat.pos[i] = 
			(2.0f * rand() / RAND_MAX - 1.0f) * (boundary[i] - shape.points[enumpoints::att1][0]);

		shape.velocity[i] = (2.0f * rand() / RAND_MAX - 1.0f) * maxspeed; //静的なものもにも一応数値だけ与える

	}
		
}
void FirstAct(Shape& shape) {
	shape.previous = shape.mat; //未使用
	shape.mat.pos += shape.velocity /* *dt */;
}

std::vector<char> mincode(3), maxcode(3);

void TreeRegister(Shape& shape, BinaryTree_comparetest& bin) {

	GetBoundary(shape); 

	//std::vector<char> mincode(3), maxcode(3);

	for (int i(0); i < 3; ++i) {
		mincode[i] = 0; maxcode[i] = 0;
		if (shape.minboundary[i] < 0.0f)	mincode[i] = -1;
		if (shape.maxboundary[i] < 0.0f)	maxcode[i] = -1;
		mincode[i] += (char)(shape.minboundary[i] / sectionlength[i]);
		maxcode[i] += (char)(shape.maxboundary[i] / sectionlength[i]);
	}

	bin.Register(shape.nodeinfo, shape.id, mincode, maxcode);

}

void BoundaryCheck(Shape& shape) { //空間を越えたものは逆側から復帰する

	for (unsigned int i(0); i < 3; ++i) {

		if (shape.mat.pos[i] >= boundary[i])
			shape.mat.pos[i] = -2.0f * boundary[i] + shape.mat.pos[i];

		if (shape.mat.pos[i] <= -boundary[i])
			shape.mat.pos[i] = 2.0f * boundary[i] + shape.mat.pos[i];

	}

}

void GetBoundary(Shape& shape) {

	switch (shape.type & Type::sort) {

	case Type::Dot: { //今回はこれのみ

		const Vector3 point(shape.mat.unitary * shape.points[adds::dot::point] + shape.mat.pos);

		shape.minboundary = point - Vector3(1.0f, 1.0f, 1.0f) * shape.points[enumpoints::att1][0];
		shape.maxboundary = point + Vector3(1.0f, 1.0f, 1.0f) * shape.points[enumpoints::att1][0];

		break;	
	}
	case Type::LineSegment: {
		const Vector3
			start(shape.mat.unitary * shape.points[adds::lineseg::start] + shape.mat.pos),
			end(shape.mat.unitary * shape.points[adds::lineseg::end] + shape.mat.pos);

		for (int i(0); i < 3; ++i) {

			float fmin(start[i]), fmax(end[i]);	
			if (start[i] > end[i]) {
				fmin = end[i];
				fmax = start[i];
			}

			shape.minboundary[i] = fmin;
			shape.maxboundary[i] = fmax;
		}

		shape.minboundary -= Vector3(1.0f, 1.0f, 1.0f) * shape.points[enumpoints::att1][0];
		shape.maxboundary += Vector3(1.0f, 1.0f, 1.0f) * shape.points[enumpoints::att1][0];

		break;
	}
	case Type::AABB: {
		const Vector3& aabbmin(shape.points[adds::aabb::min]);
		const Vector3& aabbmax(shape.points[adds::aabb::max]);

		std::vector<Vector3> dots{	
		Vector3(aabbmin),
		Vector3(aabbmin[0],aabbmin[1],aabbmax[2]),
		Vector3(aabbmin[0],aabbmax[1],aabbmin[2]),
		Vector3(aabbmin[0],aabbmax[1],aabbmax[2]),
		Vector3(aabbmax[0],aabbmin[1],aabbmin[2]),
		Vector3(aabbmax[0],aabbmin[1],aabbmax[2]),
		Vector3(aabbmax[0],aabbmax[1],aabbmin[2]),
		Vector3(aabbmax)
		};

		for (auto& dot : dots) {
			dot = shape.mat.unitary * dot + shape.mat.pos;
		}

		shape.minboundary = dots[0];	shape.maxboundary = dots[0];

		for (unsigned int i(1); i < dots.size(); ++i) {

			if (dots[i][0] < shape.minboundary[0])	shape.minboundary[0] = dots[i][0];
			else if (dots[i][0] > shape.maxboundary[0])	shape.maxboundary[0] = dots[i][0];

			if (dots[i][1] < shape.minboundary[1])	shape.minboundary[1] = dots[i][1];
			else if (dots[i][1] > shape.maxboundary[1])	shape.maxboundary[1] = dots[i][1];

			if (dots[i][2] < shape.minboundary[2])	shape.minboundary[2] = dots[i][2];
			else if (dots[i][2] > shape.maxboundary[2])	shape.maxboundary[2] = dots[i][2];

		}

		break;	
	}
	default:

		break;

	}

}

int collisioncount(0);

void Collision(BinaryTree<unsigned int>& actiontree) {

	using ObjectID = unsigned int;

	std::vector<const std::vector<ObjectID>*> list;
	list.reserve(9); //最大で9

	do {

		const std::vector<ObjectID>& mainnode(actiontree.present()); //nullならforでもスルー

		if(mainnode.size()) //collision2が不利とならないため急遽追加(余分な枝の分一々計算しないのが望ましいため)
		do { if (actiontree.present().size()) list.emplace_back(&actiontree.present()); }
		while (actiontree.NextParent());

		for (unsigned int i(0); i < mainnode.size(); ++i) {//size0なら通過

			unsigned int j(i + 1);	bool i_collision(false); //衝突処理部分を1所にしたいためにj(i+1)のように

			if (true) 
				for (auto othernode : list) {

					for (; j < othernode->size(); ++j) {

						//本題でないので具体的な衝突処理は省略
						++collisioncount;

					}
					j = 0; //nain以降は前から順に衝突処理
				}

		}

		list.clear();

	} while (actiontree.NextNode());


	// Second Process - - - - - - - - - - - - - 


	//省略


}