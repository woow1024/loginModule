#if !defined(HttpResponse_H)
#define HttpResponse_H


#include "HttpUtil.h"
#include "HttpRequest.h" 


#include <string>
#include <iostream>
#include <sstream>
using namespace std; 

#pragma warning( disable:4172 )


/////////////////////////////////////////////////////////////////////////////////////
// HttpResponse
//
// Purpose:	represent an http response

class HttpResponse :
	public HttpRequest
{
protected:

	//////////////////////////////////////////////////////////
	// http response format 
	//
	// Response Line Required
	// General Header Optional
	// Response Header Optional
	// Entity Header Optional
	// Entity Body Optional

	// static ref to server 

	// handle to file 

public:

	HttpResponse ();
	HttpResponse ( HttpRequest & request );
	virtual ~HttpResponse ();

	// create/release
	bool create  ();
	void release ();



	// content add methods
	bool	startResponse	();
	bool	startServerApp	();
	bool	addHeading		();
	void	addTagetInfo	();
	void	addListing		(); 
	void	addContentType	();

};



#endif