// 官方建议：define PY_SSIZE_T_CLEAN before including Python.h
#define PY_SSIZE_T_CLEAN
// 由于Python.h中包含一些预处理，所以最好把这个include写在最前面
#include <Python.h>

/**
  定义C函数 PyObject* (PyObject*, PyObject*)=>
  注册函数 PyMethodDef
  注册模块 PyModuleDef
  创建模块构造函数 PyInit_${module_name}
*/

static PyObject *SpamError;

// 使用PyArg_ParseTuple/PyArg_ParseTupleAndKeywords转换python传入的对象
// 使用PyLong_FromLong/Py_BuildValue转化为python对象传入python
static PyObject* spam_system(PyObject *self, PyObject *args)
{
	const char *command;
	int sts;

	if (!PyArg_ParseTuple(args, "s", &command))
		return NULL;
	sts = system(command);
	if (sts < 0) {
		PyErr_SetString(SpamError, "System command failed");
		return NULL;
	}

	/*
	如果C函数返回值时void，那么需要如下返回形式（返回None）
		Py_RETURN_NONE;
	或
		Py_INCREF(Py_None);
		return Py_None;
	 */
	return PyLong_FromLong(sts);
}


static PyMethodDef SpamMethods[] = {
	// METH_VARARGS. 一种标志，告诉解释器这将用于c函数的调用
	// 如果传递的是可以通过 PyArg_ParseTuple处理的tuple(args)类型的python参数，则使用METH_VARARGS
	// 如果传递的是可以通过PyArg_ParseTupleAndKeywords处理的kwargs类型的python参数，则使用METH_KEYWORDS
	{"system", spam_system, METH_VARARGS, "execute a shell command."},
	{NULL, NULL, 0, NULL}
};

static PyModuleDef spammodule = {
	PyModuleDef_HEAD_INIT,
	"spam",								// module name
	"spam: execute a shell command",	// doc, can be NULL
	-1,									// size of per-interpreter state of the module,
				                        // or -1 if the module keeps state in global variables.
	SpamMethods
};

// 模块的初始化函数，必须名为PyInit_${module_name}
// PyMODINIT_FUNC: define PyMODINIT_FUNC extern "C" __declspec(dllexport) PyObject*
// 该函数在打开python时不会自动调用，如果想要创建自动调用的内建函数，
// 需使用PyImport_AppendInittab()添加到PyImport_Inittab表中
PyMODINIT_FUNC
PyInit_spam(void)
{
	PyObject *m;

	m = PyModule_Create(&spammodule);
	if (m == NULL)
		return NULL;

	SpamError = PyErr_NewException("spam.error", NULL, NULL);
	Py_XINCREF(SpamError);
	if (PyModule_AddObject(m, "error", SpamError) < 0) {
		Py_XDECREF(SpamError);
		Py_CLEAR(SpamError);
		Py_DECREF(m);
		return NULL;
	}

	return m;
}