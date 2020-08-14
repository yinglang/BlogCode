#include <iostream>
#include <Python.h>
#include<string>
#include <numpy/arrayobject.h>
using namespace std;

// 如果value不为true，则打印错误信息，同时结束程序
void myassert(bool value, const char* error_string){
	if(!value){
		printf("%s", error_string);
		printf("\n");
		exit(-1);
	}
}
/**
1.create data in cpp, pass to python, return to cpp.

import API:
import_array: load numpy api
PyArray_SimpleNewFromData(len_shape, shape, dtype, data_array): 将数据转化为PyArrayObject* (numpy.array)
PyArray_NewLikeArray
PyArray_SHAPE(PyArrayObject*): 获得shape
PyArray_DATA(ret_array): 获得flatten的一维数据指针
*/
int main()
{
	Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化
	myassert(Py_IsInitialized(), "Initialized failed!");
    
  import_array();		/* load numpy api */

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");//这一步很重要，修改Python路径

	PyObject * pModule = NULL;//声明变量
	pModule = PyImport_ImportModule("test_opencv");//这里是要调用的文件名hello.py
	myassert(pModule != NULL, "test_opencv not found or got error while import test_opencv");
 
   int ok;
   // 1.create data in cpp, pass to python, return to cpp.
   PyObject * pFunc1 = PyObject_GetAttrString(pModule, "scale_up");//这里是要调用的函数名
   myassert(pFunc1 != NULL, "test_opencv.scale_up() not found!");
   PyObject* args1 = PyTuple_New(2);
   /// turn cpp array to numpy PyObject
   unsigned char array[4][4][3] = {{{1, 1, 1}, {2, 2, 2}, {3, 3, 3}, {4, 4, 4}},
                   {{2, 2, 2}, {3, 3, 3}, {4, 4, 4}, {5, 5, 5}},
                   {{3, 3, 3}, {4, 4, 4}, {5, 5, 5}, {6, 6, 6}},
                   {{4, 4, 4}, {5, 5, 5}, {6, 6, 6}, {7, 7, 7}}};
   npy_intp dims[] = {4, 4, 3};
   PyObject* input = PyArray_SimpleNewFromData(3, dims, NPY_UINT8, array); // 输入的array可以是一维flatten的指针
   PyTuple_SetItem(args1, 0, input);
   PyTuple_SetItem(args1, 1, Py_BuildValue("i", 2));
	PyObject* pRet1 = PyEval_CallObject(pFunc1, args1);
   PyArrayObject *ret_array;
   ok = PyArray_OutputConverter(pRet1, &ret_array);
	myassert(ok, "parse return result error");
   npy_intp* shape = PyArray_SHAPE(ret_array);
   unsigned char* ret_array_data = (unsigned char*)PyArray_DATA(ret_array); // 返回获取的data是一维flatten的指针    
   cout << shape[0] << " " << shape[1] << " " << shape[2] << endl;
   for(int i = 0; i < 24; i++) cout << int(ret_array_data[i]) << ",";
   cout << endl;
   Py_DECREF(pFunc1);
   Py_DECREF(args1);
   Py_DECREF(pRet1);
   
   Py_DECREF(pModule);
	Py_Finalize(); // 与初始化对应
	return 0;
}