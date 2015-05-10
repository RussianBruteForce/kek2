#ifndef BTS
#define BTS

#include <iostream>
#include <functional>
#include <QDebug>

template<class T>
class Bts {

public:
	struct Node {
		Node() = default;
		Node(T&& data):
		        data(std::move(data)) {}
		T data;
		Node *left{nullptr}, *right{nullptr};
		~Node() {
			if (left)
				delete left;
			if (right)
				delete right;
		}
	};

	Bts() = default;
	~Bts() {if (root)
	       delete root;}

	void add(T& data) {
		add(data,root);}
	/*
	 * Adding by rvalue is not used in this example project
	 */
	void add(T&& data) {
		add(std::move(data),root);}
	void traverse(auto c) { traverse(c,root);}

private:
	void add(T& data, Node *&n) {
		if (n == nullptr) {
			n = new Node;
			n->data = data;
		} else if (n->data < data) {
			data.deep();
			add(data, n->left);
		} else {
			data.deep();
			add(data, n->right);
		}
	}
	void add(T&& data, Node *&n) {
		qDebug() << "rhv";
		if (n == nullptr) {
			n = new Node{std::move(data)};
		} else if (n->data < data) {
			data.deep();
			add(std::move(data), n->left);
		} else {
			data.deep();
			add(std::move(data), n->right);
		}
	}

	bool traverse(auto& c, Node* n) {
		if (!n)
			return false;
		if (
		                traverse(c, n->left) ||
		                c(n->data) ||
		                traverse(c, n->right))
			return true;
		return false;
	}
	Node *root{nullptr};
};

#endif // BTS

