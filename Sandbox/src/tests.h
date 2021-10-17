##pragma once

#include <stdexcept>

//class BindingData
//{
//	bool bind(int object)
//	{
//		if (active_binders == 0)
//		{
//			active_binders++;
//
//			if (bound_object == object)
//			{
//				return false;
//			}
//
//			bound_object = object;
//			return true;
//		}
//
//		if (bound_object == object)
//		{
//			active_binders++;
//			return false;
//		}
//
//		throw std::runtime_error("object already bound");
//	}
//
//	bool unbind()
//	{
//		if (active_binders != 0)
//		{
//			active_binders--;
//
//			if (active_binders == 0)
//			{
//				return true;
//			}
//		}
//
//		return false;
//	}
//
//	int bound_object = 0;
//	int active_binders = 0;
//};