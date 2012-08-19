#pragma once

namespace boost {
	inline void intrusive_ptr_add_ref(IUnknown *unknown) { unknown->AddRef(); }
	inline void intrusive_ptr_release(IUnknown *unknown) { unknown->Release(); }
}