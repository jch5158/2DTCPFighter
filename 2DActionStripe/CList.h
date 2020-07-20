#pragma once

template <typename T>
class CList
{
public:

	struct Node {
		T data;
		Node* prev;
		Node* next;

		// 구조체 생성자
		Node(T data = NULL) {
			this->data = data;
			next = nullptr;
			prev = nullptr;
		}
	};

public:
	class Iterator {

	public:

		Node* node;


		// CList가 pivate에 접근할 수 있도록
		friend class CList;

		// 생성
		Iterator(Node* node = nullptr) {
			this->node = node;
		}


		Iterator NextIter() {

			Iterator iter(this->node->next);

			return iter;
		}


		void operator=(Iterator iter) {
			this->node = iter.node;
		}


		// 후위 ++ 연산자 오버로딩
		const Iterator operator++(int) {

			//노드에 넥스트가 있을 때
			if (node->next != nullptr)
			{
				const Iterator iterator(this->node);

				this->node = node->next;

				return iterator;
			}

			// 없으면 nullptr를 가리키는 노드를 return한다.
			return Iterator();
		}

		// 전위 ++ 연산자 오버로딩
		Iterator operator++() {
			//노드에 넥스트가 있을 때
			if (node->next != nullptr) {
				this->node = node->next;

				return this->node;
			}
			return Iterator();
		}

		const Iterator operator--(int) {

			//노드에 prev 있을 때
			if (node->prev != nullptr)
			{
				const Iterator iterator(this->node);

				this->node = node->prev;

				return iterator;
			}

			// 없으면 nullptr를 가리키는 노드를 return한다.
			return Iterator();
		}


		// 전위 -- 연산자 오버로딩
		Iterator operator--() {
			//노드에 prev 있을 때
			if (node->prev != nullptr) {
				this->node = node->prev;

				return this->node;
			}

			return Iterator();
		}

		T operator*() const {
			return this->node->data;
		}

		T operator->() const {
			return this->node->data;
		}

		bool operator==(const Iterator& iter) const {

			return this->node->data == iter.node->data;
		}

		bool operator!=(const Iterator& iter) const {
			return !(this->node->data == iter.node->data);
		}
		// dddd로 밀려있었네..

	};

public:

	Node* head;
	Node* tail;


	int listLength = 0;

	// CList 생성자
	CList() {

		head = new Node();
		tail = new Node();

		head->next = tail;
		tail->prev = head;

		tail->next = nullptr;
		head->prev = nullptr;
	}

	//  CList 소멸자
	~CList() {
		Node* prev = nullptr;

		while (head != nullptr) {
			prev = head;

			head = head->next;

			delete prev;
		}
	}

	// 꼬리노드 뒤에 추가하는 함수
	void PushBack(T data) {

		Node* node = new Node(data);

		InputNode(node, tail);


		//길이 체크하는 멤버변수입니다.
		this->listLength += 1;
	}

	// 헤더노드 다음에 추가
	void PushFront(T data) {

		Node* node = new Node(data);

		InputNode(node, head->next);

		//길이 체크하는 멤버변수입니다.
		this->listLength += 1;
	}

	// 특정 노드 뒤에 추가
	void insert(Iterator iter, T data) {

		Node* node = new Node(data);

		InputNode(node, iter.node);
	}


	void DataSwap(Iterator iterF ,Iterator iterE) {

		T bufferData = iterF.node->data;

		iterF.node->data = iterE.node->data;

		iterE.node->data = bufferData;
	}


	//시작값 반환
	Iterator begin() {

		Iterator iterator(head->next);

		return iterator;
	}

	// 끝값 반환
	Iterator end() {

		Iterator iterator(tail);
		return iterator;
	}

	Iterator erase(Iterator iter) {

		Iterator iterBuffer = iter;

		iterBuffer.node->prev->next = iterBuffer.node->next;

		iterBuffer.node->next->prev = iterBuffer.node->prev;

		Iterator iterator(iterBuffer.node->next);

		delete iterBuffer.node;

		//길이 체크하는 멤버변수입니다.
		this->listLength -= 1;

		return iterator;
	}

	// 특정 노드를 지우는 값이다.
	bool Remove(T data) {

		Iterator iterE = end();

		for (Iterator iter = begin(); iter != iterE; ++iter) {

			if (iter.node->data == data) {

				erase(iter);


				return true;
			}

		}

		return false;
	}



private:

	//매개변수 1 : 새로추가할 노드 , 매개변수 2 : 특정노드	
	//특정 위치의 노드뒤에 새로운 노드를 추가한다.
	void InputNode(Node* now, Node* pos) {

		now->prev = pos->prev;
		now->next = pos;

		pos->prev->next = now;
		pos->prev = now;
	}

	

};

