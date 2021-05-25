#include <iostream>
#include <string>

//https://modoocode.com/227
class MyString {
	char* string_content;  // ���ڿ� �����͸� ����Ű�� ������
	int string_length;     // ���ڿ� ����

	int memory_capacity;  // ���� �Ҵ�� �뷮

public:
	MyString();

	// ���ڿ��� ���� ����
	MyString(const char* str);

	// ���� ������
	//��� : �����ڱ⶧����
	MyString(const MyString& str);

	// �̵� ������
	//���� : �޸� ����x �ּҰ��� ����
	MyString(MyString&& str);
	//�̵� ������
	MyString& operator=(MyString&& s);

	void reserve(int size);
	MyString operator+(const MyString& s);
	// �Ҵ� ������
	// ���� �����ڶ� ���ؼ� ������ �����͸� ���ִ� �ܰ谡 �߰�
	MyString& operator=(const MyString& s);
	~MyString();

	int length() const;

	void println();
};

MyString::MyString() {
	std::cout << "������ ȣ�� ! " << std::endl;
	string_length = 0;
	memory_capacity = 0;
	string_content = NULL;
}

MyString::MyString(const char* str) {
	std::cout << "������ ȣ�� ! " << std::endl;
	string_length = strlen(str);
	memory_capacity = string_length;
	string_content = new char[string_length];

	for (int i = 0; i != string_length; i++) string_content[i] = str[i];
}
MyString::MyString(const MyString& str) {
	std::cout << "���� ������ ȣ�� ! " << std::endl;
	string_length = str.string_length;
	string_content = new char[string_length];

	for (int i = 0; i != string_length; i++)
		string_content[i] = str.string_content[i];
}
MyString::MyString(MyString&& str) {
	std::cout << "�̵� ������ ȣ�� !" << std::endl;
	string_length = str.string_length;
	string_content = str.string_content;
	memory_capacity = str.memory_capacity;

	// �ӽ� ��ü �Ҹ� �ÿ� �޸𸮸� ��������
	// ���ϰ� �Ѵ�.
	str.string_content = nullptr;
	str.string_length = 0;
	str.memory_capacity = 0;
}
MyString::~MyString() {
	if (string_content) delete[] string_content;
}
void MyString::reserve(int size) {
	if (size > memory_capacity) {
		char* prev_string_content = string_content;

		string_content = new char[size];
		memory_capacity = size;

		for (int i = 0; i != string_length; i++)
			string_content[i] = prev_string_content[i];

		if (prev_string_content != NULL) delete[] prev_string_content;
	}
}
MyString MyString::operator+(const MyString& s) {
	MyString str;
	str.reserve(string_length + s.string_length);
	for (int i = 0; i < string_length; i++)
		str.string_content[i] = string_content[i];
	for (int i = 0; i < s.string_length; i++)
		str.string_content[string_length + i] = s.string_content[i];
	str.string_length = string_length + s.string_length;
	return str;
}
MyString& MyString::operator=(const MyString& s) {
	std::cout << "����!" << std::endl;
	if (s.string_length > memory_capacity) {
		delete[] string_content;
		string_content = new char[s.string_length];
		memory_capacity = s.string_length;
	}
	string_length = s.string_length;
	for (int i = 0; i != string_length; i++) {
		string_content[i] = s.string_content[i];
	}

	return *this;
}

MyString& MyString::operator=(MyString&& s) {
	std::cout << "�̵�!" << std::endl;
	//?? �ִ��� ������ڵ�??
	string_content = s.string_content;
	memory_capacity = s.memory_capacity;
	string_length = s.string_length;

	s.string_content = nullptr;
	s.memory_capacity = 0;
	s.string_length = 0;
	return *this;
}

int MyString::length() const { return string_length; }
void MyString::println() {
	for (int i = 0; i != string_length; i++) std::cout << string_content[i];

	std::cout << std::endl;
}

template <typename T>
void my_swap(T& a, T& b) {
	T tmp(std::move(a));
	a = std::move(b);
	b = std::move(tmp);
}

int myAdd(int&& a)
{
	return a + 1;
}
int main() {
	MyString str1("abc");
	MyString str2("def");
	std::cout << "Swap �� -----" << std::endl;
	std::cout << "str1 : ";
	str1.println();
	std::cout << "str2 : ";
	str2.println();

	std::cout << "Swap �� -----" << std::endl;
	my_swap(str1, str2);
	std::cout << "str1 : ";
	str1.println();
	std::cout << "str2 : ";
	str2.println();

	MyString str3("abc");
	std::cout << "TEST 2 str4 ������-----" << std::endl;
	std::cout << "str3 : ";
	str3.println();
	
	MyString str4(std::move(str3));
	std::cout << "TEST 2 str4 ������-----" << std::endl;
	std::cout << "str4 : ";
	str4.println();
	std::cout << "str3 : ";
	str3.println();


	std::cout << "TEST 3 std::string -----" << std::endl;
	//���ڿ� �ʹ� ������ ����ȭ�Ǽ� �̵�����
	std::string&& Rstring = "���ڿ� ���ƾ� �̵��ؿ�........................";
	std::string Lstring = "���ڿ� �پ���� �ٽø�����";
	printf("Rstring addr : %p\n", Rstring.c_str());
	printf("Lstring addr : %p\n", Lstring.c_str());
	printf("Rstring ���� : %s\n", Rstring.c_str());
	printf("Lstring ���� : %s\n", Lstring.c_str());
	
	std::cout << "TEST 3 ��������� ȣ��" << std::endl;
	Lstring = Rstring;
	printf("Rstring addr : %p\n", Rstring.c_str());
	printf("Lstring addr : %p\n", Lstring.c_str());
	printf("Rstring ���� : %s\n", Rstring.c_str());
	printf("Lstring ���� : %s\n", Lstring.c_str());

	std::cout << "TEST 3 �̵������� ȣ��" << std::endl;
	Lstring = std::move(Rstring);
	printf("Rstring addr : %p\n", Rstring.c_str());
	printf("Lstring addr : %p\n", Lstring.c_str());
	printf("Rstring ���� : %s\n", Rstring.c_str());
	printf("Lstring ���� : %s\n", Lstring.c_str());
	
	int a = 1;
	int&& b = 10;//Rvalue�� �ްڴ�. ������ ���Ĵ� lvalue���
	b++;
	//printf("%d ",myAdd(b));//������ ���� b�� lvalue��.
}
