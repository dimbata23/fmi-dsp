#pragma once

#include <string>
#include <vector>

namespace dsp {

	class Horo {

	public:

		Horo();
		Horo(const Horo&) = delete;
		Horo& operator=(const Horo&) = delete;
		~Horo();

	public:

		void fill(std::ifstream& in);
		void print() const;
		void releaseLeft(const std::string& who)  { changeRelation(who, -1, false); }
		void releaseRight(const std::string& who) { changeRelation(who,  1, false); }
		void releaseBoth(const std::string& who)  { changeRelation(who,  0, false); }
		void grabLeft(const std::string& who)  { changeRelation(who, -1, true); }
		void grabRight(const std::string& who) { changeRelation(who,  1, true); }
		void grabBoth(const std::string& who)  { changeRelation(who,  0, true); }
		void checkToRemove(const std::string& who);
		bool isDisbanded() const { return disbanded; }
		void info(const std::string& who) const;
		void add(const std::string& who, const std::string& neighbor1, const std::string& neighbor2);
		void swap(const std::string& who1, const std::string& who2);

	private:

		struct Participant {

			Participant(const std::string& name) :
				name(name),
				pLeft(nullptr),
				pRight(nullptr),
				isHoldingLeft(false),
				isHoldingRight(false)
			{}

			std::string name;
			Participant* pLeft;
			Participant* pRight;
			bool isHoldingLeft;
			bool isHoldingRight;

		};

		struct ListNode {

			ListNode(const std::string& name, ListNode* pNext = nullptr, ListNode* pPrev = nullptr) :
				data(new Participant(name)),
				pNext(pNext),
				pPrev(pPrev)
			{}

			Participant* data;
			ListNode* pNext;
			ListNode* pPrev;

		};

		size_t hash(const std::string str) const;
		Participant* insert(const std::string name);
		Participant* insertAt(ListNode*& place, ListNode* node);
		bool insert(const std::string name, Participant* leftNeighbor, Participant* rightNeighbor);
		ListNode* get(const std::string& name) const;
		void changeRelation(const std::string& who, char dir, bool grab);
		void checkToRemove(Participant* who);
		void checkToRemove(ListNode* who);
		void removeFromHoro(ListNode* who);
		void disband();
		bool resize();

	private:

		std::vector<ListNode*> arr;
		Participant* first;
		int size;
		int capacity;
		short currDefaultSize;
		bool disbanded;

	};

}
