// BwWait.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


/*
 * BwWait.c
 *
 *  Created on: Aug 23 2016
 *      Author: Audrey Waschura
 */


//================================================================================
// BOOST SOFTWARE LICENSE
//
// Copyright 2020 BitWise Laboratories Inc.
// Author.......Jim Waschura
// Contact......info@bitwiselabs.com
//
//Permission is hereby granted, free of charge, to any person or organization
//obtaining a copy of the software and accompanying documentation covered by
//this license (the "Software") to use, reproduce, display, distribute,
//execute, and transmit the Software, and to prepare derivative works of the
//Software, and to permit third-parties to whom the Software is furnished to
//do so, all subject to the following:
//
//The copyright notices in the Software and this entire statement, including
//the above license grant, this restriction and the following disclaimer,
//must be included in all copies of the Software, in whole or in part, and
//all derivative works of the Software, unless such copies or derivative
//works are solely in the form of machine-executable object code generated by
//a source language processor.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
//SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
//FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//DEALINGS IN THE SOFTWARE.
//================================================================================

#include <stdio.h>
#include <stdlib.h>

#include "BwDevice.h"
#include "UtilTrim.h"

static const int DFLT_REPEAT = 20;
static const int DFLT_INTERVAL = 1;
static const int DFLT_PORT = 923;

static const int RESPONSE_BUF_SIZE = 65536;
static char responseBuffer[RESPONSE_BUF_SIZE];

int main(int argc, char* argv[])
{
	char* IPAddress = NULL;
	int Port = DFLT_PORT;
	char* Prefix = NULL;
	bool helpFlag = false;
	bool verboseFlag = false;
	bool NoCheck = false;
	bool NoPrefix = false;
	int Repeat = DFLT_REPEAT;
	char* CriteriaString = NULL;
	int Interval = DFLT_INTERVAL;
	int IsNot = 0;

	/* initialize variables for settings from getenv values */
	/*  BW_PORT, BW_IP */

	size_t len;
	char* envString;

	if (_dupenv_s(&envString, &len, "BW_PORT") == 0)
	{
		int n;
		if (envString != NULL && len>0 && sscanf_s(envString, "%d", &n) == 1)
			Port = n;
		free(envString);
	}

	if (_dupenv_s(&envString, &len, "BW_IP") == 0)
	{
		if (envString != NULL)
			IPAddress = trim_string_quotes(envString);
	}

	if (_dupenv_s(&envString, &len, "BW_PREFIX") == 0)
	{
		if (envString != NULL)
			Prefix = trim_string_quotes(envString);
	}

	/* parse command line options */
	char QueryString[2048];
	QueryString[0] = 0;

	while (*(++argv))
	{
		if (!strcmp(*argv, "-h"))              helpFlag = 1;
		else if (!strcmp(*argv, "-help"))      helpFlag = 1;
		else if (!strcmp(*argv, "-v"))         verboseFlag = 1;
		else if (!strcmp(*argv, "-port"))      Port = atoi(*(++argv));
		else if (!strcmp(*argv, "-p"))         Port = atoi(*(++argv));
		else if (!strcmp(*argv, "-ip"))        IPAddress = *(++argv);
		else if (!strcmp(*argv, "-i"))         IPAddress = *(++argv);
		else if (!strcmp(*argv, "-nocheck"))   NoCheck = true;
		else if (!strcmp(*argv, "-n"))		   NoCheck = true;
		else if (!strcmp(*argv, "-prefix"))	   Prefix = *(++argv);
		else if (!strcmp(*argv, "-x"))		   Prefix = *(++argv);
		else if (!strcmp(*argv, "-noprefix"))  NoPrefix = true;
		else if (!strcmp(*argv, "-nx"))        NoPrefix = true;
		else if (!strcmp(*argv, "-repeat"))    Repeat = atoi(*(++argv));
		else if (!strcmp(*argv, "-r"))         Repeat = atoi(*(++argv));
		else if (!strcmp(*argv, "-sleep"))     Interval = atoi(*(++argv));
		else if (!strcmp(*argv, "-s"))         Interval = atoi(*(++argv));
		else if (!strcmp(*argv, "-is"))
		{
			IsNot = false;
			CriteriaString = *(++argv);
		}
		else if (!strcmp(*argv, "-isnot"))
		{
			IsNot = true;
			CriteriaString = *(++argv);
		}
		else
		{
			strcat_s(QueryString, 2048, *argv);
			strcat_s(QueryString, 2048, " ");
		}
	}

	trim_string_quotes(QueryString);

	//if (IPAddress == NULL)
	//	IPAddress = (char*) "localhost";

	/* if -h show help and exit */

	if (helpFlag)
	{
		printf("BwWait, version 2.0, (c) BitWise Laboratories, Inc.\n");
		printf("Purpose:  Repetitiously sends query to BitWise Laboratories\n");
		printf("          device until response matches specified criteria.\n");
		printf("Usage:    BwWait [options] <-is/-isnot> <criteria> <query>\n");
		printf("Options:  -h .............. display this message\n");
		printf("          -v .............. display verbose messages\n");
		printf("          -port <N> ....... (or -p) port number, default is %d, or use BW_PORT env variable\n",DFLT_PORT);
		printf("          -ip <addr> ...... (or -i) ip address required, or use BW_IP env variable\n");
		printf("          -nocheck......... (or -n) turn on fast mode to skip error checking\n");
		printf("          -prefix <str> ... (or -x) prefix added to each Xmit string, or use BW_PREFIX env variable\n");
		printf("          -noprefix ....... (or -nx) ignore any prefix that may be set\n");
		printf("          -sleep <N> ...... (or -s) seconds to sleep between polling, default is %d seconds\n",DFLT_INTERVAL);
		printf("          -repeat <N> ..... (or -r) poll limit, default is %d, -1 means forever\n",DFLT_REPEAT);
		printf("          -is <string> .... string must be true to continue\n");
		printf("          -isnot <string> . string must be false to continue\n");

		return 0;
	}

	/* if verbose, show current values */

	if (verboseFlag)
	{
		printf("IPAddress.......[%s]\n", (IPAddress == NULL) ? "NULL" : IPAddress);
		printf("Port............%d\n", Port);
		printf("NoCheck.........%d\n", NoCheck);
		printf("NoPrefix........%d\n", NoPrefix);
		printf("Prefix..........[%s]\n",(Prefix==NULL)?"NULL": Prefix);
		printf("QueryString.....[%s]\n", QueryString);
		printf("Repeat..........%d\n", Repeat);
		printf("IsNot...........%d\n", IsNot);
		printf("CriteriaString..[%s]\n", CriteriaString);
	}

	if (IPAddress == NULL)
	{
		fprintf(stderr,"IP Address must be specified on command line or using BW_IP enviornment variable.  Use -h for help.\n");
		return 1;
	}

	if ( QueryString[0]==0 )
	{
		fprintf(stderr, "Must specify query string on command line.  Use -h for help.\n");
		return 1;
	}

	if (Repeat<1)
	{
		fprintf(stderr, "Must specify repeat value greater than zero.  Use -h for help.\n");
		return 1;
	}

	if (Interval < 0)
	{
		fprintf(stderr, "Must specify repeat value greater than or equal to zero.  Use -h for help.\n");
		return 1;
	}

	/* open socket */

	BwDevice Device;
	int retn = 1;

	Device.setFastMode( NoCheck);

	/* Connect to host */

	Device.Connect( IPAddress, Port);

	try
	{
		trim_string_quotes(QueryString);
		trim_string_quotes(CriteriaString);

		while (Repeat != 0 && retn==1 )
		{
			char* prefix = NoPrefix ? (char*)"" : Prefix;

			if (verboseFlag)
				printf("Transmit: %s%s\n", prefix,QueryString);

			Device.QueryResponse(responseBuffer, RESPONSE_BUF_SIZE, "%s%s\n", prefix, QueryString);

			if (verboseFlag)
				printf("Response: %s\n", responseBuffer);

			trim_string_quotes(responseBuffer);

			if( verboseFlag )
				printf("compare response=[%s] with criteria=%s [%s]\n", responseBuffer, IsNot ? "is_not" : "is", CriteriaString);

			/* compare response with response string */
			if (!strcmp(responseBuffer, CriteriaString))
			{
				if (!IsNot)
					retn = 0;
			}
			else if (IsNot)
				retn = 0;

			Repeat--;
			if( Repeat>0 && retn==1 )
				Sleep(Interval * 1000);
		}

		if (verboseFlag && (Repeat == 0 && retn==1) )
		{
			fprintf(stderr, "Repeat limit reached\n");
			retn = 1;
		}

		Device.Disconnect();
	}
	catch (const char* msg)
	{
		fprintf(stderr,"Error: %s\n", msg);
		Device.Disconnect();
	}
	catch (...)
	{
		fprintf(stderr,"Unknown error\n");
		Device.Disconnect();
	}

	printf("%s\n", (retn == 0) ? "Success" : "Unsuccessful");

	return retn;
}




/* EOF */
