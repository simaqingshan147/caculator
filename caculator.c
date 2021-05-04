#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct nstack {
	int* num;
	int top;
}*numstack, ns;  //存储数字的栈，逆波兰表达式求值
typedef struct mstack {
	char mark[50];
	int top;
}*markstack, ms;  //存储符号的栈，括号匹配

int topolish(char equation[][4],int,char poequation[][4]);
int polish(char equation[][4], int);
int JudgeEquation(char equation[][4], int length);
char* s_gets(char* st, int n);
int main(void)
{
	int flag = 1;
	int s;
	char equation[50][4];
	char poequation[50][4];
	puts("请输入表达式(按#退出):");
	s = 0;
	s_gets(equation[s], 4);
	while (strcmp(equation[s], "#") && s < 50)
	{
		s++;
		puts("请输入(按#退出):");
		s_gets(equation[s], 4);
	}
	puts("输入完成!");
	if (JudgeEquation(equation, s))
	{
		int length = topolish(equation, s, poequation);
		int answer = polish(poequation, length);
		printf("答案是%d\n", answer);
	}
	else
		puts("表达式错误!");

	return 0;
}

//计算后缀表达式
int polish(char equation[][4], int length)
{
	numstack nstack = (numstack)malloc(sizeof(ns));
	nstack->top = -1;
	nstack->num = (int*)malloc(sizeof(int) * 100);
	int pos = 0;
	while (pos <length)
	{
		if (!strcmp(equation[pos],"+"))
		{
			//没有两个待相加的数
			if (nstack->top < 0)
				return 0;
			else
			{
				nstack->num[nstack->top - 1] += nstack->num[nstack->top];
				nstack->top--;
			}
		}
		else if (!strcmp(equation[pos],"-"))
		{
			//没有两个待相减的数
			if (nstack->top < 0)
				return 0;
			else
			{
				nstack->num[nstack->top - 1] -= nstack->num[nstack->top];
				nstack->top--;
			}
		}
		else if (!strcmp(equation[pos],"*"))
		{
			//没有两个待相乘的数
			if (nstack->top < 0)
				return 0;
			else
			{
				nstack->num[nstack->top - 1] *= nstack->num[nstack->top];
				nstack->top--;
			}
		}
		else if (!strcmp(equation[pos],"/"))
		{
			//没有两个待相除的数
			if (nstack->top < 0)
				return 0;
			else
			{
				nstack->num[nstack->top - 1] /= nstack->num[nstack->top];
				nstack->top--;
			}
		}
		else  //为数字
		{
			int n = atoi(equation[pos]);
			nstack->num[++nstack->top] = n;
		}
		pos++;
	}
	return nstack->num[nstack->top];
}

//字符串处理函数
char* s_gets(char* st, int n)
{
	char* ret_val;
	int i = 0;

	ret_val = fgets(st, n, stdin);
	if (ret_val)
	{
		while (st[i] != '\n' && st[i] != '\0')
			i++;
		if (st[i] == '\n')
			st[i] = '\0';
		else
		{
			while (getchar() != '\n')
				continue;
		}
	}
	return ret_val;
}

//将中缀表达式转化为后缀表达式
int topolish(char equation[][4], int length,char poequation[][4])
{
	int pos = 0, top = -1, start = 0;
	char markstack[50][2];
	while (pos < length)
	{
		if (!strcmp(equation[pos], "("))
			strcpy(markstack[++top], "(");
		else if (!strcmp(equation[pos], "+"))
		{
			if (top == -1)
				strcpy(markstack[++top], "+");
			else
			{
				//加号优先级最低，除左括号外全部弹出
				while (top > -1 && strcmp(markstack[top], "("))
					strcpy(poequation[start++], markstack[top--]);
				strcpy(markstack[++top], "+");
			}
		}
		else if (!strcmp(equation[pos], "-"))
		{
			if (top == -1)
				strcpy(markstack[++top], "-");
			else
			{
				//减号优先级最低，除左括号外全部弹出
				while (top > -1 && strcmp(markstack[top], "("))
					strcpy(poequation[start++], markstack[top--]);
				strcpy(markstack[++top], "-");
			}
		}
		else if (!strcmp(equation[pos], "*"))
		{
			if (top == -1)
				strcpy(markstack[++top], "*");
			else
			{
				while (top > -1 && !strcmp(markstack[top], "/"))
					strcpy(poequation[start++], markstack[top--]);
				strcpy(markstack[++top], "*");
			}
		}
		else if (!strcmp(equation[pos], "/"))
		{
			if (top == -1)
				strcpy(markstack[++top], "/");
			else
			{
				while (top > -1 && !strcmp(markstack[top], "*"))
					strcpy(poequation[start++], markstack[top--]);
				strcpy(markstack[++top], "/");
			}
		}
		else if (!strcmp(equation[pos], ")"))
		{
			while (top > -1 && strcmp(markstack[top], "("))
				strcpy(poequation[start++], markstack[top--]);
			top--;
		}
		else//数字
			strcpy(poequation[start++],equation[pos]);
		pos++;
	}
	while (top > -1) //弹出剩余符号
		strcpy(poequation[start++], markstack[top--]);

	return start;
}

//判断表达式括号是否匹配
int JudgeEquation(char equation[][4], int length)
{
	markstack stack = (markstack)malloc(sizeof(struct mstack));
	stack->top = -1;
	int pos = 0;
	while (pos < length)
	{
		if (!strcmp(equation[pos], "("))
		{
			stack->mark[++stack->top] = '(';
			pos++;
		}
		else if (!strcmp(equation[pos], "["))
		{
			stack->mark[++stack->top] = '[';
			pos++;
		}
		else if (!strcmp(equation[pos], "{"))
		{
			stack->mark[++stack->top] = '{';
			pos++;
		}
		else if (!strcmp(equation[pos], ")"))
		{
			if (stack->top == -1)
			{
				puts("多出\")\"");
				return 0;
			}
			else
			{
				if (stack->mark[stack->top--] == '(')
				{
					pos++;
					continue;
				}
				else
				{
					puts("输入符号不相符！");
					return 0;
				}
			}
		}
		else if (!strcmp(equation[pos], "]"))
		{
			if (stack->top == -1)
			{
				puts("多出\"]\"");
				return 0;
			}
			else
			{
				if (stack->mark[stack->top--] == '[')
					pos++;
				else
				{
					puts("输入符号不相符！");
					return 0;
				}
			}
		}
		else if (!strcmp(equation[pos], "}"))
		{
			if (stack->top == -1)
			{
				puts("多出\"}\"");
				return 0;
			}
			else
			{
				if (stack->mark[stack->top--] == '{')
					pos++;
				else
				{
					puts("输入符号不相符！");
					return 0;
				}
			}
		}
		else //为数字或符号
			pos++;
	}
	if (stack->top != -1) //有多余括号
	{
		puts("左括号多于右括号!");
		return 0;
	}
	return 1;
}
