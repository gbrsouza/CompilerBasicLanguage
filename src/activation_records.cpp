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
invalid,
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
		char _char;
		double _double;
		bool _bool;
		value* _array;
		value** _matrix;
	} content;
};

struct record{
	enum{
		subroutine_label,
		parameter,
		return_value,
		for_label,
		for_variable,
		for_step,
		next_stmt_label,
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
void undefined_control_flow();

void push_parameter(value val);
value pop_parameter();
void push_function_call(label lab);
label pop_function_call();
void set_return_value(value val);
value get_return_value();
void push_value(value val);
value pop_value();
void push_loop_variable(const char* name);
void verify_loop_variable(const char* name);
void pop_loop_variable();
void push_loop_label(label lab);
label pop_loop_label();
void push_loop_step(value val);
value pop_loop_step();
void push_next_stmt_label(label lab);
label pop_next_stmt_label();

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

void undefined_control_flow(){
	cerr << "Error: undefined control flow" << endl;
	exit(-1);
}

stack<record> st;

void push_parameter(value val){
	record new_record;
	new_record.record_type = record::parameter;
	new_record.record_content.val = val;
	st.push(new_record);
}

value pop_parameter(){
	record top = st.top();
	if(top.record_type != record::parameter){
		cerr << "Error: tried to pop incompatible type from stack as parameter" << endl;
		exit(-1);
	}
	st.pop();
	return top.record_content.val;
}

void push_function_call(label lab){
	record new_record;
	new_record.record_type = record::subroutine_label;
	new_record.record_content.lab = lab;
	st.push(new_record);
}

label pop_function_call(){
	record top = st.top();
	if(top.record_type != record::subroutine_label){
		cerr << "Error: tried to pop incompatible type from stack as function label" << endl;
		exit(-1);
	}
	st.pop();
	return top.record_content.lab;
}

void set_return_value(value val){
	record new_record;
	new_record.record_type = record::return_value;
	new_record.record_content.val = val;
	st.push(new_record);
}

value get_return_value(){
	record top = st.top();
	if(top.record_type != record::return_value){
		cerr << "Error: tried to pop incompatible type from stack as return value" << endl;
		exit(-1);
	}
	st.pop();
	return top.record_content.val;
}

void push_value(value val){
	record new_record;
	new_record.record_type = record::stored_value;
	new_record.record_content.val = val;
	st.push(new_record);
}

value pop_value(){
	record top = st.top();
	if(top.record_type != record::stored_value){
		cerr << "Error: tried to pop incompatible type from stack as value" << endl;
		exit(-1);
	}
	st.pop();
	return top.record_content.val;
}

void push_loop_variable(const char* name){ } // TODO
void verify_loop_variable(const char* name){ } // TODO
void pop_loop_variable(){ } // TODO
void push_loop_label(label lab){ } // TODO
label pop_loop_label(){ } // TODO
void push_loop_step(value val){ } // TODO
value pop_loop_step(){ } // TODO
void push_next_stmt_label(label lab){ } // TODO
label pop_next_stmt_label(){ } // TODO

label get_def(const char* name){ } // TODO
value get_var(const char* name, int id1, int id2){ } // TODO

queue<value> data_buffer;

void let(const char* name, int id1, int id2, value val){ } // TODO
void def(const char* name, label lab){ } // TODO
void dim(const char* name, int id1, int id2){ } // TODO
void print(value val, bool separator){ } // TODO
void read(const char* name, int id1, int id2){ } // TODO
void input(const char* name, int id1, int id2){ } // TODO

void data(value val){
	data_buffer.push(val);
}

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

