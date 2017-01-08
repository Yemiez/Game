#pragma once
#include "component.h"

namespace osharp { namespace gui { namespace components {

	template<typename _Codec = cvt::codec_cvt<>>
	class label : public component<_Codec>
	{

	};

}}}