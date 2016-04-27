#include "containers.h"

#include <Python.h>

static PyObject* containerblock_getCredentials(PyObject *self, PyObject *args)
{
	const char *container_name;
	int status;
	struct CONTAINER_BLOCK cb;

	if (!PyArg_ParseTuple(args, "s", &container_name))
        return NULL;
	
	CONTAINER_BLOCK_read(&cb, container_name);
	return Py_BuildValue("ss", cb.container_username, cb.container_password);
}

static PyMethodDef containerblockMethods[] = {
    {"getCredentials", containerblock_getCredentials, METH_VARARGS,
     "Retrieves credentials from a secure container block"},
};

PyMODINIT_FUNC initcontainerblock(void)
{
    (void) Py_InitModule("containerblock", containerblockMethods);
}