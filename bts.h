#ifndef BTS
#define BTS

#include <functional>

/*
 * https://www.linux.org.ru/forum/development/11597146
 */
template<typename T, typename D>
struct NodeCRTP {
	NodeCRTP() = default;
	NodeCRTP(T&& data):
	        data(std::move(data)) {}
	T data;
	D *left{nullptr}, *right{nullptr};
	~NodeCRTP() noexcept {
		if (left)
			delete left;
		if (right)
			delete right;
	}
};

template<typename T>
struct BtsNode : NodeCRTP<T, BtsNode<T>> {
	using NodeCRTP<T, BtsNode<T>>::NodeCRTP;
};

template<class T>
class Bts {
	using Node = BtsNode<T>;
public:
	explicit Bts() = default;
	Bts(Bts&&) = delete;
	Bts& operator=(Bts&&) = delete;
	~Bts() {if (root)
	       delete root;}

	virtual void add(T& data) {
		inc();
		add(data,root);}
	virtual void add(T&& data) {
		inc();
		add(std::move(data),root);}
	void traverse(auto c) const { traverse(c,root);}
	auto size() {return _size;}

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

	bool traverse(auto& c, Node* n) const {
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
protected:
	void inc() {_size++;}
	std::size_t _size{0};
};

#endif // BTS

