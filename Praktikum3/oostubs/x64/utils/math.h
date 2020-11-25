/*! \file
 *  \brief Enthält die Klasse Math
 */

#pragma once

#include "types.h"

/*! \brief Die Klasse Math implementiert mathematische Hilfsfunktionen.
 */
namespace Math
{
	template <typename T>
	T abs(T a){
		return (a >= 0 ? a : -a);
	}

	template <typename T>
	T min(T a, T b)
	{
		return a > b ? b : a;
	}

	template <typename T>
	T max(T a, T b)
	{
		return a > b ? a : b;
	}

};
