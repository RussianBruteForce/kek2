#ifndef THREADEDBTS
#define THREADEDBTS

#include "bts.h"

template<typename T>
struct ThreadedBtsNode : public BtsNode<T> {
	std::int_fast8_t lf{0}, rf{1};
	ThreadedBtsNode *left{nullptr}, *right{nullptr};
};

template<typename T>
class ThreadedBts : public Bts<T>
{
	using Node = ThreadedBtsNode<T>;
public:
	ThreadedBts() {
		root = new Node;
		root->left = root->right = root;
	}

	void add(T&& data) override {
		Bts<T>::inc();
		auto n = new Node;
		n->data = std::move(data);
		if (root->left == root && root->right == root) {
			n->lf = root->lf;
			n->left = root->left;
			root->left = n;
			root->lf = 1;
			n->rf = 0;
			n->right = root;
		} else {
			auto current = root->left;
			if (!current)
				qDebug() << "alerta";
			while (true) {
				if (n->data < current->data) {
					if (!current->lf) {
						directionLeft = true;
						directionRight = false;
						break;
					} else
						current = current->left;
				} else {
					if (!current->rf) {
						directionLeft = false;
						directionRight = true;
						break;
					} else
						current = current->right;
				}
			}

			if (directionLeft) {
				n->lf = current->lf;
				n->left = current->left;
				current->left = n;
				current->lf = 1;
				n->rf = 0;
				n->right = current;
			} else if (directionRight) {
				n->rf = current->rf;
				n->right = current->right;
				current->right = n;
				current->rf = 1;
				n->lf = 0;
				n->left = current;
			} else
				EXIT_FAILURE;
		}
	}

	/*
	 * Not used in this example
	 */
	void add(T& data) override {
		qDebug() << "Adding by lvalue to tbts";
		Bts<T>::inc();
		auto n = new Node;
		n->data = data;
		if (root->left == root && root->right == root) {
			n->lf = root->lf;
			n->left = root->left;
			root->left = n;
			root->lf = 1;
			n->rf = 0;
			n->right = root;
		} else {
			auto current = root->left;
			if (!current)
				qDebug() << "alerta";
			while (true) {
				if (n->data < current->data) {
					if (!current->lf) {
						directionLeft = true;
						directionRight = false;
						break;
					} else
						current = current->left;
				} else {
					if (!current->rf) {
						directionLeft = false;
						directionRight = true;
						break;
					} else
						current = current->right;
				}
			}

			if (directionLeft) {
				n->lf = current->lf;
				n->left = current->left;
				current->left = n;
				current->lf = 1;
				n->rf = 0;
				n->right = current;
			} else if (directionRight) {
				n->rf = current->rf;
				n->right = current->right;
				current->right = n;
				current->rf = 1;
				n->lf = 0;
				n->left = current;
			} else
				EXIT_FAILURE;
		}
	}
	void traverse(auto c) override {
		auto current = root->left;

		while (current->lf == 1)
			current = current->left;

		while(current != root){
			c(current->data);
			current = next(current);
		}
	}

private:
	Node *root;
	bool directionLeft{true},
	     directionRight{false};
	Node* next(Node *const & n) const {
		if(n->rf == 0)
			return n->right;

		auto tmp = n->right;
		while(tmp->lf == 1)
			tmp = tmp->left;
		return tmp;
	}
};

#endif // THREADEDBTS

