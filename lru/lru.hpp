#ifndef SJTU_LRU_HPP
#define SJTU_LRU_HPP

#include "utility.hpp"
#include "exceptions.hpp"
#include "class-integer.hpp"
#include "class-matrix.hpp"
class Hash {
public:
	unsigned int operator () (Integer lhs) const {
		int val = lhs.val;
		return std::hash<int>()(val);
	}
};
class Equal {
public:
	bool operator () (const Integer &lhs, const Integer &rhs) const {
		return lhs.val == rhs.val;
	}
};

namespace sjtu {
template<class T> class double_list{
public:
	/**
	 * elements
	 * add whatever you want
	*/
	struct node {
		T* val;
		node *pre, *nxt;
		node() : pre(nullptr), nxt(nullptr), val(nullptr) {}
		node(const T& v, node *prev = nullptr, node *next = nullptr)
			 : val(new T(v)), pre(prev), nxt(next) {}
		~node() {
			delete val;
		}
	};
	node *head, *tail;

// --------------------------
	/**
	 * the follows are constructors and destructors
	 * you can also add some if needed.
	*/
	double_list(){
		head = new node();
		tail = new node();
		head->nxt = tail;
		tail->pre = head;
	}
	double_list(const double_list<T> &other){
		head = new node();
		tail = new node();
		head->nxt = tail;
		tail->pre = head;

		node *cur = other.head->nxt;
		while(cur != other.tail) {
			this->insert_tail(cur->val);
			cur = cur.nxt;
		}
	}
	~double_list(){
		node* cur = head;
		while(cur != nullptr) {
			node* tmp = cur->nxt;
			delete cur;
			cur = tmp;
		}
	}

	class iterator{
	public:
    	/**
		 * elements
		 * add whatever you want
		*/
		node *p;
	    // --------------------------
        /**
		 * the follows are constructors and destructors
		 * you can also add some if needed.
		*/
		iterator() : p(nullptr) {}
		iterator(node *t) : p(t) {}
		iterator(const iterator &t) : p(t.p) {}
		
		~iterator(){}
        /**
		 * iter++
		 */
		iterator operator++(int) {
			if(p->nxt == nullptr)
				throw "It's the end!"
			iterator tmp(*this);
			p = p->nxt;
			return tmp;
		}
        /**
		 * ++iter
		 */
		iterator &operator++() {
			if(p->nxt == nullptr)
				throw "It's the end!"
			p = p->nxt;
			return *this;
		}
        /**
		 * iter--
		 */
		iterator operator--(int) {
			if(p->pre->pre == nullptr)
				throw "It's the beginning!"
			iterator tmp(*this);
			p = p->pre;
			return tmp;
		}
        /**
		 * --iter
		 */
		iterator &operator--() {
			if(p->pre->pre == nullptr)
				throw "It's the beginning!"
			p = p->pre;
			return *this;
		}
		/**
		 * if the iter didn't point to a value
		 * throw " invalid"
		*/
		T &operator*() const {
			if(p == nullptr || p->pre == nullptr || p->nxt == nullptr)
				throw "invalid";
			return p->val;//注意返回值
		}
        /**
         * other operation
        */
		T *operator->() const noexcept {
			return &(p->val);
		}
		bool operator==(const iterator &rhs) const {
			return p == rhs.p;
    	}
		bool operator!=(const iterator &rhs) const {
			return p != rhs.p;
		}
	};

	class const_iterator{
	public:
		node* p;
		const_iterator() : p(nullptr) {}
		const_iterator(node* t) : p(t) {}
		const_iterator(const const_iterator &t) : p(t.p) {}
		const_iterator(const iterator &t) : p(t.p) {}
		~const_iterator(){}
		const_iterator operator++(int) {
			if(p->nxt == nullptr)
				throw "It's the end!";
			const_iterator tmp(*this);
			p =  p->nxt;
			return tmp;
		}
		const_iterator &operator++() {
			if(p->nxt == nullptr)
				throw "It's the end!";
			p = p->nxt;
			return *this;
		}
		const_iterator operator--(int) {
			if(p->pre->pre == nullptr)
				throw "It's the beginning!";
			const_iterator tmp(*this);
			p =  p->pre;
			return tmp;
		}
		const_iterator &operator--() {
			if(p->pre->pre == nullptr)
				throw "It's the beginning!";
			p = p->pre;
			return *this;
		}
		const T &operator*() const {
			if(p == nullptr || p->nxt == nullptr || p->pre == nullptr)
				throw "invalid";
			return p->val;
		}
		const T *operator->() const noexcept {
			return &(p->val);
		}
		bool operator==(const const_iterator &rhs) const {
			return p == rhs.p;
		}
		bool operator!=(const const_iterator &rhs) const {
			return p != rhs.p;
		}
	};
	/**
	 * return an iterator to the beginning
	 */
	iterator begin(){
		return iterator(head->nxt);
	}

	const_iterator cbegin() const {
		return const_iterator(head->nxt);
	}
	/**
	 * return an iterator to the ending
	 * in fact, it returns the iterator point to nothing,
	 * just after the last element.
	 */
	iterator end(){
		return iterator(tail);
	}

	const_iterator cend() const {
		return const_iterator(tail);
	}
	/**
	 * if the iter didn't point to anything, do nothing,
	 * otherwise, delete the element pointed by the iter
	 * and return the iterator point at the same "index"
	 * e.g.
	 * 	if the origin iterator point at the 2nd element
	 * 	the returned iterator also point at the
	 *  2nd element of the list after the operation
	 *  or nothing if the list after the operation
	 *  don't contain 2nd elememt.
	*/
	iterator erase(iterator pos){
		if(pos.p == nullptr || pos.p == head || pos.p == tail)
			return pos;
		
		node *to_delete = pos.p;
		node *next_node = to_delete->nxt;
		node *prev_node = to_delete->pre;
		
		prev_node->nxt = next_node;
		next_node->pre = prev_node;
		
		delete to_delete; 
		return iterator(next_node);
	}

	/**
	 * the following are operations of double list
	*/
	void insert_head(const T &val){
		node* tmp = new node(val, head, head->nxt);
		head->nxt->pre = tmp;
		head->nxt = tmp;
	}
	void insert_tail(const T &val){
		node *tmp = new node(val, tail->pre, tail);
		tail->pre->nxt = tmp;
		tail->pre = tmp;
	}
	void delete_head(){
		node* tmp = head->nxt;
		head->nxt = head->nxt->nxt;
		head->nxt->pre = head;
		delete tmp;
	}
	void delete_tail(){
		node* tmp = tail->pre;
		tail->pre = tail->pre->pre;
		tail->pre->nxt = tail;
		delete tmp;
	}
	/**
	 * if didn't contain anything, return true, 
	 * otherwise false.
	 */
	bool empty(){
		return head->nxt == tail;
	}
};

template<
	class Key,
	class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key>
> class hashmap{
public:
	using value_type = pair<const Key, T>;
	/**
	 * elements
	 * add whatever you want
	*/
	double_list<value_type> *buckets;
	size_t capacity;
	size_t size;
	double load_factor = 0.75;
	Hash hash_func;
	Equal equal_func;

// --------------------------

	/**
	 * the follows are constructors and destructors
	 * you can also add some if needed.
	*/
	hashmap() : capacity(16), size(0) {
		buckets = new double_list<value_type>[capacity];
	}
	hashmap(const hashmap &other) : capacity(other.capacity), size(0) {
		buckets = new double_list<value_type>[capacity];
		for(size_t i = 0; i < capacity; i++) {
			for(auto it = other.buckets[i].begin(); it != other.buckets[i].end(); it++) {
				this->insert(*it);
			}
		}
	}
	~hashmap(){
		delete[] buckets;
	}
	hashmap & operator=(const hashmap &other){
		if(this == &other) {
			return *this;
		}
		hashmap tmp(other);
		std::swap(capacity, tmp.capacity);
		std::swap(size, tmp.size);
		std::swap(buckets, tmp.buckets);
		return *this;
	}

	class iterator{
	public:
    	/**
         * elements
         * add whatever you want
        */
	   size_t bucket_index;
	   typename double_list<value_type>::iterator list_iterator;
	   hashmap *map_ptr;
// --------------------------
        /**
         * the follows are constructors and destructors
         * you can also add some if needed.
        */
		iterator(size_t idx, typename double_list<value_type>::iterator it, hashmap* ptr)
			: bucket_index(idx), list_iterator(it), map_ptr(ptr) {}
		iterator(const iterator &t)
			: bucket_index(t.bucket_index), list_iterator(t.list_iterator), map_ptr(t.map_ptr) {}
		
		~iterator(){}

        /**
		 * if point to nothing
		 * throw 
		*/
		value_type &operator*() const {
			if(map_ptr == nullptr || bucket_index >= map_ptr->capacity) {
				throw "point to nothing";
			}
			return *list_iterator;
		}

        /**
		 * other operation
		*/
		value_type *operator->() const noexcept {
			return &(*list_iterator);
		}
		bool operator==(const iterator &rhs) const {
			return bucket_index == rhs.bucket_index
				&& list_iterator == rhs.list_iterator
				&& map_ptr == rhs.map_ptr;
    	}
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
	};

	void clear(){
	}
	/**
	 * you need to expand the hashmap dynamically
	*/
	void expand(){
		size_t old_capacity = capacity;
		capacity <<= 1;
		double_list<value_type>* old_buckets = buckets;
		buckets = new double_list<value_type>[capacity];
		for(size_t i = 0; i < old_capacity; i++) {
			for(auto it = old_buckets[i].begin(); it != old_buckets[i].end(); it++) {
				size_t idx = getIndex((*it).first);
				buckets[idx].insert_tail(*it);
			}
		}

		delete[] old_buckets;
	}

    /**
     * the iterator point at nothing
    */
	iterator end() const{
		return iterator(capacity, buckets[capacity-1].end(), const_cast<hashmap*>(this));
	}
	/**
	 * find, return a pointer point to the value
	 * not find, return the end (point to nothing)
	*/
	iterator find(const Key &key)const{
		if(capacity == 0)return end();
		size_t idx = getIndex(key);
		for(auto it = buckets[idx].begin(); it != buckets[idx].end(); it++) {
			if(equal_func((*it).first,key)) {
				return iterator(idx, it, const_cast<hashmap*>(this));
			}
		}
		return end();
	}
	/**
	 * already have a value_pair with the same key
	 * -> just update the value, return false
	 * not find a value_pair with the same key
	 * -> insert the value_pair, return true
	*/
	sjtu::pair<iterator,bool> insert(const value_type &value_pair){
		iterator it = find(value_pair.first);
		if(it != end()) {
			it.iter->second = value_pair.second;
			return sjtu::pair<iterator,bool>(it, false);
		}

		size_t idx = getIndex(value_pair.first);
		buckets[idx].insert_tail(value_pair);
		size++;

		if((float)size / capacity > load_factor) {
			expand();
			return sjtu::pair<iterator,bool>(find(value_pair.first), true);
		}
		typename double_list<value_type>::iterator list_it = buckets[idx].end();
		list_it--; 
		iterator new_it(idx, list_it, this);
		return sjtu::pair<iterator, bool>(new_it, true);
	}
	/**
	 * the value_pair exists, remove and return true
	 * otherwise, return false
	*/
	bool remove(const Key &key){
		size_t idx = getIndex(key);
		for(auto it = buckets[idx].begin(); it != buckets[idx].end(); it++) {
			if(equal_func((*it).first, key)) {
				buckets[idx].erase(it);
				size--;
				return true;
			}
		}
		return false;
	}
private:
	size_t getIndex(const Key &key) const {
		size_t idx = hash_func(key) % capacity;
		return idx;
	}
};

template<
	class Key,
	class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key>
> class linked_hashmap :public hashmap<Key,T,Hash,Equal>{
private:
	using value_type = pair<const Key, T>;
    using list_iterator = typename double_list<value_type>::iterator;
    using base_hashmap = hashmap<Key, list_iterator, Hash, Equal>;
	double_list<value_type> global_list;
public:
	typedef pair<const Key, T> value_type;
	/**
	 * elements
	 * add whatever you want
	*/
// --------------------------
	class const_iterator;
	class iterator{
	public:
    	/**
         * elements
         * add whatever you want
        */
	   list_iterator lit;
	   linked_hashmap* map_ptr;
    // --------------------------
		iterator(list_iterator it = list_iterator(), linked_hashmap* ptr = nullptr)
		 : lit(it), map_ptr(ptr) {}
		iterator(const iterator &other)
		 : lit(other.lit), map_ptr(other.map_ptr) {}
		~iterator(){}

		/**
		 * iter++
		 */
		iterator operator++(int) {
            iterator tmp = *this; 
			++(*this); 
			return tmp;
        }
		/**
		 * ++iter
		 */
		iterator &operator++() {
            if(map_ptr == nullptr || lit == map_ptr->global_list.end()) 
                throw "invalid";
            ++lit; 
			return *this;
        }
		/**
		 * iter--
		 */
		iterator operator--(int) {
            iterator tmp = *this; 
			--(*this); 
			return tmp;
        }
		/**
		 * --iter
		 */
		iterator &operator--() {
            if(map_ptr == nullptr || lit == map_ptr->global_list.begin()) 
                throw "invalid";
            --lit; 
			return *this;
        }

		/**
		 * if the iter didn't point to a value
		 * throw "star invalid"
		*/
		value_type &operator*() const {
            if(map_ptr == nullptr || lit == map_ptr->global_list.end()) 
                throw "star invalid";
            return *lit;
		}
		value_type *operator->() const noexcept {
            return &(*lit);
		}

		/**
		 * operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
            return lit == rhs.lit;
        }
		bool operator!=(const iterator &rhs) const {
            return lit != rhs.lit;
        }
		bool operator==(const const_iterator &rhs) const {
            return lit == rhs.lit;
        }
		bool operator!=(const const_iterator &rhs) const {
            return lit != rhs.lit;
        }
	};
 
	class const_iterator {
	public:
        /**
         * elements
         * add whatever you want
        */
        list_iterator lit;
		const linked_hashmap* map_ptr;
    // --------------------------   
		const_iterator(list_iterator it = list_iterator(), const linked_hashmap* ptr = nullptr)
         : lit(it), map_ptr(ptr) {}
		const_iterator(const iterator &other)
		 : lit(other.lit), map_ptr(other.map_ptr) {}

		/**
		 * iter++
		 */
		const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
		/**
		 * ++iter
		 */
		const_iterator &operator++() {
            if(map_ptr == nullptr || lit == map_ptr->global_list.end()) throw "invalid";
            ++lit;
            return *this;
        }
		/**
		 * iter--
		 */
		const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }
		/**
		 * --iter
		 */
		const_iterator &operator--() {
            if(map_ptr == nullptr || lit == map_ptr->global_list.begin()) 
                throw "invalid";
            --lit;
            return *this;
        }

		/**
		 * if the iter didn't point to a value
		 * throw 
		*/
		const value_type &operator*() const {
            if(map_ptr == nullptr || lit == map_ptr->global_list.end()) 
                throw "star invalid";
            return *lit;
		}
		const value_type *operator->() const noexcept {
            return &(*lit);
		}

		/**
		 * operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
            return lit == rhs.lit;
        }
		bool operator!=(const iterator &rhs) const {
            return lit != rhs.lit;
        }
		bool operator==(const const_iterator &rhs) const {
            return lit == rhs.lit;
        }
		bool operator!=(const const_iterator &rhs) const {
            return lit != rhs.lit;
        }
	};
 
	linked_hashmap() {}
	linked_hashmap(const linked_hashmap &other) {
        for(auto it = other.global_list.begin(); it != other.global_list.end(); it++) {
            this->insert(*it);
        }
	}
	~linked_hashmap() {
        clear();
	}
	linked_hashmap & operator=(const linked_hashmap &other) {
        if(this == &other) return *this;
        clear();
        for(auto it = other.global_list.begin(); it != other.global_list.end(); it++) {
            this->insert(*it);
        }
        return *this;
	}

 	/**
	 * return the value connected with the Key(O(1))
	 * if the key not found, throw 
	*/
	T & at(const Key &key) {
        auto hash_it = base_hashmap::find(key);
        if(hash_it == base_hashmap::end()) 
            throw "out of bound";

        list_iterator lit = (*hash_it).second;
        value_type val = *lit;

        //保证链表头是刚用过的
        global_list.erase(lit);
        global_list.insert_head(val);

        (*hash_it).second = global_list.begin();

        return global_list.begin()->second;
	}
	const T & at(const Key &key) const {
        auto hash_it = base_hashmap::find(key);
        if(hash_it == base_hashmap::end()) 
            throw "out of bound";
        return (*hash_it).second->second;
	}
	T & operator[](const Key &key) {
        auto hash_it = base_hashmap::find(key);
        if(hash_it != base_hashmap::end()) {
            return at(key); 
        }
        // if not found, throw exception
        throw "out of bound";
	}
	const T & operator[](const Key &key) const {
        return at(key);
	}

	/**
	 * return an iterator point to the first 
	 * inserted and existed element
	 */
	iterator begin() {
        return iterator(global_list.begin(), this);
	}
	const_iterator cbegin() const {
        return const_iterator(global_list.begin(), this);
	}
    /**
	 * return an iterator after the last inserted element
	 */
	iterator end() {
        return iterator(global_list.end(), this);
	}
	const_iterator cend() const {
        return const_iterator(global_list.end(), this);
	}
  	/**
	 * if didn't contain anything, return true, 
	 * otherwise false.
	 */
	bool empty() const {
        return global_list.empty();
	}

    void clear(){
        global_list.head->nxt = global_list.tail;
        global_list.tail->pre = global_list.head;
        base_hashmap::clear();
        while(!global_list.empty()) {
            global_list.delete_head();
        }
	}

	size_t size() const {
        return base_hashmap::size;
	}
 	/**
	 * insert the value_piar
	 * if the key of the value_pair exists in the map
	 * update the value instead of adding a new element，
     * then the order of the element moved from inner of the 
     * list to the head of the list
	 * and return false
	 * if the key of the value_pair doesn't exist in the map
	 * add a new element and return true
	*/
	pair<iterator, bool> insert(const value_type &value) {
        auto hash_it = base_hashmap::find(value.first);
        //查到了就更新
		if(hash_it != base_hashmap::end()) {
			list_iterator target_list_it = (*hash_it).second;

			target_list_it->second = value.second;
			value_type new_val = *target_list_it;
            //依旧保证链表头是最近使用过的
			global_list.erase(target_list_it);
			global_list.insert_head(new_val);

			(*hash_it).second = global_list.begin();

			return pair<iterator, bool>(iterator(global_list.begin(), this), false);
		}
        //没查到就插入
		else {
            //依旧保证链表头是最近使用过的
			global_list.insert_head(value);
            list_iterator new_lit = global_list.begin();

            base_hashmap::insert(sjtu::pair<const Key, list_iterator>(value.first, new_lit));

            return pair<iterator, bool>(iterator(new_lit, this), true);
		}
	}
 	/**
	 * erase the value_pair pointed by the iterator
	 * if the iterator points to nothing
	 * throw 
	*/
	void remove(iterator pos) {
        if(pos == end() || pos.map_ptr != this) 
            throw "invalid remove";
        base_hashmap::remove(pos->first);
        global_list.erase(pos.lit);
	}
	/**
	 * return how many value_pairs consist of key
	 * this should only return 0 or 1
	*/
	size_t count(const Key &key) const {
        auto it = base_hashmap::find(key);
        return (it == base_hashmap::end()) ? 0 : 1;
	}
	/**
	 * find the iterator points at the value_pair
	 * which consist of key
	 * if not find, return the iterator 
	 * point at nothing
	*/
	iterator find(const Key &key) {
        auto hash_it = base_hashmap::find(key);
        if(hash_it == base_hashmap::end()) return end();
        return iterator((*hash_it).second, this);
	}

};

class lru{
    using lmap = sjtu::linked_hashmap<Integer,Matrix<int>,Hash,Equal>;
    using value_type = sjtu::pair<const Integer, Matrix<int> >;
private:
    lmap data;
    size_t max_size;
public:
    lru(int size) : max_size(size) {}
    ~lru() {}
    /**
     * save the value_pair in the memory
     * delete something in the memory if necessary
    */
    void save(const value_type &v) {
        data.insert(v);

		if(data.size() > (size_t)max_size) {
			auto it = data.end();
			--it;
			data.remove(it);
		}

    }
    /**
     * return a pointer contain the value
    */
    Matrix<int>* get(const Integer &v) {
        auto it = data.find(v);
		if(it == data.end()){
			return nullptr;
		}
		return &(data.at(v));
    }
    /**
     * just print everything in the memory
     * to debug or test.
     * this operation follows the order, but don't
     * change the order.
    */
    void print() {
        for(auto it = data.begin(); it != data.end(); it++) {
			std::cout << it->first.val << " " << it->second << std::endl;
		}
    }
};
}

#endif