/*
* 目的：利用bloomfilter实现天池方案1.
* 时间：2015/4/6
*/
/***************************方案*********************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include "bloomfilter.h"

const int MAXSIZE = 50000000; 
int main2(int argc, char **argv)
{
	char *poolfile = argv[1];
	char *testfile = argv[2];
	cout << poolfile << endl;
	ifstream fpool(poolfile);
	ifstream ftest(testfile);
	if(!fpool)
	{
		cout << "cannot open the file:" << poolfile << endl;
		return 0;
	}
	if(!ftest)
	{
		cout << "cannot open the file:" << testfile << endl;
		return 0;
	}
	BloomFilter bf(MAXSIZE);
	bf.setBit(fpool);
	cout << "Store OK" << endl;
	bf.checkBit(ftest);
	cout << "Check OK" << endl;
	fpool.close();
	ftest.close();
	return 0;
}

void product_id_into_set(set<string> &res, ifstream &f)
{
	string pro;
	int index;
	while(getline(f, pro))
	{
		index = pro.find_first_of(',');
		res.insert(pro.substr(0,index));
	}
	cout<<"商品读入完毕 大小："<<res.size()<<endl;
}

void my_model_01(set<string> &product, set<string> &user, BloomFilter &bf, ofstream &of)
{
	set<string>::iterator it;
	set<string>::iterator jt;
	string temp, s1,s2,s3,s4;
	int i=0;
	for(it=user.begin();it!=user.end();it++)
	{
		for(jt=product.begin();jt!=product.end();jt++)
		{
			temp = "";
			temp.append(*it).append(",").append(*jt);//中间有没有逗号很重要
			//
			s1 = temp + ",1";
			s2 = temp + ",2";
			s3 = temp + ",3";
			s4 = temp + ",4";
			if(bf.checkBit(s1)&&bf.checkBit(s3))
			{
				of<<temp<<endl;
			}
		}
		//
		if(++i%100==0)
		{
			cout<<"用户"<<*it<<"遍历完"<<i<<endl;
		}
		//cout<<"商品"<<*it<<"遍历完"<<endl;
	}
	//
	of.close();
}

void main3()
{
	//读入商品 141365535,99ctdtd,3487
	string product = "tianchi_mobile_recommend_train_item.csv";
	set<string> product_id;
	ifstream product_file(product);
	if(!product_file)
	{
		cout<<"Open file "<<product<<" error!"<<endl;
	}
	product_id_into_set(product_id, product_file);
	product_file.close();
	//读入用户信息
	string file1 = "tianchi_mobile_recommend_train_user.csv";
	set<string> user_id;
	ifstream fpool(file1);
	if(!fpool)
	{
		cout<<"Open file "<<file1<<" error!"<<endl;
	}
	BloomFilter bf(MAXSIZE);
	bf.lyb_setBit(fpool, user_id);
	cout << "Store Ok" << endl;
	cout << "用户集合的大小为 " << user_id.size() << endl;
	fpool.close();
	//进行遍历查询----数据结构常驻内存
	ofstream of("resutl.txt");
	my_model_01(product_id, user_id, bf, of);
	system("pause");
}

void testBloomFilter()
{
	BloomFilter bf(5);
	int i = 3;
	stringstream s;
	while(i-->0)
	{
		string temp;
		s<<i;
		s>>temp;
		bf.setBit(temp.append("hello"));
	}
	i = 3;
	while(i-->0)
	{
		string temp;
		s<<i;
		s>>temp;
		bool in = bf.checkBit(temp.append("hello"));
		cout<<in<<endl;
	}
}
int main()
{
	main3();
	return 0;
}
