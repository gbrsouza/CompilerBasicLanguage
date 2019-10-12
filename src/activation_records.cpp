#include <chrono>
#include <iostream>
#include <math.h>
#include <queue>
#include <random>
#include <stack>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::queue;
using std::stack;
using std::string;

enum label{

};

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
		return_value,
		for_label,
		for_variable,
		for_step,
		next_label,
		stored_value,
	} record_type;
	
	union{
		label lab;
		value val;
		string* name;
	} record_content;
};

void verify_index(value val);
int to_index(value val);

void push_parameter(value val);
value pop_parameter();
void push_function_call(label lab);
label pop_function_call();
void set_return_value(value val);
value get_return_value();
void push_value(value val);
value pop_value();

label get_def(const char* name);
value get_var(const char* name, int id1, int id2);

void let(const char* name, int id1, int id2, value val);
void def(const char* name, label lab);
void dim(const char* name, int id1, int id2);
void print(value val, bool separator);
void read(const char* name, int id1, int id2);
void input(const char* name, int id1, int id2);
void data(value val);

value to_value(int val);
value to_value(double val);
value to_value(char val);
value to_value(bool val);
value to_value(const char* val);

value operator-(value target);
value operator!(value target);
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
value operator||(value left, value right);
value operator&&(value left, value right);

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

label next_label;
value parameter;

int main(){
}

void verify_index(value val){
	if(val.value_type != value::Int){
		cerr << "Error: Non-integer value used as array index" << endl;
		exit(-1);
	}
	if(val.content._int < 0){
		cerr << "Error: Negative integer used as array index" << endl;
		exit(-1);
	}
}

int to_index(value val){
	verify_index(val);
	return val.content._int;
}

void push_parameter(value val){ } // TODO
value pop_parameter(){ } // TODO
void push_function_call(label lab){ } // TODO
label pop_function_call(){ } // TODO
void set_return_value(value val){ } // TODO
value get_return_value(){ } // TODO
void push_value(value val){ } // TODO
value pop_value(){ } // TODO

label get_def(const char* name){ } // TODO
value get_var(const char* name, int id1, int id2){ } // TODO

void let(const char* name, int id1, int id2, value val){ } // TODO
void def(const char* name, label lab){ } // TODO
void dim(const char* name, int id1, int id2){ } // TODO
void print(value val, bool separator){ } // TODO
void read(const char* name, int id1, int id2){ } // TODO
void input(const char* name, int id1, int id2){ } // TODO
void data(value val){ } // TODO

value to_value(int val){
	value ret;
	ret.value_type = value::Int;
	ret.content._int = val;
	return ret;
}

value to_value(double val){
	value ret;
	ret.value_type = value::Float;
	ret.content._double = val;
	return ret;
}

value to_value(char val){
	value ret;
	ret.value_type = value::Char;
	ret.content._char = val;
	return ret;
}

value to_value(bool val){
	value ret;
	ret.value_type = value::Bool;
	ret.content._bool = val;
	return ret;
}

value to_value(const char* val){
	value ret;
	ret.value_type = value::String;
	ret.content._string = new string(val);
	return ret;
}

value operator-(value target){ } // TODO
value operator!(value target){ } // TODO
value operator+(value left, value right){ } // TODO
value operator-(value left, value right){ } // TODO
value operator*(value left, value right){ } // TODO
value operator/(value left, value right){ } // TODO
value operator%(value left, value right){ } // TODO
value operator^(value left, value right){ } // TODO
value operator<(value left, value right){ } // TODO
value operator>(value left, value right){ } // TODO
value operator<=(value left, value right){ } // TODO
value operator>=(value left, value right){ } // TODO
value operator==(value left, value right){ } // TODO
value operator!=(value left, value right){ } // TODO
value operator||(value left, value right){ } // TODO
value operator&&(value left, value right){ } // TODO

bool verify_numeric(value val){
	return (val.value_type == value::Int || val.value_type == value::Float); 
}

value ABS(value val){
	if(!verify_numeric(val)){
		cerr << "Error: ABS call of non-numeric value" << endl;
		exit(-1);
	}
	if(val.value_type == value::Int){
		return to_value(abs(val.content._int));
	}
	return to_value(abs(val.content._double));
}

value ATN(value val){
	if(!verify_numeric(val)){
		cerr << "Error: ATN call of non-numeric value" << endl;
		exit(-1);
	}
	if(val.value_type == value::Int){
		return to_value(atan(val.content._int));
	}
	return to_value(atan(val.content._double));
}

value COS(value val){
	if(!verify_numeric(val)){
		cerr << "Error: COS call of non-numeric value" << endl;
		exit(-1);
	}
	if(val.value_type == value::Int){
		return to_value(cos(val.content._int));
	}
	return to_value(cos(val.content._double));
}

value EXP(value val){
	if(!verify_numeric(val)){
		cerr << "Error: EXP call of non-numeric value" << endl;
		exit(-1);
	}
	if(val.value_type == value::Int){
		return to_value(exp(val.content._int));
	}
	return to_value(exp(val.content._double));
}

value INT(value val){
	if(!verify_numeric(val)){
		cerr << "Error: INT call of non-numeric value" << endl;
		exit(-1);
	}
	if(val.value_type == value::Int){
		return val;
	}
	return to_value((int) (val.content._double));
}

value LOG(value val){
	if(!verify_numeric(val)){
		cerr << "Error: LOG call of non-numeric value" << endl;
		exit(-1);
	}
	if(val.value_type == value::Int){
		return to_value(log(val.content._int));
	}
	return to_value(log(val.content._double));
}

value RND(value){
	static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count() ^ (long long) (new int));
	return to_value((int) rng());
}

value SIN(value val){
	if(!verify_numeric(val)){
		cerr << "Error: SIN call of non-numeric value" << endl;
		exit(-1);
	}
	if(val.value_type == value::Int){
		return to_value(sin(val.content._int));
	}
	return to_value(sin(val.content._double));
}

value SQR(value val){
	if(!verify_numeric(val)){
		cerr << "Error: SQR call of non-numeric value" << endl;
		exit(-1);
	}
	if(val.value_type == value::Int){
		return to_value(sqrt(val.content._int));
	}
	return to_value(sqrt(val.content._double));
}

value TAN(value val){
	if(!verify_numeric(val)){
		cerr << "Error: TAN call of non-numeric value" << endl;
		exit(-1);
	}
	if(val.value_type == value::Int){
		return to_value(tan(val.content._int));
	}
	return to_value(tan(val.content._double));
}

