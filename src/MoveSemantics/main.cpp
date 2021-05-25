#include <iostream>
#include <string>

//https://modoocode.com/227
class MyString {
	char* string_content;  // 문자열 데이터를 가리키는 포인터
	int string_length;     // 문자열 길이

	int memory_capacity;  // 현재 할당된 용량

public:
	MyString();

	// 문자열로 부터 생성
	MyString(const char* str);

	// 복사 생성자
	//비고 : 생성자기때문에
	MyString(const MyString& str);

	// 이동 생성자
	//장점 : 메모리 복사x 주소값만 복사
	MyString(MyString&& str);
	//이동 연산자
	MyString& operator=(MyString&& s);

	void reserve(int size);
	MyString operator+(const MyString& s);
	// 할당 연산자
	// 복사 생성자랑 비교해서 기존의 데이터를 없애는 단계가 추가
	MyString& operator=(const MyString& s);
	~MyString();

	int length() const;

	void println();
};

MyString::MyString() {
	std::cout << "생성자 호출 ! " << std::endl;
	string_length = 0;
	memory_capacity = 0;
	string_content = NULL;
}

MyString::MyString(const char* str) {
	std::cout << "생성자 호출 ! " << std::endl;
	string_length = strlen(str);
	memory_capacity = string_length;
	string_content = new char[string_length];

	for (int i = 0; i != string_length; i++) string_content[i] = str[i];
}
MyString::MyString(const MyString& str) {
	std::cout << "복사 생성자 호출 ! " << std::endl;
	string_length = str.string_length;
	string_content = new char[string_length];

	for (int i = 0; i != string_length; i++)
		string_content[i] = str.string_content[i];
}
MyString::MyString(MyString&& str) {
	std::cout << "이동 생성자 호출 !" << std::endl;
	string_length = str.string_length;
	string_content = str.string_content;
	memory_capacity = str.memory_capacity;

	// 임시 객체 소멸 시에 메모리를 해제하지
	// 못하게 한다.
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
	std::cout << "복사!" << std::endl;
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
	std::cout << "이동!" << std::endl;
	//?? 있던거 지우는코드??
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
	std::cout << "Swap 전 -----" << std::endl;
	std::cout << "str1 : ";
	str1.println();
	std::cout << "str2 : ";
	str2.println();

	std::cout << "Swap 후 -----" << std::endl;
	my_swap(str1, str2);
	std::cout << "str1 : ";
	str1.println();
	std::cout << "str2 : ";
	str2.println();

	MyString str3("abc");
	std::cout << "TEST 2 str4 생성전-----" << std::endl;
	std::cout << "str3 : ";
	str3.println();
	
	MyString str4(std::move(str3));
	std::cout << "TEST 2 str4 생성후-----" << std::endl;
	std::cout << "str4 : ";
	str4.println();
	std::cout << "str3 : ";
	str3.println();


	std::cout << "TEST 3 std::string -----" << std::endl;
	//문자열 너무 작으면 최적화되서 이동안함
	std::string&& Rstring = "문자열 많아야 이동해요........................";
	std::string Lstring = "문자열 줄어들어야 다시만들어요";
	printf("Rstring addr : %p\n", Rstring.c_str());
	printf("Lstring addr : %p\n", Lstring.c_str());
	printf("Rstring 내용 : %s\n", Rstring.c_str());
	printf("Lstring 내용 : %s\n", Lstring.c_str());
	
	std::cout << "TEST 3 복사생성자 호출" << std::endl;
	Lstring = Rstring;
	printf("Rstring addr : %p\n", Rstring.c_str());
	printf("Lstring addr : %p\n", Lstring.c_str());
	printf("Rstring 내용 : %s\n", Rstring.c_str());
	printf("Lstring 내용 : %s\n", Lstring.c_str());

	std::cout << "TEST 3 이동생성자 호출" << std::endl;
	Lstring = std::move(Rstring);
	printf("Rstring addr : %p\n", Rstring.c_str());
	printf("Lstring addr : %p\n", Lstring.c_str());
	printf("Rstring 내용 : %s\n", Rstring.c_str());
	printf("Lstring 내용 : %s\n", Lstring.c_str());
	
	int a = 1;
	int&& b = 10;//Rvalue만 받겠다. 하지만 이후는 lvalue취급
	b++;
	//printf("%d ",myAdd(b));//컴파일 에러 b는 lvalue다.
}
