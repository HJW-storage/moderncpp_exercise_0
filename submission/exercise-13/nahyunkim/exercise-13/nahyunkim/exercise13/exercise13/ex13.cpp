#include <iostream>
#include <cstdint>


void TestUniquePtr();
void TestSharedPtr();
void Print();

static int default_constructor_count = 0;
static int copy_constructor_count = 0;
static int move_constructor_count = 0;
static int op_copy_constructor_count = 0;
static int op_move_constructor_count = 0;
static int del_constructor_count = 0;

using namespace std;

template<typename T>
class MyVector {
public:
	MyVector() // �⺻������
		: data_(nullptr),
		capacity_(3),
		size_(0)
	{
		data_ = new T[capacity_];
		++default_constructor_count;

		//cout << "������ " << endl;
	}

	~MyVector() { // �Ҹ���
		delete[] data_;
		++del_constructor_count;
		//cout << "�Ҹ���" << endl;
	}

	MyVector(MyVector&& v) noexcept { // �̵� ������
		delete[] data_;
		this->capacity_ = v.capacity_;
		this->data_ = new T[capacity_];
		this->size_ = v.size_;

		++move_constructor_count;

		//cout << "�̵�������" << endl;
	}

	MyVector& operator=(MyVector&& v) noexcept { // �̵� ������
		delete[] data_;
		this->capacity_ = v.capacity_;
		this->data_ = new T[capacity_];
		this->size_ = v.size_;

		++op_move_constructor_count;

		//cout << "�̵�������" << endl;
	}

	MyVector(MyVector& v) { // ���������
		delete[] data_;
		this->capacity_ = v.capacity_;
		this->data_ = new T[capacity_];
		this->size_ = v.size_;

		++copy_constructor_count;
		//cout << "���������" << endl;
	}

	MyVector& operator=(const MyVector & r) { // ���� ���� ������

		if (this != &r) {
			this->capacity_ = r.capacity_;
			this->data_ = r.data_;
			this->size_ = r.data_;
			
		}

		++op_copy_constructor_count;
		//cout << "������Կ����� ȣ��" << endl;
		return *this;
	}

	void push_back(const T& data) {
		if (size_ >= capacity_) {
			// �뷮 �ø���
			capacity_ = capacity_ * 2;

			// �޸� ���� �Ҵ�
			T* new_data = new T[capacity_];

			// ���� ������ ����
			std::copy(data_, data_ + size_, new_data);

			// ���� �޸� ����
			delete[] data_;

			// ���� ������ ��ü
			data_ = new_data;
		}

		data_[size_] = data;
		size_ += 1;
	}

private:
	T* data_;
	std::size_t capacity_;
	std::size_t size_;
};



void TestUniquePtr() {

	auto p = std::make_unique<MyVector<char>>();

	
	auto p2 = std::move(p);
}

void TestSharedPtr() {
	
	// MyVector ����
	auto p =std::make_shared<MyVector<char>>();
	
	auto p2 = p;  
	
	/*cout << "p.use_count(): " << p.use_count() << endl;
	cout << "p2.use_count(): " << p2.use_count() << endl;*/

	
	p2.reset();

	std::weak_ptr<MyVector<char>> weak_p = p; // weak_p�� p�� �������� ����

	auto lock = weak_p.lock();
	if (lock) {
		std::cout << "Acquired ownership\n";
	}
}




void Print() {
	std::cout << "-------------------------------------" << endl;
	std::cout << "�⺻ ������ ȣ�� Ƚ��: " << default_constructor_count << endl;
	std::cout << "���� ������ ȣ�� Ƚ��: " << copy_constructor_count << endl;
	std::cout << "�̵� ������ ȣ�� Ƚ��: " << move_constructor_count << endl;
	std::cout << "���� ���� ������ ȣ�� Ƚ��: " << op_copy_constructor_count << endl;
	std::cout << "�̵� ���� ������ ȣ�� Ƚ��: " << op_move_constructor_count << endl;
	std::cout << "�Ҹ��� ȣ�� Ƚ��: " << del_constructor_count << endl;
	std::cout << "-------------------------------------" << endl;

	default_constructor_count = 0;
	copy_constructor_count = 0;
	move_constructor_count = 0;
	op_copy_constructor_count = 0;
	op_move_constructor_count = 0;
	del_constructor_count = 0;
}

int main() {
	TestUniquePtr();
	Print();

	TestSharedPtr();

	Print();

	return 0;
}