#include "stdafx.h"
#include "sqlite3.h"
#include<stdlib.h>
/*sonme function about deal addresslist*/
void fun_add(sqlite3 *db);
void fun_find(sqlite3 *db);
void fun_del(sqlite3 *db);
void fun_all(sqlite3 *db);

int _tmain(int argc, _TCHAR* argv[])
{
	sqlite3 * db;
	char * pErr,ch;
	int x=1;
	int rc = sqlite3_open("AddressList.db",&db);
	if(rc != SQLITE_OK)
	{
		printf("Open database error\n");
		system("pause");
		return -2;
	}
	const char * sql="CREATE TABLE IF NOT EXISTS AddressList( \
					 per_id INTEGER PRIMARY KEY, \
					 per_name VARCHAR(16) NOT NULL, \
					 per_sex VARCHAR(2), \
					 per_number VARCHAR(20), \
					 per_email VARCHAR(256), \
					 per_address VARCHAR(256),\
					 per_zipcode VARCHAR(20));";
	rc=sqlite3_exec(db,sql,NULL,NULL,&pErr);
	if(rc!=SQLITE_OK)
	{
		printf("creat table failed\n");
		system("pause");
		return -1;
	}
	while(x)
	{
		printf("\n1:添加联系人     2:查找联系人    \n3:删除联系人     4:全部输出     0:退出\n请输入相应选项前的序号，按回车结束:");
		while(1!=scanf("%d",&x))		//--------------------------------------检查程序鲁棒性，如果输入错误则弹出提示文字，并重新输入
	{
		while((ch=getchar())!='\n')
			putchar(ch);
		printf("不是一个合法的输入，请验证后重新输入：");
	}
		switch (x)
		{
		case 1:
			fun_add(db);break;
		case 2:
			fun_find(db);break;
		case 3:
			fun_del(db);break;
		case 4:
			fun_all(db);break;
		case 0:
			x=0;
			printf("bay~");break;
		default:printf("input error,please check out,and try again.\n");
					system ("pause");
					x=10;break;
		}
	}
	sqlite3_close(db);
	return 0;
}

void fun_add(sqlite3 *db)
{
	while (getchar()!='\n');
	char Name[16],Sex[8],Num[32],Email[256],Address[256],Zipcode[32];
	printf("please input name:");
	gets(Name);
	printf("please input sex:");
	gets(Sex);
	printf("please input phone number:");
	gets(Num);
	printf("please input e-mail:");
	gets(Email);
	printf("please inout address:");
	gets(Address);
	printf("please input zipcode:");
	gets(Zipcode);
	char sql[512];
	sprintf(sql,"INSERT INTO AddressList VALUES (null,'%s','%s','%s','%s','%s','%s');",Name,Sex,Num,Email,Address,Zipcode);  //null是编号，自动生成，传入空即可
	int rc=sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(rc!=SQLITE_OK)
	{
		printf("error\n");
	}
}

void fun_find(sqlite3 *db)
{
	char sql[256];
	int per_num;
	sqlite3_stmt * stmt;
	printf("请输入要查询的联系人的编号：\n");
	scanf("%d",&per_num);
	sprintf(sql,"select * from AddressList where per_id='%d'",per_num);
	sqlite3_prepare(db,sql,-1,&stmt,NULL);   //第三个参数我写的是-1，这个参数含义是前面 sql 语句的长度。如果小于0，sqlite会自动计算它的长度（把sql语句当成以/0结尾的字符串）。
	if(sqlite3_step(stmt)==SQLITE_ROW)
	{
		printf("编号:%d\n姓名:%s\n电话:%s\ne-mail:%s\n地址:%s\n邮编:%s\n",
			sqlite3_column_int(stmt,0),sqlite3_column_text(stmt,1),sqlite3_column_text(stmt,2),
			sqlite3_column_text(stmt, 3), sqlite3_column_text(stmt, 4), sqlite3_column_text(stmt, 5), sqlite3_column_text(stmt, 6));
	}
	else
	{
		printf("输入错误\n");
	}
	sqlite3_finalize(stmt);
}

void fun_del(sqlite3*db)
{
	int n;
	char sql[256];
	printf("请输入要删除的联系人编号：\n");
	scanf("%d",&n);
	sprintf(sql,"delete from AddressList where per_id=%d",n);
	int rc=sqlite3_exec(db,sql,NULL,NULL,NULL);
	if(SQLITE_OK==rc)
	{
		printf("执行成功\n");
	}
	else
	{
		printf("删除失败\n");
	}
}

void fun_all(sqlite3 * db)
{
	sqlite3_stmt * stmt;
	sqlite3_prepare(db,"select *from AddressList",-1,&stmt,NULL);    
	while(sqlite3_step(stmt)==SQLITE_ROW)
	{
		printf("编号:%d\n姓名:%s\n性别:%s\n电话:%s\ne-mail:%s\n地址:%s\n邮编:%s\n\n",
			sqlite3_column_int(stmt,0),sqlite3_column_text(stmt,1),sqlite3_column_text(stmt,2),
			sqlite3_column_text(stmt, 3), sqlite3_column_text(stmt, 4), sqlite3_column_text(stmt, 5), sqlite3_column_text(stmt, 6));
	}
	sqlite3_finalize(stmt);
}