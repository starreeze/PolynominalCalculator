# 多项式计算器用户手册

### 界面

就绪后，显示输入提示字符`>>>` ，无任何多余提示。

### 赋值/存储

#### 输入格式

`<identifier>=<initializer>`  或

`<identifier>=<expression>`

将`0.1x^2-2x+3.40`赋值给f：

```
>>> f=.1 -2 3.40
```

将`3*(f+g!)`赋值给hello（等号右边可为任意合法表达式）:

```
>>> hello=3*(f+g!)
```

#### 注意

多项式标识符只能是英文字母（大小写均可），否则提示出错。不能多加空格。成功无回显，会覆盖之前的值。

```
>>> [=1 1
Syntax error: unrecognized identifier '['.
```

### 混合运算

支持在expression中输入常数（甚至可以当作实数的四则运算计算器），支持+-*/%，求导（后缀!），求逆（后缀'），定积分（前缀$）。有完善的错误检测功能。其他运算未提供支持，包括expression中的负运算（前缀-）。

优先级：! = ' > $ > * = / = % > + = -

输入格式：`<expression>`

```
>>> f=1 2 1
>>> g=-0
>>> h=.5
>>> f
x^2 + 2x + 1
>>> f!'-$[0,1]$[1.5,-0.1](.5*g)
 - 0.5x + 0.5
>>> 1'-(2.3/1.15-1%8*.69!)
 - 1
>>> f%g
Math error: divided by Zero.
>>> f=1 2 3 0
>>> f'
Math error: polynominal 'f' doesn't have an inverse element.
>>> a=f+0.0*t
Symbolic error: undefined identifier 't'.
>>> a!/f
Symbolic error: undefined identifier 'a'.
>>> f+()*g
Syntax error: empty brackets '()'.
>>> f$[0,1]g
Syntax error: illegal usage of operator '$'.
>>> (f+g)*h)
Syntax error: bracket ')' is not balanced.
>>> $[0,1,1]f
Syntax error: '[' is not followed by a pair of comma-seperated numbers.
>>> $3,2]f
Syntax error: '$' is not followed by a range.
>>> 2f4+3
Symbolic error: mixed number and identifier '2f4'.
```

### 求根

输入格式：`0=<expression>`

默认初始值x0=1.1，若不能找到根则要求用户输入x0的值，再次运算，仍找不到根则报错。

```
>>> f=1 2 3 -4
>>> 0=f
x = 0.776045
>>> g=f
>>> 0=f-g!+.0
x = 2.50976
>>> f=1 2 1
>>> 0=f
x = -1
>>> f=1 1 1
>>> 0=f
Input a number as x0: 2
Math error: cannot find a solution with given beginning value.
```

### 其他命令

`show` 打印所有输入的多项式

`exit` 退出程序

```
>>> a=1 2
>>> x=0
>>> show
a = x + 2
x = 0
>>> exit
```

