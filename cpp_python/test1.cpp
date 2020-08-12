#include <iostream>
#include <Python.h>
#include<string>
using namespace std;

/*
!!!!! attention: C++调用python时，如果python代码有问题，C++不会报错,而是直接结束程序
*/

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
	// 如果运行报错找不到encodings,需设置使用的python.exe的路径
	// Py_SetPythonHome(L"/home/hui/ide/miniconda3//envs/torch101/bin/"); 
   // Py_SetProgramName(program);  /* optional but recommended */
	
	Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化
	myassert(Py_IsInitialized(), "Initialized failed!");

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");//修改搜索python脚本路径
	PyObject * pModule = PyImport_ImportModule("test1");// import test1
	myassert(pModule != NULL, "test1 not found or got error while import test1");

	// 0. 无参数 无输出 test.do_nothing()
	PyObject * pFunc0 = PyObject_GetAttrString(pModule, "do_nothing");//这里是要调用的函数名
	myassert(pFunc0 != NULL, "test1.do_nothing() not found!");
	PyEval_CallObject(pFunc0, NULL);
	Py_DECREF(pFunc0);

	int ok;
	// 1.调用多个int输入，单个int输出的函数 res = add(10, 12)
	PyObject * pFunc1 = PyObject_GetAttrString(pModule, "add");//这里是要调用的函数名
	myassert(pFunc1 != NULL, "test1.add() not found!");
	PyObject* args1 = Py_BuildValue("ii", 10, 12); // 给python函数创建参数赋值,i表示int,ii表示两个int
	PyObject* pRet1 = PyEval_CallObject(pFunc1, args1);
	int res1;
	ok = PyArg_Parse(pRet1, "i", &res1);///转换返回类型
	myassert(ok, "parse return result error");
	cout << "res: " << res1 << endl;
	Py_DECREF(pFunc1);
	Py_DECREF(args1);
	Py_DECREF(pRet1);

	// 2.调用多个输入，多个输出的函数  res_1, res_2 = add_sub(10, 12)
	PyObject * pFunc2 = PyObject_GetAttrString(pModule, "add_sub");//这里是要调用的函数名
	myassert(pFunc2 != NULL, "test1.add_sub() not found!");
	PyObject* args2 = Py_BuildValue("ii", 10, 12); // 给python函数创建参数赋值
	PyObject* pRet2 = PyEval_CallObject(pFunc2, args2);
	int res2_1, res2_2;
	ok = PyArg_ParseTuple(pRet2, "ii", &res2_1, &res2_2);///转换返回类型
	myassert(ok, "parse return result error");
	cout << "res: (" << res2_1 << "," << res2_2 << ")" << endl;
	Py_DECREF(pFunc2);
	Py_DECREF(args2);
	Py_DECREF(pRet2);

	// 3. list_object = haha_merge_list(a, b)
	PyObject * pFunc3 = PyObject_GetAttrString(pModule, "haha_merge_list");//这里是要调用的函数名
	myassert(pFunc3 != NULL, "test1.haha_merge_list() not found!");
	PyObject* args3 = Py_BuildValue("(ii)[iii]", 10, 12, 1, 2, 3); // 给python函数创建参数赋值,(ii)(iii)表示有两个参数，每个参数是包含2个/3个int的tuple/list, ()/[]可以嵌套，也就是说可以输入多级tuple/list
	PyObject* pRet3 = PyEval_CallObject(pFunc3, args3);
	PyObject *pList;
	ok = PyArg_Parse(pRet3, "O!", &PyList_Type, &pList); /// separately, the & in &PyList_Type is super important!, O表示是一个python object，!表示如果转化失败会报错
	myassert(ok, "return result must be a list.");
	//    PyErr_SetString(PyExc_TypeError, "parameter must be a list.");
	Py_DECREF(pFunc3);
	Py_DECREF(args3);
	   // cout returned list
	Py_ssize_t n = PyList_Size(pList);
	int i;
	PyObject *pItem;
	cout << "[";
	for (i = 0; i < n; i++) {
		pItem = PyList_GetItem(pList, i);
		if (PyLong_Check(pItem)) {
			int data;
			PyArg_Parse(pItem, "i", &data);
			cout << data << ",";
		}
		else if (PyBytes_Check(pItem)) {
			char* data;
			PyArg_Parse(pItem, "s", &data);
			cout << data << ",";
		}
		else if (PyUnicode_Check(pItem)) {
			char* data;
			PyArg_Parse(pItem, "s", &data);
			cout << data << ",";
		}
		else {
			PyTypeObject* type = Py_TYPE(pItem);
			cout << type->tp_name << endl;
			Py_DECREF(type);
			PyErr_SetString(PyExc_TypeError, "element must be int or string.");
			cout << "element must be int or string." << endl;
			exit(-1);
		}
	}
	cout << "]" << endl;
	Py_DECREF(pList);
	Py_DECREF(pRet3);

	Py_DECREF(pModule);
	Py_Finalize(); // 与初始化对应
	return 0;
}