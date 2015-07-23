#include "yarralink.h"
#include "yarralink_ver.h"

#include "MrServers/MrImaging/libSBB/SBBTSat.h"
#include "MrServers/MrImaging/seq/SystemProperties.h"   
#include "MrServers/MrImaging/seq/SystemProperties.h"        
#include "MrServers/MrProtSrv/MrProt/MrSlice.h"

#ifdef VER_VE
    #include "MrServers/MrImaging/libSeqUtil/libSeqUtil.h"
    #include "MrServers/MrMeasSrv/MeasNuclei/IF/MeasKnownNuclei.h"
    #include "MrServers/MrProtSrv/MrProt/MeasParameter/MrSysSpec.h"
    #include "MrServers/MrProtSrv/MrProt/MeasParameter/MrRXSpec.h"
    #include "MrServers/MrImaging/seq/SystemProperties.h"
    #include "MrServers/MrImaging/libSeqSysProp/SysProperties.h"
#endif

#ifdef WIN32  
    #include "MrServers/MrProtSrv/MrProtocol/UILink/MrStdNameTags.h"
    #include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkLimited.h"
    #include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkSelection.h"
    #include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkArray.h"
    #include "MrServers/MrProtSrv/MrProtocol/UILink/StdProtRes/StdProtRes.h"
    #include "MrServers/MrMeasSrv/SeqIF/Sequence/Sequence.h" 
 
    #include <vector>
	#include <windows.h>
	#include <stdio.h>
	#include <tchar.h>	
	#include <iostream>
#endif

#ifndef SEQ_NAMESPACE
    #error SEQ_NAMESPACE not defined
#endif

#ifdef SEQUENCE_CLASS_YARRALINK
    SEQIF_DEFINE (SEQ_NAMESPACE::yarralink)
#endif

using namespace SEQ_NAMESPACE;
using namespace std;


static sSLICE_POS       asSLC[K_NO_SLI_MAX];


yarralink::yarralink()
{
	clientLaunched=false;
}


yarralink::~yarralink()
{
}


NLSStatus yarralink::initialize(SeqLim *pSeqLim)
{
    pSeqLim->setMyOrigFilename( __FILE__ );
    pSeqLim->setSequenceOwner( "USER" );

    pSeqLim->setSequenceHintText( (char *) "YarraLink " YL_VERSION "\nBuild: "__DATE__" "__TIME__"\n");
									
#ifdef WIN32    
    fStdImagingInitPost (pSeqLim);
#endif

    return SEQU__NORMAL;  
}


NLSStatus yarralink::prepare(MrProt *pMrProt, SeqLim *pSeqLim, SeqExpo *pSeqExpo)
{  
	NLS_STATUS  lStatus = SEQU__NORMAL;
    lStatus = fSUPrepSlicePosArray(VER_PTR(pMrProt), VER_PTR(pSeqLim), asSLC);
	
    double dTotalMeasureTimeUsec    =   1000;
    pSeqExpo->setPreScans                    ( 0 );
    pSeqExpo->setRFEnergyInSequence_Ws       ( 0 );
    pSeqExpo->setMeasuredPELines             ( 0 );
    pSeqExpo->setMeasured3dPartitions        ( 0 );
    pSeqExpo->setMeasureTimeUsec             ( dTotalMeasureTimeUsec ); 
    pSeqExpo->setTotalMeasureTimeUsec        ( dTotalMeasureTimeUsec );
	pSeqExpo->setRelevantReadoutsForMeasTime ( 0 ); 

#ifdef WIN32  
		if (!clientLaunched)
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory( &pi, sizeof(pi) );

			TCHAR cmdline[512] = TEXT("C:\\MedCom\\MriCustomer\\seq\\yarra\\ort.dll");
			
			// Start the child process
			if( !CreateProcess( NULL,   // No module name (use command line)
				cmdline,        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi )           // Pointer to PROCESS_INFORMATION structure
			) 
			{
				printf( "CreateProcess failed (%d).\n", GetLastError() );
			}

			// Close process and thread handles. 
			CloseHandle( pi.hProcess );
			CloseHandle( pi.hThread );	
			
			clientLaunched=true;
		}				
#endif
	
    return (lStatus);
}


NLSStatus yarralink::check(MrProt *pMrProt, SeqLim *pSeqLim, SeqExpo *pSeqExpo, SEQCheckMode *pSEQCheckMode)
{        
    return (SEQU__NORMAL);
}


NLSStatus yarralink::run(MrProt *pMrProt, SeqLim *pSeqLim, SeqExpo *pSeqExpo)
{
	return(SEQU__NORMAL); 	
}


NLS_STATUS yarralink::runKernel(MrProt *pMrProt,SeqLim *pSeqLim, SeqExpo *pSeqExpo, long lKernelMode, long lSlice, long lPartition, long lLine)
{
	return(SEQU__NORMAL); 	
}



// ----------------------------------------------------------------------------------
// VD/VE line interface
// ----------------------------------------------------------------------------------   

#ifndef VER_VB

    NLSStatus yarralink::initialize (SeqLim &rSeqLim)
    {
        return initialize(&rSeqLim);
    }

    NLSStatus yarralink::prepare (MrProt &rMrProt, SeqLim &rSeqLim, SeqExpo &rSeqExpo)
    {
        return prepare(&rMrProt, &rSeqLim, &rSeqExpo);
    }

    NLSStatus yarralink::check (MrProt  &rMrProt, SeqLim &rSeqLim, SeqExpo &rSeqExpo, SEQCheckMode *  pSEQCheckMode)
    {
        return check(&rMrProt, &rSeqLim, &rSeqExpo, pSEQCheckMode);
    }

    NLSStatus yarralink::run (MrProt &rMrProt, SeqLim &rSeqLim, SeqExpo &rSeqExpo)
    {
        return run(&rMrProt, &rSeqLim, &rSeqExpo);
    }

    NLS_STATUS yarralink::runKernel(MrProt &rMrProt, SeqLim &rSeqLim, SeqExpo &rSeqExpo, long lKernelMode, long lSlice, long lPartition, long lLine)
    {
        return runKernel(&rMrProt, &rSeqLim, &rSeqExpo, lKernelMode, lSlice, lPartition, lLine);
    }
	
#endif
