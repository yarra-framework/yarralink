#include "yarralink.h"
#include "yarralink_ver.h"

#include "MrServers/MrMeasSrv/SeqIF/sde_allincludes.h"
#include "MrServers/MrImaging/libSBB/SBBTSat.h"
#include "MrServers/MrImaging/seq/SystemProperties.h"   
#include "MrServers/MrImaging/seq/SystemProperties.h"        
#include "MrServers/MrProtSrv/MrProt/MrSlice.h"

#ifdef WIN32  
    #include "MrServers/MrProtSrv/MrProtocol/UILink/MrStdNameTags.h"
    #include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkLimited.h"
    #include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkSelection.h"
    #include "MrServers/MrProtSrv/MrProtocol/libUILink/UILinkArray.h"
    #include "MrServers/MrProtSrv/MrProtocol/UILink/StdProtRes/StdProtRes.h"
   // #include "MrServers/MrImaging/libUICtrl/UICtrl.h"
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

#define _COUT if(0) cout

static sSLICE_POS       asSLC[K_NO_SLI_MAX];

// ----------------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------------        
yarralink::yarralink()
{
    _COUT << "___Sequence created" << endl;
	
	clientLaunched=false;
}


// ----------------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------------   
yarralink::~yarralink()
{
    _COUT << "___Sequence destructed" << endl;
}


// ----------------------------------------------------------------------------------
// Initialization of the sequence
// ----------------------------------------------------------------------------------
NLSStatus yarralink::initialize
(
    SeqLim      *pSeqLim
)
{
    _COUT << "___Called initialize" << endl;

    // -------------------------------------------------------- 
    // Give general information about the sequence
    // -------------------------------------------------------- 

    pSeqLim->setMyOrigFilename ( __FILE__ );
    pSeqLim->setSequenceOwner    ( "USER" );

    // The sequence hint text is available via the "imprint name.dll" command
    pSeqLim->setSequenceHintText( (char *) "\n\
                                    Application: Yarra Client Launcher \n\
                                    Build: "__DATE__"   "__TIME__"\n");

#ifdef WIN32    
    // This configures some UI behavior (e.g. TD) and has to be called before my own SolveHandlers
    fStdImagingInitPost (pSeqLim);
#endif

    return SEQU__NORMAL;  
}


// ----------------------------------------------------------------------------------
// Preparation of the sequence during binary search and prior to sequence execution
// ----------------------------------------------------------------------------------
NLSStatus yarralink::prepare
(
    MrProt     *pMrProt,     // IMPORT: Measurement protocol  
    SeqLim     *pSeqLim,     // IMPORT: Sequence limits       
    SeqExpo    *pSeqExpo     // EXPORT: Returned values       
)
{
    _COUT << "___Called prepare" << endl;
    
	NLS_STATUS  lStatus = SEQU__NORMAL;
    lStatus = fSUPrepSlicePosArray (VER_PTR(pMrProt), VER_PTR(pSeqLim), asSLC);
	
    double dTotalMeasureTimeUsec    =   1000;
    pSeqExpo->setPreScans                    ( 0 );
    pSeqExpo->setRFEnergyInSequence_Ws       ( 0 );
    pSeqExpo->setMeasuredPELines             ( 0 );
    pSeqExpo->setMeasured3dPartitions        ( 0 );
    pSeqExpo->setMeasureTimeUsec             ( dTotalMeasureTimeUsec ); 
    pSeqExpo->setTotalMeasureTimeUsec        ( dTotalMeasureTimeUsec );
	pSeqExpo->setRelevantReadoutsForMeasTime ( 0 ); 

    // Preparation of sequence done. Output some debug information.
    _COUT << "___Reached end of prepare" << endl; 

	if (pSeqLim->isContextNormal())
	{
#ifdef WIN32  
		if (!clientLaunched)
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory( &pi, sizeof(pi) );

			TCHAR cmdline[512] = TEXT("C:\\MedCom\\MriCustomer\\seq\\yarra\\ort.dll");
			
			// Start the child process. 
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
				//return;
			}

			// Close process and thread handles. 
			CloseHandle( pi.hProcess );
			CloseHandle( pi.hThread );	
			
			clientLaunched=true;
		}				
#endif
	}
	
    return (lStatus);
}


// ----------------------------------------------------------------------------------
// Check of the sequence for SAR and gradient stimulation
// ----------------------------------------------------------------------------------   
NLSStatus yarralink::check
(
    MrProt       *pMrProt,            // IMP: user choice parameters  
    SeqLim       *pSeqLim,            // IMP: limits from fSEQInit()  
    SeqExpo      *pSeqExpo,           // IMP: exports from fSEQPrep() 
    SEQCheckMode *pSEQCheckMode       // unused                       
)
{        
    static const char *ptModule  = {"check"}; // Name of this module
    NLS_STATUS   lStatus         = SEQU__NORMAL;  
    return(lStatus);
}


// ----------------------------------------------------------------------------------
// Execution of the sequence
// ----------------------------------------------------------------------------------    
NLSStatus yarralink::run
(
    MrProt  *pMrProt,    // IMP: user choice parameters  
    SeqLim  *pSeqLim,    // IMP: limits from fSEQInit()  
    SeqExpo *pSeqExpo    // IMP: exports from fSEQPrep() 
)
{
    static const char *ptModule = {"run"};
	NLS_STATUS         lStatus          = SEQU__NORMAL ; 

    // ---------------------------------------------------------------------------
    #ifndef VER_VD
        fRTEBInit(&asSLC[0].getROT_MATRIX(), true);
    #else
        fRTEBInit(asSLC[0].getROT_MATRIX());
    #endif	
	
    fRTEI(1000     ,           0,        0,      0,            0,              0,            0,       0);	
    lStatus = fRTEBFinish();  
	
	return(lStatus); 	
}

NLS_STATUS yarralink::runKernel(MrProt *pMrProt,SeqLim *pSeqLim, SeqExpo *pSeqExpo, long lKernelMode, long lSlice, long lPartition, long lLine)
{
	return(SEQU__NORMAL); 	
}


// ----------------------------------------------------------------------------------
// VD line interface
// ----------------------------------------------------------------------------------   

#ifdef VER_VD

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
