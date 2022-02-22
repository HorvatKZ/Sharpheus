#include "py_pch.h"

#define SPH_PY_Point \
	SPH_PYINT_CLASS_BEGIN(Point) \
		\
		SPH_PYINT_EDIT_DM(Point, x) \
		SPH_PYINT_EDIT_DM(Point, y) \
		\
		SPH_PYINT_CONSTR_2(float, "x"_a = 0.f, float, "y"_a = 0.f) \
		\
		SPH_PYINT_OP_SELF_SELF(+) \
		SPH_PYINT_OP_SELF_SELF(-) \
		SPH_PYINT_OP_SELF_SELF(*) \
		SPH_PYINT_OP_SELF_ARG(*, float) \
		SPH_PYINT_OP_ARG_SELF(*, float) \
		SPH_PYINT_OP_SELF_ARG(/, float) \
		SPH_PYINT_OP_ARG_SELF(/, float) \
		SPH_PYINT_OP_SELF_SELF(+=) \
		SPH_PYINT_OP_SELF_SELF(-=) \
		SPH_PYINT_OP_SELF_ARG(*=, float) \
		SPH_PYINT_OP_SELF_ARG(/=, float) \
		SPH_PYINT_OP_SELF_SELF(==) \
		SPH_PYINT_OP_SELF_SELF(!=) \
		\
		SPH_PYINT_FUNC_0(Point, Length) \
		SPH_PYINT_FUNC_0(Point, LengthSquared) \
		SPH_PYINT_FUNC_1(Point, Distance, "other"_a) \
		SPH_PYINT_FUNC_1(Point, DistanceSquared, "other"_a) \
		SPH_PYINT_FUNC_0(Point, Normalize) \
		\
		SPH_PYINT_FUNC_1(Point, Rotate, "angle"_a) \
		SPH_PYINT_FUNC_0(Point, GetAngle) \
		SPH_PYINT_FUNC_1(Point, GetAngleWith, "other"_a) \
		\
		SPH_PYINT_CONST_STAT_DM(Point, Zero) \
		SPH_PYINT_CONST_STAT_DM(Point, Up) \
		SPH_PYINT_CONST_STAT_DM(Point, Down) \
		SPH_PYINT_CONST_STAT_DM(Point, Left) \
		SPH_PYINT_CONST_STAT_DM(Point, Right) \
		\
		SPH_PYINT_FUNC_STAT_1(Point, GetUnit, "angle"_a) \
		\
	SPH_PYINT_CLASS_END()


namespace py = pybind11;

using namespace pybind11::literals;

namespace Sharpheus {
	PYBIND11_MODULE(Sharpheus, handle) {
		handle.doc() = "Sharpheus Game Engine Python Interface [using pybind11]";

		SPH_PY_Point
	}
}
