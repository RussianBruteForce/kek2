#ifndef BTS
#define BTS

#include <iostream>
#include <functional>

template<class T>
class Bts {

public:
	struct Node {
		T data;
		Node *left, *right;
		~Node() {
			if (left)
				delete left;
			if (right)
				delete right;
		}
	};

	Bts(): root(nullptr) {}
	~Bts() { delete root; }

	void add(T& data) {
		add(data,root);}
	void traverse(auto c) { traverse(c,root);}

private:
	void add(T& data, Node *&n) {
		if (n == nullptr) {
			n = new Node;
			n->data = data;
			n->left = n->right = nullptr;
		} else if (n->data < data) {
			data.deep();
			add(data, n->left);
		} else {
			data.deep();
			add(data, n->right);
		}
	}

	void traverse(auto c, Node* n) {
		if (!n)
			return;
		traverse(c, n->left);
		c(n->data);
		traverse(c, n->right);
	}
	Node *root;
};

#endif // BTS

