#ifndef __PERLY__
#define __PERLY__

namespace perly {

    // grep
    template<typename _In, typename _Out, typename _Pred> 
    _Out grep(_In __first, _In __last, _Out __result, _Pred __pred) {
	return std::remove_copy_if(__first, __last, __result, std::not1(__pred));
    }

    // map
    template<typename _In, typename _Out, typename _Unary>
    _Out map(_In __first, _In __last, _Out __result, _Unary __unary_op) {
	return std::transform(__first, __last, __result, __unary_op);
    }

    // List::Util
    //
    // first, max, min, reduce, sum
    template<typename _In, typename _Pred> 
    _In first(_In __first, _In __last, _Pred __pred) {
	return std::find_if(__first, __last, __pred);
    }

    template<typename _In>
    _In max(_In __first, _In __last) {
	return std::max_element(__first, __last);
    }

    template<typename _In>
    _In min(_In __first, _In __last) {
	return std::min_element(__first, __last);
    }

    template<typename _In, typename _T, typename _Binary>
    _T reduce(_In __first, _In __last, _T __nil, _Binary __binary_op) {
	_T __reduced = __nil;
	if (__first >= __last) return __reduced;
	for (__reduced = *__first++; __first != __last; ++__first) 
	    __reduced = __binary_op(__reduced, *__first);
	return __reduced;
    }

    template<typename _In, typename _T>
    _T sum(_In __first, _In __last, _T __init) {
	for (; __first != __last; ++__first)
	    __init += *__first;
	return __init;
    }

    // List::MoreUtils
    //
    // any, all, none, notall, after, after_incl, before, before_incl, pairwise, part
    // NOTE: true => trues, and false => falses
    template<typename _In, typename _Pred>
    bool any(_In __first, _In __last, _Pred __pred) {
	for (; __first != __last; ++__first)
	    if (__pred(*__first)) return true;
	return false;
    }

    template<typename _In, typename _Pred>
    bool all(_In __first, _In __last, _Pred __pred) {
	for (; __first != __last; ++__first)
	    if (!__pred(*__first)) return false;
	return true;
    }

    template<typename _In, typename _Pred>
    bool none(_In __first, _In __last, _Pred __pred) {
	for (; __first != __last; ++__first)
	    if (__pred(*__first)) return false;
	return true;
    }

    template<typename _In, typename _Pred>
    bool notall(_In __first, _In __last, _Pred __pred) {
	for (; __first != __last; ++__first)
	    if (!__pred(*__first)) return true;
	return false;
    }

    template<typename _In, typename _Pred>
    typename std::iterator_traits<_In>::difference_type
    trues(_In __first, _In __last, _Pred __pred) {
	typename std::iterator_traits<_In>::difference_type __n = 0;
	for (; __first != __last; ++__first)
	    if (__pred(*__first)) ++__n;
	return __n;
    }

    template<typename _In, typename _Pred>
    typename std::iterator_traits<_In>::difference_type
    falses(_In __first, _In __last, _Pred __pred) {
	typename std::iterator_traits<_In>::difference_type __n = 0;
	for (; __first != __last; ++__first)
	    if (!__pred(*__first)) ++__n;
	return __n;
    }

    template<typename _In, typename _Out, typename _Pred>
    _Out after(_In __first, _In __last, _Out __result, _Pred __pred) {
	_In __match = perly::first(__first, __last, __pred);
	if (__match == __last) return __result;
	return std::copy(++__match, __last, __result);
    }

    template<typename _In, typename _Out, typename _Pred>
    _Out after_incl(_In __first, _In __last, _Out __result, _Pred __pred) {
	_In __match = perly::first(__first, __last, __pred);
	if (__match == __last) return __result;
	return std::copy(__match, __last, __result);
    }

    // Dirty.
    template<typename _In, typename _Out, typename _Pred>
    _Out before(_In __first, _In __last, _Out __result, _Pred __pred) {
	_In __match = perly::first(__first, __last, __pred);
	if (__match == __first) return __result;
	return std::copy(__first, __match, __result);
    }

    // Dirty.
    template<typename _In, typename _Out, typename _Pred>
    _Out before_incl(_In __first, _In __last, _Out __result, _Pred __pred) {
	_In __match = perly::first(__first, __last, __pred);
	if (__match == __first) return __result;
	if (__match == __last) return std::copy(__first, __last, __result);
	return std::copy(__first, ++__match, __result);
    }

    template<typename _In1, typename _In2, typename _Out, typename _Binary>
    _Out pairwise(_In1 __first1, _In1 __last1, _In2 __first2, _Out __result, _Binary __binary_op) {
	for (; __first1 != __last1; ++__first1, ++__first2, ++__result) 
	    *__result = __binary_op(*__first1, *__first2);
	return __result;
    }

    template<typename _In, typename _Out1, typename _Out2, typename _Pred>
    void part(_In __first, _In __last, _Out1 __result1, _Out2 __result2, _Pred __pred) {
	for (; __first != __last; ++__first) {
	    if (__pred(*__first)) {
		*__result1 = *__first;
		++__result1;
	    } else {
		*__result2 = *__first;
		++__result2;
	    }
	}
    }

    // Range generator
    template<typename _Out, typename _T>
    _Out range(_Out __result, _T __lb, _T __ub) {
	for (; __lb <= __ub; ++__lb, __result)
	    *__result = __lb;
	return __result;
    }

    template<typename _Out>
    _Out range(_Out __result, int __lb, int __ub) {
	if (__lb > __ub) return __result;
	for (++__ub; __lb != __ub; ++__lb, __result)
	    *__result = __lb;
	return __result;
    }

    template<typename _Out>
    _Out range(_Out __result, long __lb, long __ub) {
	if (__lb > __ub) return __result;
	for (++__ub; __lb != __ub; ++__lb, __result)
	    *__result = __lb;
	return __result;
    }

//--------------------------------------------------
//     // shuffle (List::Util)
//     template<typename _In, typename _Out>
//     _Out shuffle(_In __first, _In __last, _Out __result) {
// 	_Out __ofirst = __result;
// 	_Out __olast = std::copy(__first, __last, __result);
// 	std::random_shuffle(__ofirst, __olast);
// 	return __result;
//     }
//-------------------------------------------------- 
}

#endif
