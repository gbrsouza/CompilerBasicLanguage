#include <chrono>
#include <iostream>
#include <math.h>
#include <queue>
#include <random>
#include <sstream>
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
		char _char;
		double _double;
		bool _bool;
		struct{
			int size;
			value* content;
		} _array;
		struct{
			int size_1;
			int size_2;
			value** content; 
		} _matrix;
	} content;
	value();
	value(const value& val);
	value& operator=(const value& val);
	~value();
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
void push_loop_last_value(value val);
value pop_loop_last_value();
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

bool is_negative(value val);

value to_value(int val);
value to_value(double val);
value to_value(char val);
value to_value(bool val);
value to_value(const char* val);

value operator+(value target);
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

bool verify_numeric(value val){
	return (val.value_type == value::Int || val.value_type == value::Float); 
}

struct record{
	enum{
		subroutine_label,
		parameter,
		return_value,
		for_label,
		for_variable,
		for_step,
		for_last,
		next_stmt_label,
		stored_value,
	} record_type;
	
	union{
		label lab;
		value* val;
		string* name;
	} record_content;
	
	record(){
		record_type = stored_value;
		record_content.val = nullptr;
	}
	
	record(const record& rec){
		record_type = rec.record_type;
		record_content = rec.record_content;
	}
};

stack<record> st;

void push_parameter(value val){
	record new_record;
	new_record.record_type = record::parameter;
	new_record.record_content.val = new value(val);
	st.push(new_record);
}

value pop_parameter(){
	record top = st.top();
	if(top.record_type != record::parameter){
		cerr << "Error: tried to pop incompatible type from stack as parameter" << endl;
		exit(-1);
	}
	value val(*top.record_content.val);
	delete top.record_content.val;
	st.pop();
	return val;
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
	new_record.record_content.val = new value(val);
	st.push(new_record);
}

value get_return_value(){
	record top = st.top();
	if(top.record_type != record::return_value){
		cerr << "Error: tried to pop incompatible type from stack as return value" << endl;
		exit(-1);
	}
	value val(*top.record_content.val);
	delete top.record_content.val;
	st.pop();
	return val;
}

void push_value(value val){
	record new_record;
	new_record.record_type = record::stored_value;
	new_record.record_content.val = new value(val);
	st.push(new_record);
}

value pop_value(){
	record top = st.top();
	if(top.record_type != record::stored_value){
		cerr << "Error: tried to pop incompatible type from stack as value" << endl;
		exit(-1);
	}
	value val(*top.record_content.val);
	delete top.record_content.val;
	st.pop();
	return val;
}

void push_loop_variable(const char* name){
	record new_record;
	new_record.record_type = record::for_variable;
	new_record.record_content.name = new string(name);
	for(char& c : *new_record.record_content.name){
		c = (char) tolower(c);
	}
	st.push(new_record);
}

void verify_loop_variable(const char* name){
	record& top = st.top();
	if(top.record_type != record::for_variable){
		cerr << "Error: Next command not expected" << endl;
		exit(-1);
	}
	string s_name(name);
	for(char& c : s_name){
		c = (char) tolower(c);
	}
	if(*top.record_content.name != s_name){
		cerr << "Error: Next command on unexpected variable" << endl;
		exit(-1);
	}
}

void pop_loop_variable(){
	st.pop();
}

void push_loop_label(label lab){
	record new_record;
	new_record.record_type = record::for_label;
	new_record.record_content.lab = lab;
	st.push(new_record);
}

label pop_loop_label(){
	record top = st.top();
	if(top.record_type != record::for_label){
		cerr << "Error: tried to pop incompatible type from stack as loop label" << endl;
		exit(-1);
	}
	st.pop();
	return top.record_content.lab;
}

void push_loop_step(value val){
	record new_record;
	new_record.record_type = record::for_step;
	new_record.record_content.val = new value(val);
	st.push(new_record);
}

value pop_loop_step(){
	record top = st.top();
	if(top.record_type != record::for_step){
		cerr << "Error: tried to pop incompatible type from stack as loop step" << endl;
		exit(-1);
	}
	value val(*top.record_content.val);
	delete top.record_content.val;
	st.pop();
	return val;
}

void push_loop_last_value(value val){
	record new_record;
	new_record.record_type = record::for_last;
	new_record.record_content.val = new value(val);
	st.push(new_record);
}

value pop_loop_last_value(){
	record top = st.top();
	if(top.record_type != record::for_last){
		cerr << "Error: tried to pop incompatible type from stack as loop bound" << endl;
		exit(-1);
	}
	value val(*top.record_content.val);
	delete top.record_content.val;
	st.pop();
	return val;
}

void push_next_stmt_label(label lab){
	record new_record;
	new_record.record_type = record::next_stmt_label;
	new_record.record_content.lab = lab;
	st.push(new_record);
}

label pop_next_stmt_label(){
	record top = st.top();
	if(top.record_type != record::next_stmt_label){
		cerr << "Error: tried to pop incompatible type from stack as NEXT statement label" << endl;
		exit(-1);
	}
	st.pop();
	return top.record_content.lab;
}

string value_type_to_string(const value& val){
	switch(val.value_type){
		case value::Int:
			return "Int";
		case value::Float:
			return "Float";
		case value::Char:
			return "Char";
		case value::Bool:
			return "Bool";
		case value::String:
			return "String";
		case value::Array1d:
			return "1D Array";
		case value::Array2d:
			return "2D Array";
	}
	return "<<undefined>>";
}

void destroy(value& val){
	if(val.value_type == value::String){
		delete val.content._string;
	}
	if(val.value_type == value::Array1d){
		delete[] val.content._array.content;
	}
	if(val.value_type == value::Array2d){
		for(int i = 0; i < val.content._matrix.size_1; i++){
			delete[] val.content._matrix.content[i];
		}
		delete[] val.content._matrix.content;
	}
}

value::value(){
	value_type = Int;
	content._int = 0;
}

value::value(const value& val){
	*this = val;
}
value& value::operator=(const value& val){
	value_type = val.value_type;
	if(val.value_type == value::String){
		content._string = new string(*val.content._string);
	}
	else if(val.value_type == value::Array1d){
		content._array.size = val.content._array.size;
		content._array.content = new value[content._array.size];
		for(int i = 0; i < content._array.size; i++){
			content._array.content[i] = val.content._array.content[i];
		}
	}
	else if(val.value_type == value::Array2d){
		content._matrix.size_1 = val.content._matrix.size_1;
		content._matrix.content = new value*[content._matrix.size_1];
		content._matrix.size_2 = val.content._matrix.size_2;
		for(int i = 0; i < content._matrix.size_1; i++){
			content._matrix.content[i] = new value[content._matrix.size_2];
			for(int j = 0; j < content._matrix.size_2; j++){
				content._matrix.content[i][j] = val.content._matrix.content[i][j];
			}
		}
	}
	else{
		content = val.content;
	}
	return *this;
}

value::~value(){
	destroy(*this);
}

queue<value> data_buffer;
value* memory[286];

value& from_memory(const char* name){
	int idx;
	if(name[1] == '\0'){
		idx = tolower(name[0]) - 'a';
	}
	else{
		idx = 26 + (tolower(name[0]) - 'a') * 10 + (name[1] - '0');
	}
	if(memory[idx] == nullptr){
		memory[idx] = new value;
		memory[idx]->value_type = value::Int;
		memory[idx]->content._int = 0;
	}
	return *memory[idx];
}

void check_dim(const value& val, int id1, int id2){
	if(id2 != -1 && val.value_type != value::Array2d){
		cerr << "Error: tried to use " << value_type_to_string(val) << " as two dimensional array" << endl;
		exit(-1);
	}
	if(id1 != -1 && id2 == -1 && val.value_type != value::Array1d){
		cerr << "Error: tried to use " << value_type_to_string(val) << " as one dimensional array" << endl;
		exit(-1);
	}
}

value& access_reference(value& val, int id1, int id2){
	if(id2 != -1){
		if(id1 < 0 || id1 >= val.content._matrix.size_1 || id2 < 0 || id2 >= val.content._matrix.size_2){
			cerr << "Error: tried to access position (" << id1 << ", " << id2 << ") of matrix with dimensions ("
				<< val.content._matrix.size_1 << ", " << val.content._matrix.size_2 << ")" << endl;
			exit(-1);
		}
		return val.content._matrix.content[id1][id2];
	}
	else if(id1 != -1){
		if(id1 < 0 || id1 >= val.content._array.size){
			cerr << "Error: tried to access position (" << id1 << ") of array with dimension ("
				<< val.content._array.size << ")" << endl;
			exit(-1);
		}
		return val.content._array.content[id1];
	}
	return val;
}

label function_label[26] = { invalid,
invalid, invalid, invalid, invalid, invalid, invalid,
invalid, invalid, invalid, invalid, invalid, invalid,
invalid, invalid, invalid, invalid, invalid, invalid,
invalid, invalid, invalid, invalid, invalid, invalid,
};

label get_def(const char* name){
	return function_label[tolower(name[2]) - 'a'];
}

value get_var(const char* name, int id1, int id2){
	value& var = from_memory(name);
	check_dim(var, id1, id2);
	value& target = access_reference(var, id1, id2);
	return target;
}

void let(const char* name, int id1, int id2, value val){
	if(val.value_type == value::Array1d || val.value_type == value::Array2d){
		cerr << "Error: Tried to assign variable with " << value_type_to_string(val) << endl;
		exit(-1);
	}
	value& var = from_memory(name);
	check_dim(var, id1, id2);
	value& target = access_reference(var, id1, id2);
	destroy(target);
	target = val;
}

void def(const char* name, label lab){
	function_label[tolower(name[2]) - 'a'] = lab;
}

void dim(const char* name, int id1, int id2){
	value& target = from_memory(name);
	destroy(target);
	
	if(id2 != -1){
		target.value_type = value::Array2d;
		target.content._matrix.size_1 = id1;
		target.content._matrix.content = new value*[id1];
		target.content._matrix.size_2 = id2;
		for(int i = 0; i < id1; i++){
			target.content._matrix.content[i] = new value[id2];
			for(int j = 0; j < id2; j++){
				target.content._matrix.content[i][j].value_type = value::Int;
				target.content._matrix.content[i][j].content._int = 0;
			}
		}
	}
	else if(id1 != -1){
		target.value_type = value::Array1d;
		target.content._array.size = id1;
		target.content._array.content = new value[id1];
		for(int i = 0; i < id1; i++){
			target.content._array.content[i].value_type = value::Int;
			target.content._array.content[i].content._int = 0;
		}
	}
	else{
		target.value_type = value::Int;
		target.content._int = 0;
	}
}

void print(value val, bool separator){ // separator = true if comma and false if semicolon or empty
	if(val.value_type == value::Array1d || val.value_type == value::Array2d){
		cerr << "Error: Print command of " << value_type_to_string(val) << " is not defined" << endl;
		exit(-1);
	}
	
	static bool is_last_numeric_or_boolean = false;
	static int print_pointer = 0;
	bool is_current_numeric_or_boolean;
	std::stringstream buffer;
	
	if(val.value_type == value::String){
		is_current_numeric_or_boolean = false;
		buffer << *val.content._string;
	}
	else if(val.value_type == value::Char){
		is_current_numeric_or_boolean = false;
		buffer << val.content._char;
	}
	else if(val.value_type == value::Int){
		is_current_numeric_or_boolean = true;
		buffer << val.content._int;
	}
	else if(val.value_type == value::Float){
		is_current_numeric_or_boolean = true;
		buffer << val.content._double;
	}
	else{ // val.value_type == value::Bool
		is_current_numeric_or_boolean = true;
		buffer << (val.content._bool ? "True" : "False");
	}
	
	string printed_string = "";
	if(is_last_numeric_or_boolean && is_current_numeric_or_boolean && print_pointer < 75){
		printed_string = " ";
	}
	
	printed_string += buffer.str();
	
	for(char c : printed_string){
		if(c == '\n'){
			cout << endl;
			print_pointer = 0;
		}
		else{
			if(print_pointer == 75){
				cout << endl;
				print_pointer = 0;
			}
			cout << c;
			print_pointer++;
		}
	}
	
	is_last_numeric_or_boolean = is_current_numeric_or_boolean && (!separator || print_pointer % 15 == 0);
	
	if(separator){
		while(print_pointer % 15 != 0){
			cout << ' ';
			print_pointer++;
		}
	}
	
	std::fflush(stdout);
}

void read(const char* name, int id1, int id2){
	if(data_buffer.empty()){
		cerr << "Error: read command reached with empty data buffer" << endl;
		exit(-1);
	}
	let(name, id1, id2, data_buffer.front());
	data_buffer.pop();
}

void input(const char* name, int id1, int id2){
	string input_word;
	std::cin >> input_word;
	int dot_count = 0;
	bool is_numeric = true;
	bool is_neg = false;
	if(input_word[0] == '-'){
		input_word = input_word.substr(1);
		is_neg = true;
	}
	for(char c : input_word){
		if(c == '.'){
			dot_count++;
		}
		is_numeric &= (isdigit(c) != 0);
	}
	value val;
	if(!input_word.empty() && is_numeric && dot_count == 1 && input_word[0] != '.' && input_word.back() != '.'){
		if(is_neg){
			input_word = string("-") + input_word;
		}
		val.value_type = value::Float;
		val.content._double = std::stod(input_word);
	}
	else if(!input_word.empty() && is_numeric && dot_count == 0){
		if(is_neg){
			input_word = string("-") + input_word;
		}
		val.value_type = value::Int;
		val.content._int = std::stoi(input_word);
	}
	else{
		if(is_neg){
			input_word = string("-") + input_word;
		}
		val.value_type = value::String;
		val.content._string = new string(input_word);
	}
	let(name, id1, id2, val);
}

void data(value val){
	data_buffer.push(val);
}

bool is_negative(value val){
	if(!verify_numeric(val)){
		cerr << "Error: step is not numeric." << endl;
		exit(-1);
	}
	if(val.value_type == value::Int){
		return val.content._int < 0;
	}
	return val.content._double < 0;
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

string binary_operator_error(string op, const value& left, const value& right){
	return "Error: Undefined operator " + op + " between " + 
		value_type_to_string(left) + " and " + value_type_to_string(right);
}

double to_double(const value& val){ // receives numeric value and returns double
	if(val.value_type == value::Int){
		return (double) val.content._int;
	}
	return val.content._double;
}

value operator+(value target){
	if(!verify_numeric(target)){
		cerr << "Error: Undefined unary operator + of " << value_type_to_string(target) << endl;
		exit(-1);
	}
	return target;
}

value operator-(value target){
	if(!verify_numeric(target)){
		cerr << "Error: Undefined unary operator - of " << value_type_to_string(target) << endl;
		exit(-1);
	}
	if(target.value_type == value::Int){
		target.content._int = -target.content._int;
	}
	else{
		target.content._double = -target.content._double;
	}
	return target;
}

value operator!(value target){
	if(target.value_type != value::Bool){
		cerr << "Error: Undefined unary operator ! of " << value_type_to_string(target) << endl;
		exit(-1);
	}
	target.content._bool = !target.content._bool;
	return target;
}

value operator+(value left, value right){
	if(!verify_numeric(left) || !verify_numeric(right)){
		cerr << binary_operator_error("+", left, right) << endl;
		exit(-1);
	}
	if(left.value_type == value::Float || right.value_type == value::Float){
		return to_value(to_double(left) + to_double(right));
	}
	return to_value(left.content._int + right.content._int);
}

value operator-(value left, value right){
	if(!verify_numeric(left) || !verify_numeric(right)){
		cerr << binary_operator_error("-", left, right) << endl;
		exit(-1);
	}
	if(left.value_type == value::Float || right.value_type == value::Float){
		return to_value(to_double(left) - to_double(right));
	}
	return to_value(left.content._int - right.content._int);
}

value operator*(value left, value right){
	if(!verify_numeric(left) || !verify_numeric(right)){
		cerr << binary_operator_error("*", left, right) << endl;
		exit(-1);
	}
	if(left.value_type == value::Float || right.value_type == value::Float){
		return to_value(to_double(left) * to_double(right));
	}
	return to_value(left.content._int * right.content._int);
}

value operator/(value left, value right){
	if(!verify_numeric(left) || !verify_numeric(right)){
		cerr << binary_operator_error("/", left, right) << endl;
		exit(-1);
	}
	if(left.value_type == value::Float || right.value_type == value::Float){
		return to_value(to_double(left) / to_double(right));
	}
	return to_value(left.content._int / right.content._int);
}

value operator%(value left, value right){
	if(left.value_type != value::Int || right.value_type != value::Int){
		cerr << binary_operator_error("%", left, right) << endl;
		exit(-1);
	}
	return to_value(left.content._int % right.content._int);
}

value operator^(value left, value right){
	if(!verify_numeric(left) || !verify_numeric(right)){
		cerr << binary_operator_error("^", left, right) << endl;
		exit(-1);
	}
	if(left.value_type == value::Float || right.value_type == value::Float){
		return to_value(pow(to_double(left), to_double(right)));
	}
	return to_value(pow(left.content._int, right.content._int));
}

value operator<(value left, value right){
	if(!verify_numeric(left) || !verify_numeric(right)){
		cerr << binary_operator_error("<", left, right) << endl;
		exit(-1);
	}
	if(left.value_type == value::Float || right.value_type == value::Float){
		return to_value(to_double(left) < to_double(right));
	}
	return to_value(left.content._int < right.content._int);
}

value operator>(value left, value right){
	if(!verify_numeric(left) || !verify_numeric(right)){
		cerr << binary_operator_error(">", left, right) << endl;
		exit(-1);
	}
	if(left.value_type == value::Float || right.value_type == value::Float){
		return to_value(to_double(left) > to_double(right));
	}
	return to_value(left.content._int > right.content._int);
}

value operator<=(value left, value right){
	if(!verify_numeric(left) || !verify_numeric(right)){
		cerr << binary_operator_error("<=", left, right) << endl;
		exit(-1);
	}
	if(left.value_type == value::Float || right.value_type == value::Float){
		return to_value(to_double(left) <= to_double(right));
	}
	return to_value(left.content._int <= right.content._int);
}
value operator>=(value left, value right){
	if(!verify_numeric(left) || !verify_numeric(right)){
		cerr << binary_operator_error(">=", left, right) << endl;
		exit(-1);
	}
	if(left.value_type == value::Float || right.value_type == value::Float){
		return to_value(to_double(left) >= to_double(right));
	}
	return to_value(left.content._int >= right.content._int);
}

value operator==(value left, value right){
	if(left.value_type != right.value_type || left.value_type == value::Array1d || left.value_type == value::Array2d){
		cerr << binary_operator_error("==", left, right) << endl;
		exit(-1);
	}
	bool ret;
	if(left.value_type == value::Int){
		ret = (left.content._int == right.content._int);
	}
	else if(left.value_type == value::Float){
		ret = (left.content._double == right.content._double);
	}
	else if(left.value_type == value::Char){
		ret = (left.content._char == right.content._char);
	}
	else if(left.value_type == value::Bool){
		ret = (left.content._bool == right.content._bool);
	}
	else{
		ret = (*left.content._string == *right.content._string);
	}
	return to_value(ret);
}

value operator!=(value left, value right){
	if(left.value_type != right.value_type || left.value_type == value::Array1d || left.value_type == value::Array2d){
		cerr << binary_operator_error("!=", left, right) << endl;
		exit(-1);
	}
	bool ret;
	if(left.value_type == value::Int){
		ret = (left.content._int != right.content._int);
	}
	else if(left.value_type == value::Float){
		ret = (left.content._double != right.content._double);
	}
	else if(left.value_type == value::Char){
		ret = (left.content._char != right.content._char);
	}
	else if(left.value_type == value::Bool){
		ret = (left.content._bool != right.content._bool);
	}
	else{
		ret = (*left.content._string != *right.content._string);
	}
	return to_value(ret);
}

value operator||(value left, value right){
	if(left.value_type != value::Bool || right.value_type != value::Bool){
		cerr << binary_operator_error("||", left, right) << endl;
		exit(-1);
	}
	return to_value(left.content._bool || right.content._bool);
}

value operator&&(value left, value right){
	if(left.value_type != value::Bool || right.value_type != value::Bool){
		cerr << binary_operator_error("&&", left, right) << endl;
		exit(-1);
	}
	return to_value(left.content._bool && right.content._bool);
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

