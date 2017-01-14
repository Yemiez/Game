#pragma once
#include "component.h"

namespace osharp { namespace gui { namespace components {

	template<typename _Codec = cvt::codec_cvt<>>
	class label : public component<_Codec>
	{
	public:
		using codec = _Codec;
		using base = component<_Codec>;
		using string_type = typename base::string_type;

	public:
		label( )
		{}


	};

}}}