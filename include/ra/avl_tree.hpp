#include <iostream>
#include <algorithm>


//node class for avl-tree
template <class T>
class node{
    public:

        //value type of node data
        using value_type = T;

        //members of node
        value_type data_;
        node* parent_;      //parent node used for backtracking
        node* left_;
        node* right_;
        int height_;

        node(value_type key, node* parent): data_(key), parent_(parent), left_(nullptr), right_(nullptr), height_(1){}
};

//avl-tree class
//T is node data type and Compare defines ordering
template <class T, class Compare>
class avl_tree{

    private:
        node<T>* root_;
        Compare comp_;  //comparison object

    public:

        //iterator class for moving through our avl-tree
        class avl_iterator{
            public:
                using reference = node<T>&;
                using pointer = node<T>*;

                avl_iterator(): iter_(), t_(){};

                avl_iterator(node<T>* node, avl_tree tree): iter_(node), t_(tree){}

                //copy constructor
                avl_iterator(const avl_iterator& other): iter_(other.iter_), t_(other.t_){}

                //access node data that our iterator is pointing to
                auto& operator*(){
                    return iter_->data_;
                }

                //access iter pointer
                pointer operator->(){
                    return iter_;
                }

                //copy assign iter pointer
                avl_iterator& operator=(const avl_iterator& other){
                    iter_ = other.get_iter();
                    t_ = other.get_tree();
                    return (*this);
                }

                //returns iterator pointing to next node in tree after iter node
                avl_iterator operator++(int){
                    avl_iterator new_iter(*this);
                    iter_ = t_.successor(iter_);
                    return new_iter;
                }

                //returns iterator pointing to previous node in tree before iter node
                avl_iterator operator--(int){
                    avl_iterator new_iter(*this);
                    iter_ = t_.predecessor(iter_);
                    return new_iter;
                }

                node<T>* get_iter() const{
                    return iter_;
                }

                avl_tree get_tree() const{
                    return t_;
                }

                //check if we are at the end of the tree
                //i.e. at a nullptr
                bool equals_end() const{
                    return iter_ == nullptr;
                }

                //check if we are at the start of the tree
                //two cases: tree is empty, then yes
                //or iter is pointing to smallest node in tree
                bool equals_start(){
                    if(iter_ == nullptr){
                        if(t_.empty()){
                            return true;
                        }else{
                            return false;
                        }
                    }

                    if(iter_->data_ == t_.min_value()->data_){
                        return true;
                    }
                    return false;
                }

            private:
                node<T>* iter_; //points to the node in our tree for iterating through
                avl_tree t_;    //reference for calling avl-tree functions
        };

        using value_type = T;

        avl_tree(): root_(nullptr), comp_(Compare{}){}

        //construct avl-tree with specific comparator
        explicit avl_tree(const Compare& comp): root_(nullptr), comp_(comp){}

        //copy construct avl-tree
        avl_tree(const avl_tree& other): root_(nullptr), comp_(other.comp_){
            root_ = clone_tree(other.root_);
        }

        //destructor
        //calls clear to delete nodes
        ~avl_tree(){
            clear();
        }

        //deletes nodes
        void clear(){
            clear(root_);
            root_ = nullptr;
        }

        // avl_tree& operator=(const avl_tree& other){
        //     comp_ = other.comp_;
        //     root_ = clone_tree(other.root_);  
        //     return *this;
        // }

        //move assignment
        avl_tree& operator=(avl_tree&& other){
            clear(root_);
            root_ = other.root_;
            comp_ = other.comp_;

            other.root_ = nullptr;

            return *this;
        }

        //carries out the node deletion
        void clear(node<T>* root){
            if(root == nullptr){
                return;
            }

            clear(root->left_);
            clear(root->right_);

            delete(root);
            root = nullptr;
        }

        //clones nodes from another tree using it's root
        node<T>* clone_tree(node<T>* root){
            if(root == nullptr){
                return nullptr;

            }else{
                node<T>* new_node = new node<T>(root->data_, root->parent_);
                new_node->left_ = clone_tree(root->left_);
                new_node->right_ = clone_tree(root->right_);

                return new_node;
            }
        }

        //swap avl-tree data
        void swap(avl_tree& other){
            std::swap(root_, other.root_);
            std::swap(comp_, other.comp_);
        }

        //return height of node
        int height(node<T>* root){
            if(root == nullptr){
                return 0;
            }else{
                return root->height_;
            }
        }

        //height is the maximum of the left and right subtrees + 1 since we just added a node
        void update_height(node<T>* root){
            if(root != nullptr){
                int ht = 1;

                if(root->left_ != nullptr){
                    ht = root->left_->height_ + 1;
                }

                if(root->right_ != nullptr){
                    ht = std::max(ht, root->right_->height_ + 1);
                }
                root->height_ = ht;
            }
        }

        //balance of node is the subtraction of the left subtree height from the right subtree height
        int balance_factor(node<T>* root){
            return height(root->left_) - height(root->right_);
        }

        //carries out a right rotation for a tree like below given that the left and right subtrees have a balance factor > |1|
        //
        //      y
        //     / \
        //    x   T3
        //   / \
        //  T1 T2
        //
        //gives:
        //
        //      x
        //     / \
        //    T1  y
        //       / \
        //      T2  T3
        node<T>* right_rotate(node<T>* y){
            node<T>* x = y->left_;
            node<T>* T2 = x->right_;

            //set T2's new parent to y
            if(T2 != nullptr){
                T2->parent_ = y;
            }

            //perform rotation
            x->right_ = y;
            y->left_ = T2;

            //update parents
            x->parent_ = y->parent_;
            y->parent_ = x;

            //set parent of x to have x either on left or right based on if x is less or greater than x's parent
            if(x->parent_!= nullptr){
                if(comp_(y->data_.first, x->parent_->data_.first)){
                    x->parent_->left_ = x;
                }else if(x->parent_ != nullptr){
                    x->parent_->right_ = x;
                }
            }

            //set y to x since they have now switched and we need to update that particular node in insertion/deletion
            y = x;

            //update the height of all the things associated with y
            update_height(y->left_);
            update_height(y->right_);
            update_height(y);
            update_height(y->parent_);

            return y;
        }

        //carries out a left rotation for a tree like below given that the left and right subtrees have a balance factor > |1|
        //      x
        //     /  \
        //    T1   y 
        //        / \
        //       T2  T3
        //
        //gives:
        //
        //      y
        //     / \
        //    x   T3
        //   / \
        //  T1 T2
        node<T>* left_rotate(node<T>* x){
            node<T>* y = x->right_;
            node<T>* T2 = y->left_;

            x->right_ = T2;

            //set y's left node parent to x
            if(y->left_ != nullptr){
                y->left_->parent_ = x;
            }

            //perform rotation
            y->left_ = x;

            //update parents
            y->parent_ = x->parent_;
            x->parent_ = y;

            //set parent of y to have y either on left or right based on if y is less or greater than y's parent
            if(y->parent_ != nullptr){
                if(comp_(x->data_.first, y->parent_->data_.first)){
                    y->parent_->left_ = y;
                }else{
                    y->parent_->right_ = y;
                }
            }

            //update heights
            update_height(x->left_);
            update_height(x->right_);
            update_height(x);
            update_height(x->parent_);

            return y;
        }

        //return minimum
        node<T>* min_value(){
            return min_value(root_);
        }

        //return minimum
        node<T>* min_value(node<T>* root){
            node<T>* cur = root;

            if(cur){
                while(cur->left_ != nullptr){
                    cur = cur->left_;
                }
            }
            return cur;
        }

        //return maximum
        node<T>* max_value(node<T>* root){
            node<T>* cur = root;

            if(cur){
                while(cur->right_ != nullptr){
                    cur = cur->right_;
                }
            }
            return cur;
        }

        //returns the previous node from our current node
        node<T>* predecessor(node<T>* root){
            if(root == nullptr){
                return root;
            }

            //we are the start so there is no previous node
            //return the our current node
            if(root->data_ == min_value(root_)->data_){
                return root;
            }

            //return the largest value in the left tree as our previous node
            if(root->left_ != nullptr){
                return max_value(root->left_);
            }

            //no left subtree, return the first node that is less than our current node
            node<T>* cur = root->parent_;

            while(comp_(root->data_.first, cur->data_.first)){
                cur = cur->parent_;
            }
            return cur;
        }

        //return the next largest node from our current node
        node<T>* successor(node<T>* root){
            if(root == nullptr){
                return root;
            }
            
            //if we are at the largest node then return the end (nullptr) since we are now at the place one past the last element
            if(root->data_ == max_value(root_)->data_){
                return end();
            }
            
            //return minimum node in right tree
            if(root->right_ != nullptr){
                return min_value(root->right_);
            }

            //no left subtree, return the first node that is greater than our current node
            node<T>* cur = root->parent_;

            while(comp_(cur->data_.first, root->data_.first)){
                cur = cur->parent_;
            }
            return cur;
        }

        //insert key value into tree
        void insert(value_type key){
            root_ = insert(root_, nullptr, key);
        }

        //insert key value into tree
        node<T>* insert(node<T>* root, node<T>* parent, value_type key){
            int bf;

            //perform normal binary search tree insertion
            if(root == nullptr){
                node<T>* new_node = new node<T>(key, parent);
                return new_node;

            }else if(comp_(key.first, root->data_.first)){
                root->left_ = insert(root->left_, root, key);

            }else if(comp_(root->data_.first, key.first)){
                root->right_ = insert(root->right_, root, key);

            }else{
                return root;
            }
    
            //after we insert the new node we will need to update the height of all ancestors
            update_height(root);

            //get balance factor to check if the height between subtrees of root are greater than |1|
            bf = balance_factor(root);

            if(bf > 1){
                //left left: the longer subtree is on the root's left left node
                //rotate right root
                if(comp_(key.first, root->left_->data_.first)){
                    return right_rotate(root);

                //left right: the longer subtree is on the root's left right node
                //rotate left root left then rotate right root
                }else{
                    root->left_ = left_rotate(root->left_);
                    return right_rotate(root);
                }
            }else if(bf < -1){

                //right right: the longer subtree is on the root's right right node
                //rotate left root
                if(comp_(root->right_->data_.first, key.first)){
                    return left_rotate(root);

                //right left: the longer subtree is on the root's right left node
                //rotate right root right then rotate left root
                }else{
                    root->right_ = right_rotate(root->right_);
                    return left_rotate(root);
                }
            }

            return root;
        }

        //delete node from tree
        void delete_node(value_type key){
            root_ = delete_node(root_, key);
        }

        //delete node from tree
        node<T>* delete_node(node<T>* root, value_type key){
            int bf;

            //perform normal binary search tree delete
            if(root == nullptr){
                return root;

            }else if(comp_(key.first, root->data_.first)){
                root->left_ = delete_node(root->left_, key);

            }else if(comp_(root->data_.first, key.first)){
                root->right_ = delete_node(root->right_, key);

            //avl-tree delete part
            }else{
                node<T>* remove;    //used to point at node for deletion

                //case where both left and right nodes are null
                if(root->left_ == nullptr && root->right_ == nullptr){
                    remove = root;

                    //set new child for root's parent if there is a parent
                    if(root->parent_ != nullptr){
                        if(comp_(root->parent_->data_.first, root->data_.first)){
                            root->parent_->right_ = nullptr;
                        }else{
                            root->parent_->left_ = nullptr;
                        }
                        //update the height
                        update_height(root->parent_);
                    }

                    root = nullptr;
                    delete remove;

                //case where only root's right child node is not null
                }else if(root->left_ == nullptr){
                    remove = root->right_;  //we will overwrite the root and delete the right child below

                    //set new child for root's parent if there is a parent
                    if(root->parent_ != nullptr){
                        if(comp_(root->parent_->data_.first, root->data_.first)){
                            root->parent_->right_ = root->right_;
                        }else{
                            root->parent_->left_ = root->right_;
                        }
                        //update the height
                        update_height(root->parent_);
                    }
                    //set right child's parent to root's parent
                    root->right_->parent_ = root->parent_;
                    
                    *root = *root->right_;  //copy contents of right child to root
                    delete remove;

                //case where root's left child node is not null
                }else if(root->right_ == nullptr){
                    remove = root->left_;   //we will overwrite the root and delete the left child below

                    //set new child for root's parent if there is a parent
                    if(root->parent_ != nullptr){
                        if(comp_(root->parent_->data_.first, root->data_.first)){
                            root->parent_->right_ = root->left_;
                        }else{
                            root->parent_->left_ = root->left_;
                        }
                        //update the height
                        update_height(root->parent_);
                    }
                    //set left child's parent to root's parent
                    root->left_->parent_ = root->parent_;

                    *root = *root->left_;   //copy contents of left child to root
                    delete remove;
                    
                //case where both left and right child's are not null
                }else{
                    //find the smallest valued node of the right subtree for replacing the root
                    remove = min_value(root->right_);

                    //copy data to root
                    root->data_ = remove->data_;    

                    //delete the smallest node we have copied the data from
                    root->right_ = delete_node(root->right_, remove->data_);    
                }
            }

            //if the root is now null there is no need to balance things
            if(root == nullptr){
                return root;
            }

            //update height
            update_height(root);

            //get balance factor
            bf = balance_factor(root);

            //rotation
            if(bf > 1){
                int bf_left_subtree = balance_factor(root->left_);

                //left left: the longer subtree is on the root's left left node
                //rotate right root
                if(bf_left_subtree >= 0){
                    return right_rotate(root);

                //left right: the longer subtree is on the root's left right node
                //rotate left root left then rotate right root
                }else{
                    root->left_ = left_rotate(root->left_);
                    return right_rotate(root);
                }
            }else if(bf < -1){
                int bf_right_subtree = balance_factor(root->right_);

                //right right: the longer subtree is on the root's right right node
                //rotate left root
                if(bf_right_subtree <= 0){
                    return left_rotate(root);

                //right left: the longer subtree is on the root's right left node
                //rotate right root right then rotate left root
                }else{
                    root->right_ = right_rotate(root->right_);
                    return left_rotate(root);
                }
            }
            return root;
        }

        //search for key value
        node<T>* search(value_type key){
            return search(root_, key);
        }

        //search for key value
        node<T>* search(node<T>* root, value_type key){

            //standard bst search
            if(root != nullptr){
                if(root->data_.first == key.first){
                    return root;

                }else if(comp_(root->data_.first, key.first)){
                    return search(root->right_, key);

                }else{
                    return search(root->left_, key);
                }
            }
            return nullptr;
        }

        //find minimim
        node<T>* start(){
            return min_value(root_);
        }

        //end (nullptr)
        node<T>* end(){
            return nullptr;
        }

        //check if tree is empty
        bool empty(){
            return root_ == nullptr;
        }

        //print pre-order ordering of nodes
        void pre_order(){
            pre_order(root_);
        }

        //print pre-order ordering of nodes
        void pre_order(node<T>* root){
            if(root != nullptr){
                std::cout << root->data_.first << " ";// << root->data_.second << " ";
                pre_order(root->left_);
                pre_order(root->right_);
            }
        }

        //print pre-order ordering of nodes with parent node also
        void pre_order_with_parent(){
            pre_order_with_parent(root_);
        }

        //print pre-order ordering of nodes with parent node also
        void pre_order_with_parent(node<T>* root){
            if(root != nullptr){
                std::cout << "root: " << root->data_.first << " "; //<< root->data_.second << " ";

                if(root->parent_ == nullptr){
                    std::cout << "parent: nullptr ";
                }else{
                    std::cout << "parent: " << root->parent_->data_.first << " ";// << root->data_.second << " ";
                }
                pre_order_with_parent(root->left_);
                pre_order_with_parent(root->right_);
            }
        }
};