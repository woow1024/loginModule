#pragma once

template <class _Pair>
struct _Select1st : public unary_function<_Pair, typename _Pair::first_type> {
	const typename _Pair::first_type& operator()(const _Pair& __x) const {
		return __x.first;
	}
};

// From SGI STL
template <class _Pair>
struct _Select2nd : public unary_function<_Pair, typename _Pair::second_type>
{
	const typename _Pair::second_type& operator()(const _Pair& __x) const {
		return __x.second;
	}
};

template <class _Pair> struct select1st : public _Select1st<_Pair> {};
template <class _Pair> struct select2nd : public _Select2nd<_Pair> {};
