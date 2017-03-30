#pragma once

namespace op {

	template <typename main_t, typename callback_t = void> class Assignment {
	public:
		//*value = p0; Callback_o
		Assignment<main_t, callback_t> const &operator=(main_t const &p0) const;
		//return(*value), callback_c
		operator main_t &() const;

		void(callback_t::*callback_o)(main_t const &p0);	//Operator callback
		void(callback_t::*callback_c)();					//Cast callback

		explicit Assignment(main_t *const nvalue, callback_t *const ncallback_p, void(callback_t::*ncallback_o)(main_t const &p0) = nullptr, void(callback_t::*ncallback_c)() = nullptr);

		main_t *const value;
		callback_t *const callback_p;
	};

	template <typename main_t> class Assignment<main_t, void> {
	public:
		Assignment<main_t, void> const &operator=(main_t const &p0) const;
		operator main_t &() const;

		void(*callback_o)(main_t const &p0);
		void(*callback_c)();

		explicit Assignment(main_t *const nvalue, void(*ncallback_o)(main_t const &p0) = nullptr, void(*ncallback_c)() = nullptr);

		main_t *const value;
	};

}

template <typename main_t, typename callback_t> op::Assignment<main_t, callback_t> const &op::Assignment<main_t, callback_t>::operator=(main_t const &p0) const {
	if (callback_o != nullptr)
		(callback_p->*callback_o)(p0);
	return(*this);
}

template <typename main_t, typename callback_t> op::Assignment<main_t, callback_t>::operator main_t &() const {
	if (callback_c != nullptr)
		(callback_p->*callback_c)();
	return(*value);
}

template <typename main_t, typename callback_t> op::Assignment<main_t, callback_t>::Assignment(main_t *const nvalue, callback_t *const ncallback_p, void(callback_t::*ncallback_o)(main_t const &p0), void(callback_t::*ncallback_c)()) :
	value(nvalue), callback_p(ncallback_p), callback_o(ncallback_o), callback_c(ncallback_c) {
}

template <typename main_t> op::Assignment<main_t, void> const &op::Assignment<main_t, void>::operator=(main_t const &p0) const {
	if (callback_o != nullptr)
		(*callback_o)(p0);
	return(*this);
}

template <typename main_t> op::Assignment<main_t, void>::operator main_t &() const {
	if (callback_c != nullptr)
		(*callback_c)();
	return(*value);
}

template <typename main_t> op::Assignment<main_t, void>::Assignment(main_t *const nvalue, void(*ncallback_o)(main_t const &p0), void(*ncallback_c)()) :
	value(nvalue), callback_o(ncallback_o), callback_c(ncallback_c) {
}
