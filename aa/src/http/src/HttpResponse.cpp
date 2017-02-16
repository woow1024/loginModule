
#include "HttpResponse.h" 

#pragma warning( disable:4800 )



// static link to server 

//////////////////////////////////////////////////////////////////////////////////
// HttpResponse

HttpResponse::HttpResponse () :
	HttpRequest()
{

}


HttpResponse::HttpResponse ( HttpRequest & request ) :
	HttpRequest()

{
	setState(request);
}


HttpResponse::~HttpResponse ()
{
	release();
}

// create/release
bool HttpResponse::create ()
{
	// perform init
	//_hFile = 0;

	return true;
}

void HttpResponse::release ()
{
	// cleanup request
	HttpRequest::release();
  
}

bool HttpResponse::startServerApp ()
{
	addError( idHttpInternalError );
	return false;
}



bool HttpResponse::startResponse ()
{
	bool bWait = false;

	 

	return bWait;
}


bool HttpResponse::addHeading ()
{
	bool more = false;

	
	return more;
}

void HttpResponse::addTagetInfo ()
{
	
}

void HttpResponse::addListing ()
{
}
 

void HttpResponse::addContentType ()
{
	// get the extension
	string fullPath = getFullPath();
	string strExt   = StringUtil::mid( fullPath, fullPath.rfind('.') );

	// add content type
	string strType = "";
	if ( StringUtil::compareNoCase(strExt,".html") )
		strType = "text/html";
	else if ( StringUtil::compareNoCase(strExt,".xml") )
		strType = "text/xml";

	if ( !strType.empty() )
		addHeader( idParamContentType, strType );

}




/*
// create/release

bool HttpResponse::create ()
{
	// get response stream
	string & srchStr = _responseStrm.str();
	cout << srchStr;

	// set token string
	string tokenStr = "\r\n";

	// get response
	long first = 0; long last = 0;
	if ( !getParam(first,last,tokenStr,srchStr,_response) )
		return false;

	// get header
	if ( !getParam(first,last,tokenStr,srchStr,_header) )
		return true;


	// get response header
	if ( !getParam(first,last,tokenStr,srchStr,_responseHeader) )
		return true;

	// get response header
	if ( !getParam(first,last,tokenStr,srchStr,_entityHeader) )
		return true;

	// get response header
	if ( !getParam(first,last,tokenStr,srchStr,_entityBody) )
		return true;


	return true;
}


*/

