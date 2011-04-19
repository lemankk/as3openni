/****************************************************************************
*                                                                           *
*   Project:	AS3OpenNI-Bridge	                                       	*
*   Version:	Beta 1.0.0                                                  *
*   Author:     Tony Birleffi												*
*   URL: 		http://code.google.com/p/as3openni/							*
*                                                                           *
****************************************************************************/

/****************************************************************************
*                                                                           *
*   AS3OpenNI-Bridge Beta 1.0.0	                                            *
*   Copyright (C) 2011 Tony Birleffi. All Rights Reserved.                  *
*                                                                           *
*   This file has been provided pursuant to a License Agreement containing  *
*   restrictions on its use. This data contains valuable trade secrets      *
*   and proprietary information of PrimeSense Ltd. and is protected by law. *
*                                                                           *
****************************************************************************/

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

//-----------------------------------------------------------------------------
// Namespaces
//-----------------------------------------------------------------------------
using namespace std;
using namespace boost;

//-----------------------------------------------------------------------------
// Error Handling
//-----------------------------------------------------------------------------
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

//-----------------------------------------------------------------------------
// Init Methods
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    return 0;
}