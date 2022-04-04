
#include <assert.h>
#include <iostream>
#include <string>

//
template <typename T>
class MyVector
{
	public:

		/*******************
		 * Static constants
		 ******************/

		/// Default capacity
		static constexpr size_t DEFAULT_CAPACITY = 64;

		/// Minimum capacity
		static constexpr size_t MINIMUM_CAPACITY = 8;

		/*****************************
		 * Constructors / Destructors
		 ****************************/

		/// Normal constructor
		MyVector(size_t capacity = MyVector::DEFAULT_CAPACITY) {

		/*
			size_ = 0;
			capacity_ = capacity;
			elements_ = new T[capacity];
			*/
			if (capacity < MINIMUM_CAPACITY){
				capacity = MINIMUM_CAPACITY;
			} 
				capacity_ = capacity ;
				elements_ = new T[capacity];
		}

		/// Copy constructor
		MyVector(const MyVector& other) {
		
			size_ = other.size_;
			capacity_ = other.capacity_;
			elements_ = new T [other.capacity_];
			// copy all elements
			for ( size_t i = 0; i < other.size_; i++) {
				elements_[i] = other[i];
			} 
		}

		/**
		 * Destructor
		 * Should call clear() so each element gets its destructor called.
		 * Then, deallocate the internal array and make it a nullptr, if its not already a nullptr.
		 */
		~MyVector() {
		
			clear();
			delete [] elements_ ;
			elements_ = nullptr;
		}

		/************
		 * Operators
		 ************/

		///	Assignment operator
		MyVector& operator=(const MyVector& rhs) {
		
			if ( capacity_ < rhs.size_ ) {
				size_ = rhs.size_; // set the size
				capacity_ = rhs.capacity_; // set the capacity
				delete[] elements_; // delete old array
				elements_ = new T[capacity_]; // create new array
			}
			// copy element
			for ( size_t i = 0; i < size_ ; i++){
				elements_[i] = rhs.elements_[i];
			}
			// return this object ?
			return *this;
		}


		/// Operator overload to at()
		T& operator[](size_t index) const {

			// element at index
			return elements_[index];
		}

		/************
		 * Accessors
		 ************/

		/// Return the number of valid elements in our data
		size_t size() const {

			return size_;
		}

		/// Return the capacity of our internal array
		size_t capacity() const {

			return capacity_;
		}

		/**
		 * Check whether our vector is empty
		 * Return true if we have zero elements in our array (regardless of capacity)
		 * Otherwise, return false
		 */
		bool empty() const {

			if (size_ == 0 ) {
			return true;
			} else {
			return false;
			}
		}

		/// Return a reference to the element at an index
		T& at(size_t index) const {

			if ( index < 0 || index >= size_) {
				throw std::range_error("the index is out of bounds");
			}
			return elements_[index];
		}

		/***********
		 * Mutators
		 ***********/

		/**
		 * Reserve capacity in advance, if our capacity isn't currently large enough.
		 * Useful if we know we're about to add a large number of elements, and we'd like to avoid the overhead of many internal changes to capacity.
		 */
		void reserve(size_t capacity) {

			// capacity is full no need to retur anything
			if ( capacity <= capacity_ ) {
				return;
			}
			// reserve the capacity
			T* copy = new T[capacity];
				for (size_t i = 0; i < size_; i++) {
				copy[i] = elements_[i];
			}
			// delete old array (destructor)
			if (elements_ != nullptr) {
				delete [] elements_;
			}
			// set new array of element and capacity
			elements_ = copy;
			capacity_ = capacity;
			
		}

		/**
		 * Set an element at an index.
		 * Throws range error if outside the size boundary.
		 * Returns a reference to the newly set element (not the original)
		 */
		T& set(size_t index, const T& element) {

			if (index > size_ ) {
				throw std::range_error("index out of bound" );
			}
			else{
				at(index)=element; // set element at an index
			}
			return elements_[index];

		}

		/**
		 * Add an element onto the end of our vector, increasing the size by 1
		 * Should rely on the insert() function to avoid repeating code.
		 * Returns a reference to the newly inserted element
		 */
		T& push_back(const T& element) {

			/*if (size_ == capacity_){
				throw std::range_error("capacity is full");
			}
			*/
			// try to double capacity to have enough capacity?
			if ( size_ >= capacity_) {
				reserve(2*capacity_);
			}
			elements_[size_] = element;
			insert(size_,element);
			return elements_[size_];
		}

		/**
		 * Remove the last element in our vector, decreasing the size by 1
		 * Should rely on the erase() function to avoid repeating code.
		 * Returns the new size.
		 */
		size_t pop_back() {

			// TODO: Your code here
			if (size_ == 0) { 
				throw std::range_error("Attempted use pop_back on empty vector");
			}
			return elements_[size_--];
		}

		/**
		 * Insert an element at some index in our vector, increasing the size by 1
		 * Returns a reference to the newly added element (not the original).
		 */
		T& insert(size_t index, const T& element) {

			/*if (size_ >= capacity_) {
				throw std::range_error("not enough capacity");
			} */
			if ( index > size_) {
				// index = size_;
				throw std::range_error("the index is out of bounds");
			}
			std::move_backward(elements_+index, elements_ + size_, elements_ + size_+ 1);
			/*for (size_t i = size_; i > index; i--){
				elements_[i] = elements_[i-1];
			} 
			*/
			elements_[index] = element;
			increaseSize();
			return elements_[index];
		}

		/**
		 * Erase one element in our vector at the specified index, decreasing the size by 1
		 * Throws std::range_error if the index is out of bounds.
		 * Calls the erased element's destructor.
		 * Returns the new size.
		 */
		size_t erase(size_t index) {

			if (index >= size_) {
				throw std::range_error("the index is out of bounds");
			}
			std::move(elements_ + index +1, elements_ + size_, elements_ + index);
			size_--;
			return index;
		}

		/**
		 * Calls each element's destructor, then clears our internal
		 * data by setting size to zero and resetting the capacity.
		*/
		void clear() {

			while(size_ != 0) elements_[size_--].~T();
		}

	/**
	 * Begin private members and methods.
	 * Private methods can often contain useful helper functions,
	 * or functions to reduce repeated code.
	*/
	private:

		/// Number of valid elements currently in our vector
		size_t size_ = 0;

		/// Capacity of our vector; The actual size of our internal array
		size_t capacity_ = 0;

		/**
		 * Our internal array of elements of type T.
		 * Starts off as a null pointer.
		 */
		T* elements_ = nullptr;

		/**
		 * Increases the size of our vector by 1 element.
		 * If more capacity is needed, it is doubled.
		 */
		void increaseSize() {

			if ( size_ == capacity_ ) {
				capacity_ = 2*capacity_;
			}
			size_ ++;
		}

		/**
		 * Decreases the size of our vector by 1 element.
		 * If our size is less than a third of our capacity, cuts our capacity in half.
		 * Capacity should always be at least MyVector::MINIMUM_CAPACITY
		 */
		void decreaseSize() {

			if ( size_ < (1/3*capacity_) ) {
				capacity_ = (capacity_/0.5);
			}
			else {
				size_ --;
			}
		}

		/**
		 * Helper function that is called whenever we need to change the capacity of our vector
		 * Should throw std::range_error when asked to change to a capacity that cannot hold our existing elements.
		 */
		void changeCapacity(size_t c) {

			if ( c > capacity_ ) {
				throw std::range_error ("Can not hold exist element");
			} 
			else {
				capacity_ = c;
			}
		}

		/**
		 * Copy another vector's elements into our own, by value.
		 * Does not simply copy the other vector's array's pointer
		 * Does not care about matching the capacity exactly.
		 * This is a helper function relied upon by the copy constuctor and the assignment operator
		 */
		void copyOther(const MyVector& other) {

			for (int i = 0; i < size_; i++) {
				elements_[i] = other.elements_[i];
			}
		}

		/**
		 * Receives a pointer to an array of elements, and their size.
		 * Throw an std::range_error if we don't already have enough capacity.
		 * Then, create a new temporary array, and copy each element by value.
		 * Next, delete our internal array if it wasn't a null pointer.
		 * Next, assign the new temporary array pointer to our internal array pointer.
		 * Finally, change our size attribute to match.
		 */
		void copyElements(T* pElements, size_t size) {

			if (capacity_ > MINIMUM_CAPACITY) {
				throw std::range_error("Don't have enough capacity");
			}
			T *temp_array = new T[size];
			// copy
			for (int i = 0; i < size_; i++) {
				temp_array[i] = pElements[i];
			}
			delete [] elements_;
			elements_ = temp_array;
			size_ = size;
		}

		//
		void assertCapacity(size_t c) const {

			if (c > capacity_) {
				throw std::range_error("c is past the limit of capacity");
			}
		}

		//
		void assertBounds(size_t index, std::string message = "") const {

			if ( index > size_) {
				throw std::range_error(message);
			}
		}
};
