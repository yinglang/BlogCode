// 官方建议：define PY_SSIZE_T_CLEAN before including Python.h
#define PY_SSIZE_T_CLEAN
// 由于Python.h中包含一些预处理，所以最好把这个include写在最前面
#include <Python.h>
#include <numpy/arrayobject.h>
#include <math.h>

static PyObject *SpamError;

// 使用PyArg_ParseTuple/PyArg_ParseTupleAndKeywords转换python传入的对象
// 使用PyLong_FromLong/Py_BuildValue转化为python对象传入python
static PyObject* cos_func_np(PyObject *self, PyObject *args)
{
	// 注意在cpp里面调用的printf在python调用时是打印不出来的,在ipython调用时候会在ipython退出时打印
	printf("hello world\n");
	PyArrayObject *in_array;
	if (!PyArg_ParseTuple(args, "O!", &PyArray_Type, &in_array)) {
		PyErr_SetString(SpamError, "parse args fialed");
		return NULL;
	}

	PyObject* out_array = PyArray_NewLikeArray(in_array, NPY_ANYORDER, NULL, 0);
	if (out_array == NULL) {
		PyErr_SetString(SpamError, "malloc memory for output failed");
		return NULL;
	}

	PyArrayIterObject *in_iter = (PyArrayIterObject*)PyArray_IterNew((PyObject*)in_array);
	PyArrayIterObject *out_iter = (PyArrayIterObject*)PyArray_IterNew((PyObject*)out_array);
	if (in_iter == NULL || out_iter == NULL) {
		PyErr_SetString(SpamError, "create iter failed.");
		Py_XDECREF(out_array);
		Py_XDECREF(in_iter);
		Py_XDECREF(out_iter);
		return NULL;
	}

	// while loop
	while (in_iter->index < in_iter->size &&
		out_iter->index < out_iter->size) {
		double* in_dataptr = (double*)in_iter->dataptr;
		double* out_dataptr = (double*)out_iter->dataptr;

		*out_dataptr = cos(*in_dataptr);

		PyArray_ITER_NEXT(in_iter);
		PyArray_ITER_NEXT(out_iter);
	}

	Py_DECREF(in_iter);
	Py_DECREF(out_iter);
// 	Py_DECREF(out_array); // 段错误 (核心已转储)，由于out_array是要返回的，所以这里不能释放了
	return out_array;
}


static PyMethodDef CosNumpyMethods[] = {
	// METH_VARARGS. 一种标志，告诉解释器这将用于c函数的调用
	// 如果传递的是可以通过 PyArg_ParseTuple处理的tuple(args)类型的python参数，则使用METH_VARARGS
	// 如果传递的是可以通过PyArg_ParseTupleAndKeywords处理的kwargs类型的python参数，则使用METH_KEYWORDS
	{"cos_func_np", cos_func_np, METH_VARARGS, "do cos for numpy.array."},
	{NULL, NULL, 0, NULL}
};


static PyModuleDef CosNumpymodule = {
	PyModuleDef_HEAD_INIT,
	"CosNumpy",								// module name
	"CosNumpy: do cos for numpy.array",	// doc, can be NULL
	-1,									// size of per-interpreter state of the module,
										// or -1 if the module keeps state in global variables.
	CosNumpyMethods
};

// 模块的初始化函数，必须名为PyInit_${module_name}
// PyMODINIT_FUNC: define PyMODINIT_FUNC extern "C" __declspec(dllexport) PyObject*
// 该函数在打开python时不会自动调用，如果想要创建自动调用的内建函数，
// 需使用PyImport_AppendInittab()添加到PyImport_Inittab表中
PyMODINIT_FUNC
	PyInit_CosNumpy(void)
{
   // 使用numpy.array 千万不要忘了这句
	import_array();
   
	PyObject *m;
	m = PyModule_Create(&CosNumpymodule);
	if (m == NULL)
		return NULL;

	SpamError = PyErr_NewException("CosNumpy.error", NULL, NULL);
	Py_XINCREF(SpamError);
	if (PyModule_AddObject(m, "error", SpamError) < 0) {
		Py_XDECREF(SpamError);
		Py_CLEAR(SpamError);
		Py_DECREF(m);
		return NULL;
	}
	return m;
}
