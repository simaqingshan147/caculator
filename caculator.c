#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct nstack {
	int* num;
	int top;
}*numstack, ns;  //�洢���ֵ�ջ���沨�����ʽ��ֵ
typedef struct mstack {
	char mark[50];
	int top;
}*markstack, ms;  //�洢���ŵ�ջ������ƥ��

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
	puts("��������ʽ(��#�˳�):");
	s = 0;
	s_gets(equation[s], 4);
	while (strcmp(equation[s], "#") && s < 50)
	{
		s++;
		puts("������(��#�˳�):");
		s_gets(equation[s], 4);
	}
	puts("�������!");
	if (JudgeEquation(equation, s))
	{
		int length = topolish(equation, s, poequation);
		int answer = polish(poequation, length);
		printf("����%d\n", answer);
	}
	else
		puts("���ʽ����!");

	return 0;
}

//�����׺���ʽ
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
			//û����������ӵ���
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
			//û���������������
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
			//û����������˵���
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
			//û���������������
			if (nstack->top < 0)
				return 0;
			else
			{
				nstack->num[nstack->top - 1] /= nstack->num[nstack->top];
				nstack->top--;
			}
		}
		else  //Ϊ����
		{
			int n = atoi(equation[pos]);
			nstack->num[++nstack->top] = n;
		}
		pos++;
	}
	return nstack->num[nstack->top];
}

//�ַ���������
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

//����׺���ʽת��Ϊ��׺���ʽ
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
				//�Ӻ����ȼ���ͣ�����������ȫ������
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
				//�������ȼ���ͣ�����������ȫ������
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
		else//����
			strcpy(poequation[start++],equation[pos]);
		pos++;
	}
	while (top > -1) //����ʣ�����
		strcpy(poequation[start++], markstack[top--]);

	return start;
}

//�жϱ��ʽ�����Ƿ�ƥ��
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
				puts("���\")\"");
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
					puts("������Ų������");
					return 0;
				}
			}
		}
		else if (!strcmp(equation[pos], "]"))
		{
			if (stack->top == -1)
			{
				puts("���\"]\"");
				return 0;
			}
			else
			{
				if (stack->mark[stack->top--] == '[')
					pos++;
				else
				{
					puts("������Ų������");
					return 0;
				}
			}
		}
		else if (!strcmp(equation[pos], "}"))
		{
			if (stack->top == -1)
			{
				puts("���\"}\"");
				return 0;
			}
			else
			{
				if (stack->mark[stack->top--] == '{')
					pos++;
				else
				{
					puts("������Ų������");
					return 0;
				}
			}
		}
		else //Ϊ���ֻ����
			pos++;
	}
	if (stack->top != -1) //�ж�������
	{
		puts("�����Ŷ���������!");
		return 0;
	}
	return 1;
}
