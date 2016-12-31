#pragma once
#include "handle.h"
#include "../formatting/utf.h"

namespace osharp { namespace gui {

	enum DialogType : long
	{
		AbortRetryIgnore = 0x00000002L,
		CancelTryContinue = 0x00000006L,
		Help = 0x00004000L,
		OK = 0x00000000L,
		OKCancel = 0x00000001L,
		RetryCancel = 0x00000005L,
		YesNo = 0x00000004L,
		YesNoCancel = 0x00000003L,
		IconExclamation = 0x00000030L,
		IconWarning = IconExclamation,
		IconAsterisk = 0x00000040L,
		IconInformation = IconAsterisk,
		IconQuestion = 0x00000020L,
		IconHand = 0x00000010L,
		IconError = IconHand,
		IconStop = IconHand,
		RightText = 0x00080000L,
		RTLReading = 0x00100000L,
		SetForeground = 0x00010000L,
		Topmost = 0x00040000L,
		ServiceNotification = 0x00200000L
	};

	enum DialogResult
	{
		Ok = 1,
		Cancel = 2,
		Abort = 3,
		Retry = 4,
		Ignore = 5,
		Yes = 6,
		No = 7,
		TryAgain = 10,
		Continue = 11,
	};

	namespace abstractions
	{
		extern DialogResult MessageBoxW( handle parent,
										 std::wstring message,
										 std::wstring caption,
										 long type );
	}

	template<typename _Codec>
	class MsgBox
	{
	public:
		using string_type = typename _Codec::string_type;
		using converter = cvt::codec_cvt<formats::utf_w>;

	public:
		MsgBox( ) = delete;

	public:
		static DialogResult ShowDialog( const string_type &message,
										const string_type &caption,
										long type = YesNo )
		{
			return ShowDialog( nullptr,
							   message,
							   caption,
							   type );
		}

		static DialogResult ShowDialog( handle parent,
										const string_type &message,
										const string_type &caption,
										long type = YesNo )
		{
			return abstractions::MessageBoxW( parent,
											  converter::cvt( message ),
											  converter::cvt( caption ),
											  type );
		}

	};



} }