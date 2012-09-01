					#ifndef  LVV_STL_H
					#define  LVV_STL_H

					#include <stddef.h>

					#include <algorithm>
					#include <iterator>
					#include <string>
					#include <deque>
					#include <tuple>

					#include "range.h"

					namespace sto {

/////////////////////////////////////////////////////////////////////////////////////////  MEMBERS ALIASES

//  +Ct   ---   begin(),  	(n/a for c-arrays, use std::begin)

	template<typename Ct>		// do we need to care about r-value-ness here?
	eIF <is_range<Ct>()  &&  !std::is_array<Ct>::value,  cl_iterator<Ct>>
operator+      (Ct&& C) { return std::begin(C); };	// does not work with r-values


//  -Ct   ---   end(),  	(n/a for c-arrays, use std::end)
	template<typename Ct>
	eIF <is_range<Ct>()  &&  !std::is_array<Ct>::value,  cl_iterator<Ct>>
operator-      (Ct&& C) { return  std::end(C); };


	template<typename Ct>
	eIF <has_size<Ct>(), size_t>
operator~      (const Ct& C) { return size(C); };


//  if(!Ct)  --- (!Ct.empty())
//  if(Ct)   --- not implemented,  use  !!Ct instead
	template<typename Ct>
	eIF <has_empty<Ct>(), bool>
operator!      (const Ct& C) { return C.empty(); };


//  ++T, T++  ---  front()/back()/.first/.second  (n/a for c-arrays)

	template<typename Ct>
	eIF <is_range<Ct>()  &&  !std::is_array<Ct>::value, cl_reference<Ct>>
operator++      (Ct&& C) { return std::forward<cl_reference<Ct>>(*std::begin(C)); };

	template<typename Ct>
	eIF <is_range<Ct>()  &&  !std::is_array<Ct>::value, cl_reference<Ct>>
operator++      (Ct&& C, int) { return std::forward<cl_reference<Ct>>(*std::prev(sto::endz(C))); };




//  x << Ct >> x   ---  remove head / tail;   usage: scc 'dlong V{1,2,3};  i << V >> j; __ i, V, j;'   prints: 1 {2} 3 
	template<typename Ct, typename T>
	eIF <is_elem_of<T, Ct>() &&   has_pop_back<Ct>(),   Ct&&>
operator>>      (Ct&& C, T&& x)    { x = C.back();   C.pop_back();   return  std::forward<Ct>(C); };


	template<typename Ct, typename T>
	eIF <is_elem_of<T,Ct>()  &&   has_pop_front<Ct>(),   Ct&&>
operator<<      (T& x, Ct&& C)    { x = C.front();  C.pop_front();  return  std::forward<Ct>(C); };





// --Ct, Ct--  ---  pop_back/pop_front;     usage:   scc 'vint V{1,2}, W;  W << --V;  __ V, W;'   prints:    {2}, {1}
	template<typename Ct>
	eIF <is_range<Ct>(), Ct>
operator--      (Ct&& C)         { C.pop_front();   return  std::forward<Ct>(C); };


	template<typename Ct>
	eIF <is_range<Ct>(), Ct>
operator--      (Ct&& C, int)    { C.pop_back();    return  std::forward<Ct>(C); };


//////  X >> Cl << X

	namespace detail {
		template<class Ct, class X>  eIF<has_push_back  <Ct>(), Ct&&>  append_elem(Ct&& C1, X&& x)   { C1.push_back (std::forward<X>(x));  return std::forward<Ct>(C1); };
		template<class Ct, class X>  eIF<has_push       <Ct>(), Ct&&>  append_elem(Ct&& C1, X&& x)   { C1.push      (std::forward<X>(x));  return std::forward<Ct>(C1); };
		template<class Ct, class X>  eIF<has_1arg_insert<Ct>(), Ct&&>  append_elem(Ct&& C1, X&& x)   { C1.insert    (std::forward<X>(x));  return std::forward<Ct>(C1); };

		template<class Ct, class X>  eIF<has_push_front <Ct>(), Ct&&> prepend_elem(Ct&& C1, X&& x)   { C1.push_front(std::forward<X>(x));  return std::forward<Ct>(C1); };
		template<class Ct, class X>  eIF<has_push       <Ct>(), Ct&&> prepend_elem(Ct&& C1, X&& x)   { C1.push      (std::forward<X>(x));  return std::forward<Ct>(C1); };
		template<class Ct, class X>  eIF<has_1arg_insert<Ct>(), Ct&&> prepend_elem(Ct&& C1, X&& x)   { C1.insert    (std::forward<X>(x));  return std::forward<Ct>(C1); };
	}

	// Cl << x
	template<class Ct, class X>
	eIF <is_elem_of<X,Ct>(),  Ct&&>
operator << (Ct&& C1, X&& x)            {  detail::append_elem(std::forward<Ct>(C1),  std::forward<X>(x));   return  std::forward<Ct>(C1); };


	// Cl << Cl2
	template<class Ct, class Ct2> 
	eIF <have_same_elem<Ct,Ct2>(),  Ct&&>
operator <<  (Ct&& C1, Ct2&& C2)         {  for (auto &&x: C2)  detail::append_elem(std::forward<Ct>(C1), cl_elem_fwd<Ct2>(x));   return  std::forward<Ct>(C1); };



//////  T >> Cl 
	// x >> Cl 
	template<class Ct, class X>
	eIF <is_elem_of<X,Ct>(),  Ct&&>
operator >> (X&& x, Ct&& C1)            {  detail::prepend_elem(std::forward<Ct>(C1),  std::forward<X>(x));   return  std::forward<Ct>(C1); };


	// Ct >> Cl
	template<class Ct, class Cl> 
	eIF <have_same_elem<Cl,Ct>(),  Cl&&>
operator >>  (Ct&& C1, Cl&& C2)  {
	auto it = endz(C1);
	if (it != std::begin(C1))  it--;
	while(it != std::begin(C1))
		detail::prepend_elem(std::forward<Cl>(C2), cl_elem_fwd<Ct>(*it--));
	return  std::forward<Cl>(C2);
 };



//////////////////////////////////////////////////////////////////////////////////////////////  Ct op T

	namespace detail {


		// Ct / x     usage: scc 'copy(v9/2, v9/5,oi)'
			template<typename Ct, typename X>
			eIF <is_elem_of<X, Ct>(), cl_iterator<Ct>>
		find_elem(Ct&& C, const X& x)   { return std::find(std::begin(C), endz(C), x); };



		// Ct / f
			template<typename Ct, typename F>
			eIF <is_callable<F, bool(cl_elem_type<Ct>)>::value, cl_iterator<Ct>>
		find_elem(Ct&& C, const F& pred)  { return  std::find_if(std::begin(C), endz(C), pred); };


			
		//  Ct1 / Ct2   ---  search() --> it
			template<typename Ct1, typename Ct2>
			eIF <have_same_elem<Ct1, Ct2>(), cl_iterator<Ct1>>
		find_elem(Ct1&& C1, const Ct2& C2)    {  return std::search(std::begin(C1), endz(C1), std::begin(C2), endz(C2)); }; }


////////  Ct / T  
// ---  non callable
	template<typename Ct, typename T>
	eIF <is_range<Ct>() , cl_iterator<Ct>>
operator / (Ct&& C, const T& t)                                { return  detail::find_elem(std::forward<Ct>(C), t); };

// ---  plain func
	template<typename Ct>
	eIF <is_range<Ct>() , cl_iterator<Ct>>
operator / (Ct&& C, bool(*t)(cl_elem_type<Ct>))                { return  detail::find_elem(std::forward<Ct>(C), t); };

// ---  func obj, lambda
	template<typename Ct>
	eIF <is_range<Ct>() , cl_iterator<Ct>>
operator / (Ct&& C, std::function<bool(cl_elem_type<Ct>)> t)   { return  detail::find_elem(std::forward<Ct>(C), t); };



////////  Ct % T

//  ---  non callable
	template<typename Ct, typename T>
	eIF <is_range<Ct>(), bool>
operator % (Ct&& C, const T& t)                               { return  endz(C) != detail::find_elem(std::forward<Ct>(C), t); };

//  ---  plain func
	template<typename Ct>
	eIF <is_range<Ct>(), bool>
operator % (Ct&& C, bool(*t)(cl_elem_type<Ct>))               { return  endz(C) != detail::find_elem(std::forward<Ct>(C), t); };

//  ---  func obj, lambda
	template<typename Ct>
	eIF <is_range<Ct>(), bool>
operator % (Ct&& C, std::function<bool(cl_elem_type<Ct>)> t)  { return  endz(C) != detail::find_elem(std::forward<Ct>(C), t); };

////////////////////////////////////////////////////////////////////////////////////////////////// MAP / TRANSFORM
	
	
	namespace detail {
		template<typename T>	static void	cstr_zstop(const T&    ret) {};
		__attribute__((unused))	static void	cstr_zstop(      char* ret) { *++ret = '\0'; };
	};


//  Ct * F   ---  transform(+C,-C,+D,F) -> D   (temporary demo, should really return  collection, not a container)

	// overload for generic Ret 
	template<
		typename Ct,
		typename F,
		typename T = cl_elem_type<Ct>,
		//typename Ret=typename std::function<rm_qualifier<F>>::result_type,
		typename Ret= decltype(std::declval<F>()(std::declval<T>()))
	> 
	eIF <is_range<Ct>()  &&  is_callable<F, Ret(T)>::value, std::vector<Ret>>
operator *       (Ct&& C, const F& f)    {
	std::vector<Ret> D;
	auto ret = std::transform(std::begin(C), endz(C), back_inserter(D), f);
	detail::cstr_zstop(ret);
	return  D;
 };


	// overload for :  Ret == T   (needed for oveloaded functions like abs)
	template<
		typename Ct,
		typename T = cl_elem_type<Ct>,
		typename Ret= T
	> 
	eIF <is_range<Ct>(), std::vector<Ret>>
operator *       (Ct&& C, T (*f)(T) )    {
	std::vector<Ret> D;
	auto ret = std::transform(std::begin(C), endz(C), std::back_inserter(D), f);
	detail::cstr_zstop(ret);
	return  D;
 };


	// overload for :  lambdas
	template<
		typename Ct,
		typename T = cl_elem_type<Ct>,
		typename Ret= T
	> 
	eIF <is_range<Ct>(), std::vector<Ret>>
operator *       (Ct&& C, std::function<T(T)> f )    {
	std::vector<Ret> D;
	auto ret = std::transform(std::begin(C), endz(C), std::begin(D), f);
	detail::cstr_zstop(ret);
	return  D;
 };


//  Ct || F   ---  accumulate(+C+1,-C, ++C, F) -> D  		 

	// overload for: std::min
	template< typename Ct, typename T = cl_elem_type<Ct>, typename R = T > 
	eIF <is_range<Ct>(), R>
operator ||       (Ct&& C, const R& (*f)(const T&, const T&) )    {
	auto i = std::begin(std::forward<Ct>(C));
	std::advance(i,1);
	return  std::accumulate(i, std::end(std::forward<Ct>(C)), C.front(), f);
 };


	// overload for: lambda, std::plus
	template< typename Ct, typename T = cl_elem_type<Ct>, typename R = T > 
	eIF <is_range<Ct>(), R>
operator ||       (Ct&& C, identity<std::function<T(const T&, const T&)>> f )    {
	auto i = std::begin(std::forward<Ct>(C));
	std::advance(i,1);
	const T init = C.front();
	return  std::accumulate(i, std::end(std::forward<Ct>(C)), init, f);
 };



/////////////////////////////////////////////////////////////////////////////////////////////////////  TUPLE / PAIR

template<typename U, typename V>   U&     operator++   (std::pair<U,V>& P)      { return P.first;  };
template<typename U, typename V>   V&     operator++   (std::pair<U,V>& P, int) { return P.second; };


// ++Tpl 
	template <class... Types>
	typename std::tuple_element<std::tuple_size<std::tuple<Types...> >::value-1, typename std::tuple<Types...> >::type&
operator++	(typename std::tuple<Types...>& Tpl, int)  {  return  std::get<std::tuple_size<std::tuple<Types...> >::value-1>(Tpl); };

// ++Tpl  (const)
	template <class... Types>
	const typename std::tuple_element<std::tuple_size<std::tuple<Types...> >::value-1, typename std::tuple<Types...> >::type&
operator++	(const typename std::tuple<Types...>& Tpl, int)  {  return  std::get<std::tuple_size<std::tuple<Types...> >::value-1>(Tpl); };


// Tpl++ 
	template <class... Types>
	typename std::tuple_element<0, std::tuple<Types...> >::type&
operator++	(typename std::tuple<Types...>& Tpl)  {  return  std::get<0>(Tpl); };

// Tpl++  (const) 
	template <class... Types>
	const typename std::tuple_element<0, std::tuple<Types...> >::type&
operator++	(const typename std::tuple<Types...>& Tpl)  {  return  std::get<0>(Tpl); };


	template <class... Types>
	constexpr size_t
operator~	(const typename std::tuple<Types...>& Tpl)  {  return  std::tuple_size<std::tuple<Types...> >::value; };


/////////////////////////////////////////////////////////////////////////////////////////////////////  STACK


//  Stack--
	template<typename Ct>
	eIF <is_stack<Ct>(), Ct>
operator--      (Ct&& C, int)    { C.pop();   return std::forward<Ct>(C); };

//  Stack >> x
//	scc 'int i=10; (stack<int>() << 1 << 2) >> i;  i'
//	3
//	scc 'int i=10; stack<int> st; st << 1 << 2 << 3;  st >> i; __ st, i;'
//	[1, 2] 3

	template<typename Ct, typename Xt>
	eIF <is_stack<Ct>()  &&  is_elem_of<Xt,Ct>(),  Ct>
operator>>      (Ct&& C, Xt&& x)    { x = C.top();  C.pop();   return std::forward<Ct>(C); };


//  Stack++
//	scc ' __ (stack<int>() << 1 << 2 << 3)++;'
//	3
//	scc 'stack<int> st; st << 1 << 2 << 3; st++'
//	3

	template<typename Ct>
 	eIF <is_stack<Ct>(), cl_elem_type<Ct>>
operator++      (Ct&& C, int)    { return C.top(); };



/////////////////////////////////////////////////////////////////////////////////////////////////////  QUEUE


//  --Queue
	template<typename Ct>
	eIF <is_queue<Ct>(), Ct> &
operator--      (Ct& C)    { C.pop();   return C; };

//  x << Queue
	template<typename Ct>
	eIF <is_queue<Ct>(), typename Ct::value_type> &
operator<<      (typename Ct::value_type& x, Ct& C)    { x = C.front();  C.pop();   return x; };


//  Queue++
	template<typename Ct>
	eIF <is_queue<Ct>(), typename Ct::value_type> &
operator++      (Ct& C, int)    { return C.back(); };

//  ++Queue
	template<typename Ct>
	eIF <is_queue<Ct>(), typename Ct::value_type> &
operator++      (Ct& C)    { return C.front(); };

//  !Queue
	template<typename Ct>
	eIF <is_queue<Ct>(), size_t>
operator~      (const Ct& C)    { return C.size(); };


/////////////////////////////////////////////////////////////////////////////////////////////////////  ITERATOR


//  It / x   ---  find() --> it	   usage: scc 'copy(+v9/2, +v9/5, oi),  does not work with pointers (C++ constrain)
	template<typename It>
	eIF <is_iterator<It>(),  It>
operator/       (It&& i, const typename std::iterator_traits<It>::value_type x)    {  while(*i != x) ++i;    return std::forward<It>(i); };

/*
	template<typename It, typename Ct>
	eIF <is_iterator<It>::value,  It>
operator <<       (It&& it, Ct&& C)    {};
*/

					};
					#endif	// LVV_STL_H
