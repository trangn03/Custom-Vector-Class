

//
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


//
#include "MyVector.hpp"
#include "puhp-tests/PuhPTests.h"


//
using std::cout, std::cin, std::endl;
using std::string;
using std::vector;


//
using PuhPTests::Tests;
using PuhPTests::Test;


//
MyVector<int> makeVector(size_t count = 100)
{
	MyVector<int> v;
	for ( size_t i = 0; i < count; i++ ) {
		int val = i * 2 - (count / 2);
		v.push_back(val);
	}
	
	return v;
}

//
bool checkIntegerVectorsEqual(const MyVector<int>& v1, const MyVector<int>& v2)
{
	//
	if ( v1.size() != v2.size() ) {
		return false;
	}
	
	for ( size_t i = 0; i < v1.size(); i++ ) {
		if ( v1.at(i) != v2.at(i) ) {
			return false;
		}
	}
	
	return true;
}

//
bool checkIntegerVectorsEqual(const MyVector<int>& v1, const std::vector<int>& v2)
{
	//
	if ( v1.size() != v2.size() ) {
		return false;
	}
	
	for ( size_t i = 0; i < v1.size(); i++ ) {
		if ( v1.at(i) != v2.at(i) ) {
			return false;
		}
	}
	
	return true;
}

//
void printVector(const MyVector<int>& v, string label = "NoLabel") {
	if ( !label.empty() ) {
		cout << label << endl;
	}
	for (size_t i = 0; i < v.size(); i++) {
		cout << v.at(i) << endl;
	}
	cout << endl;
}

//
Test pushBackTests()
{
	//
	Test t("push_back tests");
	
	//
	t.assertNoException(
		[&t]()
		{
			//
			{
				auto v = makeVector(500);
				t.assertEqual(v.size(), 500, 1, "Size should be 500 after push_back 500 elements.");
				t.assertEqual(v.capacity(), 512, 1, "Check capacity");
				for ( int i : {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}) {
					v.push_back(i);
				}
				t.assertEqual(v.capacity(), 512, 1, "Check capacity");
				v.push_back(5);
				t.assertEqual(v.capacity(), 1024, 1, "Check capacity");
				t.assertEqual(v.size(), 513, 1, "Size should be 513 after additional pushes");
			}
			
			//
			{
				auto v = makeVector(1100);
				t.assertEqual(v.size(), 1100, 1, "Size should be 1100 after push_back 1100 elements.");
				t.assertEqual(v.capacity(), 2048, 1, "Check capacity");
			}
			
		}, 2, "Make a vector with push_back calls"
	);
	
	return t;
}

//
Test capacityTests()
{
	//
	Test t("Capacity tests");
	
	//
	t.assertNoException(
		[&t]()
		{
			//	Make a MyVector with a mirror to a normal std::vector
			auto v = makeVector(1000);
			std::vector<int> sv;
			for ( size_t i = 0; i < v.size(); i++ ) {
				sv.push_back(v.at(i));
			}
			
			//
			v.reserve(2000);
			t.assertEqual(v.capacity(), 2000, 1, "Capacity should match new 2000 value");
			t.assertTrue(checkIntegerVectorsEqual(v, sv), 2, "reserve() shouldn't have corrupted vector data");
			
			//
			v.reserve(1000);
			t.assertEqual(v.capacity(), 2000, 1, "reserve() requests should not result in less capacity than already held");
			
		}, 1, "Valid capacity calls"
	);
	
	return t;
}

//
Test rangeTests()
{
	//
	Test t("Range tests", 0);
	auto v = makeVector(100);
	
	//	Try to grab invalid index
	t.assertException(
		[&t, &v]()
		{
			v.at(-1);
		}, 1, "Grab invalid index"
	);
	t.assertException(
		[&t, &v]()
		{
			v.at(100);
		}, 1, "Grab invalid index"
	);
	//	Try to grab valid index!
	t.assertNoException(
		[&t, &v]()
		{
			t.assertEqual(v.at(0), -50, 1, "Check for correct number at index 0");
			t.assertEqual(v.at(35), 20, 1, "Check for correct number at index 35");
			t.assertEqual(v.at(99), 148, 1, "Check for correct number at index 99");
		}, 1, "Grab some values"
	);
	
	//	Try to insert at invalid index
	t.assertException(
		[&t, &v]()
		{
			v.insert(-1, 1);
		}, 1, "Insert at invalid index"
	);
	t.assertException(
		[&t, &v]()
		{
			v.insert(101, 1);
		}, 1, "Insert at invalid index 101"
	);
	
	//	Try to insert at valid index and check the result
	t.assertNoException(
		[&t, &v]()
		{
			v.insert(77, 22);
			t.assertEqual(v.at(76), 102, 1, "Check for correct number at index 76");
			t.assertEqual(v.at(77), 22, 1, "Check for correct number at index 77");
			t.assertEqual(v.at(78), 104, 1, "Check for correct number at index 78");
		}, 1, "Grab invalid index"
	);
	
	//	Check that the [] operator works
	t.assertNoException(
		[&t]()
		{
			//
			auto v = makeVector();
			t.assertEqual(v[1], -48, 1, "Check access operator [] at index 1");
			t.assertEqual(v[27], 4, 1, "Check access operator [] at index 27");
			t.assertEqual(v[97], 144, 1, "Check access operator [] at index 97");
			
		}, 1, "Check access operator"
	);
	//	Check that the [] operator throws for invalid index -1
	t.assertException(
		[&t]()
		{
			auto v = makeVector();
			v[-1];
			
		}, 1, "Check access operator throws for index -1"
	);
	//	Check that the [] operator throws for invalid index 100
	t.assertException(
		[&t]()
		{
			auto v = makeVector();
			v[100];
			
		}, 1, "Check access operator throws for index 100"
	);
	
	// Check pop_back
	t.assertNoException(
		[&t]()
		{
			//
			auto v = makeVector(1);
			v.pop_back();
			t.assertTrue(v.empty(), 1, "Single-element vector should become empty after pop_back");
			
			//
			v.push_back(1);
			v.push_back(2);
			v.pop_back();
			t.assertTrue(v.size() == 1 && v[0] == 1, 1, "Checking pop_back logic");
			
		}, 1, "Valid pop_back operations shouldn't throw an exception"
	);
	t.assertException(
		[&t]()
		{
			auto v = makeVector(0);
			v.pop_back();
			
		}, 1, "Pop back should throw on an empty vector"
	);
	
	return t;
}

//
Test copyTests()
{
	//
	Test t("Copy tests");
	
	//
	t.assertNoException(
		[&t]()
		{
			//
			auto v1 = makeVector();
			MyVector<int> v2, v3(v1);
			
			//
			v2 = v1;
			
			//
			t.assertTrue(checkIntegerVectorsEqual(v1, v2), 1, "Assignment operator (equality check)");
			t.assertTrue(v1.size() == v2.size(), 1, "Assignment operator (size)");
			t.assertTrue(v1.capacity() == v2.capacity(), 1, "Assignment operator (size)");
			
			//
			t.assertTrue(checkIntegerVectorsEqual(v1, v3), 1, "Copy constructor (equality check)");
			t.assertTrue(v1.size() == v3.size(), 1, "Copy constructor (size)");
			t.assertTrue(v1.capacity() == v3.capacity(), 1, "Copy constructor (size)");
			
		}, 1, "Copy tests shouldn't throw any exceptions"
	);
	
	return t;
}

//
Test deletionTests()
{
	//
	Test t("Deletion tests");
	
	//
	t.assertNoException(
		[&t]()
		{
			//
			auto v = makeVector(100);
			
			//
			v.erase(47);
			t.assertTrue(v.size() == 99, 1, "Erase an element in the middle (size)");
			t.assertTrue(v.capacity() == 128, 1, "Erase an element in the middle (capacity)");
			t.assertEqual(v.at(88), 128, 1, "Erase an element in the middle (check another element)");
			
			//
			v.erase(0);
			t.assertTrue(v.size() == 98, 1, "Erase first element (size)");
			t.assertTrue(v.capacity() == 128, 1, "Erase first element (capacity)");
			t.assertEqual(v.at(88), 130, 1, "Erase first element (check another element)");
			
		}, 1, "Valid deletion tests shouldn't throw exceptions"
	);
	
	//
	t.assertException(
		[&t]()
		{
			//
			auto v = makeVector(1);
			
			//
			v.erase(0);
			v.erase(0);
			
		}, 1, "Erasing 2 elements from a 1 element vector should throw exception"
	);
	
	return t;
}

//
Test destructorTests()
{
	//
	Test t("Destructor tests");
	
	//	Local classes, lel
	class MyThrowner
	{
		// hai
	};
	class MyThrower
	{
		public:
			~MyThrower() noexcept(false) {
				if (this->throw_enabled == true) {
					throw MyThrowner();
				}
			}
			bool throw_enabled = false;
	};
	
	//
	t.assertException(
		[&t]()
		{
			//
			MyVector<MyThrower> v;
			v.push_back(MyThrower());
			v[0].throw_enabled = true;
			
			//
			try
			{
				v.clear();
			}
			catch(const MyThrowner& e)
			{
				// yey
				v[0].throw_enabled = false;
				throw;
			}
			catch( ... )
			{
				// oh nyo
			}
			v[0].throw_enabled = false;
			
		}, 1, "Vector should call element destructors when clear()ing"
	);
	
	//
	t.assertException(
		[&t]()
		{
			//
			MyVector<MyThrower> v;
			v.push_back(MyThrower());
			v.push_back(MyThrower());
			v[0].throw_enabled = true;
			v[1].throw_enabled = true;
			
			//
			try
			{
				v.erase(0);
			}
			catch(const MyThrowner& e)
			{
				// yey
				v[0].throw_enabled = false;
				v[1].throw_enabled = false;
				throw;
			}
			catch( ... )
			{
				// oh nyo
			}
			v[0].throw_enabled = false;
			v[1].throw_enabled = false;
			
		}, 1, "Vector should call element destructors when erasing()ing"
	);
	
	//
	t.assertException(
		[&t]()
		{
			//
			MyVector<MyThrower> v;
			v.push_back(MyThrower());
			v.push_back(MyThrower());
			v[0].throw_enabled = true;
			v[1].throw_enabled = true;
			
			//
			try
			{
				v.set(1, MyThrower());
			}
			catch(const MyThrowner& e)
			{
				// yey
				v[0].throw_enabled = false;
				v[1].throw_enabled = false;
				throw;
			}
			catch( ... )
			{
				// oh nyo
			}
			v[0].throw_enabled = false;
			v[1].throw_enabled = false;
			
		}, 1, "Vector should call element destructors when set()ing"
	);
	
	return t;
}

//
Test emptyTests()
{
	//
	Test t("Empty tests");
	
	//
	t.assertNoException(
		[&t]()
		{
			//
			auto v = makeVector(0);
			
			//
			t.assertTrue(v.empty(), 1, "empty() should work with empty vector");
			
			//
			v.push_back(1);
			t.assertFalse(v.empty(), 1, "empty() should not work with non-empty vector");
			
			//
			v.erase(0);
			t.assertTrue(v.empty(), 1, "empty() should work with empty vector (again)");
			
			//
			v.push_back(1);
			v.push_back(1);
			v.push_back(1);
			v.push_back(1);
			v.push_back(1);
			v.clear();
			t.assertTrue(v.empty(), 1, "empty() should work with empty vector (again)");
			
		}, 1, "empty() tests should not throw exceptions"
	);
	
	return t;
}

//
int main()
{
	//
	Tests tests;
	
	//
	std::vector<std::function<Test()>> fs = {
		pushBackTests,
		rangeTests,
		capacityTests,
		copyTests,
		deletionTests,
		destructorTests,
		emptyTests
	};
	
	//
	for ( auto f : fs ) {
		tests.add(f());
	}
	
	//
	cout << endl;
	tests.printTestsSummary();
	
	//
	tests.writeResultsAsGradescopeJson();
	
	return 0;
}





