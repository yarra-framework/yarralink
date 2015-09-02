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



void launchClient()
{
#ifdef WIN32  

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

#endif	
}


#ifdef WIN32  
    unsigned ui_WIP_CheckBox_GetLabelId(LINK_BOOL_TYPE* const, char* arg_list[], long lIndex)
    {
        static const char* const label1 = "Click to open   >>>";
        static const char* const label2 = YL_VERSION;
        static const char* const label3 = "Info at http://yarra.rocks";

		switch (lIndex) 
		{
		case 0:
			arg_list[0] = (char*)label1; 
			break;
		case 1:
			arg_list[0] = (char*)label2; 
			break;
		case 2:
			arg_list[0] = (char*)label3; 
			break;
		}

        return MRI_STD_STRING;
    }

    bool ui_WIP_CheckBox_GetOptions(LINK_BOOL_TYPE* const pThis , std::vector<unsigned>& rOptionVector, unsigned long& rulVerify, long lIndex)
    {
        rulVerify = LINK_BOOL_TYPE::VERIFY_OFF;
        rOptionVector.resize(2);
        rOptionVector[0] = false;
        rOptionVector[1] = true;

        return true;
    }

    bool ui_WIP_CheckBox_GetValue(LINK_BOOL_TYPE* const pThis , long lIndex)
    {
        return 0;
    }

    bool ui_WIP_CheckBox_SetValue(LINK_BOOL_TYPE* const pThis, bool value, long lIndex)
    {
		if (value)
		{
			launchClient();
		}
		
		return true;
    }
#endif	


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
	
	if ( LINK_BOOL_TYPE* pBool=_create<LINK_BOOL_TYPE>(pSeqLim, MR_TAG_SEQ_WIP1, (long) 0))
	{       
		pBool->registerGetLabelIdHandler  (ui_WIP_CheckBox_GetLabelId);
		pBool->registerGetOptionsHandler  (ui_WIP_CheckBox_GetOptions);
		pBool->registerGetValueHandler    (ui_WIP_CheckBox_GetValue);
		pBool->registerSetValueHandler    (ui_WIP_CheckBox_SetValue);
	}	
	
	if ( LINK_BOOL_TYPE* pBool=_create<LINK_BOOL_TYPE>(pSeqLim, MR_TAG_SEQ_WIP13, (long) 1))
	{       
		pBool->registerGetLabelIdHandler  (ui_WIP_CheckBox_GetLabelId);
		pBool->registerGetOptionsHandler  (ui_WIP_CheckBox_GetOptions);
		pBool->registerGetValueHandler    (ui_WIP_CheckBox_GetValue);
		pBool->registerSetValueHandler    (ui_WIP_CheckBox_SetValue);
	}	

	if ( LINK_BOOL_TYPE* pBool=_create<LINK_BOOL_TYPE>(pSeqLim, MR_TAG_SEQ_WIP14, (long) 2))
	{       
		pBool->registerGetLabelIdHandler  (ui_WIP_CheckBox_GetLabelId);
		pBool->registerGetOptionsHandler  (ui_WIP_CheckBox_GetOptions);
		pBool->registerGetValueHandler    (ui_WIP_CheckBox_GetValue);
		pBool->registerSetValueHandler    (ui_WIP_CheckBox_SetValue);
	}	

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

    if ( pSeqLim->isContextNormal() )
    {
		if (!clientLaunched)
		{
			launchClient();
			clientLaunched=true;
		}				
    }

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
