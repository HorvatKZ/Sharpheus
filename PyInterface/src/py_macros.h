#pragma once

#define SPH_PYINT_CLASS_BEGIN(Class) py::class_<Class>(handle, #Class)
#define SPH_PYINT_CLASS_END() ;

#define SPH_PYINT_CONSTR_0() .def(py::init<>())
#define SPH_PYINT_CONSTR_1(type1, arg1) .def(py::init<type1>(), arg1)
#define SPH_PYINT_CONSTR_2(type1, arg1, type2, arg2) .def(py::init<type1, type2>(), arg1, arg2)

#define SPH_PYINT_FUNC_0(Class, Func) .def(#Func, &Class::Func)
#define SPH_PYINT_FUNC_1(Class, Func, arg1) .def(#Func, &Class::Func, arg1)
#define SPH_PYINT_FUNC_2(Class, Func, arg1, arg2) .def(#Func, &Class::Func, arg1, arg2)

#define SPH_PYINT_FUNC_STAT_0(Class, Func) .def_static(#Func, &Class::Func)
#define SPH_PYINT_FUNC_STAT_1(Class, Func, arg1) .def_static(#Func, &Class::Func, arg1)
#define SPH_PYINT_FUNC_STAT_2(Class, Func, arg1, arg2) .def_static(#Func, &Class::Func, arg1, arg2)

#define SPH_PYINT_OP_SELF_SELF(Operator) .def(py::self Operator py::self)
#define SPH_PYINT_OP_SELF_ARG(Operator, arg) .def(py::self Operator arg())
#define SPH_PYINT_OP_ARG_SELF(Operator, arg) .def(arg() Operator py::self)


#define SPH_PYINT_EDIT_DM(Class, DataMember) .def_readwrite(#DataMember, &Class::DataMember)
#define SPH_PYINT_CONST_DM(Class, DataMember) .def_readonly(#DataMember, &Class::DataMember)
#define SPH_PYINT_EDIT_STAT_DM(Class, DataMember) .def_readwrite_static(#DataMember, &Class::DataMember)
#define SPH_PYINT_CONST_STAT_DM(Class, DataMember) .def_readonly_static(#DataMember, &Class::DataMember)