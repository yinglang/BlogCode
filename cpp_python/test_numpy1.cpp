#include <iostream>
#include <Python.h>
#include<string>
using namespace std;

// 如果value不为true，则打印错误信息，同时结束程序
void myassert(bool value, const char* error_string){
	if(!value){
		printf("%s", error_string);
		printf("\n");
		exit(-1);
	}
}

int main()
{
	Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化
	myassert(Py_IsInitialized(), "Initialized failed!");

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");//这一步很重要，修改Python路径

	PyObject * pModule = NULL;//声明变量
	pModule = PyImport_ImportModule("test_numpy1");//这里是要调用的文件名hello.py
	myassert(pModule != NULL, "test_numpy1 not found or got error while import test_numpy1");
 
   int ok;
   // 1.调用多个输入，单个输出的函数 res = add(10, 12)
   PyObject * pFunc1 = PyObject_GetAttrString(pModule, "inner_product");//这里是要调用的函数名
   myassert(pFunc1 != NULL, "test_numpy1.inner_product() not found!");
   PyObject* args1 = Py_BuildValue("((iiii)(iiii))", 1, 2, 3, 4, 2, 3, 4, 5); // 给python函数创建参数赋值
	PyObject* pRet1 = PyEval_CallObject(pFunc1, args1);
   int res1;
   ok = PyArg_Parse(pRet1, "i", &res1);///转换返回类型
	myassert(ok, "parse return result error");
   cout << "res: " << res1 << endl;
   Py_DECREF(pFunc1);
   Py_DECREF(args1);
   Py_DECREF(pRet1);
   
   Py_DECREF(pModule);
	Py_Finalize(); // 与初始化对应
	return 0;
}