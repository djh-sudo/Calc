# Calc
* 用C(C++)编写一个简易计算器，不用后缀/前缀表达式实现
* 给定一个字符串，如`(((5))+4/((3))-7*(6-5/(3-(6-4))))`,计算其结果

对于我们而言，很简单，只需要先计算括号，再根据优先级计算即可,但是对于计算机而言，这有些困难，因为字符串一般需要按照某种方式扫描，但计算的顺序并非从左往右
在数据结构中，我们可以采取中缀转前缀(后缀)的方式，将中缀表达式转为前缀，然后编程实现，但从编译原理角度讲，这里还有另外一种方式。

* 成品大致如下所示

运行环境 Windows 10 Visual Studio 2019

![300](https://github.com/djh-sudo/Calc/blob/main/res.gif)

* 大致原理

1.首先需要将字符串分割为一个个`token`
即比如字符串`3+5*2`,需要被拆解为`3,+5,*,2`,这些都是一个个`token`

2.用户输入检查

针对于用户输入的字符串，需要进行检查，由于存在很多不确定性，直接交给程序运算可能存在较大风险。
这里我们的运算符一共有五种，`+,-,*,/,%`,其中前两种`(+,-)`优先级相同，后三种`(*,/,%)`优先级相同，意味着在没有括号的情况下，只需要从左向右运算即可。
检查规则如下
* 第一个`token`必须是`(`或者数字`(digit)`
* 数字`(digit)`的前面必须是`(`或者运算符`op`
* 运算符`op`前面必须是`)`或者数字`(digit)`
* 左括号`(`的前面必须是运算符`op`或者`(`
* 右括号`)`的前面必须是数字`digit`或者`)`
如果未能通过上述检查，则后续的代码无需执行，如果通过则继续执行(虽然可能依旧会存在错误)
上述检查过程可以认为是一种编译优化，先大致检查一遍，不做上述检查也可以，因为在运行时，会进一步抛出错误。

3.表达式求值

需要把`tokens`变成语法树`Syntax Tree`,然后有一种递归的思想，去计算整个表达式的值
例如 `3+5*2`,其语法树`Syntax Tree`为
<div style="align: center">
<img src="https://github.com/djh-sudo/Calc/blob/main/Syntax_Tree.png"></img>
</div>

我们一开始计算的是`N+M`,这里`N=3`,`M`需要进一步递归，发现`M=5*2`,当`M`计算完成之后，才能算出整个表达式的值。
这里递归的思想可以用以下三段代码表示
```C
/*获取一个因子
从上述的思想中，我们不能发现，最后的式子总能写成只有+和-构成的简易表达式
因此，我们获取的因子可能已经是一个数N，也可以是一个复杂的表达式，如M
*/
int Factor() {
	if (is_number(current_token)&&current_token!="") {  		//数字
		int r = stoi(current_token);                      	//字符串转整型数字
		current_token = Next_token();                     	//当前token向前移动
		return r;                                         	//返回数字
	}
	if (current_token == "(") {                         		//左括号,证明这是一个复杂式子
		current_token = Next_token();                     	//token前移动
		int r = Exp1();                                   	//计算表达式值
		if (current_token != ")") {                       	//计算完成后，token一定是右括号
			SyntaxError(7);                                 //括号不匹配,抛出语法错误
		}
		current_token = Next_token();                    	//当前token向前移动
		return r;
	}
	else
		SyntaxError(8);                                   	//表达式有误，抛出语法错误
}
```
低优先级运算符求值
```C
int Exp1() {
	int t1 = Exp2();				//优先计算
	string oper = current_token;			//获取操作符
	while (oper == "+" || oper == "-") {		//操作符只能是+/-
		current_token = Next_token();		//获取下一个token
		int t2 = Exp2();			//获取右操作数
		if (oper == "+") {
			t1 += t2;
		}
		else {
			t1 -= t2;
		}
		oper = current_token;
	}
	return t1;					//返回计算结果
}
```
高优先级运算符求值
```C
int Exp2() {
	int t1 = Factor();				//获取左操作数
	string oper = current_token;			//获取运算符
	while (oper == "*" || oper == "/" || oper == "%") {
		current_token = Next_token();
		int t2 = Factor();			//获取右操作数
		if (oper == "*") {
			t1 *= t2;
		}
		if (oper == "/") {
			if (t2 == 0) {
				SyntaxError(9);		//除数为0，抛出异常
				t2 = 1;
			}
			t1 /= t2;
		}
		if (oper == "%") {
			if (t2 == 0) {
				SyntaxError(9);		//除数为0，抛出异常
				t2 = 1;
			}
			t1 %= t2;
		}
		oper = current_token;
	}
	return t1;					//返回结果
}
```
在调用的时候，调用Exp1()即可,其余代码可以参考源文件。
