#include "ra/avl_tree.hpp"
#include <limits>

//map class implemented using avl-tree
//key values in tree are std::pairs
template <class Key, class T, class Compare = std::less<Key>>
class map{

    private:
        avl_tree<std::pair<Key, T>, Compare> tree_;
        std::size_t size_;

    public:
        using tree_type = avl_tree<std::pair<Key, T>, Compare>;
        using value_type = std::pair<Key, T>;
        using iterator = tree_type::avl_iterator;
        using const_iterator = const tree_type::avl_iterator;
        using node_type = node<std::pair<Key, T>>;
        using size_type = std::size_t;

        //map constructor
        map(): size_(std::size_t(0)){}

        map(map& other){
            if(this != &other){
                tree_ = avl_tree(other.tree_);
                size_ = other.size_;
            }
        }

        map(map&& other){
            if(this != &other){
                tree_ = std::move(other.tree_);
                size_ = other.size_;
                other.size_ = 0;
            }
        }

        //map destructor
        ~map(){}

        //map copy assignment
        map& operator=( const map& other ){
            if(this != &other){
                tree_ = avl_tree(other.tree_);
                size_ = other.size_;
            }
            return *this;
        }

        //map move assignment
        map& operator=( map&& other ){
            if(this != &other){
                size_ = other.size_;
                tree_ = std::move(other.tree_);
                other.size_ = 0;
            }
            return *this;
        }
        
        //index map for key,
        //if found return value
        //else insert key with a default value of 0
        T& operator[]( const Key& key ){
            node_type* t = tree_.search(std::pair<Key, T>(key, T()));

            if(t == nullptr){
                size_++;
                return insert(std::pair<Key, T>(key, T())).first->data_.second;
            }
            return t->data_.second;
        }

        //check map for key,
        //if found return value
        //else throw exception std::out_of_range
        T& at( const Key& key ){
            node_type* t = tree_.search(std::pair<Key, T>(key, T()));

            if(t == nullptr){
                throw std::out_of_range("key is not in map");
            }
            return t->data_.second;
        }

        //const
        //check map for key,
        //if found return value
        //else throw exception std::out_of_range
        const T& at( const Key& key ) const{
            node_type* t = tree_.search(std::pair<Key, T>(key, T()));

            if(t == nullptr){
                throw std::out_of_range("key is not in map");
            }
            return t->data_.second;
        }

        //insert key value pair
        //return pair that has the iterator pointing to the new value and true
        //or iterator pointing to nullptr and false
        std::pair<iterator, bool> insert( const value_type& value ){
            node_type* t = tree_.search(value);

            if(t == nullptr){
                tree_.insert(value);
                size_++;
                return std::pair<iterator, bool>(iterator(tree_.search(value), tree_), true);
            }
            return std::pair<iterator, bool>(iterator(tree_.search(value), tree_), false);
        }

        //return iterator to start of map
        iterator begin(){
            return iterator(tree_.start(), tree_);
        }

        //const
        //return const iterator to start of map
        const_iterator begin() const{
            return const_iterator(tree_.start(), tree_);
        }

        //return iterator to end of map
        iterator end(){
            return iterator(tree_.end(), tree_);
        }

        //const
        //return const iterator to end of map
        const_iterator end() const{
            return const_iterator(tree_.end(), tree_);
        }

        //check if map is empty
        bool empty() const{
            return size_ == size_type(0);
        }

        //return size
        size_type size() const{
            return size_;
        }

        //maximum map size
        size_type max_size() const{
            return std::numeric_limits<int>::max();
        }

        //erase map element at pos and return iterator pointing to next element
        iterator erase( const_iterator pos ){
            size_--;
            iterator del = pos;
            iterator i = pos;
            i++;
            tree_.delete_node(del->data_);
            return i;
        }

        //swap map data
        void swap( map& other ){
            std::swap(size_, other.size_);
            tree_.swap(other.tree_);
        }

        //clear map
        void clear(){
            tree_.clear();
            size_ = 0;
        }
};