#pragma once
#include "ybf/ascii.h"
#include "ybf/bundle.h"
#include "ybf/double.h"
#include "ybf/float.h"
#include "ybf/header.h"
#include "ybf/int.h"
#include "ybf/long.h"
#include "ybf/short.h"
#include "ybf/uint.h"
#include "ybf/ulong.h"
#include "ybf/byte_array.h"

namespace ybf
{
	template<typename T, typename...Args>
	static ybf_ptr to( Args&&...args )
	{
		return std::make_shared<T>( std::forward<Args>( args )... );
	}

	extern ybf_ptr import( ybf::ybf_header &header );

}