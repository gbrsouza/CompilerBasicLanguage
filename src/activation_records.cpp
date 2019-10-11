#include <queue>
#include <stack>
#include <string>

using std::queue;
using std::stack;
using std::string;

enum label{

};

label next_label;

struct value;
struct record;

value parameter;

void verify_index(value val);
int to_index(value val);

void push_parameter(value val);
value pop_parameter();
void push_function_call(label lab);
label pop_function_call();
value get_return_value();
void set_return_value(value val);

void let(char* name, int id1, int id2, value val);
void def(char* name, label lab);
label get_def(char* name);
void dim(char* name, int id1, int id2);
value get(char* name, int id1, int id2);
void print(value val);
void read(char* name, int id1, int id2);
void input(char* name, int id1, int id2);
void data(int val);
void data(double val);

value operator+(value left, value right);
value operator-(value left, value right);
value operator*(value left, value right);
value operator/(value left, value right);
value operator%(value left, value right);
value operator^(value left, value right);
value operator<(value left, value right);
value operator>(value left, value right);
value operator<=(value left, value right);
value operator>=(value left, value right);
value operator==(value left, value right);
value operator!=(value left, value right);

value ABS(value val);
value ATN(value val);
value COS(value val);
value EXP(value val);
value INT(value val);
value LOG(value val);
value RND(value val);
value SIN(value val);
value SQR(value val);
value TAN(value val);

int main(){
	
}

struct value{
	enum{
		Int,
		String,
		Char,
		Float,
		Bool,
		Array1d,
		Array2d,
	} value_type;
	union{
		int _int;
		string* _string;
		double _double;
		bool _bool;
		value* _array;
		value** _matrix;
	} content;
};

struct record{
	enum{
		function_call,
		gosub_stmt,
		parameter,
		for_label,
		for_variable,
		for_step,
		next_label,
	} record_type;
	
	union{
		label lab;
		value val;
		string* name;
	} record_content;
};
